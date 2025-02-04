#ifndef NETWORK_H
#define NETWORK_H

#include "structure.h"

#include <winsock2.h>
#include <thread>
#include <random>
#include <vector>
#include <mutex>

class Network
{
    std::vector<Message> newMessages;

    sockaddr_in serverAddr, clientAddr{};
    SOCKET clientSocket;

    std::thread* threadReceiveMessage{}, *threadSendMessage{};
    std::mutex sendMutex, receiveMutex;

    bool isConnected, stop_flag_receive, stop_flag_send;

    void ReceiveMessages(SOCKET socket, std::vector<Message>& queue_Received_Messages);

    void SendMessages(SOCKET socket);

public:
    Network();
    ~Network();

    void startThreadReceiveMessage(std::vector<Message>& queue_Received_Messages);

    void startThreadSendMessage();

    void stopThreadReceiveMessage();

    void stopThreadSendMessage();

    void closeConnection() const;

    void addMessageToBuffer(std::string message);

    bool tryConnection();

    static std::string generateCode(int length);
};

#endif // NETWORK_H
