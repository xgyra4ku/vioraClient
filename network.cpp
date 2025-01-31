#include "network.h"
#include "mainwindow.h"
#include "structure.h"

#include <QMainWindow>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

Network::Network(MainWindow& w)
    : isConnected(false)
    , clientSocket{}
    , serverAddr{}
    , stop_flag(false)
    , w(&w)
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
    stop_flag = true;
    if (threadReceiveMessage->joinable()){
        threadReceiveMessage->join();
    }
    closesocket(clientSocket);
    WSACleanup();
}
void Network::startThreadReceiveMessage(){
    stop_flag = false;
    threadReceiveMessage = new std::thread(&Network::ReceiveMessages, this, clientSocket, std::ref(*w));
}

void Network::stopThreadReceiveMessage(){
    stop_flag = true;
    if (threadReceiveMessage->joinable()){
        threadReceiveMessage->join();
    }
}

void Network::ReceiveMessages(SOCKET socket, MainWindow &w) {
    Message bufferMsg;
    std::vector<char> receivedBuffer(1024);
    while (!stop_flag) {
        if (isConnected) {
            int bytesReceived = recv(socket, receivedBuffer.data(), receivedBuffer.size(), 0);
            if (bytesReceived <= 0) {
                std::cout << "Disconnected from server\n";
                isConnected = false;
                //break;
            }
            receivedBuffer.resize(bytesReceived);
            bufferMsg.deserialize(receivedBuffer);

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
        return false;
    }
    std::cout << "Connected to server\n";
    return true;

}



