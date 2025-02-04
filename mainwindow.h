#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QListWidget> // Для отображения сообщений
#include <QVBoxLayout> // Для размещения виджетов
#include <QScrollArea>
#include <vector>
#include <thread>
#include <mutex>

#include "structure.h"
#include "network.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void addSentMessage(const QString &message);   // Отправить сообщение
    void addReceivedMessage(const QString &message); // Получить сообщение

private slots:

    void on_pushButton_pressed();
    void ProcessingMessage();
    void startThreadProcessingMessage();
    void stopThreadProcessingMessage();

private:
    Ui::MainWindow *ui;

    QScrollArea *scrollArea;     // Область прокрутки
    QWidget *scrollWidget;       // Виджет для размещения сообщений
    QVBoxLayout *messageLayout; // Динамически создаваемый макет

    Network *network{};

    std::vector<Message> queue_Received_Messages;

    std::thread *threadProcessingMessage{};
    std::mutex mutexProcessingMessage;

    bool stop_flag_processing_message;

};
#endif // MAINWINDOW_H
