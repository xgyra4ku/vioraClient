#include "network.h"
#include "mainwindow.h"
#include "structure.h"

#include <iostream>
#include <utility>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <random>
#include <chrono>
#include <vector>
#include <mutex>

Network::Network()
    : serverAddr{}
    , clientSocket{}
    , isConnected(false)
    , stop_flag_receive(false)
    , stop_flag_send(false)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
}

Network::~Network() {
    stop_flag_receive = true;
    stop_flag_send = true;
    if (threadReceiveMessage->joinable()){
        threadReceiveMessage->join();
    }
    if (threadSendMessage->joinable()){
        threadSendMessage->join();
    }
    closesocket(clientSocket);
    WSACleanup();
}
void Network::startThreadReceiveMessage(std::vector<Message>& queue_Received_Messages){
    stop_flag_receive = false;
    threadReceiveMessage = new std::thread(&Network::ReceiveMessages, this, clientSocket, std::ref(queue_Received_Messages));
}

void Network::stopThreadReceiveMessage(){
    stop_flag_receive = true;
    if (threadReceiveMessage->joinable()){
        threadReceiveMessage->join();
    }
}

void Network::startThreadSendMessage(){
    stop_flag_send = false;
    threadSendMessage = new std::thread(&Network::SendMessages, this, clientSocket);
}

void Network::stopThreadSendMessage(){
    stop_flag_send = true;
    if (threadReceiveMessage->joinable()){
        threadReceiveMessage->join();
    }
}


void Network::addMessageToBuffer(std::string message) {
    Message msg;
    msg.data["msg"] = std::move(message);
    msg.data["key"] = generateCode(6);
    msg.data["type"] = "message";

    std::lock_guard<std::mutex> lock(sendMutex);
    newMessages.push_back(msg);
}

void Network::SendMessages(SOCKET socket) {
    while (!stop_flag_send) {
        if (isConnected) {
            sendMutex.lock();
            if (newMessages.empty()) {
                sendMutex.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                continue;
            }
            Message bufferMsg = newMessages.back();
            newMessages.pop_back();
            sendMutex.unlock();
            std::vector<char> buffer = bufferMsg.serialize();
            int bytesSent = send(socket, buffer.data(), buffer.size(), 0);
            if (bytesSent <= 0) {
                std::cout << "Disconnected from server\n";
                isConnected = false;
                //break;
            }
        } else {
            std::cout << "Try connection to server" << std::endl;
            isConnected = tryConnection();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void Network::ReceiveMessages(SOCKET socket, std::vector<Message>& queue_Received_Messages) {
    while (!stop_flag_receive) {
        if (isConnected) {
            Message bufferMsg;
            std::vector<char> receivedBuffer(1024);
            const int bytesReceived = recv(socket, receivedBuffer.data(), receivedBuffer.size(), 0);
            if (bytesReceived <= 0) {
                std::cout << "Disconnected from server\n";
                isConnected = false;
                //break;
            }
            receivedBuffer.resize(bytesReceived);
            bufferMsg.deserialize(receivedBuffer);
            queue_Received_Messages.push_back(bufferMsg);

            for (const auto& [key, value] : bufferMsg.data) {
                std::cout << key << ": " << value << std::endl;
            }
            //// Передача сообщения в поток Qt
            //QString message = QString::fromUtf8(buffer);
            //QMetaObject::invokeMethod(&w, [message, &w]() {
            //    w.addReceivedMessage(message);
            //});
        } else {
            isConnected = tryConnection();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Try connection to server" << std::endl;
        }
    }
}

bool Network::tryConnection(){
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed\n";
        return true
        ;
    }
    std::cout << "Connected to server\n";
    return true;
}

std::string Network::generateCode(const int length = 6) {
    const std::string characters =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string code;
    for(int i = 0; i < length; ++i) {
        code += characters[distribution(generator)];
    }
    return code;
}

void Network::closeConnection() const {
    closesocket(clientSocket);
    WSACleanup();
}


