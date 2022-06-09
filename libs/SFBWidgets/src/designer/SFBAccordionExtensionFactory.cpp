#include "designer/SFBAccordionExtensionFactory.h"

#include "designer/SFBAccordionContainerExtension.h"
#include "widgets/SFBAccordion.h"



SFBAccordionExtensionFactory::SFBAccordionExtensionFactory(QExtensionManager *parent) :
    QExtensionFactory(parent)
{

}

QObject *SFBAccordionExtensionFactory::createExtension(QObject *object, const QString &iid, QObject *parent) const
{
    SFBAccordion *widget = qobject_cast<SFBAccordion*>(object);

    if (widget && (iid == Q_TYPEID(QDesignerContainerExtension))) {
        return new SFBAccordionContainerExtension(widget, parent);
    } else {
        return 0;
    }
}
