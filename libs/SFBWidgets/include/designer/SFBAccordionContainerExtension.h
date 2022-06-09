#ifndef SFBACCORDIONCONTAINEREXTENSION_H
#define SFBACCORDIONCONTAINEREXTENSION_H

#include <QObject>
#include <QDesignerContainerExtension>

class SFBAccordion;


class SFBAccordionContainerExtension : public QObject, public QDesignerContainerExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerContainerExtension)

public:
    SFBAccordionContainerExtension(SFBAccordion *widget, QObject *parent = 0);

    void addWidget(QWidget *widget);
    void insertWidget(int index, QWidget *widget);
    QWidget *widget(int index) const;
    void remove(int index);
    int count() const;

    void setCurrentIndex(int index);
    int currentIndex() const;

private:
    SFBAccordion *mWidget;
};

#endif // SFBACCORDIONCONTAINEREXTENSION_H
