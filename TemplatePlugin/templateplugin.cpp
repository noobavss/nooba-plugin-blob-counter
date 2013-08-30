#include "templateplugin.h"
#include <QtCore>
#include <opencv2/core/core.hpp>

TemplatePlugin::TemplatePlugin()
{

}

TemplatePlugin::~TemplatePlugin()
{

}

bool TemplatePlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{

    return true;
}

bool TemplatePlugin::init()
{
    return true;
}

bool TemplatePlugin::release()
{
    return true;
}

PluginInfo TemplatePlugin::getPluginInfo() const
{
    PluginInfo pluginInfo(
        "Template Plugin",
        0,
        1,
        "Plugin Description goes here",
        "Plugin Creator");
    return pluginInfo;
}


// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(TemplatePlugin, TemplatePlugin);
#endif
