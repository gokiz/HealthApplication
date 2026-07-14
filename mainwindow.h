#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QSqlDatabase>
#include <QCloseEvent>

// Sınıf adı projenle uyumlu olarak MainWindow yapıldı
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QLineEdit *kiloGiris;
    QLineEdit *boyGiris;
    QLabel *suDurumEtiketi;
    int toplamSu;
    bool suHedefBildirildi = false; // Aynı gün içinde tebrik mesajının tekrar tekrar çıkmasını engeller

    QSpinBox *suHedefGirisi;
    int suHedefi; // Kullanıcının belirlediği günlük su hedefi (ml), QSettings ile kalıcı

    QCheckBox *gunKutulari[7];
    QLabel *bilgiCubugu;

    QSqlDatabase db;
    void veritabaniBaslat();
    void verileriYukle();
    void suKaydet();
    void sporKaydet();
    void sporHaftalikSifirlamaKontrol(); // Yeni ISO haftasına geçildiyse spor tablosunu sıfırlar

    void vkeHesapla();
    void suEkle();
    void suSifirla();
    void suHedefiAyarla();
    void sporDurumuGuncelle();
    void gecmisiGoster();
};

#endif // MAINWINDOW_H