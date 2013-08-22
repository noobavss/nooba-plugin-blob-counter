#ifndef TEMPLATEPLUGIN_H
#define TEMPLATEPLUGIN_H

#include "templateplugin_global.h"

#include <noobapluginapi.h>
#include <opencv2/core/core.hpp>

class TEMPLATEPLUGIN_EXPORT TemplatePlugin: public NoobaPluginAPI
{
    Q_OBJECT
    Q_INTERFACES(NoobaPluginAPI)

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
