#ifndef SFBEFFECTTYPES_H
#define SFBEFFECTTYPES_H

#include <QColor>
#include <QVector>
#include <QImage>

enum SFBEffectType
{
    Effect_Fill,
    Effect_Shadow,
    Effect_Stroke,
    Effect_Unknown
};

Q_DECLARE_METATYPE(SFBEffectType)


enum SFBBlendMode
{
    Blend_Normal,
    Blend_Multiply,
    Blend_Screen,
    Blend_Overlay,
    Blend_Lighten,
    Blend_Darken,
    Blend_ColorDodge,
    Blend_ColorBurn,
    Blend_SoftLight,
    Blend_HardLight,
    Blend_Difference,
    Blend_Exclusion,
    Blend_Unknown
};

Q_DECLARE_METATYPE(SFBBlendMode)


enum SFBFillType
{
    Fill_Color,
    Fill_Gradient,
    Fill_Pattern,
    Fill_Unknown
};

Q_DECLARE_METATYPE(SFBFillType)


enum SFBStrokePosition
{
    Stroke_Center,
    Stroke_Inside,
    Stroke_Outside,
    Stroke_Unknown
};

Q_DECLARE_METATYPE(SFBStrokePosition)


enum SFBShadowType
{
    Shadow_Outer,
    Shadow_Inner,
    Shadow_Unknown
};

Q_DECLARE_METATYPE(SFBShadowType)


/**
 * @brief A class containing static methods for manipulating SFB Effect Types
 */
class SFBEffectTypes
{

public:
    static SFBEffectType EffectTypeFromString(const QString & value);
    static QString EffectTypeToString(SFBEffectType value);

    static QStringList BlendModeOptions();
    static SFBBlendMode BlendModeFromString(const QString & value);
    static QString BlendModeToString(SFBBlendMode value);

    static QStringList FillTypeOptions();
    static SFBFillType FillTypeFromString(const QString & value);
    static QString FillTypeToString(SFBFillType value);

    static QStringList StrokePositionOptions();
    static SFBStrokePosition StrokePositionFromString(const QString & value);
    static QString StrokePositionToString(SFBStrokePosition value);

    static QStringList ShadowTypeOptions();
    static SFBShadowType ShadowTypeFromString(const QString & value);
    static QString ShadowTypeToString(SFBShadowType value);

};


#endif // SFBEFFECTTYPES_H
