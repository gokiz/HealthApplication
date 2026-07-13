#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    // Grafik arayüz olay döngüsünü başlatır
    QApplication app(argc, argv);

    // Temel boş bir pencere (Widget) nesnesi oluşturuyoruz
    QWidget anaPencere;
    anaPencere.setWindowTitle("Boş Pencere");
    anaPencere.resize(400, 300); // Genişlik: 400, Yükseklik: 300

    // Pencereyi ekranda gösterir
    anaPencere.show();

    return app.exec();
}