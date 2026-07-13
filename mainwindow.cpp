#include "mainwindow.h" // Kendi header dosyanı çağırıyor
#include <QMessageBox>

// Constructor ismi sınıf adıyla aynı olmak zorunda
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), toplamSu(0) {
    this->setWindowTitle("Health & Sport Tracker ");
    this->resize(400, 720);

    // BMI Tasarımı
    QLabel *vkeBaslik = new QLabel("<b>BODY MASS INDEX (BMI)</b>", this);
    vkeBaslik->setGeometry(50, 20, 300, 30);

    kiloGiris = new QLineEdit(this);
    kiloGiris->setGeometry(160, 60, 150, 30);
    kiloGiris->setPlaceholderText("e.g., 70");

    boyGiris = new QLineEdit(this);
    boyGiris->setGeometry(160, 110, 150, 30);
    boyGiris->setPlaceholderText("e.g., 175");

    QPushButton *vkeButon = new QPushButton("Calculate BMI", this);
    vkeButon->setGeometry(160, 160, 150, 40);

    // Water Tracker Tasarımı
    QLabel *suBaslik = new QLabel("<b>DAILY WATER TRACKER</b>", this);
    suBaslik->setGeometry(50, 250, 300, 30);

    suDurumEtiketi = new QLabel("Water consumed today: 0 ml\nGoal: 2000 ml", this);
    suDurumEtiketi->setGeometry(50, 270, 300, 50);

    QPushButton *suEkleButon = new QPushButton("+1 Glass (250 ml)", this);
    suEkleButon->setGeometry(50, 330, 140, 40);
    QPushButton *suSifirlaButon = new QPushButton("Reset Water", this);
    suSifirlaButon->setGeometry(210, 330, 140, 40);

    QLabel *sportBaslik = new QLabel("<b>7-DAY SPORTS TRACKER</b>",this);
    sportBaslik->setGeometry(50,400,300,30);

    QStringList gunler = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    for (int i = 0; i < 7; ++i) {
        gunKutulari[i] = new QCheckBox(gunler[i], this);
        // Kutucukları yan yana 45'er piksel arayla diziyoruz
        gunKutulari[i]->setGeometry(50 + (i * 45), 440, 45, 30);

        // Kutucuklardan birine tıklandığında durumu güncelle
        connect(gunKutulari[i], &QCheckBox::toggled, this, &MainWindow::sporDurumuGuncelle);
    }

    // Status Bar
    bilgiCubugu = new QLabel("Application ready...", this);
    bilgiCubugu->setGeometry(10, 690, 380, 20);
    bilgiCubugu->setStyleSheet("color: gray; font-size: 11px; border-top: 1px solid #ccc;");

    // Bağlantılar
    connect(vkeButon, &QPushButton::clicked, this, &MainWindow::vkeHesapla);
    connect(suEkleButon, &QPushButton::clicked, this, &MainWindow::suEkle);
    connect(suSifirlaButon, &QPushButton::clicked, this, &MainWindow::suSifirla);

}

void MainWindow::vkeHesapla() {
    QString kiloText = kiloGiris->text();
    QString boyText = boyGiris->text();

    if (kiloText.isEmpty() || boyText.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Fields cannot be left empty!");
        bilgiCubugu->setText("Error: Missing input data!");
        return;
    }

    double vke = kiloText.toDouble() / ((boyText.toDouble() / 100.0) * (boyText.toDouble() / 100.0));
    bilgiCubugu->setText("Last BMI Calculation: " + QString::number(vke, 'f', 2));

    QString durum;
    if(vke < 18.5) durum = "Underweight";
    else if(vke >= 18.5 && vke < 25.0) durum = "Normal Weight";
    else if(vke >= 25.0 && vke < 30.0) durum = "Overweight";
    else durum = "Obese";

    QMessageBox::information(this, "Your BMI Result", "BMI: " + QString::number(vke, 'f', 2) + "\nStatus: " + durum);
}

void MainWindow::suEkle() {
    toplamSu += 250;
    suDurumEtiketi->setText("Water consumed today: " + QString::number(toplamSu) + " ml\nGoal: 2000 ml");
    bilgiCubugu->setText("Water added: +" + QString::number(toplamSu) + " ml");

    if (toplamSu == 2000) { //sadece ilk ulaştığında gösterir
        QMessageBox::information(this, "Congratulations!", "You have reached your daily water consumption goal! 🎉💧");
    }
}
// Sıfırlama fonksiyonunun gövdesi
void MainWindow::suSifirla() {
    toplamSu = 0;
    suDurumEtiketi->setText("Water consumed today: 0 ml\nGoal: 2000 ml");
    bilgiCubugu->setText("Water tracker has been reset.");
}

// Spor takip çizelgesi kontrol fonksiyonu
void MainWindow::sporDurumuGuncelle() {
    int tamamlananGun = 0;
    for (int i = 0; i < 7; ++i) {
        if (gunKutulari[i]->isChecked()) {
            tamamlananGun++;
        }
    }
    bilgiCubugu->setText("Sports Tracker: " + QString::number(tamamlananGun) + " / 7 days completed.");

    if (tamamlananGun == 7) {
        QMessageBox::information(this, "Excellent!", "You completed your sports routine for the entire week! 🏃‍🔥");
    }
}