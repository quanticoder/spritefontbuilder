#include "SFBGenerationSettings.h"


const QString SFBGenerationSettings::kRootKey = QString("GenerationSettings");
const QString SFBGenerationSettings::kWidthKey = QString("width");
const QString SFBGenerationSettings::kHeightKey = QString("height");
const QString SFBGenerationSettings::kColorKey = QString("color");
const QString SFBGenerationSettings::kPaddingKey = QString("padding");
const QString SFBGenerationSettings::kSpacingKey = QString("spacing");


SFBGenerationSettings::SFBGenerationSettings() :
    width(0),
    height(0),
    color(QColor(0,0,0,0)),
    padding(2),
    spacing(2)
{

}


SFBGenerationSettings::SFBGenerationSettings(const QDomElement & element) :
    width(0),
    height(0),
    color(QColor(0,0,0,0)),
    padding(2),
    spacing(2)
{
    readFromXmlNode(element);
}


bool SFBGenerationSettings::writeToXmlStream(QXmlStreamWriter &writer)
{
    if ( writer.hasError() ) {
        return false;
    }

    writer.writeStartElement(kRootKey);
    writer.writeAttribute(kWidthKey, QString::number(width));
    writer.writeAttribute(kHeightKey, QString::number(height));
    writer.writeAttribute(kColorKey, color.name(QColor::HexArgb));
    writer.writeAttribute(kPaddingKey, QString::number(padding));
    writer.writeAttribute(kSpacingKey, QString::number(spacing));
    writer.writeEndElement();

    return true;
}


void SFBGenerationSettings::readFromXmlNode(const QDomElement &element)
{
    QString xmlWidth = element.attribute(kWidthKey);
    QString xmlHeight = element.attribute(kHeightKey);
    QString xmlColor = element.attribute(kColorKey);
    QString xmlPadding = element.attribute(kPaddingKey);
    QString xmlSpacing = element.attribute(kSpacingKey);

    if ( !xmlWidth.isEmpty() ) { width = xmlWidth.toInt(); }
    if ( !xmlHeight.isEmpty() ) { height = xmlHeight.toInt(); }
    if ( !xmlColor.isEmpty() ) { color = QColor(xmlColor); }
    if ( !xmlPadding.isEmpty() ) { padding = xmlPadding.toInt(); }
    if ( !xmlSpacing.isEmpty() ) { spacing = xmlSpacing.toInt(); }
}
