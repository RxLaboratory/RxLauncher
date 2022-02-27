QT += core gui androidextras svg xml qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    androidmanager.cpp \
    androidutils.cpp \
    appbutton.cpp \
    appinfo.cpp \
    appsettingswidget.cpp \
    appslistwidget.cpp \
    duqf-widgets/progressslider.cpp \
    duqf-widgets/touchbutton.cpp \
    duqf-widgets/touchlistwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    settingswidget.cpp

HEADERS += \
    androidmanager.h \
    androidutils.h \
    appbutton.h \
    appinfo.h \
    appsettingswidget.h \
    appslistwidget.h \
    duqf-app/app-style.h \
    duqf-app/app-version.h \
    duqf-widgets/progressslider.h \
    duqf-widgets/touchbutton.h \
    duqf-widgets/touchlistwidget.h \
    mainwindow.h \
    settingswidget.h

FORMS += \
    appsettingswidget.ui \
    appslistwidget.ui \
    mainwindow.ui \
    settingswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_ABIS = armeabi-v7a

RESOURCES += \
    resources.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/src/org/rxlab/launcher/AppInfo.java \
    android/src/org/rxlab/launcher/AppManager.java

