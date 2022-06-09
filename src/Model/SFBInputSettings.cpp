#include "SFBInputSettings.h"


const QString SFBInputSettings::kCharacterSetNehe = QString(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
const QString SFBInputSettings::kCharacterSetAscii = QString(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþ");

const QString SFBInputSettings::kRootKey = QString("InputSettings");
const QString SFBInputSettings::kFontFamilyKey = QString("fontFamily");
const QString SFBInputSettings::kFontStyleKey = QString("fontStyle");
const QString SFBInputSettings::kFontSizeKey = QString("fontSize");
const QString SFBInputSettings::kCharactersKey = QString("characters");


SFBInputSettings::SFBInputSettings() :
    fontFamily("Arial"),
    fontSize(64),
    characters(kCharacterSetNehe)
{

}


SFBInputSettings::SFBInputSettings(const QDomElement &element) :
    fontFamily("Arial"),
    fontSize(64),
    characters(kCharacterSetNehe)
{
    readFromXmlNode(element);
}


QVector<QChar> SFBInputSettings::uniqueCharacters()
{
    QVector<QChar> results;

    for( int i = 0, iSize = characters.size(); i < iSize; ++i )
    {
        QChar c = characters[i];

        if (results.contains(c) == false)
        {
            results.append(c);
        }
    }

    return results;
}


bool SFBInputSettings::writeToXmlStream(QXmlStreamWriter &writer)
{
    if ( writer.hasError() ) {
        return false;
    }

    writer.writeStartElement(kRootKey);
    writer.writeAttribute(kFontFamilyKey, fontFamily);
    writer.writeAttribute(kFontStyleKey, fontStyle);
    writer.writeAttribute(kFontSizeKey, QString::number(fontSize));
    writer.writeAttribute(kCharactersKey, characters);
    writer.writeEndElement();

    return true;
}


void SFBInputSettings::readFromXmlNode(const QDomElement &element)
{
    QString xmlFontFamily = element.attribute(kFontFamilyKey);
    QString xmlFontStyle = element.attribute(kFontStyleKey);
    QString xmlFontSize = element.attribute(kFontSizeKey);
    QString xmlCharacters = element.attribute(kCharactersKey);

    if ( !xmlFontFamily.isEmpty() ) {
        fontFamily = xmlFontFamily;
    }

    if ( !xmlFontStyle.isEmpty() ) {
        fontStyle = xmlFontStyle;
    }

    if ( !xmlFontSize.isEmpty() ) {
        fontSize = xmlFontSize.toFloat();
    }

    if ( !xmlCharacters.isEmpty() ) {
        characters = xmlCharacters;
    }
}
