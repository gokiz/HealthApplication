#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

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
    QLabel *bilgiCubugu;
    int toplamSu;

    void vkeHesapla();
    void suEkle();
};

#endif // MAINWINDOW_H