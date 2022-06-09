#include "SFBEffectTypes.h"


// TODO: Convert all text strings to constants!


SFBEffectType SFBEffectTypes::EffectTypeFromString(const QString & value)
{
    QString lower = value.toLower();

    if      ( lower.compare("fill") == 0 )        { return SFBEffectType::Effect_Fill; }
    else if ( lower.compare("shadow") == 0 )      { return SFBEffectType::Effect_Shadow; }
    else if ( lower.compare("stroke") == 0 )      { return SFBEffectType::Effect_Stroke; }

    return SFBEffectType::Effect_Unknown;
}


QString SFBEffectTypes::EffectTypeToString(SFBEffectType value)
{
    switch(value) {
        case SFBEffectType::Effect_Fill:    return QString("Fill");
        case SFBEffectType::Effect_Shadow:  return QString("Shadow");
        case SFBEffectType::Effect_Stroke:  return QString("Stroke");
        default: return QString("Unknown");
    }
}


QStringList SFBEffectTypes::BlendModeOptions()
{
    return QStringList() << "Normal" << "Multiply" << "Screen" << "Overlay" << "Lighten" << "Darken"
                         << "Color Dodge" << "Color Burn" << "Soft Light" << "Hard Light" << "Difference" << "Exclusion";
}


SFBBlendMode SFBEffectTypes::BlendModeFromString(const QString & value)
{
    QString lower = value.toLower();

    if      ( lower.compare("normal") == 0 )        { return SFBBlendMode::Blend_Normal; }
    else if ( lower.compare("multiply") == 0 )      { return SFBBlendMode::Blend_Multiply; }
    else if ( lower.compare("screen") == 0 )        { return SFBBlendMode::Blend_Screen; }
    else if ( lower.compare("overlay") == 0 )       { return SFBBlendMode::Blend_Overlay; }
    else if ( lower.compare("lighten") == 0 )       { return SFBBlendMode::Blend_Lighten; }
    else if ( lower.compare("darken") == 0 )        { return SFBBlendMode::Blend_Darken; }
    else if ( lower.compare("color dodge") == 0 )   { return SFBBlendMode::Blend_ColorDodge; }
    else if ( lower.compare("color burn") == 0 )    { return SFBBlendMode::Blend_ColorBurn; }
    else if ( lower.compare("soft light") == 0 )    { return SFBBlendMode::Blend_SoftLight; }
    else if ( lower.compare("hard light") == 0 )    { return SFBBlendMode::Blend_HardLight; }
    else if ( lower.compare("difference") == 0 )    { return SFBBlendMode::Blend_Difference; }
    else if ( lower.compare("exclusion") == 0 )     { return SFBBlendMode::Blend_Exclusion; }

    return SFBBlendMode::Blend_Unknown;
}


QString SFBEffectTypes::BlendModeToString(SFBBlendMode value)
{
    switch(value) {
        case SFBBlendMode::Blend_Normal:    return QString("Normal");
        case SFBBlendMode::Blend_Multiply:  return QString("Multiply");
        case SFBBlendMode::Blend_Screen:    return QString("Screen");
        case SFBBlendMode::Blend_Overlay:   return QString("Overlay");
        case SFBBlendMode::Blend_Lighten:   return QString("Lighten");
        case SFBBlendMode::Blend_Darken:    return QString("Darken");
        case SFBBlendMode::Blend_ColorDodge: return QString("Color Dodge");
        case SFBBlendMode::Blend_ColorBurn: return QString("Color Burn");
        case SFBBlendMode::Blend_SoftLight: return QString("Soft Light");
        case SFBBlendMode::Blend_HardLight: return QString("Hard Light");
        case SFBBlendMode::Blend_Difference: return QString("Difference");
        case SFBBlendMode::Blend_Exclusion: return QString("Exclusion");
        default: return QString("Unknown");
    }
}


QStringList SFBEffectTypes::FillTypeOptions()
{
    return QStringList() << "Color" << "Gradient" << "Pattern";
}


SFBFillType SFBEffectTypes::FillTypeFromString(const QString & value)
{
    QString lower = value.toLower();

    if ( lower.compare("color") == 0 ) {
        return SFBFillType::Fill_Color;
    } else if ( lower.compare("gradient") == 0 ) {
        return SFBFillType::Fill_Gradient;
    } else if ( lower.compare("pattern") == 0 ) {
        return SFBFillType::Fill_Pattern;
    }

    return SFBFillType::Fill_Unknown;
}


QString SFBEffectTypes::FillTypeToString(SFBFillType value)
{
    switch(value) {
        case SFBFillType::Fill_Color: return QString("Color");
        case SFBFillType::Fill_Gradient: return QString("Gradient");
        case SFBFillType::Fill_Pattern: return QString("Pattern");
        default: return QString("Unknown");
    }
}


QStringList SFBEffectTypes::StrokePositionOptions()
{
    return QStringList() << "Center" << "Inside" << "Outside";
}


SFBStrokePosition SFBEffectTypes::StrokePositionFromString(const QString & value)
{
    QString lower = value.toLower();

    if      ( lower.compare("center") == 0 )    { return SFBStrokePosition::Stroke_Center; }
    else if ( lower.compare("inside") == 0 )    { return SFBStrokePosition::Stroke_Inside; }
    else if ( lower.compare("outside") == 0 )   { return SFBStrokePosition::Stroke_Outside; }

    return SFBStrokePosition::Stroke_Unknown;
}


QString SFBEffectTypes::StrokePositionToString(SFBStrokePosition value)
{
    switch(value) {
        case SFBStrokePosition::Stroke_Center:  return QString("Center");
        case SFBStrokePosition::Stroke_Inside:  return QString("Inside");
        case SFBStrokePosition::Stroke_Outside: return QString("Outside");
        default: return QString("Unknown");
    }
}


QStringList SFBEffectTypes::ShadowTypeOptions()
{
    return QStringList() << "Outer" << "Inner";
}


SFBShadowType SFBEffectTypes::ShadowTypeFromString(const QString & value)
{
    QString lower = value.toLower();

    if      ( lower.compare("outer") == 0 )    { return SFBShadowType::Shadow_Outer; }
    else if ( lower.compare("inner") == 0 )    { return SFBShadowType::Shadow_Inner; }

    return SFBShadowType::Shadow_Unknown;
}


QString SFBEffectTypes::ShadowTypeToString(SFBShadowType value)
{
    switch(value) {
        case SFBShadowType::Shadow_Outer:  return QString("Outer");
        case SFBShadowType::Shadow_Inner:  return QString("Inner");
        default: return QString("Unknown");
    }
}
