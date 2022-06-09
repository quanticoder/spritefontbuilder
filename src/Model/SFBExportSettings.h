#ifndef SFBEXPORTSETTINGS_H
#define SFBEXPORTSETTINGS_H

#include <QtCore>

#include "SFBFile/SFBDocumentElement.h"
#include "Exporters/SFBFontExporterTypes.h"

class SFBExportSettings : public SFBDocumentElement
{
public:
    static const QString kRootKey;

private:
    static const QString kFormatKey;
    static const QString kPathKey;
    static const QString kRetinaCopyKey;

public:
    SFBExportSettings();
    SFBExportSettings(const QDomElement & element);

public:
    virtual bool writeToXmlStream(QXmlStreamWriter &writer);
    virtual void readFromXmlNode(const QDomElement & element);

public:
    SFBExportFileType format;     //< File Format to Export
    QString path;       //< Path
    bool retinaCopy;    //< Should we also save a retina @2x version?
};

Q_DECLARE_METATYPE(SFBExportSettings)

#endif // SFBEXPORTSETTINGS_H
