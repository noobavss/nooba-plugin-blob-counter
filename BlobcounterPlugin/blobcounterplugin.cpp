#include "blobcounterplugin.h"
#include <QtCore>

// opencv includes
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <QDebug>


BlobcounterPlugin::BlobcounterPlugin()
{

}

BlobcounterPlugin::~BlobcounterPlugin()
{
    blobEventWriterNode.closeFile();
}

bool BlobcounterPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{
    Q_UNUSED(params)
    img_mask = in.clone();

    // bgs->process(...) method internally shows the foreground mask image

//    if(background_subtractor == "StaticFrameDifference"){
//        staticBGS.process(in, img_mask);
//    }else if(background_subtractor == "MixtureOfGaussianV2"){
//        mogBGS.process(in, img_mask);
//    }else{
       // mogBGS.process(in, img_mask);
        staticBGS.process(in, img_mask);
//    }
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2,2));
    cv::dilate(img_mask,img_mask, element,cv::Point(-1,-1),1);
    cv::erode(img_mask,img_mask, element,cv::Point(-1,-1),2);

    //cv::imshow("BGS",img_mask);
    blobTrackingNode.process(in, img_mask, img_blob);

    cv::cvtColor(img_blob, out, CV_BGR2GRAY);

    //cv::imshow("mask", img_mask);
    //cv::imshow("blob", img_blob);
    // Perform blob counting
    blobCounting.setInput(img_blob);
    blobCounting.setTracks(blobTrackingNode.getTracks());
    blobCounting.process();

    return true;
}

bool BlobcounterPlugin::init()
{
    QDateTime timestamp = QDateTime::currentDateTime();
    QStringList enable_disable_list;
    QStringList bgsList;

    blobCounting.setFrameIndex(0);
    enable_disable_list.append("Enable");
    enable_disable_list.append("Disable");

    bgsList.append("StaticFrameDifference");
    bgsList.append("MixtureOfGaussianV2");

    QDir dir(QDir::home());
    if(!dir.exists("NoobaVSS")){
        dir.mkdir("NoobaVSS");
    }
    dir.cd("NoobaVSS");
    if(!dir.exists("data")){
        dir.mkdir("data");
    }
    dir.cd("data");
    if(!dir.exists("text")){
        dir.mkdir("text");
    }
    dir.cd("text");

    output_file = dir.absoluteFilePath(timestamp.currentDateTime().toString("yyyy-MM-dd-hhmm") + "-blobcounts.txt").toLocal8Bit();
    createStringParam("output_file",output_file,false);
    createIntParam("inactive_time",10,1000,0);
    createDoubleParam("distance_threshold",100.0,1000.0,1.0);
    createIntParam("min_area",100,100000,0);
    createIntParam("max_area",20000,100000,0);

    //createMultiValParam("enable_file_output",enable_disable_list);
    createMultiValParam("enable_file_output",enable_disable_list);

    createMultiValParam("Background Subtractor",bgsList);

    blobEventWriterNode.openFile(output_file);
    blobTrackingNode.setInactiveTimeThreshold(10);
    blobTrackingNode.setDistanceThreshold(100.0);

    background_subtractor = "StaticFrameDifference";
    debugMsg("TrackingPlugin initialized");
    return true;
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
        "This counts the blobs moving acroaa a predefined line",
        "Chathuranga Hettiarachchi");
    return pluginInfo;
}


void BlobcounterPlugin:: onStringParamChanged(const QString& varName, const QString& val){
    if(varName == "output_file"){
        output_file = val;
        blobEventWriterNode.openFile(output_file);
        debugMsg("output_file set to "  + val);
    }
}

void BlobcounterPlugin::onIntParamChanged(const QString &varName, int val){
    if(varName == "inactive_time"){

        blobTrackingNode.setInactiveTimeThreshold(val);
        debugMsg("inactive_time set to "  + QString("%1").arg(val));
    }
    else if(varName == "min_area"){

        blobTrackingNode.setMinArea(val);
        debugMsg("min_area set to "  + QString("%1").arg(val));
    }
    else if(varName == "max_area"){

        blobTrackingNode.setMaxArea(val);
        debugMsg("max_area set to "  + QString("%1").arg(val));
    }

}

void BlobcounterPlugin::onDoubleParamChanged(const QString &varName, double val){

    if(varName == "distance_threshold"){
            blobTrackingNode.setDistanceThreshold(val);
            debugMsg("distance_threshold set to "  + QString("%1").arg(val));
    }
}

void BlobcounterPlugin::onMultiValParamChanged(const QString &varName, const QString &val){
    if(varName == "enable_file_output"){

            debugMsg("enable_file_output set to " + val);
    }
    else if(varName == "show_blob_mask"){
        if(val == "Enable"){
        blobTrackingNode.toggleBlobMaskOutput(true);
        }
        else{
            blobTrackingNode.toggleBlobMaskOutput(false);
        }
        debugMsg("show_blob_mask set to " + val);
    }
    else if(varName == "Background Subtractor"){
        if(val == "StaticFrameDifference"){
            background_subtractor = "StaticFrameDifference";
        }
        else if(val == "MixtureOfGaussianV2"){
            background_subtractor = "MixtureOfGaussianV2";
        }
        else{
            background_subtractor = "StaticFrameDifference";
        }
        debugMsg("Background Subtractor set to " + val);
    }
}

void BlobcounterPlugin::onCaptureEvent(QList<DetectedEvent> captured_event){

    PluginPassData eventData;
    foreach(DetectedEvent e, captured_event){
        //debugMsg(QString(e.getIdentifier() + " " + e.getMessage() + " %1").arg(e.getConfidence()));
        eventData.appendStrList(QString(e.getIdentifier() + " " + e.getMessage() + " %1").arg(e.getConfidence()));
    }
    //out_stream << frameIndex << "," << (*track).first << ","<< blob->centroid.x << "," << blob->centroid.y << "|";

    outputData(eventData);
    //outputDataRequest(eventData);

}


// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(BlobcounterPlugin, BlobcounterPlugin);
#endif
