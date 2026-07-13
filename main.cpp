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
    anaPencere.setWindowTitle("Health Application-Login");
    anaPencere.resize(400, 300); // Genişlik: 400, Yükseklik: 300


    QLabel *kiloEtiketi = new QLabel("Weight(kg): ", &anaPencere);
    kiloEtiketi->setGeometry(50,80,100,30);


    QLineEdit *kiloGiris = new QLineEdit(&anaPencere);
    kiloGiris->setGeometry(160,80,150,30);
    kiloGiris->setPlaceholderText("Example: 70");


    QPushButton *button = new QPushButton("Save", &anaPencere);
    button->setGeometry(150, 130, 100, 40);

    // Butona tıklandığında girilen kiloyu mesaj olarak gösterelim
    QObject::connect(button, &QPushButton::clicked, &anaPencere, [kiloGiris, &anaPencere]() {
        QString girilenKilo = kiloGiris->text(); // Kutudaki yazıyı alıyoruz

        if(girilenKilo.isEmpty()) {
            QMessageBox::warning(&anaPencere, "Warning", "Enter your weight!");
        } else {
            QMessageBox::information(&anaPencere, "Successful", "Weigh was recorded: " + girilenKilo + " kg");
        }
    });

    // Pencereyi ekranda gösterir
    anaPencere.show();

    return app.exec();
}