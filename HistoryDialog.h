#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>

// Su geçmişi, BMI geçmişi ve BMI grafiğini sekmeler halinde gösteren pencere
class historydialog : public QDialog {
    Q_OBJECT

public:
    explicit historydialog(QWidget *parent = nullptr);

private:
    void suGecmisiSekmesiOlustur(class QTabWidget *sekmeler);
    void bmiGecmisiSekmesiOlustur(class QTabWidget *sekmeler);
    void bmiGrafigiSekmesiOlustur(class QTabWidget *sekmeler);
};

#endif // HISTORYDIALOG_H