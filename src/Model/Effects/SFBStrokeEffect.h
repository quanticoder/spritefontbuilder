#ifndef SFBSTROKEEFFECT_H
#define SFBSTROKEEFFECT_H

#include "SFBEffect.h"


struct SFBStrokeEffectSettings
{
    bool enabled;               //< Is this effect enabled?
    SFBBlendMode blendMode;     //< Blend mode for this effect.
    float opacity;              //< Blend opacity for this effect.
    float width;                //< Stroke Width.
    QColor color;               //< Stroke Color.
    SFBStrokePosition position; //< Stroke position.

    SFBStrokeEffectSettings() :
        enabled(true),
        blendMode(SFBBlendMode::Blend_Normal),
        opacity(1.0f),
        width(1),
        color(QColor(0,0,0,255)),
        position(SFBStrokePosition::Stroke_Outside)
    {

    }
};

Q_DECLARE_METATYPE(SFBStrokeEffectSettings);


class SFBStrokeEffect : public SFBEffect
{
public:
    typedef std::shared_ptr<SFBStrokeEffect> Ptr;

public:
    static const QString kWidthKey;
    static const QString kColorKey;
    static const QString kPositionKey;

public:
    SFBStrokeEffect();


    // Abstract Methods
public:

    ///** Get the minimum size required to render this effect on the given glyph. */
    //virtual QSize sizeRequiredForGlyph(SFBGlyph& glyph) = 0;

    /** Return a clone (deep copy) of this effect so that we can duplicate effects on the object. */
    virtual SFBEffect::Ptr clone() { return SFBEffect::Ptr(new SFBStrokeEffect(*this)); }

    /** Get the amount of padding required over the base size for this glyph to accomodate for this effect. */
    virtual SFBPadding paddingRequiredForGlyph(SFBGlyph& glyph);

    /** Apply this effect to the given glyph. */
    virtual void applyToGlyph(SFBGlyph& glyph);

    /** Set default parameters for this effect. */
    virtual void setDefaultParameters();

    /** Scale the settings for this effect when the document is scaled to ensure stroke width etc are scaled appropriately. */
    virtual void scaleEffect(float factor);


protected:

    /** Write the parameters for this effect to the given XML stream. */
    virtual bool writeSubclassToXmlStream(QXmlStreamWriter &writer);

    /** Read the parameters for this effect from the given Xml Attributes. */
    virtual void readSubclassFromXmlNode(const QDomElement &element);


    // Settings
public:

    /** Get a settings struct from the the effect's parameter hashmap. */
    SFBStrokeEffectSettings getSettings();

    /** Set settings from struct. */
    void setSettings(SFBStrokeEffectSettings settings);

};

#endif // SFBSTROKEEFFECT_H
