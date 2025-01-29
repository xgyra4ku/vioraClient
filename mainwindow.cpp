#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLabel>
#include <QScrollBar>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , messageLayout(new QVBoxLayout) // Создаем макет
{
    ui->setupUi(this);

    // Настраиваем содержимое scrollArea
    ui->scrollAreaMessage->setWidgetResizable(true);

    // Убираем существующий макет (если он есть) у scrollAreaWidgetContents
    if (ui->scrollAreaWidgetContents->layout()) {
        delete ui->scrollAreaWidgetContents->layout();
    }

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addSentMessage(const QString &message)
{
    // Создаем виджет сообщения
    QLabel *messageLabel = new QLabel("Вы: " + message);
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


void MainWindow::addReceivedMessage(const QString &message)
{
    QLabel *messageLabel = new QLabel("Собеседник: " + message);
    messageLabel->setAlignment(Qt::AlignLeft);
    messageLabel->setStyleSheet("background-color: #7d7d7d; padding: 5px; border-radius: 8px;");

    messageLayout->addWidget(messageLabel);

    QTimer::singleShot(0, this, [this]() {
        ui->scrollAreaMessage->verticalScrollBar()->setValue(
            ui->scrollAreaMessage->verticalScrollBar()->maximum()
            );
    });
}

void MainWindow::on_pushButton_pressed()
{

}

