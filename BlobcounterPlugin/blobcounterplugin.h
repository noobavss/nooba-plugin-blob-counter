#ifndef BLOBCOUNTERPLUGIN_H
#define BLOBCOUNTERPLUGIN_H

#include "blobcounterplugin_global.h"
#include "noobapluginapi.h"

#include <QImage>
#include <QObject>

#include <detectedevent.h>
#include <featurenode.h>
#include <linecrossingnode.h>
#include <linecrosscountnode.h>
#include <countanomalynode.h>

//#include "filewriternode.h"

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
//    void onStringParamChanged(const QString& varName, const QString& val);
    void onIntParamChanged(const QString &varName, int val);
//    void onDoubleParamChanged(const QString &varName, double val);
    void onMultiValParamChanged(const QString &varName, const QString &val);
//    void onPointParamChanged(const QString& varName, const QPointF& val);

    void onLineParamUpdated(const QString &varName, const QString frameViewerTitle, QLine line);

    void inputData(const QStringList &strList, QList<QImage> imageList);

    void onCaptureEvent(QList<DetectedEvent> captured_event);

signals:
    void generateEvent(QList<DetectedEvent> generated_event);

private:
    LineCrossingNode lineCrossDetector;
    LineCrossCountNode lineCrossCounter;
    CountAnomalyNode crossCountAnomalyNode;
    QImage convertToQImage(cv::Mat &cvImg);
};

#endif // BLOBCOUNTERPLUGIN_H
