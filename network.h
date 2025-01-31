#ifndef NETWORK_H
#define NETWORK_H

#include "mainwindow.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

class Network
{
    MainWindow *w;
    sockaddr_in serverAddr, clientAddr;
    SOCKET clientSocket;
    void ReceiveMessages(SOCKET socket, MainWindow &w);

    std::thread* threadReceiveMessage;
    bool isConnected, stop_flag;
public:
    Network(MainWindow& w);
    ~Network();

    void startThreadReceiveMessage();

    void stopThreadReceiveMessage();

    bool tryConnection();
};

#endif // NETWORK_H
