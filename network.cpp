#include "network.h"
#include "mainwindow.h"

#include <QMainWindow>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

Network::Network()
    : isConnected(false)
    , clientSocket{}
    , serverAddr{}
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

void Network::ReceiveMessages(SOCKET socket, MainWindow &w) {
    char buffer[1024];
    while (true) {
        if (isConnected) {
            int bytesReceived = recv(socket, buffer, sizeof(buffer), 0);
            if (bytesReceived <= 0) {
                std::cout << "Disconnected from server\n";
                break;
            }
            buffer[bytesReceived] = '\0';
            std::cout << "Received: " << buffer << std::endl;
            // Передача сообщения в поток Qt
            QString message = QString::fromUtf8(buffer);
            QMetaObject::invokeMethod(&w, [message, &w]() {
                w.addReceivedMessage(message);
            });
        }
    }
}

