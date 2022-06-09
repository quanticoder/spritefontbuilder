#include "SFBFontExporter.h"

#include <QDir>
#include <QFileInfo>
#include <QApplication>

#include "SFBFontExporters.h"
#include "SFBFontExporterTypes.h"
#include "../SFBDocument.h"
#include "../SFBExportSettings.h"

//
// Constants
//

QString SFBFontExporter::kExportFileTypeUnknown = QString("Unknown");
QString SFBFontExporter::kExportFileTypeBMFontAscii = QString("BMFont_Ascii");


//
// Static Methods
//


QString SFBFontExporter::ExportFileTypeToUITitle(SFBExportFileType value)
{
    switch( value )
    {
        case ExportType_BMFont_Ascii: return QString("BMFont (Ascii)");
        default: break;
    }

    return kExportFileTypeUnknown;
}


QString SFBFontExporter::ExportFileTypeToString(SFBExportFileType value)
{
    switch( value )
    {
        case ExportType_BMFont_Ascii: return kExportFileTypeBMFontAscii;
        default: break;
    }

    return "Unknown";
}


SFBExportFileType SFBFontExporter::ExportFileTypeFromString(QString value)
{
    if ( value.compare(kExportFileTypeBMFontAscii, Qt::CaseInsensitive) == 0 ) {
        return ExportType_BMFont_Ascii;
    }

    return ExportType_Unknown;
}


QVector<SFBExportFileType> SFBFontExporter::ExportFileTypeList()
{
    QVector<SFBExportFileType> list;
    list.append(ExportType_BMFont_Ascii);
    return list;
}


SFBFontExporter::Ptr SFBFontExporter::FontExporterForSettings(const SFBExportSettings &settings)
{
    if ( settings.format == ExportType_BMFont_Ascii ) {
        return SFBFontExporterBMFont::Ptr(new SFBFontExporterBMFont());
    }

    return SFBFontExporter::Ptr(nullptr);
}


//
// Object Lifecycle
//


SFBFontExporter::SFBFontExporter()
{

}


QString SFBFontExporter::fullPathFromExportPath(SFBDocument * doc, const QString &path)
{
    if ( path.isNull() ) {
        return QString();
    }

    QFileInfo pathInfo = QFileInfo(path);

    if ( pathInfo.isAbsolute() ) {
        return path;
    }

    // Relative path, first attempt to make it relative to the document path folder
    QString rootPath = doc->documentPath();

    if ( rootPath.isNull() ) {
        rootPath = QDir::homePath();
    }

    QFileInfo rootInfo = QFileInfo(rootPath);
    QDir rootDir = (rootInfo.isDir()) ? QDir(rootPath) : QFileInfo(rootPath).dir();
    QString finalPath = rootDir.absolutePath() + "/" + path;
    return finalPath;
}
