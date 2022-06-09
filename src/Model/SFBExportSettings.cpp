#include "SFBExportSettings.h"

#include "Exporters/SFBFontExporter.h"

const QString SFBExportSettings::kRootKey = QString("ExportSettings");
const QString SFBExportSettings::kFormatKey = QString("format");
const QString SFBExportSettings::kPathKey = QString("path");
const QString SFBExportSettings::kRetinaCopyKey = QString("retinaCopy");


SFBExportSettings::SFBExportSettings() :
    format(ExportType_Unknown),
    path(),
    retinaCopy(false)
{

}


SFBExportSettings::SFBExportSettings(const QDomElement &element) :
    format(ExportType_Unknown),
    path(),
    retinaCopy(false)
{
    readFromXmlNode(element);
}


bool SFBExportSettings::writeToXmlStream(QXmlStreamWriter &writer)
{
    if ( writer.hasError() ) {
        return false;
    }

    writer.writeStartElement(kRootKey);
    writer.writeAttribute(kFormatKey, SFBFontExporter::ExportFileTypeToString(format));
    writer.writeAttribute(kPathKey, path);
    writer.writeAttribute(kRetinaCopyKey, QString::number((retinaCopy == true) ? 1 : 0));
    writer.writeEndElement();

    return true;
}


void SFBExportSettings::readFromXmlNode(const QDomElement &element)
{
    QString xmlFormat = element.attribute(kFormatKey);
    QString xmlPath = element.attribute(kPathKey);
    QString xmlRetinaCopy = element.attribute(kRetinaCopyKey);

    if ( !xmlFormat.isEmpty() ) { format = SFBFontExporter::ExportFileTypeFromString(xmlFormat); }
    if ( !xmlPath.isEmpty() ) { path = xmlPath; }
    if ( !xmlRetinaCopy.isEmpty() ) { retinaCopy = (xmlRetinaCopy.toInt() == 1); }
}

