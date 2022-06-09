#ifndef SFBTYPES_H
#define SFBTYPES_H

#include <QtCore>
#include <QtGui>
#include <QColor>

#include "SFBGradient.h"

#include "SFBInputSettings.h"
#include "SFBGenerationSettings.h"
#include "SFBExportSettings.h"

struct SFBPadding
{
    float top;
    float right;
    float bottom;
    float left;

    SFBPadding(float paddingTop, float paddingRight, float paddingBottom, float paddingLeft) {
        top = paddingTop;
        right = paddingRight;
        bottom = paddingBottom;
        left = paddingLeft;
    }

    SFBPadding() {
        top = right = bottom = left = 0.f;
    }
};

Q_DECLARE_METATYPE(SFBPadding)


struct SFBPattern
{
    QImage image;           //< Raw image data to use as the pattern.
    float scale;            //< Scale of the provided image.
};

Q_DECLARE_METATYPE(SFBPattern)


#endif // SFBTYPES_H
