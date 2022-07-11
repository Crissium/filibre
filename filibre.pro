QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += optimize_full

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
windows {
	INCLUDEPATH += 3rdparty/mpv/include
	INCLUDEPATH += 3rdparty/curl/include
	INCLUDEPATH += 3rdparty/nlohmann/include
	LIBS += -L"3rdparty/mpv/lib" -lmpv
	LIBS += -L"3rdparty/curl/lib" -lcurl
	LIBS += -lpugixml
}

unix {
	CONFIG += link_pkgconfig
	PKGCONFIG += pugixml mpv libcurl nlohmann_json
}

include($$PWD/src/src.pri)
include($$PWD/resources/resources.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
	COPYING \
	README.md \
	icon.ico

RC_ICONS += icon.ico
