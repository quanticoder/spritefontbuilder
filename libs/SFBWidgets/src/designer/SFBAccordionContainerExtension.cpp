#include "designer/SFBAccordionContainerExtension.h"

#include "widgets/SFBAccordion.h"


SFBAccordionContainerExtension::SFBAccordionContainerExtension(SFBAccordion *widget, QObject *parent) :
    QObject(parent)
{
    mWidget = widget;
}


void SFBAccordionContainerExtension::addWidget(QWidget *widget)
{
    mWidget->addPage(widget);
}


void SFBAccordionContainerExtension::insertWidget(int index, QWidget *widget)
{
    mWidget->insertPage(index, widget);
}


QWidget* SFBAccordionContainerExtension::widget(int index) const
{
    return mWidget->widget(index);
}


void SFBAccordionContainerExtension::remove(int index)
{
    mWidget->removePage(index);
}


int SFBAccordionContainerExtension::count() const
{
    return mWidget->pageCount();
}


void SFBAccordionContainerExtension::setCurrentIndex(int index)
{
    mWidget->setCurrentIndex(index);
}


int SFBAccordionContainerExtension::currentIndex() const
{
    return mWidget->currentIndex();
}

