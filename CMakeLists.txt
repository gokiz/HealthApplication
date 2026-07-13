cmake_minimum_required(VERSION 3.16)
project(HealthApplication VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6'nın grafik arayüz (Widgets) modülünü dahil ediyoruz
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)

# Derlenecek dosyalarımızı listeliyoruz
add_executable(HealthApplication
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainWindow.ui
)

# Kütüphaneleri projeye bağlıyoruz
target_link_libraries(HealthApplication PRIVATE Qt6::Core Qt6::Widgets Qt6::Sql)