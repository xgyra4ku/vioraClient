#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QListWidget> // Для отображения сообщений
#include <QVBoxLayout> // Для размещения виджетов
#include <QScrollArea>

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
    ~MainWindow();

    void addSentMessage(const QString &message);   // Отправить сообщение
    void addReceivedMessage(const QString &message); // Получить сообщение

private slots:
    void on_pushButton_pressed();

private:
    Ui::MainWindow *ui;

    QScrollArea *scrollArea;     // Область прокрутки
    QWidget *scrollWidget;       // Виджет для размещения сообщений
    QVBoxLayout *messageLayout; // Динамически создаваемый макет
};
#endif // MAINWINDOW_H
