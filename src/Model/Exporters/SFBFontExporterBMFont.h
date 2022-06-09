#ifndef SFBFONTEXPORTERBMFONT_H
#define SFBFONTEXPORTERBMFONT_H

#include "SFBFontExporter.h"

class SFBFontExporterBMFont : public SFBFontExporter
{
public:
    SFBFontExporterBMFont();


public:
    virtual bool exportFont(SFBDocument *doc);

private:
    bool writeDocumentToPath(SFBDocument * doc, const QString &path);
};

#endif // SFBFONTEXPORTERBMFONT_H
