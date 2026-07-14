#include "historydialog.h"

#include <QTabWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QSqlTableModel>
#include <QHeaderView>
#include <QSqlQuery>
#include <QLabel>
#include <QPainter>
#include <QAbstractItemView>

#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QChart>



historydialog::historydialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("History & Statistics");
    resize(600, 500);

    QTabWidget *sekmeler = new QTabWidget(this);

    suGecmisiSekmesiOlustur(sekmeler);
    bmiGecmisiSekmesiOlustur(sekmeler);
    bmiGrafigiSekmesiOlustur(sekmeler);

    QVBoxLayout *anaLayout = new QVBoxLayout(this);
    anaLayout->addWidget(sekmeler);
}

void historydialog::suGecmisiSekmesiOlustur(QTabWidget *sekmeler) {
    QWidget *suSekmesi = new QWidget();
    QVBoxLayout *suLayout = new QVBoxLayout(suSekmesi);

    QTableView *suTablo = new QTableView(suSekmesi);
    QSqlTableModel *suModel = new QSqlTableModel(suTablo);
    suModel->setTable("water_tracker");
    suModel->setSort(0, Qt::DescendingOrder); // water_date sütununa göre azalan sıralama (en yeni üstte)
    suModel->select();
    suModel->setHeaderData(0, Qt::Horizontal, "Date");
    suModel->setHeaderData(1, Qt::Horizontal, "Amount (ml)");

    suTablo->setModel(suModel);
    suTablo->horizontalHeader()->setStretchLastSection(true);
    suTablo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    suTablo->setSelectionBehavior(QAbstractItemView::SelectRows);

    suLayout->addWidget(suTablo);
    sekmeler->addTab(suSekmesi, "Water History");
}

void historydialog ::bmiGecmisiSekmesiOlustur(QTabWidget *sekmeler) {
    QWidget *bmiSekmesi = new QWidget();
    QVBoxLayout *bmiLayout = new QVBoxLayout(bmiSekmesi);

    QTableView *bmiTablo = new QTableView(bmiSekmesi);
    QSqlTableModel *bmiModel = new QSqlTableModel(bmiTablo);
    bmiModel->setTable("bmicalculations");
    bmiModel->setSort(0, Qt::DescendingOrder); // id sütununa göre azalan sıralama (en yeni üstte)
    bmiModel->select();
    bmiModel->setHeaderData(0, Qt::Horizontal, "ID");
    bmiModel->setHeaderData(1, Qt::Horizontal, "Weight (kg)");
    bmiModel->setHeaderData(2, Qt::Horizontal, "Height (cm)");
    bmiModel->setHeaderData(3, Qt::Horizontal, "BMI");
    bmiModel->setHeaderData(4, Qt::Horizontal, "Status");

    bmiTablo->setModel(bmiModel);
    bmiTablo->horizontalHeader()->setStretchLastSection(true);
    bmiTablo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bmiTablo->setSelectionBehavior(QAbstractItemView::SelectRows);

    bmiLayout->addWidget(bmiTablo);
    sekmeler->addTab(bmiSekmesi, "BMI History");
}

void historydialog::bmiGrafigiSekmesiOlustur(QTabWidget *sekmeler) {
    QWidget *grafikSekmesi = new QWidget();
    QVBoxLayout *grafikLayout = new QVBoxLayout(grafikSekmesi);

    QSqlQuery sorgu("SELECT id, bmi FROM bmicalculations ORDER BY id ASC");

    QLineSeries *seri = new QLineSeries();
    seri->setName("BMI");

    int sayac = 1;
    double minBmi = 100.0, maxBmi = 0.0;
    bool veriVar = false;

    while (sorgu.next()) {
        double bmiDeger = sorgu.value(1).toDouble();
        seri->append(sayac, bmiDeger);
        minBmi = qMin(minBmi, bmiDeger);
        maxBmi = qMax(maxBmi, bmiDeger);
        sayac++;
        veriVar = true;
    }

    if (!veriVar) {
        QLabel *bosEtiket = new QLabel("No BMI records yet. Calculate your BMI first.", grafikSekmesi);
        bosEtiket->setAlignment(Qt::AlignCenter);
        grafikLayout->addWidget(bosEtiket);
        delete seri;
    } else {
        QChart *grafik = new QChart();
        grafik->addSeries(seri);
        grafik->setTitle("BMI History Over Time");
        grafik->legend()->hide();

        QValueAxis *xEksen = new QValueAxis();
        xEksen->setTitleText("Measurement #");
        xEksen->setLabelFormat("%d");
        xEksen->setTickCount(qMin(sayac, 10));
        grafik->addAxis(xEksen, Qt::AlignBottom);
        seri->attachAxis(xEksen);

        QValueAxis *yEksen = new QValueAxis();
        yEksen->setTitleText("BMI");
        yEksen->setRange(qMax(0.0, minBmi - 3), maxBmi + 3);
        grafik->addAxis(yEksen, Qt::AlignLeft);
        seri->attachAxis(yEksen);

        QChartView *grafikGorunumu = new QChartView(grafik, grafikSekmesi);
        grafikGorunumu->setRenderHint(QPainter::Antialiasing);
        grafikLayout->addWidget(grafikGorunumu);
    }

    sekmeler->addTab(grafikSekmesi, "BMI Chart");
}