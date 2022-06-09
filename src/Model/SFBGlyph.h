#ifndef SFBGLYPH_H
#define SFBGLYPH_H

#include <QPainter>
#include "SFBTypes.h"


// Forward Declaration
class SFBEffect;


class SFBGlyph
{
public:
    typedef QPair<QChar, int> KerningPair;

public:
    SFBGlyph();

    SFBGlyph(QFont aFont, QChar aChar);

public:
    void setCharacter(QFont aFont, QChar aChar);

    void expandSizeForEffect(SFBEffect * effect);

    void prepareImages();

    void composeFinalImage();

public:
    QFont font;         //< Font
    QChar character;    //< Input value

    QPainterPath path;  //< Used during the generation process
    QRect minSize;      //< Used during the generation process
    QImage backgroundImage;     //< Used during the generation process to render background shadows
    QImage maskImage;           //< Original glyph painted black, to be used as an exclusion mask for some effects
    QImage tempImage;           //< Available for all effects to use as a buffer (so we don't have to keep creating/destroying images).
    QImage image;       //< Used during the generation process

    QSize baseSize;    //< Size of just the glyph, so we can later calculate difference between the
    SFBPadding effectPadding;   //< How much additional padding have we added on top of the original glyph?
    QRect atlasRect;        //< Output from the generation process

    QVector<KerningPair> kerningPairs;  //< Vector of kerning pairs

private:
    bool mHasPreparedImages;    //< Has the prepareImages method been called?
};

#endif // SFBGLYPH_H
