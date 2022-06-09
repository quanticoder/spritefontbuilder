#ifndef SFBFONTEXPORTER_H
#define SFBFONTEXPORTER_H

#include <QPair>
#include <string>
#include <memory>
#include "SFBFontExporterTypes.h"
#include "../SFBSpriteFont.h"


// Forward Declaration
class SFBDocument;
class SFBExportSettings;


/**
 * @brief An abstract base class for exporters that can save a sprite font.
 */
class SFBFontExporter
{
    // Typedefs
public:
    typedef std::shared_ptr<SFBFontExporter> Ptr;

    // Constants
public:
    static QString kExportFileTypeUnknown;
    static QString kExportFileTypeBMFontAscii;

    // Static Methods
public:
    static QString ExportFileTypeToUITitle(SFBExportFileType value);
    static QString ExportFileTypeToString(SFBExportFileType value);
    static SFBExportFileType ExportFileTypeFromString(QString value);
    static QVector<SFBExportFileType> ExportFileTypeList();
    static SFBFontExporter::Ptr FontExporterForSettings(const SFBExportSettings &settings);

public:
    SFBFontExporter();

public:
    virtual bool exportFont(SFBDocument *doc) = 0;

    const std::string & getLastError() { return mLastError; }

    /** Given a path (which could be just a filename, a relative path or an absolute path) retrieve the desired save path. */
    QString fullPathFromExportPath(SFBDocument *doc, const QString & path);

protected:
    std::string mLastError;
};

#endif // SFBFONTEXPORTER_H
