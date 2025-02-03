#include <QApplication>
#include <QMainWindow>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        if (const QString baseName = "voicebet_" + QLocale(locale).name(); translator.load(":/i18n/" + baseName)) {
            QApplication::installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return QApplication::exec();
}
