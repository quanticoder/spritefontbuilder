#ifndef SFBACCORDIONHEADER_H
#define SFBACCORDIONHEADER_H

#include <memory>

#include <QFrame>
#include <QStyleOptionToolBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QApplication>
#include <QPushButton>
#include <QLabel>


//
// Forward Declarations
//
class SFBAccordion;
class SFBAccordionPage;


/**
 * @brief The header panel for an accordion page is based on the button interface. Clicking the button opens that panel.
 */
class SFBAccordionHeader : public QWidget
{
    Q_OBJECT

public:
    SFBAccordionHeader(SFBAccordionPage *page);

    void setSelected(bool b);
    void setOpen(bool b);
    void setIndex(int newIndex) { mIndexInPage = newIndex; }

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void setText(const QString &text) { mTitleButton->setText(text); }
    void setIcon(const QIcon &icon) { mTitleButton->setIcon(icon); }

    QPushButton * arrowButton() { return mArrowButton; }
    QPushButton * button() { return mTitleButton; }

    void setAccessory(QWidget * widget);
    QWidget * accessory();
    QWidget * accessory() const;

protected:
    void paintEvent(QPaintEvent *ev);

public slots:
    void onTitleButtonClicked();
    void onArrowButtonClicked();

private:
    SFBAccordionPage * mPage;

    QHBoxLayout * mLayout;
    QPushButton * mArrowButton;
    QPushButton * mTitleButton;
    QWidget * mAccessory;

    QIcon mOpenIcon;
    QIcon mClosedIcon;

    bool mSelected;
    bool mOpen;
    int mIndexInPage;
};


#endif // SFBACCORDIONHEADER_H
