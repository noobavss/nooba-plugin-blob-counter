#include "blobcounterplugin.h"
#include <QtCore>

#include <QImage>
#include <QDebug>
//#include <QPoint>
//#include <QPointF>
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
    //connect(this, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    connect(this, SIGNAL(generateEvent(QList<DetectedEvent>)), &lineCrossDetector, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&lineCrossDetector, SIGNAL(generateEvent(QList<DetectedEvent>)), &lineCrossCounter, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&lineCrossCounter, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    connect(&lineCrossCounter, SIGNAL(generateEvent(QList<DetectedEvent>)), &crossCountAnomalyNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&crossCountAnomalyNode, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));

    createFrameViewer("CountingLine");

    createIntParam("point1-x",500,1024,0);
    createIntParam("point1-y",0,768,0);
    createIntParam("point2-x",500,1024,0);
    createIntParam("point2-y",700,768,0);

//    createPointParam("Point1",QPoint(500,0));
//    createPointParam("Point2",QPoint(500,700));

    createIntParam("time_window(frames)",300,10000,0);
    createIntParam("min_of_count_range",0,100,0);
    createIntParam("max_of_count_range",2,100,0);

    crossCountAnomalyNode.setTime_window(300);
    crossCountAnomalyNode.setMax_of_count_range(2);
    crossCountAnomalyNode.setMin_of_count_range(0);

    QStringList rangeoptions;
    rangeoptions << "Out Range"
                 << "In Range";
    createMultiValParamRequest("Anomaly Region",rangeoptions);

    crossCountAnomalyNode.setAnomal_range(false);
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

void BlobcounterPlugin::onIntParamChanged(const QString &varName, int val){
    if(varName == "point1-x"){
        lineCrossDetector.setPoint1_x(val);
        debugMsg("point1-x set to"  + QString("%1").arg(val));
    }
    else if(varName == "point1-y"){
        lineCrossDetector.setPoint1_y(val);
        debugMsg("point1-y set to "  + QString("%1").arg(val));
    }
    else if(varName == "point2-x"){
        lineCrossDetector.setPoint2_x(val);
        debugMsg("point2-x set to "  + QString("%1").arg(val));
    }
    else if(varName == "point2-y"){
        lineCrossDetector.setPoint2_y(val);
        debugMsg("point2-y set to "  + QString("%1").arg(val));
    }
    else if(varName == "time_window(frames)"){
        crossCountAnomalyNode.setTime_window(val);
        debugMsg("time_window(frames) set to "  + QString("%1").arg(val));
    }
    else if(varName == "min_of_count_range"){
        crossCountAnomalyNode.setMin_of_count_range(val);
        debugMsg("min_of_count_range set to "  + QString("%1").arg(val));
    }
    else if(varName == "max_of_count_range"){
        crossCountAnomalyNode.setMax_of_count_range(val);
        debugMsg("max_of_count_range set to "  + QString("%1").arg(val));
    }
}

//void BlobcounterPlugin::onPointParamChanged(const QString& varName, const QPointF& val){
//    QPoint p;
//    p = val.toPoint();
//    if(varName == "Point1"){

//        lineCrossDetector.setPoint1_x(p.x());
//        lineCrossDetector.setPoint1_y(p.y());
//        debugMsg(QString("Point1 set to (%1,%2)").arg(p.x()).arg(p.y()));
//    }
//    else if(varName == "Point2"){
//        lineCrossDetector.setPoint2_x(p.x());
//        lineCrossDetector.setPoint2_y(p.y());
//        debugMsg(QString("Point2 set to (%1,%2)").arg(p.x()).arg(p.y()));
//    }
//    return;
//}

void BlobcounterPlugin::onMultiValParamChanged(const QString &varName, const QString &val){
    if(varName == "Anomaly Region"){
        if(val == "In Range"){
            crossCountAnomalyNode.setAnomal_range(true);
            debugMsg("Anomaly Region set to In Range");
        }
        else{
            crossCountAnomalyNode.setAnomal_range(false);
            debugMsg("Anomaly Region set to Out Range");
        }
    }
}

void BlobcounterPlugin::inputData(const QStringList &strList, QList<QImage> imageList){

    QList<DetectedEvent> receivedEvents;
    QStringList stringList = strList;
    foreach(QString str,stringList){
        //debugMsg("recv" + str);
        QList<QString> parameters = str.split(" ");
        receivedEvents.append(DetectedEvent(parameters.at(0),parameters.at(1),parameters.at(2).toFloat()));
    }

    QImage temp = imageList.at(0);

    cv::Mat lineviewer(temp.height(),temp.width(),CV_8UC3,(uchar*)temp.bits(),temp.bytesPerLine());
    cv::line(lineviewer,cv::Point(lineCrossDetector.getPoint1().x(),lineCrossDetector.getPoint1().y()),
             cv::Point(lineCrossDetector.getPoint2().x(),lineCrossDetector.getPoint2().y()),
             cv::Scalar(0,0,255),3);


    updateFrameViewer("CountingLine",convertToQImage(lineviewer));

    emit generateEvent(receivedEvents);
    return;
}

QImage BlobcounterPlugin::convertToQImage(cv::Mat &cvImg)
{
    if (cvImg.channels()== 1){
        QImage img((uchar*)cvImg.data, cvImg.cols, cvImg.rows, cvImg.step1(), QImage::Format_Indexed8);
        return img;
    }
    else{
        QImage img((uchar*)cvImg.data, cvImg.cols, cvImg.rows, cvImg.step1(), QImage::Format_RGB888);
        return img;
    }
}
// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(BlobcounterPlugin, BlobcounterPlugin);
#endif
