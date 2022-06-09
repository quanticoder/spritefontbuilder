#include "SFBEffect.h"

#include "SFBFillEffect.h"
#include "SFBShadowEffect.h"
#include "SFBStrokeEffect.h"


const QString SFBEffect::kRootKey = QString("Effect");
const QString SFBEffect::kEffectTypeKey = QString("effectType");
const QString SFBEffect::kEnabledKey = QString("effectEnabled");
const QString SFBEffect::kBlendModeKey = QString("effectBlendMode");
const QString SFBEffect::kOpacityKey = QString("effectOpacity");


SFBEffect::Ptr SFBEffect::CreateEffectFromType(const SFBEffectType &effectType)
{
    switch(effectType)
    {
        case SFBEffectType::Effect_Fill: return SFBEffect::Ptr(new SFBFillEffect());
        case SFBEffectType::Effect_Shadow: return SFBEffect::Ptr(new SFBShadowEffect());
        case SFBEffectType::Effect_Stroke: return SFBEffect::Ptr(new SFBStrokeEffect());
        default: return SFBEffect::Ptr(nullptr);
    }
}


SFBEffect::Ptr SFBEffect::CreateEffectFromXmlNode(const QDomElement &element)
{
    QString xmlEffectType = element.attribute(kEffectTypeKey);
    SFBEffectType effectType = SFBEffectTypes::EffectTypeFromString(xmlEffectType);

    if ( effectType == SFBEffectType::Effect_Unknown ) {
        return SFBEffect::Ptr(nullptr);
    }

    SFBEffect::Ptr effect = CreateEffectFromType(effectType);

    if ( effect == nullptr ) {
        return effect;
    }

    effect->readFromXmlNode(element);
    return effect;
}


SFBEffect::SFBEffect() :
    mEnabled(true),
    mBlendMode(SFBBlendMode::Blend_Normal),
    mOpacity(1.0f),
    mInitialized(false),
    mEffectType(SFBEffectType::Effect_Unknown)
{

}


void SFBEffect::initialize()
{
    if ( mInitialized ) {
        return;
    }

    setDefaultParameters();

    mInitialized = true;
}


//
// Optional Virtual Method Stubs
//


void SFBEffect::willApplyEffectToGlyphs(const QVector<SFBGlyph> & /*glyphs*/)
{

}


void SFBEffect::didApplyEffectToGlyphs()
{

}


//
// XML Stream Manipulation
//

bool SFBEffect::writeToXmlStream(QXmlStreamWriter &writer)
{
    if ( writer.hasError() ) {
        return false;
    }

    writer.writeStartElement(kRootKey);
    writer.writeAttribute(kEffectTypeKey, SFBEffectTypes::EffectTypeToString(mEffectType));
    writer.writeAttribute(kEnabledKey, QString::number((mEnabled) ? 1 : 0));
    writer.writeAttribute(kBlendModeKey, SFBEffectTypes::BlendModeToString(mBlendMode));
    writer.writeAttribute(kOpacityKey, QString::number(mOpacity));

    writeSubclassToXmlStream(writer);

    writer.writeEndElement();   // </Root>
    return true;
}


void SFBEffect::readFromXmlNode(const QDomElement &element)
{
    initialize();

    QString xmlEnabled = element.attribute(kEnabledKey);
    QString xmlBlendMode = element.attribute(kBlendModeKey);
    QString xmlOpacity = element.attribute(kOpacityKey);

    mEnabled = (xmlEnabled.toInt() == 1);
    mBlendMode = SFBEffectTypes::BlendModeFromString(xmlBlendMode);
    mOpacity = xmlOpacity.toFloat();

    if ( mOpacity < 0 ) { mOpacity = 0.0f; }
    if ( mOpacity > 1.0f ) { mOpacity = 1.0f; }

    readSubclassFromXmlNode(element);
}


//
// Utility Methods
//


QString SFBEffect::getTitle()
{
    return SFBEffectTypes::EffectTypeToString(mEffectType);
}


QPixmap SFBEffect::getPreview(int width, int height, int pointSize)
{
    SFBGlyph glyph = SFBGlyph(QFont("Arial", pointSize), 'A');
    glyph.expandSizeForEffect(this);
    glyph.prepareImages();
    this->applyToGlyph(glyph);
    glyph.composeFinalImage();

    QPixmap icon = QPixmap(width, height);
    icon.fill(QColor(0,0,0,0));

    int x = (width * 0.5f) - (glyph.image.width() * 0.5f);
    int y = (height * 0.5f) - (glyph.image.height() * 0.5f);

    QPainter painter;
    painter.begin(&icon);
    painter.drawImage(x, y, glyph.image);
    painter.end();

    return icon;
}


//
// Parameters
//

QVariant SFBEffect::getParameter(const QString& key)
{
    if ( !mInitialized ) {
        initialize();
    }

    return mParameters[key];
}


void SFBEffect::setParameter(const QString& key, QVariant& value)
{
    if ( !mInitialized ) {
        initialize();
    }

   mParameters[key] = value;
   mHasUnsavedData = true;
}


SFBEffectType SFBEffect::getEffectType() const
{
    return mEffectType;
}


bool SFBEffect::needsSaving() const
{
    return mHasUnsavedData;
}


void SFBEffect::setEnabled(bool value)
{
    mEnabled = value;
    mHasUnsavedData = true;
}


bool SFBEffect::isEnabled() const
{
    return mEnabled;
}


void SFBEffect::setBlendMode(SFBBlendMode blendMode)
{
    mBlendMode = blendMode;
    mHasUnsavedData = true;
}


SFBBlendMode SFBEffect::blendMode() const
{
    return mBlendMode;
}


QPainterPath SFBEffect::shrinkPath(const QPainterPath &path, float amount)
{
    QPainterPathStroker stroker;
    stroker.setWidth(2 * amount);

    QPainterPath strokePath = stroker.createStroke(path);
    return path.subtracted(strokePath);
}


QPainterPath SFBEffect::growPath(const QPainterPath &path, float amount)
{
    QPainterPathStroker stroker;
    stroker.setWidth(2 * amount);

    QPainterPath strokePath = stroker.createStroke(path);
    return path.united(strokePath);
}


QPainter::CompositionMode SFBEffect::compositionForBlendMode(const SFBBlendMode& mode)
{
    switch(mode)
    {
    case SFBBlendMode::Blend_Unknown: return QPainter::CompositionMode_SourceOver;
    case SFBBlendMode::Blend_Normal: return QPainter::CompositionMode_SourceOver;
    case SFBBlendMode::Blend_Multiply: return QPainter::CompositionMode_Multiply;
    case SFBBlendMode::Blend_Screen: return QPainter::CompositionMode_Screen;
    case SFBBlendMode::Blend_Overlay: return QPainter::CompositionMode_Overlay;
    case SFBBlendMode::Blend_Lighten: return QPainter::CompositionMode_Lighten;
    case SFBBlendMode::Blend_Darken: return QPainter::CompositionMode_Darken;
    case SFBBlendMode::Blend_ColorDodge: return QPainter::CompositionMode_ColorDodge;
    case SFBBlendMode::Blend_ColorBurn: return QPainter::CompositionMode_ColorBurn;
    case SFBBlendMode::Blend_SoftLight: return QPainter::CompositionMode_SoftLight;
    case SFBBlendMode::Blend_HardLight: return QPainter::CompositionMode_HardLight;
    case SFBBlendMode::Blend_Difference: return QPainter::CompositionMode_Difference;
    case SFBBlendMode::Blend_Exclusion: return QPainter::CompositionMode_Exclusion;
    }

    return QPainter::CompositionMode_SourceOver;
}


void SFBEffect::setNeedsSavingToFalse()
{
    mHasUnsavedData = false;
}
