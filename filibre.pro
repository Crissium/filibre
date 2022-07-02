QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += optimize_full

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
windows {
	LIBS += -lpugixml -lmpv
}

unix {
	CONFIG += link_pkgconfig
	PKGCONFIG += pugixml mpv
}

SOURCES += \
	collectiondisplay.cpp \
    film.cpp \
    filmcollection.cpp \
	filmitem.cpp \
    main.cpp \
    mainwindow.cpp \
	mpvplayerdialog.cpp \
	mpvwidget.cpp

HEADERS += \
	collectiondisplay.h \
    film.h \
    filmcollection.h \
	filmitem.h \
    mainwindow.h \
	mpvplayerdialog.h \
	mpvwidget.h \
	qthelper.h

FORMS += \
    mainwindow.ui \
    mpvplayerdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
	COPYING

RESOURCES += \
	res.qrc
