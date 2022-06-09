#ifndef SFBEFFECT_H
#define SFBEFFECT_H

#include <memory>
#include <QHash>
#include <QVariant>

#include "SFBEffectTypes.h"
#include "../SFBTypes.h"
#include "../SFBGlyph.h"

#include "../SFBFile/SFBDocumentElement.h"


class SFBEffect : public SFBDocumentElement
{
    friend class SFBDocument;

public:
    static const QString kRootKey;
private:
    static const QString kEffectTypeKey;
    static const QString kEnabledKey;
    static const QString kBlendModeKey;
    static const QString kOpacityKey;

public:
    typedef std::shared_ptr<SFBEffect> Ptr;


    // Static Methods
public:
    static SFBEffect::Ptr CreateEffectFromType(const SFBEffectType &effectType);
    static SFBEffect::Ptr CreateEffectFromXmlNode(const QDomElement &element);


    // Object Lifecycle
public:
    SFBEffect();

    void initialize();


    // Abstract Methods
public:

    ///** Get the minimum size required to render this effect on the given glyph. */
    //virtual QSize sizeRequiredForGlyph(SFBGlyph& glyph) = 0;

    /** Return a clone (deep copy) of this effect so that we can duplicate effects on the object. */
    virtual SFBEffect::Ptr clone() = 0;

    /** Get the amount of padding required over the base size for this glyph to accomodate for this effect. */
    virtual SFBPadding paddingRequiredForGlyph(SFBGlyph& glyph) = 0;

    /** Apply this effect to the given glyph. */
    virtual void applyToGlyph(SFBGlyph& glyph) = 0;

    /** Called before the apply method is called from multiple threads, but can be used to setup readonly variables for the process. */
    virtual void willApplyEffectToGlyphs(const QVector<SFBGlyph> &glyphs);

    /** Called after the apply method is called from multiple threads, used for cleaning up. */
    virtual void didApplyEffectToGlyphs();

    /** Setup the default parameters for this effect. */
    virtual void setDefaultParameters() = 0;

    /** Scale the settings for this effect when the document is scaled to ensure stroke width etc are scaled appropriately. */
    virtual void scaleEffect(float factor) = 0;

protected:
    /** Write the parameters for this effect to the given XML stream. */
    virtual bool writeSubclassToXmlStream(QXmlStreamWriter &writer) = 0;

    /** Read the parameters for this effect from the given Xml Attributes. */
    virtual void readSubclassFromXmlNode(const QDomElement &element) = 0;


    // Document Element Methods
public:
    virtual bool writeToXmlStream(QXmlStreamWriter &writer);

protected:
    // Hide this, as we cannot instantiate a generic effect and then change it's type
    virtual void readFromXmlNode(const QDomElement &element);


    // Utility Methods
public:
    /** Get the title for this effect for the user. */
    QString getTitle();

    /** Get a preview icon for this effect. */
    QPixmap getPreview(int width, int height, int pointSize);


    // Managing Settings on this Effect
public:

    /** Get a parameter that has been set on this glyph. */
    QVariant getParameter(const QString& key);

    /** Set a parameter on this glyph. */
    void setParameter(const QString& key, QVariant& value);

    /** Get the underlying type of this effect. */
    SFBEffectType getEffectType() const;

    /** Has this effect got unsaved data on it? */
    bool needsSaving() const;

    /** Set whether this effect should be applied. */
    void setEnabled(bool value);

    /** Get whether this effect should be applied. */
    bool isEnabled() const;

    /** Set the blend mode for this effect. */
    void setBlendMode(SFBBlendMode blendMode);

    /** Get the blend mode for this effect. */
    SFBBlendMode blendMode() const;


    // Internal Methods
protected:

    /** Shrink Path. */
    QPainterPath shrinkPath(const QPainterPath &path, float amount);

    /** Grow Path. */
    QPainterPath growPath(const QPainterPath &path, float amount);

    /** Get composition mode for blend type. */
    QPainter::CompositionMode compositionForBlendMode(const SFBBlendMode& mode);

    /** Set the unsaved data flag. */
    void setNeedsSavingToFalse();


    // Protected Properties
protected:
    bool mEnabled;                   //< Is this action enabled?
    SFBBlendMode mBlendMode;         //< Blend mode for this action.
    float mOpacity;                  //< Blend opacity for this action.

    bool mInitialized;                      //< Must be initialized before first use.
    bool mHasUnsavedData;                   //< Does this effect have unsaved data?
    SFBEffectType mEffectType;              //< Effect type (Fill, Shadow, Stroke).
    QHash<QString,QVariant> mParameters;    //< Effect settings.
};

#endif // SFBEFFECT_H
