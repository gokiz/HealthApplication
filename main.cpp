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
    anaPencere.resize(400, 520); // Genişlik: 400, Yükseklik: 300

    QLabel *vkeBaslik = new QLabel ("<b>BODY MASS INDEX (BMI)</b>");
    vkeBaslik->setGeometry(50,20,300,30);


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

    QLabel *ayrac = new QLabel("-----------------------------------------------------------------");
    ayrac->setGeometry(50,220,300,20);
    ayrac->setStyleSheet("color: #718096;");

    QLabel *suDurumEtiketi = new QLabel("The amount of water drunk today: 0 ml\nGoal: 2000ml", &anaPencere);
    suDurumEtiketi->setGeometry(50,290,300,50);
    suDurumEtiketi->setStyleSheet("font-size: 14px; font-weight:bold; color: #3182ce;");

    QPushButton *suEkleButon = new QPushButton("Add one cup(250 ml)", &anaPencere);
    suEkleButon->setGeometry(100,360,200,30);

    QPushButton *suSifirlaButon = new QPushButton("Reset", &anaPencere);
    suSifirlaButon->setGeometry(100, 415, 200, 30);

    int *toplamSu = new int(0);
    QObject::connect(suEkleButon, &QPushButton::clicked, &anaPencere, [toplamSu, suDurumEtiketi, &anaPencere]() {
        *toplamSu += 250;
        suDurumEtiketi->setText("Bugün içilen su: " + QString::number(*toplamSu) + " ml\nHedef: 2000 ml");

        if(*toplamSu >= 2000) {
            QMessageBox::information(&anaPencere, "Congratulations!", "You have reached the target. 🎉💧");
        }
    });

    // Sıfırlama Buton Bağlantısı
    QObject::connect(suSifirlaButon, &QPushButton::clicked, &anaPencere, [toplamSu, suDurumEtiketi]() {
        *toplamSu = 0;
        suDurumEtiketi->setText("The amount of water drunk today: 0 ml\nGoal: 2000 ml");
    });

    // Pencereyi ekranda gösterir
    anaPencere.show();

    return app.exec();
}