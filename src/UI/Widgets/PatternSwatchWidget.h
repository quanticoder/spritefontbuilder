#ifndef PATTERNSWATCHWIDGET_H
#define PATTERNSWATCHWIDGET_H

#include <QtCore>
#include <QWidget>
#include "../../Model/SFBTypes.h"

class PatternSwatchWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(SFBPattern patternValue READ patternValue WRITE setPatternValue)


    // Object Lifecycle
public:

    PatternSwatchWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~PatternSwatchWidget();

public:

    /** Get the pattern value. */
    SFBPattern patternValue() const;

    /** Set the pattern value and update the widget background. */
    void setPatternValue(const SFBPattern &pattern);


protected:

    /** When the user clicks on this widget, we want to present a color picker. */
    virtual void mousePressEvent(QMouseEvent *ev);

    /** Updated draw method as we want to be able to draw a nice background. */
    virtual void paintEvent(QPaintEvent *ev);


    // Signals
signals:
    void patternValueChanged(SFBPattern);

    // Member Variables
private:
    SFBPattern mPatternValue;
};

#endif // PATTERNSWATCHWIDGET_H
