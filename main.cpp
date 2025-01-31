#include <QApplication>
#include <QMainWindow>

// void ReceiveMessages(SOCKET socket,MainWindow &w) {
//     char buffer[1024];
//     while (true) {
//         int bytesReceived = recv(socket, buffer, sizeof(buffer), 0);
//         if (bytesReceived <= 0) {
//             std::cout << "Disconnected from server\n";
//             break;
//         }
//         buffer[bytesReceived] = '\0';
//         std::cout << "Received: " << buffer << std::endl;
//         // Передача сообщения в поток Qt
//         QString message = QString::fromUtf8(buffer);
//         QMetaObject::invokeMethod(&w, [message, &w]() {
//             w.addReceivedMessage(message);
//         });
//     }
// }

// void net(MainWindow& w) {
//     WSADATA wsaData;
//     if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//         std::cerr << "WSAStartup failed\n";
//     }

//     SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (clientSocket == INVALID_SOCKET) {
//         std::cerr << "Socket creation failed\n";
//         WSACleanup();
//     }

//     sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(12345);
//     inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

//     if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
//         std::cerr << "Connect failed\n";
//         closesocket(clientSocket);
//         WSACleanup();
//     }

//     std::cout << "Connected to server\n";

//     std::thread(ReceiveMessages, clientSocket, std::ref(w)).detach();


//     std::string message;
//     while (true) {
//         std::getline(std::cin, message);
//         send(clientSocket, message.c_str(), message.size(), 0);
//     }

//     closesocket(clientSocket);
//     WSACleanup();
// }

#include "core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Core core(a);
    core.run();
    return a.exec();
}
