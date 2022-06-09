#ifndef SFBFONTEXPORTERPNG_H
#define SFBFONTEXPORTERPNG_H

#include "SFBFontExporter.h"

class SFBFontExporterPNG : public SFBFontExporter
{
public:
    SFBFontExporterPNG();


public:
    virtual bool exportFont(SFBDocument *doc);
};

#endif // SFBFONTEXPORTERPNG_H
