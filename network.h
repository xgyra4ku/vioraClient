#ifndef NETWORK_H
#define NETWORK_H

#include "mainwindow.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

class Network
{
private:
    sockaddr_in serverAddr, clientAddr;
    SOCKET clientSocket;
    void ReceiveMessages(SOCKET socket, MainWindow &w);

    std::thread threadReceiveMessage;
    bool isConnected;
public:
    Network();
};

#endif // NETWORK_H
