cmake_minimum_required(VERSION 3.16)
project(HealthApplication VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6 modülleri:
#  - Widgets: arayüz
#  - Sql: veritabanı (SQLite)
#  - Charts: BMI geçmiş grafiği (Qt Maintenance Tool'da "Qt Charts" bileşeni
#            kurulu olmalı; Linux'ta genelde qt6-charts-dev paketiyle gelir)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql Charts)

# NOT: Proje saf C++ (Qt Designer .ui dosyası olmadan) yazıldığı için
# mainWindow.ui referansı yok - böyle bir dosya olmadığından build hatası verirdi.
add_executable(HealthApplication
    main.cpp
    mainwindow.h
    mainwindow.cpp
    historydialog.h
    historydialog.cpp
)

target_link_libraries(HealthApplication PRIVATE Qt6::Core Qt6::Widgets Qt6::Sql Qt6::Charts)