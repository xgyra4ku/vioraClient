#ifndef NETWORK_H
#define NETWORK_H

#include "mainwindow.h"
#include "structure.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <random>

class Network
{
    std::vector<Message> newMessages;

    MainWindow *w;
    sockaddr_in serverAddr, clientAddr;
    SOCKET clientSocket;

    std::thread* threadReceiveMessage, *threadSendMessage;
    bool isConnected, stop_flag_receive, stop_flag_send;

    void ReceiveMessages(SOCKET socket, MainWindow &w);

    void SendMessages(SOCKET socket, MainWindow &w);

public:
    Network(MainWindow& w);
    ~Network();

    void startThreadReceiveMessage();

    void stopThreadReceiveMessage();

    void startThreadSendMessage();

    void stopThreadSendMessage();

    void closeConnection();

    void addMessageToBuffer(std::string message);

    bool tryConnection();

    static std::string generateCode(int length);
};

#endif // NETWORK_H
