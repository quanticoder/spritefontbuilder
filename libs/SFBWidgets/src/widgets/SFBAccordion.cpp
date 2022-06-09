#include "widgets/SFBAccordion.h"

#include <QScrollArea>
#include <QEvent>
#include <QMessageBox>
#include <QDebug>

#include "widgets/SFBAccordionPage.h"


/**
 * @brief Constructs a new accordion object.
 * @param parent Parent widget
 * @param f Rendering flags
 */
SFBAccordion::SFBAccordion(QWidget *parent, Qt::WindowFlags f) :
    QFrame(parent, f),
    mPageList(),
    mLayout(NULL),
    mCurrentPage(NULL)
{    
    setObjectName("sfb_accordion");

    mLayout = new QVBoxLayout(this);
    mLayout->setObjectName("sfb_accordion_layout");
    mLayout->setMargin(0);
    mLayout->setSpacing(0);
    setLayout(mLayout);

    mScrollContentLayout = new QVBoxLayout();
    mScrollContentLayout->setObjectName("sfb_accordion_content_layout");
    mScrollContentLayout->setMargin(0);
    mScrollContentLayout->setSpacing(0);

    mScrollContent = new QFrame(this);
    mScrollContent->setLayout(mScrollContentLayout);
    mScrollContent->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

    QSpacerItem * bottomSpacer = new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    mScrollContentLayout->addSpacerItem(bottomSpacer);

    mScrollArea = new QScrollArea(parent);
    mScrollArea->setWidget(mScrollContent);
    mScrollArea->setWidgetResizable(true);
    mScrollArea->show();
    //mScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mScrollArea->setFrameStyle(QFrame::NoFrame);

    mLayout->addWidget(mScrollArea);

    setBackgroundRole(QPalette::Button);

    createDefaultIcons();
}


/**
 * @brief Destroys the accordion object.
 */
SFBAccordion::~SFBAccordion()
{

}


void SFBAccordion::createDefaultIcons()
{
    int width = 22;
    int height = 22;
    int border = 6;

    QVector<QPoint> v1;
    v1.push_back(QPoint(border, height-border));
    v1.push_back(QPoint(width-border, height/2));
    v1.push_back(QPoint(border, border));

    QVector<QPoint> v2;
    v2.push_back(QPoint(border, border));
    v2.push_back(QPoint(width-border, border));
    v2.push_back(QPoint(width/2, height-border));

    QPolygon rightArrow = QPolygon(v1);
    QPolygon downArrow = QPolygon(v2);

    QPixmap pixRight(width, height);
    QPixmap pixDown(width, height);

    QPainter painter;
    painter.setRenderHint(QPainter::Antialiasing);

    pixRight.fill(QColor(0,0,0,0));
    pixDown.fill(QColor(0,0,0,0));

    painter.begin(&pixRight);
    painter.setPen(QColor(40,40,40));
    painter.setBrush(QBrush(QColor(90,90,90), Qt::SolidPattern));
    painter.drawPolygon(rightArrow, Qt::WindingFill);
    painter.end();

    painter.begin(&pixDown);
    painter.setPen(QColor(40,40,40));
    painter.setBrush(QBrush(QColor(90,90,90), Qt::SolidPattern));
    painter.drawPolygon(downArrow, Qt::WindingFill);
    painter.end();

    mPageOpenIcon = QIcon(pixDown);
    mPageClosedIcon = QIcon(pixRight);
}




/**
 * @brief Insert a new page to the accordion with the given base widget as the content area.
 * @param index Index for the new page. Inserted at end if an invalid index is provided (-1).
 * @param widget The base widget for the page content.
 * @param icon Icon to show on the page header.
 * @param text Title for the page header.
 * @return The index of the inserted page, or -1 if the insert failed.
 */
int SFBAccordion::insertPage(int index, QWidget *widget, const QString &text, const QIcon &icon, QWidget *accessory)
{
    if (!widget) {
        return -1;
    }

    connect(widget, SIGNAL(destroyed(QObject*)), this, SLOT(widgetDestroyed(QObject*)));
    widget->setParent(this);

    if ( !text.isNull() ) {
        widget->setWindowTitle(text);
    }

    if ( widget->windowTitle().isEmpty() ) {
        QString title = QString("Page %1").arg(pageCount() + 1);
        widget->setWindowTitle(title);
    }

    SFBAccordionPage * page = new SFBAccordionPage(this, widget);
    page->setIcon(icon);

    if ( accessory != NULL ) {
        page->setAccessory(accessory);
    }

    if ( index < 0 || index >= mPageList.count() ) {
        index = mPageList.count();
    }

    mPageList.insert(index, page);
    mScrollContentLayout->insertWidget(index, page);

    closeAllPages();

    updateTabs();
    itemInserted(index);
    return index;
}


int SFBAccordion::addPage(QWidget *item, const QString &text, const QIcon &icon, QWidget *accessory)
{
    return insertPage(-1, item, text, icon, accessory);
}


int SFBAccordion::pageCount() const
{
    return mPageList.count();
}


void SFBAccordion::openPage(int index)
{
    SFBAccordionPage * p = getPage(index);

    if ( p ) {
        p->open();
    }
}


void SFBAccordion::closePage(int index)
{
    SFBAccordionPage * p = getPage(index);

    if ( p ) {
        p->close();
    }
}


void SFBAccordion::togglePage(int index)
{
    SFBAccordionPage * p = getPage(index);

    if ( p ) {
        p->toggle();
    }
}


void SFBAccordion::closeAllPages()
{
    for (auto it = mPageList.begin(); it != mPageList.end(); ++it)
    {
        (*it)->close();
    }
}



//
// Get Icons
//


QIcon& SFBAccordion::pageOpenIcon()
{
    return mPageOpenIcon;
}


QIcon& SFBAccordion::pageClosedIcon()
{
    return mPageClosedIcon;
}


//
// Designer Properties
//

QString SFBAccordion::currentItemText() const
{
    return mCurrentPage ? mCurrentPage->text() : QString();
}

void SFBAccordion::setCurrentItemText(const QString &value)
{
    if ( mCurrentPage ) {
        mCurrentPage->setText(value);
        emit currentItemTextChanged();
    }
}

QString SFBAccordion::currentItemName() const
{
    return mCurrentPage ? mCurrentPage->widget()->objectName() : QString();
}

void SFBAccordion::setCurrentItemName(const QString &value)
{
    if ( mCurrentPage ) {
        mCurrentPage->widget()->setObjectName(value);
        emit currentItemNameChanged();
    }
}

QIcon SFBAccordion::currentItemIcon() const
{
    return mCurrentPage ? mCurrentPage->icon() : QIcon();
}

void SFBAccordion::setCurrentItemIcon(const QIcon &value)
{
    if ( mCurrentPage ) {
        mCurrentPage->setIcon(value);
        emit currentItemIconChanged();
    }
}



//
// Slots
//

void SFBAccordion::setCurrentIndex(int index)
{
    SFBAccordionPage *c = getPage(index);

    if (!c || mCurrentPage == c) {
        return;
    }

    c->setSelected(true);

    if (mCurrentPage) {
        closeAllPages();
        mCurrentPage->setSelected(false);
    }

    mCurrentPage = c;
    mCurrentPage->open();

    updateTabs();
    emit currentChanged(index);
}


/**
 * @brief Removes the embedded item at the given position. Note that the widget on the page is not deleted.
 * @param index
 */
void SFBAccordion::removePage(int index)
{
    if (QWidget *w = widget(index))
    {
        disconnect(w, SIGNAL(destroyed(QObject*)), this, SLOT(widgetDestroyed(QObject*)));
        w->setParent(this);
        // destroy internal data
        widgetDestroyed(w);
        itemRemoved(index);
    }
}


/**
 * Returns the index of the currently selected page.
 * @return The index of the current page, or -1 if no page selected.
 */
int SFBAccordion::currentIndex() const
{
    return mCurrentPage ? indexOf(mCurrentPage->widget()) : -1;
}

/*!
    Returns a pointer to the current widget, or 0 if there is no such item.

    \sa currentIndex(), setCurrentWidget()
*/

QWidget * SFBAccordion::currentWidget() const
{
    return mCurrentPage ? mCurrentPage->widget() : 0;
}

/*!
  Makes\a widget the current widget. The \a widget must be an item in this tool box.

  \sa addItem(), setCurrentIndex(), currentWidget()
 */
void SFBAccordion::setCurrentWidget(QWidget *widget)
{
    int i = indexOf(widget);
    if (i >= 0)
        setCurrentIndex(i);
    else
        qWarning("SFBAccordion::setCurrentWidget: widget not contained in tool box");
}


/**
 * @brief Returns a pointer to the widget on the requested page.
 * @param index The index of the page from which to get the widget from.
 * @return
 */
QWidget *SFBAccordion::widget(int index) const
{
    if (index < 0 || index >= (int) mPageList.size()) {
        return NULL;
    }

    return mPageList.at(index)->widget();
}


/**
 * @brief Returns the page index for the given widget.
 * @param widget
 * @return
 */
int SFBAccordion::indexOf(QWidget *widget) const
{
    // Iterate using indices, as we need an integer response
    for( int i = 0, count = mPageList.count(); i < count; ++i )
    {
        const SFBAccordionPage * page = mPageList.at(i);

        if ( page->widget() == widget ) {
            return i;
        }
    }

    return -1;
}


/**
 * @brief Set the page at the provided index to the given enabled status.
 * @param index
 * @param enabled
 */
void SFBAccordion::setItemEnabled(int index, bool enabled)
{
    SFBAccordionPage *c = getPage(index);

    if (!c) {
        return;
    }

    c->header()->setEnabled(enabled);

    if (!enabled && c == mCurrentPage) {
        int curIndexUp = index;
        int curIndexDown = curIndexUp;
        const int count = mPageList.count();
        while (curIndexUp > 0 || curIndexDown < count-1) {
            if (curIndexDown < count-1) {
                if (getPage(++curIndexDown)->header()->isEnabled()) {
                    index = curIndexDown;
                    break;
                }
            }
            if (curIndexUp > 0) {
                if (getPage(--curIndexUp)->header()->isEnabled()) {
                    index = curIndexUp;
                    break;
                }
            }
        }
        setCurrentIndex(index);
    }
}


/**
 * Sets the text of the item at position \a index to \a text.
 *
 * If the provided text contains an ampersand character ('&'), a
 * mnemonic is automatically created for it. The character that
 * follows the '&' will be used as the shortcut key. Any previous
 * mnemonic will be overwritten, or cleared if no mnemonic is defined
 * by the text. See the \l {QShortcut#mnemonic}{QShortcut}
 * documentation for details (to display an actual ampersand, use
 * '&&').
 */
void SFBAccordion::setItemText(int index, const QString &text)
{
    SFBAccordionPage *c = getPage(index);

    if (c) {
        c->setText(text);
    }
}


/**
 * @brief Sets the icon of the item at position \a index to \a icon.
 * @param index
 * @param icon
 */
void SFBAccordion::setItemIcon(int index, const QIcon &icon)
{
    SFBAccordionPage *c = getPage(index);
    if (c)
        c->setIcon(icon);
}


/**
 * @brief Set the tooltip of the item at the given position.
 * @param index
 * @param toolTip
 */
void SFBAccordion::setItemToolTip(int index, const QString &toolTip)
{
    SFBAccordionPage *c = getPage(index);
    if (c)
        c->setToolTip(toolTip);
}


/**
 * @brief Returns true if the item at position 'index' is enabled; otherwise returns false.
 * @param index
 * @return
 */
bool SFBAccordion::isItemEnabled(int index) const
{
    const SFBAccordionPage *c = getPage(index);
    return c && c->header()->isEnabled();
}


/**
 * @brief Returns the text of the item at position \a index, or an empty string if \a index is out of range.
 * @param index
 * @return
 */
QString SFBAccordion::itemText(int index) const
{
    const SFBAccordionPage *c = getPage(index);
    return (c ? c->text() : QString());
}


/*!
    Returns the icon of the item at position \a index, or a null
    icon if \a index is out of range.
*/

QIcon SFBAccordion::itemIcon(int index) const
{
    const SFBAccordionPage *c = getPage(index);
    return (c ? c->icon() : QIcon());
}


QString SFBAccordion::itemToolTip(int index) const
{
    const SFBAccordionPage *c = getPage(index);
    return (c ? c->toolTip() : QString());
}


void SFBAccordion::changeEvent(QEvent *ev)
{
    if(ev->type() == QEvent::StyleChange) {
        updateTabs();
    }

    QFrame::changeEvent(ev);
}


void SFBAccordion::itemInserted(int index)
{
    Q_UNUSED(index)
}


void SFBAccordion::itemRemoved(int index)
{
    Q_UNUSED(index)
}


//
// Private
//


SFBAccordionPage *SFBAccordion::getPage(QWidget *widget) const
{
    if (!widget) {
        return NULL;
    }

    for (PageList::ConstIterator it = mPageList.constBegin(); it != mPageList.constEnd(); ++it) {
        if ((*it)->widget() == widget) {
            return (*it);
        }
    }

    return NULL;
}


SFBAccordionPage *SFBAccordion::getPage(int index)
{
    if (index >= 0 && index < mPageList.size()) {
        return mPageList[index];
    }

    return NULL;
}


const SFBAccordionPage *SFBAccordion::getPage(int index) const
{
    if (index >= 0 && index < mPageList.size()) {
        return mPageList.at(index);
    }

    return NULL;
}


void SFBAccordion::updateTabs()
{
    SFBAccordionHeader *lastHeader = (mCurrentPage && mCurrentPage->header()) ? mCurrentPage->header() : 0;
    bool after = false;
    int index = 0;

    for (index = 0; index < mPageList.count(); ++index) {
        const SFBAccordionPage * page = mPageList.at(index);
        SFBAccordionHeader *tB = page->header();
        // update indexes, since the updates are delayed, the indexes will be correct
        // when we actually paint.
        tB->setIndex(index);
        QWidget *tW = page->widget();

        if (after) {
            QPalette p = tB->palette();
            p.setColor(tB->backgroundRole(), tW->palette().color(tW->backgroundRole()));
            tB->setPalette(p);
            tB->update();
        } else if (tB->backgroundRole() != QPalette::Window) {
            tB->setBackgroundRole(QPalette::Window);
            tB->update();
        }

        after = tB == lastHeader;
    }
}


void SFBAccordion::widgetDestroyed(QObject *object)
{
    // no verification - vtbl corrupted already
    QWidget *p = (QWidget*)object;
    SFBAccordionPage *c = getPage(p);

    if (!p || !c)
        return;

    mLayout->removeWidget(c);
    c->deleteLater();

    bool removeCurrent = (c == mCurrentPage);
    mPageList.removeAt(indexOf(p));

    if (!mPageList.count()) {
        mCurrentPage = NULL;
        emit currentChanged(-1);
    } else if (removeCurrent) {
        mCurrentPage = NULL;
        setCurrentIndex(0);
    }
}


void SFBAccordion::relayout()
{
    delete mLayout;
    mLayout = new QVBoxLayout(this);
    mLayout->setMargin(0);

    for (PageList::Iterator it = mPageList.begin(); it != mPageList.end(); ++it) {
        mScrollContentLayout->addWidget((*it));
    }
}
