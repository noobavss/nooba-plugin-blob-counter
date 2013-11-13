#include "blobcounterplugin.h"
#include <QtCore>

#include <QDebug>
// opencv includes
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>


BlobcounterPlugin::BlobcounterPlugin()
{

}

BlobcounterPlugin::~BlobcounterPlugin()
{

}

bool BlobcounterPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{
    Q_UNUSED(params)
    out = in;

    return true;
}

bool BlobcounterPlugin::init()
{
    connect(this, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    connect(&blobCounter, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    connect(this, SIGNAL(generateEvent(QList<DetectedEvent>)), &blobCounter, SLOT(captureEvent(QList<DetectedEvent>)));

    return true;
}

bool BlobcounterPlugin::release()
{
    return true;
}

PluginInfo BlobcounterPlugin::getPluginInfo() const
{
    PluginInfo pluginInfo(
        "Blob Counter Plugin",
        0,
        1,
        "This counts the blobs moving across a predefined line",
        "Chathuranga Hettiarachchi");
    return pluginInfo;
}

void BlobcounterPlugin::onCaptureEvent(QList<DetectedEvent> captured_event){


    foreach(DetectedEvent e, captured_event){
        debugMsg(QString(e.getIdentifier() + " " + e.getMessage() + " %1").arg(e.getConfidence()));
    }
    return;
}


void BlobcounterPlugin::inputData(const PluginPassData& data){

    QList<DetectedEvent> receivedEvents;
    foreach(QString str,data.strList()){
        //debugMsg("recv" + str);
        QList<QString> parameters = str.split(" ");
        receivedEvents.append(DetectedEvent(parameters.at(0),parameters.at(1),parameters.at(2).toFloat()));
    }
    emit generateEvent(receivedEvents);
    return;
}



// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(BlobcounterPlugin, BlobcounterPlugin);
#endif
