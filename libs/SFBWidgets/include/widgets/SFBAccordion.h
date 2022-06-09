#ifndef SFBACCORDION_H
#define SFBACCORDION_H

#include <memory>

#include <QFrame>
#include <QIcon>
#include <QStyleOptionToolBox>
#include <QScrollArea>
#include <QVBoxLayout>

#include "../common/SFBWidgetsGlobal.h"
#include "widgets/SFBAccordionPage.h"


class SFB_EXPORT SFBAccordion : public QFrame
{
    typedef QList<SFBAccordionPage *> PageList;

    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
    Q_PROPERTY(QString currentItemText READ currentItemText WRITE setCurrentItemText STORED false NOTIFY currentItemTextChanged)
    Q_PROPERTY(QString currentItemName READ currentItemName WRITE setCurrentItemName STORED false NOTIFY currentItemNameChanged)
    Q_PROPERTY(QIcon currentItemIcon READ currentItemIcon WRITE setCurrentItemIcon STORED false NOTIFY currentItemIconChanged)
    Q_PROPERTY(int pageCount READ pageCount)


public:
    explicit SFBAccordion(QWidget *parent, Qt::WindowFlags f = 0);
    ~SFBAccordion();

    void createDefaultIcons();

    int addPage(QWidget *item, const QString &text = QString(), const QIcon &icon = QIcon(), QWidget *accessory = NULL);
    int insertPage(int index, QWidget *item, const QString &text = QString(), const QIcon &icon = QIcon(), QWidget *accessory = NULL);
    void removePage(int index);

    int currentIndex() const;
    QWidget *currentWidget() const;
    QWidget *widget(int index) const;
    int indexOf(QWidget *widget) const;
    int pageCount() const;

    void openPage(int index);
    void closePage(int index);
    void togglePage(int index);

    void closeAllPages();

    // Get Icons for Page Headers
    QIcon& pageOpenIcon();
    QIcon& pageClosedIcon();

    // Designer Properties
    QString currentItemText() const;
    void setCurrentItemText(const QString &value);

    QString currentItemName() const;
    void setCurrentItemName(const QString &value);

    QIcon currentItemIcon() const;
    void setCurrentItemIcon(const QIcon &value);


public slots:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *widget);


signals:
    void currentChanged(int index);
    void currentItemTextChanged();
    void currentItemNameChanged();
    void currentItemIconChanged();


protected:
    void itemInserted(int index);
    void itemRemoved(int index);
    void changeEvent(QEvent *);


protected:
    void setItemEnabled(int index, bool enabled);
    void setItemText(int index, const QString &text);
    void setItemIcon(int index, const QIcon &icon);
    void setItemToolTip(int index, const QString &toolTip);
    bool isItemEnabled(int index) const;
    QString itemText(int index) const;
    QIcon itemIcon(int index) const;
    QString itemToolTip(int index) const;


private slots:
    void widgetDestroyed(QObject*);


private:
    SFBAccordionPage *getPage(QWidget *widget) const;
    const SFBAccordionPage *getPage(int index) const;
    SFBAccordionPage *getPage(int index);

    void updateTabs();
    void relayout();


private:
    PageList mPageList;

    QVBoxLayout * mLayout;

    QScrollArea * mScrollArea;
    QFrame * mScrollContent;
    QVBoxLayout * mScrollContentLayout;

    SFBAccordionPage * mCurrentPage;

    QIcon mPageOpenIcon;
    QIcon mPageClosedIcon;

};

#endif // SFBACCORDION_H
