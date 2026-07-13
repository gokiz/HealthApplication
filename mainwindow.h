#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSqlDatabase>

// Sınıf adı projenle uyumlu olarak MainWindow yapıldı
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    QLineEdit *kiloGiris;
    QLineEdit *boyGiris;
    QLabel *suDurumEtiketi;
    int toplamSu;

    QCheckBox *gunKutulari[7];
    QLabel *bilgiCubugu;

    QSqlDatabase db;
    void veritabaniBaslat();
    void verileriYukle();
    void suKaydet();
    void sporKaydet();


    void vkeHesapla();
    void suEkle();
    void suSifirla();
    void sporDurumuGuncelle();
};

#endif // MAINWINDOW_H