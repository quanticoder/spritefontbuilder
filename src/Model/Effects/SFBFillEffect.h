#ifndef SFBFILLEFFECT_H
#define SFBFILLEFFECT_H

#include "SFBEffect.h"
#include "../SFBTypes.h"

struct SFBFillEffectSettings
{
    bool enabled;               //< Is this effect enabled?
    SFBBlendMode blendMode;     //< Blend mode for this effect.
    float opacity;              //< Blend opacity for this effect.
    SFBFillType fillType;       //< Color, Gradient or Pattern Fill?
    QColor color;               //< Color - Used only for Color Fill Mode.
    SFBGradient gradient;       //< Gradient - Used only for Gradient Fill Mode.
    SFBPattern pattern;         //< Pattern Info - Used only for Pattern Fill Mode.
    int inset;                  //< Shrink the path by N pixels before filling (shine effects).

    SFBFillEffectSettings() :
        enabled(true),
        blendMode(SFBBlendMode::Blend_Normal),
        opacity(1.0f),
        fillType(SFBFillType::Fill_Color),
        color(QColor(255,0,0,255)),
        gradient(),
        pattern(),
        inset(0)
    {
        gradient.setDefaultLinearGradient();
    }
};

Q_DECLARE_METATYPE(SFBFillEffectSettings);


class SFBFillEffect : public SFBEffect
{
public:
    typedef std::shared_ptr<SFBFillEffect> Ptr;

private:
    static const QString kPatternElementKey;

public:
    static const QString kFillTypeKey;
    static const QString kColorKey;
    static const QString kGradientKey;
    static const QString kPatternKey;
    static const QString kPatternScaleKey;
    static const QString kInsetKey;



public:
    SFBFillEffect();


    // Abstract Methods
public:

    ///** Get the minimum size required to render this effect on the given glyph. */
    //virtual QSize sizeRequiredForGlyph(SFBGlyph& glyph) = 0;

    /** Return a clone (deep copy) of this effect so that we can duplicate effects on the object. */
    virtual SFBEffect::Ptr clone() { return SFBEffect::Ptr(new SFBFillEffect(*this)); }

    /** Get the amount of padding required over the base size for this glyph to accomodate for this effect. */
    virtual SFBPadding paddingRequiredForGlyph(SFBGlyph& glyph);

    /** Apply this effect to the given glyph. */
    virtual void applyToGlyph(SFBGlyph& glyph);

    /** Before we apply this effect, resize the input pattern if in pattern mode. */
    virtual void willApplyEffectToGlyphs(const QVector<SFBGlyph> &glyphs);

    /** Clear up the cached image. */
    virtual void didApplyEffectToGlyphs();

    /** Set default parameters for this effect. */
    virtual void setDefaultParameters();

    /** Scale the settings for this effect when the document is scaled to ensure stroke width etc are scaled appropriately. */
    virtual void scaleEffect(float factor);


protected:

    /** Write the parameters for this effect to the given XML stream. */
    virtual bool writeSubclassToXmlStream(QXmlStreamWriter &writer);

    /** Read the parameters for this effect from the given Xml Attributes. */
    virtual void readSubclassFromXmlNode(const QDomElement &element);


    // Internal Methods
public:
    /** Get a settings struct from the the effect's parameter hashmap. */
    SFBFillEffectSettings getSettings();

    /** Set settings from struct. */
    void setSettings(SFBFillEffectSettings settings);


    // Private Properties
private:
    QImage mCachedPatternImage;
};

#endif // SFBFILLEFFECT_H
