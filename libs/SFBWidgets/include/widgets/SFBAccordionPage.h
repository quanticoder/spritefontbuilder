#ifndef SFBACCORDIONPAGE_H
#define SFBACCORDIONPAGE_H

#include <memory>

#include "SFBAccordionHeader.h"


// Forward Declaration
class SFBAccordion;


/**
 * @brief Represents a single page on an accordion widget and provides functionality for manipulating the page.
 */
class SFBAccordionPage : public QWidget
{
    Q_OBJECT

public:
    SFBAccordionPage(SFBAccordion * parent, QWidget * widget);

public:
    SFBAccordion * parent();

    void setSelected(bool value);

    QString text() const;
    void setText(const QString &text);

    QIcon icon() const;
    void setIcon(const QIcon &icon);

    QString toolTip() const;
    void setToolTip(const QString &tip);

    void setAccessory(QWidget * widget);
    QWidget * accessory();
    QWidget * accessory() const;

    SFBAccordionHeader * header();          //< Todo: Remove this and encapsulate
    SFBAccordionHeader * header() const;    //< Todo: Remove this and encapsulate
    QWidget * widget();
    QWidget * widget() const;


    bool operator==(const SFBAccordionPage& other) const
    {
        return mWidget == other.mWidget;
    }

    void open();
    void close();
    void toggle();
    void makeCurrent();

private:
    SFBAccordion * mParent;

    QVBoxLayout * mLayout;
    SFBAccordionHeader * mHeader;
    QWidget * mWidget;

    bool mIsOpen;
};

#endif // SFBACCORDIONPAGE_H
