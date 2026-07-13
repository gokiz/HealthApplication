#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

// --- YENİ ÖZELLİK: SAĞLIK UYGULAMASI SINIFI ---
// Kodun daha düzenli olması ve büyümesi için her şeyi bir sınıf (class) içinde topluyoruz
class SaglikUygulamasi : public QWidget {
public:
    SaglikUygulamasi(QWidget *parent = nullptr) : QWidget(parent) {
        // Pencere ayarları
        this->setWindowTitle("Sağlık Uygulaması - OOP Sürümü");
        this->resize(400, 550);

        // --- VKE Bölümü ---
        QLabel *vkeBaslik = new QLabel("<b>VÜCUT KİTLE ENDEKSİ (VKE)</b>", this);
        vkeBaslik->setGeometry(50, 20, 300, 30);

        kiloGiris = new QLineEdit(this);
        kiloGiris->setGeometry(160, 60, 150, 30);
        kiloGiris->setPlaceholderText("Örn: 70");

        boyGiris = new QLineEdit(this);
        boyGiris->setGeometry(160, 110, 150, 30);
        boyGiris->setPlaceholderText("Örn: 175");

        QPushButton *vkeButon = new QPushButton("VKE Hesapla", this);
        vkeButon->setGeometry(160, 160, 150, 40);

        // --- Su Takip Bölümü ---
        QLabel *suBaslik = new QLabel("<b>GÜNLÜK SU TAKİPÇİSİ</b>", this);
        suBaslik->setGeometry(50, 250, 300, 30);

        suDurumEtiketi = new QLabel("Bugün içilen su: 0 ml\nHedef: 2000 ml", this);
        suDurumEtiketi->setGeometry(50, 290, 300, 50);

        QPushButton *suEkleButon = new QPushButton("+1 Bardak Ekle", this);
        suEkleButon->setGeometry(100, 360, 200, 40);

        // --- YENİ GÖRSEL ÖZELLİK: BİLGİLENDİRME ÇUBUĞU ---
        bilgiCubugu = new QLabel("Uygulama hazır. Veri girişi bekliyor...", this);
        bilgiCubugu->setGeometry(10, 520, 380, 20);
        bilgiCubugu->setStyleSheet("color: gray; font-size: 11px; border-top: 1px solid #ccc;");

        // Buton Bağlantıları (Sinyaller)
        connect(vkeButon, &QPushButton::clicked, this, &SaglikUygulamasi::vkeHesapla);
        connect(suEkleButon, &QPushButton::clicked, this, &SaglikUygulamasi::suEkle);
    }

private:
    // Sınıf değişkenleri (State)
    QLineEdit *kiloGiris;
    QLineEdit *boyGiris;
    QLabel *suDurumEtiketi;
    QLabel *bilgiCubugu;
    int toplamSu = 0;

    // VKE Hesaplama Fonksiyonu
    void vkeHesapla() {
        QString kiloText = kiloGiris->text();
        QString boyText = boyGiris->text();

        if (kiloText.isEmpty() || boyText.isEmpty()) {
            QMessageBox::warning(this, "Uyarı", "Alanlar boş bırakılamaz!");
            bilgiCubugu->setText("Hata: Eksik veri girişi!");
            return;
        }

        double vke = kiloText.toDouble() / ((boyText.toDouble() / 100.0) * (boyText.toDouble() / 100.0));
        bilgiCubugu->setText("Son Hesaplama VKE: " + QString::number(vke, 'f', 2));

        QMessageBox::information(this, "Sonuç", "VKE: " + QString::number(vke, 'f', 2));
    }

    // Su Ekleme Fonksiyonu
    void suEkle() {
        toplamSu += 250;
        suDurumEtiketi->setText("Bugün içilen su: " + QString::number(toplamSu) + " ml\nHedef: 2000 ml");
        bilgiCubugu->setText("Su eklendi: +" + QString::number(toplamSu) + " ml");

        if (toplamSu >= 2000) {
            QMessageBox::information(this, "Tebrikler", "Su hedefine ulaşıldı!");
        }
    }
};

// Ana fonksiyonumuz artık çok daha temiz!
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Sınıfımızdan bir nesne üreterek uygulamayı başlatıyoruz
    SaglikUygulamasi uygulama;
    uygulama.show();

    return app.exec();
}