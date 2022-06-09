#include "SFBFontExporterPNG.h"

#include "../SFBDocument.h"

SFBFontExporterPNG::SFBFontExporterPNG()
{

}


bool SFBFontExporterPNG::exportFont(SFBDocument *doc)
{
    QString path = doc->getExportSettings().path;

    if ( doc == nullptr || doc->getSpriteFont().isNull ) {
        mLastError = "Null document or document with null sprite font provided for exporting.";
        return false;
    }

    if ( path.isNull() ) {
        mLastError = "Invalid path provided.";
        return false;
    }

    bool result = doc->getSpriteFont().textureAtlas.save(path, "PNG");

    if ( result == false ) {
        mLastError = "Save failed.";
    }

    return result;
}
