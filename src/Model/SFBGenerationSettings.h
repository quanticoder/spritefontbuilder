#ifndef SFBGENERATIONSETTINGS_H
#define SFBGENERATIONSETTINGS_H

#include <QtCore>
#include <QColor>

#include "SFBFile/SFBDocumentElement.h"

class SFBGenerationSettings : SFBDocumentElement
{
public:
    static const QString kRootKey;

private:
    static const QString kWidthKey;
    static const QString kHeightKey;
    static const QString kColorKey;
    static const QString kPaddingKey;
    static const QString kSpacingKey;

public:
    SFBGenerationSettings();
    SFBGenerationSettings(const QDomElement & element);

public:
    virtual bool writeToXmlStream(QXmlStreamWriter &writer);
    virtual void readFromXmlNode(const QDomElement &element);

public:
    int width;          //< -1 for auto, otherwise measured in pixels.
    int height;         //< -1 for auto, otherwise measured in pixels.
    QColor color;       //< Color of background texture.
    int padding;        //< Pixel padding around the edge of the texture.
    int spacing;        //< Minimum pixel spacing between glyphs.

};

Q_DECLARE_METATYPE(SFBGenerationSettings)

#endif // SFBGENERATIONSETTINGS_H
