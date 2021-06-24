QT       += core gui sql  serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    sonda.cpp \
    tanque.cpp \
    select_fechas.cpp \
    butons.cpp \
    cale.cpp \
    frame.cpp

HEADERS += \
    dialog.h \
    mainwindow.h \
    sonda.h \
    tanque.h \
    select_fechas.h \
    butons.h \
    cale.h \
    frame.h

FORMS += \
    dialog.ui \
    mainwindow.ui \
    select_fechas.ui \
    butons.ui \
    cale.ui \
    frame.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Imagenes/Resourse.qrc \

DISTFILES += \
    Imagenes/Fecha y hora.png

LIBS +=-L/usr/local/lib -lwiringPi
