#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    // Grafik arayüz olay döngüsünü başlatır
    QApplication app(argc, argv);

    // Temel boş bir pencere (Widget) nesnesi oluşturuyoruz
    QWidget anaPencere;
    anaPencere.setWindowTitle("Interactive Window");
    anaPencere.resize(400, 300); // Genişlik: 400, Yükseklik: 300
    QPushButton *button = new QPushButton("Click Here", &anaPencere);
    button->setGeometry(150, 130, 100, 40);


    QObject::connect(button, &QPushButton:: clicked, &anaPencere, [&anaPencere](){
        QMessageBox::information(&anaPencere, "Information", "You have taken the first step towards the health app.");
    });
    // Pencereyi ekranda gösterir
    anaPencere.show();

    return app.exec();
}