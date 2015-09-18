#include "starterwindow.h"
#include "ekurisplashscreen.h"
#include "buildcarwindow.h"

#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EkuriSplashScreen splashScreen;
    splashScreen.show();

    QTime time = QTime::currentTime();

    while(time.secsTo(QTime::currentTime()) < 1) {
        QApplication::processEvents();
    }

    StarterWindow s;
    s.show();

    splashScreen.finish(&s);

    return a.exec();
}
