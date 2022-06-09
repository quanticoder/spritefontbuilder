#ifndef SFBACCORDIONEXTENSIONFACTORY_H
#define SFBACCORDIONEXTENSIONFACTORY_H

#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>

class SFBAccordionExtensionFactory : public QExtensionFactory
{

public:
    SFBAccordionExtensionFactory(QExtensionManager *parent = 0);

protected:
    QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const;

};

#endif // SFBACCORDIONEXTENSIONFACTORY_H
