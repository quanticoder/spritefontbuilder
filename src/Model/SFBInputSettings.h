#ifndef SFBINPUTSETTINGS_H
#define SFBINPUTSETTINGS_H

#include <QtCore>

#include "SFBFile/SFBDocumentElement.h"

class SFBInputSettings : SFBDocumentElement
{
public:
    static const QString kCharacterSetNehe;
    static const QString kCharacterSetAscii;

    static const QString kRootKey;

private:
    static const QString kFontFamilyKey;
    static const QString kFontStyleKey;
    static const QString kFontSizeKey;
    static const QString kCharactersKey;

public:
    SFBInputSettings();
    SFBInputSettings(const QDomElement & element);

    QVector<QChar> uniqueCharacters();

public:
    virtual bool writeToXmlStream(QXmlStreamWriter &writer);
    virtual void readFromXmlNode(const QDomElement & element);

public:
    QString fontFamily;     //< Name of the font with which to generate the atlas.
    QString fontStyle;      //< Text name of the style (Bold, Italic, Black, Condensed, etc)
    float fontSize;         //< Size of the font to generate.
    QString characters;     //< Which characters will be included?
};

Q_DECLARE_METATYPE(SFBInputSettings)

#endif // SFBINPUTSETTINGS_H
