#ifndef CORE_H
#define CORE_H

#include "mainwindow.h"
#include "network.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

class Core
{
    MainWindow* w;
    Network* network;
public:
    explicit Core(QApplication &a);

    ~Core();
    void run();
};

#endif // CORE_H
