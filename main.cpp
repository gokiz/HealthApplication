#include <QApplication>
#include "mainwindow.h" // mainwindow.h çağırıyoruz

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Nesneyi MainWindow sınıfından üretiyoruz
    MainWindow uygulama;
    uygulama.show();

    return app.exec();
}