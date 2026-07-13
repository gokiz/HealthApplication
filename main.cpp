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
    anaPencere.setWindowTitle("Health Application- BMI Calculator");
    anaPencere.resize(400, 350); // Genişlik: 400, Yükseklik: 300


    QLabel *kiloEtiketi = new QLabel("Weight(kg): ", &anaPencere);
    kiloEtiketi->setGeometry(50,60,100,30);


    QLineEdit *kiloGiris = new QLineEdit(&anaPencere);
    kiloGiris->setGeometry(160,60,150,30);
    kiloGiris->setPlaceholderText("Example: 70");

    QLabel *boyEtiketi = new QLabel("Height: ", &anaPencere);
    boyEtiketi->setGeometry(50,110,100,30);

    QLineEdit *boyGiris = new QLineEdit(&anaPencere);
    boyGiris->setGeometry(160,110,150,30);
    boyGiris->setPlaceholderText("Example:175");

    QPushButton *button = new QPushButton("Calculate BMI", &anaPencere);
    button->setGeometry(160, 180, 150, 40);

    // Butona tıklandığında girilen kiloyu mesaj olarak gösterelim
    QObject::connect(button, &QPushButton::clicked, &anaPencere, [kiloGiris, boyGiris, &anaPencere]() {
        QString kiloText = kiloGiris->text();
        QString boyText = boyGiris->text();

        if(kiloText.isEmpty() || boyText.isEmpty()) {
            QMessageBox::warning(&anaPencere, "Warning", "Please fill in all the fields!");
            return;
        }
        double kilo = kiloText.toDouble();
        double boy = boyText.toDouble() / 100.0;
        double bmi = kilo / (boy * boy);

        QString durum;
        if(bmi < 18.5) {
            durum = "Weak";
        } else if(bmi >= 18.5 && bmi < 25.0) {
            durum = "Normal";
        } else if(bmi >= 25.0 && bmi < 30.0) {
            durum = "Overweight";
        } else {
            durum = "Obese";
        }

        // Sonucu kullanıcıya gösteriyoruz (QString::number ile sayıyı virgülden sonra 2 basamakla sınırlandırdık)
        QMessageBox::information(&anaPencere, "BMI Result",
                                 "Body Mass Index: " + QString::number(bmi, 'f', 2) + "\nStatus: " + durum);
    });

    // Pencereyi ekranda gösterir
    anaPencere.show();

    return app.exec();
}