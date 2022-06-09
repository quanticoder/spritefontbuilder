#ifndef GRADIENTSWATCHWIDGET_H
#define GRADIENTSWATCHWIDGET_H

#include <QtCore>
#include <QWidget>
#include "../../Model/SFBTypes.h"

class GradientSwatchWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(SFBGradient gradient READ gradient WRITE setGradient)


    // Object Lifecycle
public:

    GradientSwatchWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~GradientSwatchWidget();

public:

    /** Get the displayed gradient. */
    SFBGradient gradient() const;

    /** Set the displayed gradient and update the widget. */
    void setGradient(const SFBGradient& gradient);


protected:

    /** When the user clicks on this widget, we want to present a gradient builder dialog. */
    virtual void mousePressEvent(QMouseEvent *ev);

    /** Updated draw method as we want to be able to draw a nice background. */
    virtual void paintEvent(QPaintEvent *ev);


    // Signals
signals:
    void gradientChanged(SFBGradient);


    // Member Variables
private:
    SFBGradient mGradient;
    QPixmap mBackground;
};

#endif // GRADIENTSWATCHWIDGET_H
