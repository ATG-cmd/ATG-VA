#include "mainwindow.h"

#include <QSplashScreen>
#include <QTimer>
#include <QProgressBar>
#include <QThread>
#include <QApplication>
#include <QStyleFactory>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

  //  a.setStyle("fusion");
//    QApplication::setStyle(QStyleFactory::create("fusion"));
//    qDebug() << QStyleFactory::keys();

  /*  QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("C:/Users/dark_/Desktop/CistemVR abdiel/logo 800x400.png"));
    QProgressBar *pb = new QProgressBar(splash);
    pb->setGeometry(320,300,200,25);
    splash->show();

    for(int i = 0; i <= 100; i++)
    {
     pb->setValue(i);
     QThread::msleep(50);
    }
    w.show();
     splash->close()*/;
   w.showFullScreen();
   // w.show();
    return a.exec();
}

