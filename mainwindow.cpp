#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLabel>
#include <QScrollBar>
#include <QTimer>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scrollArea(new QScrollArea)
    , scrollWidget(new QWidget)
    , messageLayout(new QVBoxLayout)
    , stop_flag_processing_message(false)
{
    std::cout << 1 << std::endl;
    ui->setupUi(this);

    // Настраиваем содержимое scrollArea
    ui->scrollAreaMessage->setWidgetResizable(true);

    // Убираем существующий макет (если он есть) у scrollAreaWidgetContents
    if (ui->scrollAreaWidgetContents->layout()) {
        delete ui->scrollAreaWidgetContents->layout();
    }
    std::cout << 2 << std::endl;

    // Устанавливаем динамически созданный макет
    ui->scrollAreaWidgetContents->setLayout(messageLayout);

    // Настраиваем свойства макета
    messageLayout->setContentsMargins(0, 0, 0, 0); // Убираем отступы
    messageLayout->setSpacing(5); // Задаем расстояние между сообщениями
    // В конструкторе MainWindow
    QTimer::singleShot(0, this, [this]() {
        ui->scrollAreaMessage->verticalScrollBar()->setValue(
            ui->scrollAreaMessage->verticalScrollBar()->maximum()
            );
    });
    std::cout << 3 << std::endl;
    network = new Network();
    network->startThreadReceiveMessage(queue_Received_Messages);
    network->startThreadSendMessage();
    startThreadProcessingMessage();

}

MainWindow::~MainWindow() {
    network->stopThreadReceiveMessage();
    network->stopThreadSendMessage();
    network->closeConnection();
    stopThreadProcessingMessage();
    delete ui;
    delete network;
}

void MainWindow::startThreadProcessingMessage() {
    stop_flag_processing_message = false;
    threadProcessingMessage = new std::thread(&MainWindow::ProcessingMessage, this);
}

void MainWindow::stopThreadProcessingMessage() {
    stop_flag_processing_message = true;
    if (threadProcessingMessage->joinable()) {
        threadProcessingMessage->join();
    }
}


void MainWindow::addSentMessage(const QString &message) {
    // Создаем виджет сообщения
    auto *messageLabel = new QLabel("Вы: " + message);
    messageLabel->setAlignment(Qt::AlignRight);
    messageLabel->setStyleSheet("background-color: #25b5b1; padding: 5px; border-radius: 8px;");

    // Добавляем сообщение в макет
    messageLayout->addWidget(messageLabel);

    // Прокручиваем вниз с использованием QTimer
    QTimer::singleShot(0, this, [this]() {
        ui->scrollAreaMessage->verticalScrollBar()->setValue(
            ui->scrollAreaMessage->verticalScrollBar()->maximum()
            );
    });
}


void MainWindow::addReceivedMessage(const QString &message) {
    auto *messageLabel = new QLabel("Собеседник: " + message);
    messageLabel->setAlignment(Qt::AlignLeft);
    messageLabel->setStyleSheet("background-color: #7d7d7d; padding: 5px; border-radius: 8px;");

    messageLayout->addWidget(messageLabel);

    QTimer::singleShot(0, this, [this]() {
        ui->scrollAreaMessage->verticalScrollBar()->setValue(
            ui->scrollAreaMessage->verticalScrollBar()->maximum()
            );
    });
}

void MainWindow::on_pushButton_pressed() {
    QString message = ui->lineEdit->text();
    ui->lineEdit->clear();
    addSentMessage(message);
    network->addMessageToBuffer(message.toStdString());
}

void MainWindow::ProcessingMessage() {
    while (!stop_flag_processing_message) {
        if (!queue_Received_Messages.empty()) {
            addReceivedMessage(queue_Received_Messages.back().data["msg"].c_str());
            queue_Received_Messages.pop_back();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}


