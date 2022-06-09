#ifndef SFBSPRITEFONT_H
#define SFBSPRITEFONT_H

#include <QVector>
#include <QImage>
#include "SFBGlyph.h"

struct SFBSpriteFont
{
    QVector<SFBGlyph> glyphs;
    QImage textureAtlas;
    bool doGlyphsFit;
    bool isNull;
    double generationTime;

    SFBSpriteFont() :
        textureAtlas(),
        doGlyphsFit(false),
        isNull(true)
    { }
};

#endif // SFBSPRITEFONT_H
