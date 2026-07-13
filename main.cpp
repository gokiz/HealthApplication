#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    // Grafik arayüz olay döngüsünü başlatır
    QApplication app(argc, argv);

    // Temel boş bir pencere (Widget) nesnesi oluşturuyoruz
    QWidget anaPencere;
    anaPencere.setWindowTitle("Health Application-Weight & Height");
    anaPencere.resize(400, 350); // Genişlik: 400, Yükseklik: 300


    QLabel *kiloEtiketi = new QLabel("Weight(kg): ", &anaPencere);
    kiloEtiketi->setGeometry(50,60,100,30);


    QLineEdit *kiloGiris = new QLineEdit(&anaPencere);
    kiloGiris->setGeometry(160,60,150,30);
    kiloGiris->setPlaceholderText("Example: 70");

    QLabel *boyEtiketi = new QLabel("Weight: ", &anaPencere);
    boyEtiketi->setGeometry(50,110,100,30);

    QLineEdit *boyGiris = new QLineEdit(&anaPencere);
    boyGiris->setGeometry(160,110,150,30);
    boyGiris->setPlaceholderText("Example:175");

    QPushButton *button = new QPushButton("Save", &anaPencere);
    button->setGeometry(160, 180, 150, 40);

    // Butona tıklandığında girilen kiloyu mesaj olarak gösterelim
    QObject::connect(button, &QPushButton::clicked, &anaPencere, [kiloGiris, boyGiris, &anaPencere]() {
        QString kilo = kiloGiris->text();
        QString boy = boyGiris->text();

        if(kilo.isEmpty() || boy.isEmpty()) {
            QMessageBox::warning(&anaPencere, "Warning", "Please fill in all the fields!");
        } else {
            QMessageBox::information(&anaPencere, "Successful",
                                     "data was received!\nWeight: " + kilo + " kg\nHeight: " + boy + " cm");
        }
    });

    // Pencereyi ekranda gösterir
    anaPencere.show();

    return app.exec();
}