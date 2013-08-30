#ifndef TEMPLATEPLUGIN_H
#define TEMPLATEPLUGIN_H

#include "templateplugin_global.h"
#include "noobapluginapi.h"

#include <QObject>

class TEMPLATEPLUGIN_EXPORT TemplatePlugin: public NoobaPluginAPI
{
    Q_OBJECT
    Q_INTERFACES(NoobaPluginAPI)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "nooba.plugins.qt5.template-plugin" FILE "templatePlugin.json")
#endif

public:
    TemplatePlugin();
    ~TemplatePlugin();

    bool procFrame(const cv::Mat &in, cv::Mat &out, ProcParams &params);
    bool init();
    bool release();
    PluginInfo getPluginInfo() const;

private:

};

#endif // TEMPLATEPLUGIN_H
