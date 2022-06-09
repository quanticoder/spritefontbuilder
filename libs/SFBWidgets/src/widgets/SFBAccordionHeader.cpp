#include "widgets/SFBAccordionHeader.h"
#include "widgets/SFBAccordion.h"
#include "widgets/SFBAccordionPage.h"


SFBAccordionHeader::SFBAccordionHeader(SFBAccordionPage * page) :
    QWidget(page->parent()),
    mPage(page),
    mAccessory(NULL),
    mSelected(false),
    mIndexInPage(-1)
{
    setBackgroundRole(QPalette::Window);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    setFocusPolicy(Qt::NoFocus);

    mOpenIcon = mPage->parent()->pageOpenIcon();
    mClosedIcon = mPage->parent()->pageClosedIcon();

    mLayout = new QHBoxLayout(this);
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0,0,0,0);

    mArrowButton = new QPushButton(this);
    mArrowButton->setFlat(true);
    mArrowButton->setMaximumSize(24, 200);
    mArrowButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    mArrowButton->setIcon(mClosedIcon);
    mLayout->addWidget(mArrowButton);

    mTitleButton = new QPushButton(this);
    mTitleButton->setFlat(true);
    mTitleButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mTitleButton->setStyleSheet("text-align: left");
    mTitleButton->setFont(mPage->parent()->font());
    mLayout->addWidget(mTitleButton);

    connect(mTitleButton, SIGNAL(clicked()), this, SLOT(onTitleButtonClicked()));
    connect(mArrowButton, SIGNAL(clicked()), this, SLOT(onArrowButtonClicked()));

    setLayout(mLayout);
}



void SFBAccordionHeader::setOpen(bool b)
{
    mOpen = b;
    mArrowButton->setIcon(b ? mOpenIcon : mClosedIcon);
    update();
}


void SFBAccordionHeader::setSelected(bool b)
{
    mSelected = b;

    if ( mSelected )
    {
        mTitleButton->setStyleSheet("text-align: left; color: white;");
    }
    else
    {
        mTitleButton->setStyleSheet("text-align: left; color: black");
    }

    update();
}


QSize SFBAccordionHeader::sizeHint() const
{
    return QSize(80, 24);
}


QSize SFBAccordionHeader::minimumSizeHint() const
{
    return QSize(80, 24);
}


//
// Accessory Widget
//


void SFBAccordionHeader::setAccessory(QWidget * widget)
{
    if ( mAccessory != NULL )
    {
        mLayout->removeWidget(mAccessory);
        delete mAccessory;
        mAccessory = NULL;
    }

    if ( widget != NULL )
    {
        mAccessory = widget;
        mAccessory->setParent(this);
        mLayout->addWidget(mAccessory);
    }
}


QWidget * SFBAccordionHeader::accessory()
{
    return mAccessory;
}


QWidget * SFBAccordionHeader::accessory() const
{
    return mAccessory;
}



void SFBAccordionHeader::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);

    QColor c1 = mSelected ? QColor(110, 170, 230) : QColor(230, 230, 230);
    QColor c2 = mSelected ? QColor( 55, 100, 180) : QColor(180, 180, 180);
    QColor lineColor = mSelected ? QColor(45, 90, 170) : QColor(170, 170, 170);

    QLinearGradient gradient(rect().topLeft(), rect().bottomLeft());
    gradient.setColorAt(0, c1);
    gradient.setColorAt(1, c2);
    painter.fillRect(rect(), gradient);

    painter.setPen(lineColor);
    painter.drawLine(rect().topLeft(), rect().topRight());
    painter.drawLine(rect().bottomLeft(), rect().bottomRight());
}


//
// Signals
//

void SFBAccordionHeader::onTitleButtonClicked()
{
    mPage->makeCurrent();
}


void SFBAccordionHeader::onArrowButtonClicked()
{
    mPage->toggle();
}


