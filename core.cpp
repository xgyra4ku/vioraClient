#include "core.h"

Core::Core(QApplication &a)
{
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "voicebet_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    w = new MainWindow();
    w->show();
    network = new Network(*w);
}

Core::~Core() {
    delete network;
    delete w;
    network->stopThreadReceiveMessage();
}


void Core::run(){
    network->startThreadReceiveMessage();

}
