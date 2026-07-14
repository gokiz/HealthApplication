#include "mainwindow.h" // Kendi header dosyanı çağırıyor
#include "historydialog.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDate>
#include <QSettings>

// Constructor ismi sınıf adıyla aynı olmak zorunda
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), toplamSu(0) {

    // Kayıtlı su hedefini yükle (yoksa varsayılan 2000 ml)
    QSettings ayarlar("FBU", "HealthSportTracker");
    suHedefi = ayarlar.value("water/dailyGoal", 2000).toInt();

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

    // Ayarlanabilir su hedefi
    QLabel *suHedefEtiketi = new QLabel("Daily Goal (ml):", this);
    suHedefEtiketi->setGeometry(50, 285, 130, 30);

    suHedefGirisi = new QSpinBox(this);
    suHedefGirisi->setGeometry(185, 285, 90, 30);
    suHedefGirisi->setRange(500, 10000);
    suHedefGirisi->setSingleStep(250);
    suHedefGirisi->setValue(suHedefi);

    QPushButton *suHedefKaydetButon = new QPushButton("Set Goal", this);
    suHedefKaydetButon->setGeometry(285, 285, 100, 30);

    suDurumEtiketi = new QLabel("Water consumed today: 0 ml\nGoal: " + QString::number(suHedefi) + " ml", this);
    suDurumEtiketi->setGeometry(50, 320, 300, 50);

    QPushButton *suEkleButon = new QPushButton("+1 Glass (250 ml)", this);
    suEkleButon->setGeometry(50, 380, 140, 40);
    QPushButton *suSifirlaButon = new QPushButton("Reset Water", this);
    suSifirlaButon->setGeometry(210, 380, 140, 40);

    QLabel *sportBaslik = new QLabel("<b>7-DAY SPORTS TRACKER</b>", this);
    sportBaslik->setGeometry(50, 460, 300, 30);

    QStringList gunler = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    for (int i = 0; i < 7; ++i) {
        gunKutulari[i] = new QCheckBox(gunler[i], this);
        // Kutucukları yan yana 45'er piksel arayla diziyoruz
        gunKutulari[i]->setGeometry(50 + (i * 45), 500, 45, 30);

        // Kutucuklardan birine tıklandığında durumu güncelle
        connect(gunKutulari[i], &QCheckBox::toggled, this, &MainWindow::sporDurumuGuncelle);
    }

    QPushButton *gecmisButon = new QPushButton("History && Charts", this);
    gecmisButon->setGeometry(50, 550, 300, 40);

    // Status Bar
    bilgiCubugu = new QLabel("Application ready...", this);
    bilgiCubugu->setGeometry(10, 690, 380, 20);
    bilgiCubugu->setStyleSheet("color: gray; font-size: 11px; border-top: 1px solid #ccc;");

    // Bağlantılar
    connect(vkeButon, &QPushButton::clicked, this, &MainWindow::vkeHesapla);
    connect(suEkleButon, &QPushButton::clicked, this, &MainWindow::suEkle);
    connect(suSifirlaButon, &QPushButton::clicked, this, &MainWindow::suSifirla);
    connect(suHedefKaydetButon, &QPushButton::clicked, this, &MainWindow::suHedefiAyarla);
    connect(gecmisButon, &QPushButton::clicked, this, &MainWindow::gecmisiGoster);

    veritabaniBaslat();
    sporHaftalikSifirlamaKontrol();
    verileriYukle(); //eski
}

MainWindow::~MainWindow() {
    if (db.isOpen()) {
        db.close();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Uygulama kapatılırken o anki su miktarının kaybolmamasını garanti ediyoruz
    suKaydet();
    QMainWindow::closeEvent(event);
}

void MainWindow::veritabaniBaslat() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("health_data.db");

    if (!db.open()) {
        bilgiCubugu->setText("Database Error: " + db.lastError().text());
        return;
    }

    QSqlQuery sorgu;
    // BMI Tablosu
    sorgu.exec("CREATE TABLE IF NOT EXISTS bmicalculations (id INTEGER PRIMARY KEY AUTOINCREMENT, weight REAL, height REAL, bmi REAL, status TEXT)");

    // SU TABLOSU: Her güne ait benzersiz bir tarih satırı tutar
    sorgu.exec("CREATE TABLE IF NOT EXISTS water_tracker ("
               "water_date TEXT PRIMARY KEY, "
               "amount INTEGER)");

    // Spor Tablosu
    sorgu.exec("CREATE TABLE IF NOT EXISTS sports_tracker (day_id INTEGER PRIMARY KEY, checked INTEGER)");
    for (int i = 0; i < 7; i++) {
        QSqlQuery ekleSorgu;
        ekleSorgu.prepare("INSERT OR IGNORE INTO sports_tracker (day_id, checked) VALUES (:day_id, 0)");
        ekleSorgu.bindValue(":day_id", i);
        ekleSorgu.exec();
    }
}

void MainWindow::sporHaftalikSifirlamaKontrol() {
    // Uygulama her açıldığında, içinde bulunulan ISO haftası daha önce
    // kaydedilenden farklıysa (yani yeni bir haftaya - pratikte pazartesiye -
    // geçilmişse) spor takip tablosunu otomatik olarak sıfırlar.
    QSettings ayarlar("FBU", "HealthSportTracker");

    int guncelYil = 0;
    int guncelHafta = QDate::currentDate().weekNumber(&guncelYil);
    QString guncelHaftaKimligi = QString::number(guncelYil) + "-W" + QString::number(guncelHafta);

    QString sonSifirlananHafta = ayarlar.value("sports/lastResetWeek", "").toString();

    if (sonSifirlananHafta != guncelHaftaKimligi) {
        QSqlQuery sorgu;
        sorgu.exec("UPDATE sports_tracker SET checked = 0");
        ayarlar.setValue("sports/lastResetWeek", guncelHaftaKimligi);
    }
}

void MainWindow::verileriYukle() {
    // Bugünün tarihini YYYY-MM-DD formatında alıyoruz (Örn: 2026-07-13)
    QString bugun = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery suSorgu;
    suSorgu.prepare("SELECT amount FROM water_tracker WHERE water_date = :date");
    suSorgu.bindValue(":date", bugun);

    if (suSorgu.exec() && suSorgu.next()) {
        toplamSu = suSorgu.value(0).toInt();
    } else {
        toplamSu = 0; // Eğer bugün daha önce hiç su içilmediyse 0'dan başla
    }

    suHedefBildirildi = (toplamSu >= suHedefi); // Hedefe zaten ulaşılmışsa tekrar tebrik mesajı çıkmasın
    suDurumEtiketi->setText("Water consumed today: " + QString::number(toplamSu) + " ml\nGoal: " + QString::number(suHedefi) + " ml");

    // Spor verisini yükleme kısmı (ayrı bir sorgu nesnesiyle)
    QSqlQuery sporSorgu;
    if (sporSorgu.exec("SELECT day_id, checked FROM sports_tracker ORDER BY day_id")) {
        while (sporSorgu.next()) {
            int dayId = sporSorgu.value(0).toInt();
            int isChecked = sporSorgu.value(1).toInt();
            if (dayId >= 0 && dayId < 7) {
                gunKutulari[dayId]->blockSignals(true);
                gunKutulari[dayId]->setChecked(isChecked == 1);
                gunKutulari[dayId]->blockSignals(false);
            }
        }
    }

    int tamamlananGun = 0;
    for (int i = 0; i < 7; i++) if (gunKutulari[i]->isChecked()) tamamlananGun++;
    bilgiCubugu->setText("Welcome back! " + QString::number(tamamlananGun) + "/7 days of sports completed.");
}

void MainWindow::suKaydet() {
    QString bugun = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery sorgu;
    // Tarih satırı varsa miktarını günceller, yoksa o tarihe özel yeni satır açar
    sorgu.prepare("INSERT OR REPLACE INTO water_tracker (water_date, amount) VALUES (:date, :amount)");
    sorgu.bindValue(":date", bugun);
    sorgu.bindValue(":amount", toplamSu);

    if (!sorgu.exec()) {
        bilgiCubugu->setText("Water Save Error: " + sorgu.lastError().text());
    }
}

void MainWindow::sporKaydet() {
    for (int i = 0; i < 7; ++i) {
        QSqlQuery sorgu;
        sorgu.prepare("UPDATE sports_tracker SET checked = :checked WHERE day_id = :day_id");
        sorgu.bindValue(":checked", gunKutulari[i]->isChecked() ? 1 : 0);
        sorgu.bindValue(":day_id", i);
        sorgu.exec();
    }
}

void MainWindow::vkeHesapla() {
    QString kiloText = kiloGiris->text().trimmed();
    QString boyText = boyGiris->text().trimmed();

    if (kiloText.isEmpty() || boyText.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Fields cannot be left empty!");
        return;
    }

    bool kiloOk = false, boyOk = false;
    double kilo = kiloText.toDouble(&kiloOk);
    double boyCm = boyText.toDouble(&boyOk);

    if (!kiloOk || !boyOk || kilo <= 0 || boyCm <= 0) {
        QMessageBox::warning(this, "Warning", "Please enter valid positive numbers for weight and height!");
        return;
    }

    double boy = boyCm / 100.0;
    double vke = kilo / (boy * boy);

    QString durum;
    if (vke < 18.5) durum = "Underweight";
    else if (vke < 25.0) durum = "Normal Weight";
    else if (vke < 30.0) durum = "Overweight";
    else durum = "Obese";

    // --- SQLITE KAYIT ALANI ---
    QSqlQuery insertSorgu;
    insertSorgu.prepare("INSERT INTO bmicalculations (weight, height, bmi, status) "
                        "VALUES (:weight, :height, :bmi, :status)");
    insertSorgu.bindValue(":weight", kilo);
    insertSorgu.bindValue(":height", boyCm); // cm olarak kaydediyoruz
    insertSorgu.bindValue(":bmi", vke);
    insertSorgu.bindValue(":status", durum);

    if (insertSorgu.exec()) {
        bilgiCubugu->setText("BMI calculation tracked and saved.");
    } else {
        bilgiCubugu->setText("BMI Save Error: " + insertSorgu.lastError().text());
    }

    QMessageBox::information(this, "Your BMI Result", "BMI: " + QString::number(vke, 'f', 2) + "\nStatus: " + durum);
}

void MainWindow::suEkle() {
    toplamSu += 250;
    suDurumEtiketi->setText("Water consumed today: " + QString::number(toplamSu) + " ml\nGoal: " + QString::number(suHedefi) + " ml");
    bilgiCubugu->setText("Water added: +250 ml (Total: " + QString::number(toplamSu) + " ml)");

    suKaydet(); // ANA HATA DÜZELTMESİ: eskiden bu satır hiç çağrılmıyordu, bu yüzden su kaydı kalıcı olmuyordu

    if (toplamSu >= suHedefi && !suHedefBildirildi) {
        suHedefBildirildi = true;
        QMessageBox::information(this, "Congratulations!", "You have reached your daily water consumption goal! 🎉💧");
    }
}

// Sıfırlama fonksiyonunun gövdesi
void MainWindow::suSifirla() {
    toplamSu = 0;
    suHedefBildirildi = false;
    suDurumEtiketi->setText("Water consumed today: 0 ml\nGoal: " + QString::number(suHedefi) + " ml");
    suKaydet();
    bilgiCubugu->setText("Water tracker reset and saved.");
}

void MainWindow::suHedefiAyarla() {
    suHedefi = suHedefGirisi->value();

    QSettings ayarlar("FBU", "HealthSportTracker");
    ayarlar.setValue("water/dailyGoal", suHedefi);

    suHedefBildirildi = (toplamSu >= suHedefi);
    suDurumEtiketi->setText("Water consumed today: " + QString::number(toplamSu) + " ml\nGoal: " + QString::number(suHedefi) + " ml");
    bilgiCubugu->setText("Daily water goal updated to " + QString::number(suHedefi) + " ml.");
}

// Spor takip çizelgesi kontrol fonksiyonu
void MainWindow::sporDurumuGuncelle() {
    int tamamlananGun = 0;
    for (int i = 0; i < 7; ++i) {
        if (gunKutulari[i]->isChecked()) {
            tamamlananGun++;
        }
    }
    sporKaydet();
    bilgiCubugu->setText("Sports Tracker: " + QString::number(tamamlananGun) + " / 7 days completed.");

    if (tamamlananGun == 7) {
        QMessageBox::information(this, "Excellent!", "You completed your sports routine for the entire week! 🏃‍🔥");
    }
}

void MainWindow::gecmisiGoster() {
    historydialog dialog(this);
    dialog.exec();
}