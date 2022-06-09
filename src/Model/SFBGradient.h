#ifndef SFBGRADIENT_H
#define SFBGRADIENT_H

#include <QGradient>


enum SFBGradientStyle
{
    Gradient_Unknown,
    Gradient_Linear,
    Gradient_Radial,
    Gradient_Conical
};


struct SFBGradientStop
{
    static const QString kColorKey;
    static const QString kPositionKey;

    QColor color;
    float position;

    SFBGradientStop();
    SFBGradientStop(const QString &descriptor);
    SFBGradientStop(const QColor& pColor, float pPosition);

    QString toString();

    void fromString(const QString &value);
};

Q_DECLARE_METATYPE(SFBGradientStop)


class SFBGradient
{
    //
    // Constants
    //
private:
    static const QString kGradientStyleUnknown;
    static const QString kGradientStyleLinear;
    static const QString kGradientStyleRadial;
    static const QString kGradientStyleConical;

    static const QString kStyleKey;
    static const QString kStopsKey;
    static const QString kAngleKey;
    static const QString kScaleKey;


    //
    // Static Methods
    //
public:
    /** Convert a gradient style enum value to a text representation. */
    static QString GradientStyleToString(const SFBGradientStyle &gradientStyle);

    /** Convert a text representation of a gradient style back to an enum value. */
    static SFBGradientStyle GradientStyleFromString(const QString &value);


    //
    // Object Lifecycle
    //
public:
    /** Construct a new gradient, which isn't valid to start with. */
    SFBGradient();

    /** Construct a new gradient, from a descriptive string. */
    SFBGradient(const QString &stringRepresentation);

    /** Copy Constructor. */
    SFBGradient(const SFBGradient &other);

    /** Set this gradient to be a default linear gradient. */
    void setDefaultLinearGradient();

    /** Is this gradient valid? */
    bool isValid();

    /** Get a drawable linear gradient to fill a horizontal 'gradient stop editor' (even if this isn't a linear gradient). */
    QLinearGradient linearGradientForEditor(const QRectF & rect);

    /** Get a drawable linear gradient to fill the given rect from this gradient. */
    QLinearGradient linearGradientForRect(const QRectF & rect);

    /** Get a drawable radial gradient to fill the given rect from this gradient. */
    QRadialGradient radialGradientForRect(const QRectF & rect);

    /** Apply this gradient, using the given painter, in the provided rect. */
    void fillRect(QPainter& painter, const QRectF & rect);

    /** Apply this gradient, using the given painter, in the provided path. */
    void fillPath(QPainter& painter, const QPainterPath & path);

    /** Convert a gradient to a string representation for serialization. */
    QString stringRepresentation();

    /** Read in the values for this gradient object from a descriptive string. */
    void fromStringRepresentation(const QString &value);


    // Public properties
public:
    SFBGradientStyle style;
    QVector<SFBGradientStop> stops;
    float angle;
    float scale;

};

Q_DECLARE_METATYPE(SFBGradient)

#endif // SFBGRADIENT_H
