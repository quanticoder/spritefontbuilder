#ifndef SFBFONTEXPORTERTYPES_H
#define SFBFONTEXPORTERTYPES_H

#include <QColor>
#include <QVector>
#include <QImage>

enum SFBExportFileType
{
    ExportType_Unknown,
    ExportType_BMFont_Ascii
};

Q_DECLARE_METATYPE(SFBExportFileType)

#endif // SFBFONTEXPORTERTYPES_H
