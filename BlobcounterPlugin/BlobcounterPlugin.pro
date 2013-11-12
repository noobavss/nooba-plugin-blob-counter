######################################################################
# Automatically generated by qmake (2.01a) Fri Aug 23 13:22:33 2013
######################################################################
CONFIG(debug, debug|release):    message(Debug build!)
CONFIG(release, debug|release):    message(Release build!)

QT -= gui

TARGET = blobcounterPlugin

CONFIG(debug, debug|release): TARGET = BlobcounterPlugin_Debug
CONFIG(release, debug|release): TARGET = BlobcounterPlugin_Release

TEMPLATE = lib
CONFIG += dll

DEFINES += BLOBCOUNTERPLUGIN_LIB

# Input
HEADERS += blobcounterplugin.h blobcounterplugin_global.h \
    package_tracking/BlobTrackingNode.h \
    package_bgs/StaticFrameDifferenceBGS.h \
    package_bgs/MixtureOfGaussianV2BGS.h \
    package_analysis/BlobCounting.h \
    filewriternode.h \
    package_tracking/cvblob/cvblob.h \
    detectedevent.h \
    featurenode.h
SOURCES += blobcounterplugin.cpp \
    package_tracking/BlobTrackingNode.cpp \
    package_bgs/StaticFrameDifferenceBGS.cpp \
    package_bgs/MixtureOfGaussianV2BGS.cpp \
    package_analysis/BlobCounting.cpp \
    filewriternode.cpp \
    package_tracking/cvblob/cvtrack.cpp \
    package_tracking/cvblob/cvlabel.cpp \
    package_tracking/cvblob/cvcontour.cpp \
    package_tracking/cvblob/cvcolor.cpp \
    package_tracking/cvblob/cvblob.cpp \
    package_tracking/cvblob/cvaux.cpp \
    detecedevent.cpp \
    featurenode.cpp

CONFIG(debug, debug|release): DESTDIR = $$PWD/../../../NoobaVSS_build/NoobaFE/Debug/plugins/
CONFIG(release, debug|release): DESTDIR = $$PWD/../../../NoobaVSS_build/NoobaFE/Release/plugins/

# include opencv
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    # remove sym links to the shared lib from the DESTDIR
    QMAKE_POST_LINK = find $$DESTDIR -maxdepth 1 -type l -exec rm -f {} \;
}

CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../NoobaVSS_build/NoobaPluginAPI/Debug/ -lNoobaPluginAPId
CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../NoobaVSS_build/NoobaPluginAPI/Release/ -lNoobaPluginAPI

INCLUDEPATH += $$PWD/../../../NoobaVSS/NoobaPluginAPI/
DEPENDPATH += $$PWD/../../../NoobaVSS/NoobaPluginAPI/
