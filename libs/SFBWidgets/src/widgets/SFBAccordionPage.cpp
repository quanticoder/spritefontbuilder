#include "widgets/SFBAccordionPage.h"
#include "widgets/SFBAccordion.h"

SFBAccordionPage::SFBAccordionPage(SFBAccordion *parent, QWidget *widget) :
    mParent(parent),
    mHeader(NULL),
    mWidget(widget),
    mIsOpen(true)
{
    setObjectName("sfb_accordion_page");

    mLayout = new QVBoxLayout(this);
    mLayout->setMargin(0);
    mLayout->setSpacing(0);
    setLayout(mLayout);

    mHeader = new SFBAccordionHeader(this);
    mHeader->setObjectName(QLatin1String("sfb_accordion_header"));
    mHeader->show();
    mLayout->addWidget(mHeader);

    mWidget->setParent(this);
    mWidget->hide();
    mLayout->addWidget(mWidget);

    if ( mWidget != NULL ) {
        setText(mWidget->windowTitle());
    }
}


SFBAccordion * SFBAccordionPage::parent()
{
    return mParent;
}


void SFBAccordionPage::setSelected(bool value)
{
    mHeader->setSelected(value);

    if ( value == true ) {
        open();
    }
}


QString SFBAccordionPage::text() const
{
    return (mHeader && mHeader->button()) ? mHeader->button()->text() : QString();
}


void SFBAccordionPage::setText(const QString &text)
{
    if (mHeader && mHeader->button()) {
        mHeader->button()->setText(text);
    }

    if (mWidget) {
        mWidget->setWindowTitle(text);
    }
}


QIcon SFBAccordionPage::icon() const
{
    return (mHeader && mHeader->button()) ? mHeader->button()->icon() : QIcon();
}


void SFBAccordionPage::setIcon(const QIcon &icon)
{
    if (mHeader && mHeader->button()) {
        mHeader->button()->setIcon(icon);
    }
}


QString SFBAccordionPage::toolTip() const
{
    return (mHeader) ? mHeader->toolTip() : QString();
}


void SFBAccordionPage::setToolTip(const QString &tip)
{
    if (mHeader) {
        mHeader->setToolTip(tip);
    }
}


void SFBAccordionPage::setAccessory(QWidget * widget)
{
    mHeader->setAccessory(widget);
}


QWidget * SFBAccordionPage::accessory()
{
    return mHeader->accessory();
}


QWidget * SFBAccordionPage::accessory() const
{
    return mHeader->accessory();
}


SFBAccordionHeader * SFBAccordionPage::header()
{
    return mHeader;
}


SFBAccordionHeader * SFBAccordionPage::header() const
{
    return mHeader;
}



QWidget * SFBAccordionPage::widget()
{
    return mWidget;
}


QWidget * SFBAccordionPage::widget() const
{
    return mWidget;
}


void SFBAccordionPage::open()
{
    if ( mIsOpen ) {
        return;
    }

    mWidget->show();
    mHeader->setOpen(true);
    mIsOpen = true;
}


void SFBAccordionPage::close()
{
    if ( !mIsOpen ) {
        return;
    }

    mWidget->hide();
    mHeader->setOpen(false);
    mIsOpen = false;
}


void SFBAccordionPage::toggle()
{
    if ( mIsOpen ) {
        close();
    } else {
        open();
    }
}


void SFBAccordionPage::makeCurrent()
{
    mParent->setCurrentIndex(mParent->indexOf(mWidget));
}
