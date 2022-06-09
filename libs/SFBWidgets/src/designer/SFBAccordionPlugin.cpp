#include "designer/SFBAccordionPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QDesignerFormWindowInterface>
#include <QtDesigner/QDesignerContainerExtension>
#include <QtDesigner/QDesignerPropertySheetExtension>

#include "widgets/SFBAccordion.h"

#include "designer/SFBAccordionExtensionFactory.h"
#include "designer/SFBAccordionContainerExtension.h"

SFBAccordionPlugin::SFBAccordionPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}


//
// Plugin Methods
//

void SFBAccordionPlugin::initialize(QDesignerFormEditorInterface * formEditor)
{
    if (m_initialized)
        return;

    QExtensionManager *manager = formEditor->extensionManager();
    QExtensionFactory *factory = new SFBAccordionExtensionFactory(manager);

    Q_ASSERT(manager != 0);
    manager->registerExtensions(factory, Q_TYPEID(QDesignerContainerExtension));

    m_initialized = true;
}

bool SFBAccordionPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *SFBAccordionPlugin::createWidget(QWidget *parent)
{
    SFBAccordion * widget = new SFBAccordion(parent);
    return widget;
}

QString SFBAccordionPlugin::name() const
{
    return QLatin1String("SFBAccordion");
}

QString SFBAccordionPlugin::group() const
{
    return QLatin1String("SFB Widgets");
}

QIcon SFBAccordionPlugin::icon() const
{
    return QIcon();
}

QString SFBAccordionPlugin::toolTip() const
{
    return QLatin1String("An extended QToolBox container with multiple sections.");
}

QString SFBAccordionPlugin::whatsThis() const
{
    return QLatin1String("An accordion container based around the QToolBox widget but extending it to allow for custom widgets to be placed in the page header and for other advanced features.");
}

bool SFBAccordionPlugin::isContainer() const
{
    return true;
}

QString SFBAccordionPlugin::domXml() const
{
    return QLatin1String("<ui language=\"c++\"> displayname=\"SFB Accordion\">\n"
                         " <widget class=\"SFBAccordion\" name=\"sfbAccordion\"/>\n"
                         " <customwidgets>\n"
                         "  <customwidget>\n"
                         "   <class>SFBAccordion</class>\n"
                         "   <addpagemethod>addPage</addpagemethod>\n"
                         "  </customwidget>\n"
                         " </customwidgets>\n"
                         "</ui>");
}

QString SFBAccordionPlugin::includeFile() const
{
    return QLatin1String("widgets/SFBAccordion.h");
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(SFBWidgets, SFBAccordionPlugin)
#endif // QT_VERSION < 0x050000
