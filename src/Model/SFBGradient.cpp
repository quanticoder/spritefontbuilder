#include "SFBGradient.h"

#include <QPainter>
#include <QtMath>
#include <QVector2D>
#include <QtDebug>


//
// Gradient Stop Methods
//

const QString SFBGradientStop::kColorKey = QString("color");
const QString SFBGradientStop::kPositionKey = QString("position");


SFBGradientStop::SFBGradientStop() :
    color(QColor(0,0,0,255)),
    position(0.0f)
{

}


SFBGradientStop::SFBGradientStop(const QString &descriptor)
{
    fromString(descriptor);
}


SFBGradientStop::SFBGradientStop(const QColor& pColor, float pPosition) :
    color(pColor),
    position(pPosition)
{

}


QString SFBGradientStop::toString()
{
    QStringList stopElements;
    stopElements.append(kPositionKey + ": " + QString::number(position));
    stopElements.append(kColorKey + ": " + color.name(QColor::HexArgb));
    return "(" + stopElements.join(", ") + ")";
}


void SFBGradientStop::fromString(const QString &value)
{
    QString s = value.trimmed();

    if ( s.isEmpty() ) {
        return;
    }

    if ( s[0] == '(' ) {
        s = s.mid(1, s.length() - 2);
    }

    QStringList elements = s.split(",");

    for( QStringList::Iterator it = elements.begin(); it != elements.end(); ++it ) {
        QStringList kvPair = (*it).trimmed().split(":");

        if ( kvPair.size() != 2 ) {
            continue;
        }

        QString key = kvPair[0].trimmed();
        QString value = kvPair[1].trimmed();

        if ( key.compare(kPositionKey) == 0 )
        {
            position = value.toFloat();
        }
        else if ( key.compare(kColorKey) == 0 )
        {
            color = QColor(value);
        }
    }
}


//
// Static Methods
//


const QString SFBGradient::kGradientStyleUnknown = QString("Unknown");
const QString SFBGradient::kGradientStyleLinear = QString("Linear");
const QString SFBGradient::kGradientStyleRadial = QString("Radial");
const QString SFBGradient::kGradientStyleConical = QString("Conical");

const QString SFBGradient::kStyleKey = QString("gradientStyle");
const QString SFBGradient::kStopsKey = QString("stops");
const QString SFBGradient::kAngleKey = QString("angle");
const QString SFBGradient::kScaleKey = QString("scale");



QString SFBGradient::GradientStyleToString(const SFBGradientStyle &gradientStyle)
{
    switch( gradientStyle )
    {
    case SFBGradientStyle::Gradient_Linear: return kGradientStyleLinear;
    case SFBGradientStyle::Gradient_Radial: return kGradientStyleRadial;
    case SFBGradientStyle::Gradient_Conical: return kGradientStyleConical;
    default: return kGradientStyleUnknown;
    }
}


SFBGradientStyle SFBGradient::GradientStyleFromString(const QString &value)
{
    if ( value.compare(kGradientStyleLinear, Qt::CaseInsensitive) == 0 ) { return SFBGradientStyle::Gradient_Linear; }
    if ( value.compare(kGradientStyleRadial, Qt::CaseInsensitive) == 0 ) { return SFBGradientStyle::Gradient_Radial; }
    if ( value.compare(kGradientStyleConical, Qt::CaseInsensitive) == 0 ) { return SFBGradientStyle::Gradient_Conical; }

    return SFBGradientStyle::Gradient_Unknown;
}



//
// Object lifecycle
//


SFBGradient::SFBGradient() :
    style(SFBGradientStyle::Gradient_Unknown),
    angle(45),
    scale(1.0f)
{

}


SFBGradient::SFBGradient(const QString &stringRepresentation)
{
    fromStringRepresentation(stringRepresentation);
}


SFBGradient::SFBGradient(const SFBGradient &other)
{
    style = other.style;
    angle = other.angle;
    scale = other.scale;
    stops = QVector<SFBGradientStop>(other.stops);
}


void SFBGradient::setDefaultLinearGradient()
{
    style = SFBGradientStyle::Gradient_Linear;
    angle = 45;
    scale = 1.0f;

    stops.clear();
    stops.push_back(SFBGradientStop(QColor(0,0,0), 0.0f));
    stops.push_back(SFBGradientStop(QColor(255,255,255), 1.0f));
}


bool SFBGradient::isValid()
{
    return (style != SFBGradientStyle::Gradient_Unknown);
}


QLinearGradient SFBGradient::linearGradientForEditor(const QRectF& rect)
{
    QPointF startPoint = QPointF(rect.left(), rect.top());
    QPointF endPoint = QPointF(rect.right(), rect.top());

    QLinearGradient linearGradient(startPoint, endPoint);

    for( auto it = stops.begin(); it != stops.end(); ++it ) {
        linearGradient.setColorAt((*it).position, (*it).color);
    }

    return linearGradient;
}


QLinearGradient SFBGradient::linearGradientForRect(const QRectF &rect)
{
    QPointF midPoint = rect.center();
    float radians = qDegreesToRadians(angle);
    QVector2D dir = QVector2D(qCos(radians), qSin(radians));
    float length = qMax(rect.width(), rect.height());

    if ( qSin(radians) > 0 ) {
        length = qMin(length, (float)(rect.height() / qSin(radians)));
    }

    if ( qCos(radians) > 0 ) {
        length = qMin(length, (float)(rect.width() / qCos(radians)));
    }

    QVector2D line = dir * length * 0.5f * scale;

    QPointF startPoint(midPoint.x() - line.x(), midPoint.y() - line.y());
    QPointF endPoint(midPoint.x() + line.x(), midPoint.y() + line.y());

    QLinearGradient linearGradient(startPoint, endPoint);

    for( auto it = stops.begin(); it != stops.end(); ++it ) {
        linearGradient.setColorAt((*it).position, (*it).color);
    }

    return linearGradient;
}


QRadialGradient SFBGradient::radialGradientForRect(const QRectF &rect)
{
    QPointF midPoint = rect.center();
    float length = qMin(rect.width(), rect.height());

    QRadialGradient radialGradient(midPoint, length * 0.5f * scale);

    for( auto it = stops.begin(); it != stops.end(); ++it ) {
        radialGradient.setColorAt((*it).position, (*it).color);
    }

    return radialGradient;
}


void SFBGradient::fillRect(QPainter& painter, const QRectF & rect)
{
    if ( style == SFBGradientStyle::Gradient_Linear )
    {
        QLinearGradient gradient = linearGradientForRect(rect);
        painter.fillRect(rect, gradient);
    }
    else if ( style == SFBGradientStyle::Gradient_Radial )
    {
        QRadialGradient gradient = radialGradientForRect(rect);
        painter.fillRect(rect, gradient);
    }
    else
    {
        painter.fillRect(rect, Qt::black);
    }
}


void SFBGradient::fillPath(QPainter &painter, const QPainterPath &path)
{
    QRectF bounds = path.boundingRect();

    if ( style == SFBGradientStyle::Gradient_Linear )
    {
        QLinearGradient gradient = linearGradientForRect(bounds);
        painter.fillPath(path, gradient);
    }
    else if ( style == SFBGradientStyle::Gradient_Radial )
    {
        QRadialGradient gradient = radialGradientForRect(bounds);
        painter.fillPath(path, gradient);
    }
    else
    {
        painter.fillPath(path, Qt::black);
    }
}


QString SFBGradient::stringRepresentation()
{
    // Example gradient...
    // "gradientStyle: Linear; angle:45; scale: 1.0; stops: (0.0, #00000000) (1.0, #FFFF0000)"
    QStringList stopList;

    for (auto it = stops.begin(); it != stops.end(); ++it)
    {
        stopList.append((*it).toString());
    }

    QStringList elements;
    elements.append(kStyleKey + ": " + GradientStyleToString(style));
    elements.append(kAngleKey + ": " + QString::number(angle));
    elements.append(kScaleKey + ": " + QString::number(scale));
    elements.append(kStopsKey + ": " + stopList.join(" "));

    return elements.join("; ");
}


void SFBGradient::fromStringRepresentation(const QString &value)
{
    QStringList elements = value.split(";");
    stops.clear();

    for (auto it = elements.constBegin(); it != elements.constEnd(); ++it)
    {
        const QString & s = (*it);
        int delim = s.indexOf(':');

        if ( delim < 0 ) {
            continue;
        }

        QString key = s.left(delim).trimmed();
        QString value = s.right(s.length() - delim - 1).trimmed();

        if ( key.compare(kStyleKey, Qt::CaseInsensitive) == 0 )
        {
            style = GradientStyleFromString(value);
        }
        else if ( key.compare(kAngleKey) == 0 )
        {
            angle = value.toFloat();
        }
        else if ( key.compare(kScaleKey) == 0 )
        {
            scale = value.toFloat();
        }
        else if ( key.compare(kStopsKey) == 0 )
        {
            QRegExp re = QRegExp("(\\([^\\)]+\\))");
            QStringList stopDescriptors;
            int pos = 0;

            while ((pos = re.indexIn(value, pos)) != -1) {
                stopDescriptors << re.cap(1);
                pos += re.matchedLength();
            }

            for( int i = 0; i < stopDescriptors.size(); ++i ) {
                stops.append(SFBGradientStop(stopDescriptors[i]));
            }
        }
    }
}

