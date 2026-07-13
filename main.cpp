#include <QApplication>
#include <QWidget>
#include <QPushButton>

int main(int argc, char *argv[])
{
    // Grafik arayüz olay döngüsünü başlatır
    QApplication app(argc, argv);

    // Temel boş bir pencere (Widget) nesnesi oluşturuyoruz
    QWidget anaPencere;
    anaPencere.setWindowTitle("Butonlu Pencere");
    anaPencere.resize(400, 300); // Genişlik: 400, Yükseklik: 300
    QPushButton *button = new QPushButton("tıkla", &anaPencere);

    button->setGeometry(150, 130, 100, 40);
    // Pencereyi ekranda gösterir
    anaPencere.show();

    return app.exec();
}