#include "SFBFillEffect.h"

#include <cmath>

const QString SFBFillEffect::kPatternElementKey = QString("FillPattern");

const QString SFBFillEffect::kFillTypeKey = QString("fillType");
const QString SFBFillEffect::kColorKey = QString("fillColor");
const QString SFBFillEffect::kGradientKey = QString("fillGradient");
const QString SFBFillEffect::kPatternKey = QString("fillPattern");
const QString SFBFillEffect::kPatternScaleKey = QString("scale");
const QString SFBFillEffect::kInsetKey = QString("fillInset");


SFBFillEffect::SFBFillEffect()
{
    mEffectType = SFBEffectType::Effect_Fill;
}


void SFBFillEffect::scaleEffect(float factor)
{
    SFBFillEffectSettings settings = getSettings();
    settings.inset *= factor;
    setSettings(settings);
}


SFBPadding SFBFillEffect::paddingRequiredForGlyph(SFBGlyph& /*glyph*/)
{
    return SFBPadding(0,0,0,0);
}


void SFBFillEffect::applyToGlyph(SFBGlyph& glyph)
{
    SFBFillEffectSettings settings = getSettings();
    QPainterPath path = glyph.path;

    if ( settings.inset > 0 )
    {
        path = shrinkPath(path, settings.inset);
    }

    QPainter painter;
    painter.begin(&(glyph.image));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setOpacity(settings.opacity);
    painter.setCompositionMode(compositionForBlendMode(mBlendMode));

    if ( settings.fillType == SFBFillType::Fill_Color )
    {
        painter.fillPath(path, settings.color);
    }
    else if ( settings.fillType == SFBFillType::Fill_Gradient )
    {
        settings.gradient.fillPath(painter, path);
    }
    else if ( settings.fillType == SFBFillType::Fill_Pattern )
    {
        painter.fillPath(path, QBrush(mCachedPatternImage));
    }

    painter.end();
}


void SFBFillEffect::willApplyEffectToGlyphs(const QVector<SFBGlyph> &glyphs)
{
    SFBFillEffectSettings settings = getSettings();

    if ( settings.fillType == SFBFillType::Fill_Pattern && !settings.pattern.image.isNull() )
    {
        QSize maxSize = QSize(0,0);

        for( auto it = glyphs.constBegin(); it != glyphs.constEnd(); ++it )
        {
            const SFBGlyph & glyph = (*it);

            if ( glyph.minSize.width() > maxSize.width() ) {
                maxSize.setWidth(glyph.minSize.width());
            }

            if ( glyph.minSize.height() > maxSize.height() ) {
                maxSize.setHeight(glyph.minSize.height());
            }
        }

        mCachedPatternImage = settings.pattern.image.scaled(maxSize);
    }
}


void SFBFillEffect::didApplyEffectToGlyphs()
{
    mCachedPatternImage = QImage();
}


void SFBFillEffect::setDefaultParameters()
{
    setSettings(SFBFillEffectSettings());
}


SFBFillEffectSettings SFBFillEffect::getSettings()
{
    SFBFillEffectSettings settings;
    settings.enabled = mEnabled;
    settings.blendMode = mBlendMode;
    settings.opacity = mOpacity;
    settings.fillType = mParameters[kFillTypeKey].value<SFBFillType>();
    settings.color = mParameters[kColorKey].value<QColor>();
    settings.gradient = mParameters[kGradientKey].value<SFBGradient>();
    settings.pattern = mParameters[kPatternKey].value<SFBPattern>();
    settings.inset = mParameters[kInsetKey].toInt();

    return settings;
}


bool SFBFillEffect::writeSubclassToXmlStream(QXmlStreamWriter &writer)
{
    SFBFillEffectSettings settings = getSettings();

    writer.writeAttribute(kFillTypeKey, SFBEffectTypes::FillTypeToString(settings.fillType));
    writer.writeAttribute(kColorKey, settings.color.name(QColor::HexArgb));
    writer.writeAttribute(kGradientKey, settings.gradient.stringRepresentation());
    writer.writeAttribute(kInsetKey, QString::number(settings.inset));

    if ( settings.fillType == Fill_Pattern )
    {
        QByteArray imgData;
        QBuffer buffer(&imgData);
        buffer.open(QBuffer::WriteOnly);
        settings.pattern.image.save(&buffer, "PNG");

        writer.writeStartElement(kPatternElementKey);
        writer.writeAttribute(kPatternScaleKey, QString::number(settings.pattern.scale));
        writer.writeCDATA(imgData.toBase64());
        writer.writeEndElement();
    }

    return true;
}


void SFBFillEffect::readSubclassFromXmlNode(const QDomElement &element)
{
    SFBFillEffectSettings settings = getSettings();

    QString xmlFillType = element.attribute(kFillTypeKey);
    QString xmlColor = element.attribute(kColorKey);
    QString xmlGradient = element.attribute(kGradientKey);
    QString xmlInset = element.attribute(kInsetKey);

    if ( !xmlFillType.isEmpty() )   { settings.fillType = SFBEffectTypes::FillTypeFromString(xmlFillType); }
    if ( !xmlColor.isEmpty() )      { settings.color = QColor(xmlColor); }
    if ( !xmlGradient.isEmpty() )   { settings.gradient = SFBGradient(xmlGradient); }
    if ( !xmlInset.isEmpty() )      { settings.inset = xmlInset.toInt(); }

    QDomElement patternNode = element.firstChildElement(kPatternElementKey);

    if ( !patternNode.isNull() )
    {
        QByteArray base64Image = patternNode.text().toUtf8();
        QByteArray imgData = QByteArray::fromBase64(base64Image);
        QImage image = QImage::fromData(imgData, "PNG");

        if ( !image.isNull() ) {
            settings.pattern.image = image;
        }

        QString xmlPatternScale = patternNode.attribute(kPatternScaleKey);

        if ( !xmlPatternScale.isEmpty() ) {
            settings.pattern.scale = xmlPatternScale.toFloat();
        }
    }

    setSettings(settings);
}


void SFBFillEffect::setSettings(SFBFillEffectSettings settings)
{
    mEnabled = settings.enabled;
    mBlendMode = settings.blendMode;
    mOpacity = settings.opacity;

    mParameters[kFillTypeKey] = QVariant::fromValue<SFBFillType>(settings.fillType);
    mParameters[kColorKey] = QVariant::fromValue<QColor>(settings.color);
    mParameters[kGradientKey] = QVariant::fromValue<SFBGradient>(settings.gradient);
    mParameters[kPatternKey] = QVariant::fromValue<SFBPattern>(settings.pattern);
    mParameters[kInsetKey] = QVariant::fromValue<int>(settings.inset);
}

