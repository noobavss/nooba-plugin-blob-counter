#ifndef BLOBCOUNTERPLUGIN_H
#define BLOBCOUNTERPLUGIN_H

#include "blobcounterplugin_global.h"
#include "noobapluginapi.h"

#include <QObject>

#include <detectedevent.h>
#include <featurenode.h>

#include "package_bgs/StaticFrameDifferenceBGS.h"
#include "package_bgs/MixtureOfGaussianV2BGS.h"
#include "package_tracking/BlobTrackingNode.h"

#include "filewriternode.h"

#include "package_analysis/BlobCounting.h"

class BLOBCOUNTERPLUGIN_EXPORT BlobcounterPlugin: public NoobaPluginAPI
{
    Q_OBJECT
    Q_INTERFACES(NoobaPluginAPI)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "nooba.plugins.qt5.blobcounter-plugin" FILE "blobcounterPlugin.json")
#endif

public:
    BlobcounterPlugin();
    ~BlobcounterPlugin();

    bool procFrame(const cv::Mat &in, cv::Mat &out, ProcParams &params);
    bool init();
    bool release();
    PluginInfo getPluginInfo() const;

public slots:

    /**
     * These functions will be called when the parameters are changed by the
     * user.
     */
    void onStringParamChanged(const QString& varName, const QString& val);
    void onIntParamChanged(const QString &varName, int val);
    void onDoubleParamChanged(const QString &varName, double val);
    void onMultiValParamChanged(const QString &varName, const QString &val);
    void onCaptureEvent(QList<DetectedEvent> captured_event);

private:
    QString output_file;
    QString background_subtractor;
    cv::Mat img_blob;
    cv::Mat img_mask;
    StaticFrameDifferenceBGS staticBGS;
    MixtureOfGaussianV2BGS mogBGS;
    BlobCounting blobCounting;
    BlobTrackingNode blobTrackingNode;
    FileWriterNode blobEventWriterNode;

};

#endif // BLOBCOUNTERPLUGIN_H
