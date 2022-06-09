#include "SFBDocument.h"

#include <QApplication>
#include <QtCore>
#include <QtConcurrent>
#include <QtXml>
#include <cmath>

//
// Prepare Glyph Operation is a Functor for easy multi-tasking
//

void SFBPrepareGlyphFunctor::operator()(SFBGlyph& glyph)
{
    glyph.prepareImages();
}


void SFBComposeFinalGlyphFunctor::operator ()(SFBGlyph& glyph)
{
    glyph.composeFinalImage();
}


void SFBApplyEffectFunctor::operator ()(SFBGlyph& glyph)
{
    if ( glyph.image.width() == 0 || glyph.image.height() == 0 ) {
        return;
    }

    mEffect->applyToGlyph(glyph);
}


SFBDocument::SFBDocument(QObject *parent) :
    QObject(parent)
{

}


void SFBDocument::initDefaultDocument()
{
    SFBEffect::Ptr fill1 = SFBEffect::Ptr(new SFBFillEffect());
    addEffect(fill1);

    SFBShadowEffect::Ptr shadow1 = SFBShadowEffect::Ptr(new SFBShadowEffect());
    addEffect(shadow1);

    SFBEffect::Ptr stroke = SFBEffect::Ptr(new SFBStrokeEffect());
    addEffect(stroke);
}


SFBDocument::Ptr SFBDocument::clone()
{
    SFBDocument::Ptr copy = SFBDocument::Ptr(new SFBDocument());
    copy->setInputSettings(getInputSettings());
    copy->setGenerationSettings(getGenerationSettings());
    copy->setExportSettings(getExportSettings());

    for( SFBEffect::Ptr effect : mEffects )
    {
        copy->addEffect(effect->clone());
    }

    return copy;
}


const SFBSpriteFont & SFBDocument::getSpriteFont()
{
    if ( mSpriteFont.isNull || mIsDirty ) {
        generateSpriteFont();
    }

    return mSpriteFont;
}


SFBInputSettings SFBDocument::getInputSettings()
{
    return mInputSettings;
}


void SFBDocument::setInputSettings(const SFBInputSettings& settings)
{
    mInputSettings = settings;
    mIsDirty = true;
    mHasUnsavedData = true;
}


SFBGenerationSettings SFBDocument::getGenerationSettings()
{
    return mGenerationSettings;
}


void SFBDocument::setGenerationSettings(const SFBGenerationSettings& settings)
{
    mGenerationSettings = settings;
    mIsDirty = true;
    mHasUnsavedData = true;
}


SFBExportSettings SFBDocument::getExportSettings()
{
    return mExportSettings;
}


void SFBDocument::setExportSettings(const SFBExportSettings &settings)
{
    mExportSettings = settings;
    //mIsDirty = true;          // Export settings don't affect the atlas
    mHasUnsavedData = true;
}


int SFBDocument::getEffectCount()
{
    return mEffects.count();
}


SFBEffect::Ptr SFBDocument::getEffectAtIndex(int index)
{
    return mEffects[index];
}


void SFBDocument::addEffect(SFBEffect::Ptr effect)
{
    effect->initialize();
    mEffects.push_back(effect);
}


void SFBDocument::insertEffect(int index, SFBEffect::Ptr effect)
{
    effect->initialize();
    mEffects.insert(index, effect);
    mHasUnsavedData = true;
}


void SFBDocument::removeEffect(int index)
{
    mEffects.removeAt(index);
    mHasUnsavedData = true;
}


void SFBDocument::removeEffect(SFBEffect::Ptr effect)
{
    int count = mEffects.count();
    int index = -1;

    for( int i = 0; i < count; ++i ) {
        if ( mEffects[i].get() == effect.get() ) {
           index = i;
        }
    }

    if ( index >= 0 ) {
        removeEffect(index);
    }

    mHasUnsavedData = true;
}


void SFBDocument::scale(float scale)
{
    mInputSettings.fontSize *= scale;

    for( SFBEffect::Ptr effect : mEffects )
    {
        effect->scaleEffect(scale);
    }
}


bool SFBDocument::needsSaving()
{
    return mHasUnsavedData;
}


void SFBDocument::generateSpriteFont()
{
    mSpriteFont = SFBSpriteFont();
    mSpriteFont.isNull = false;

    if ( !generateGlyphs() ) {
        mSpriteFont.isNull = true;
        return;
    }

    if ( !layoutGlyphsOnAtlas() ) {
        mSpriteFont.isNull = true;
        return;
    }

    emit spriteFontUpdated(mSpriteFont);
}


void SFBDocument::setDocumentPath(const QString &path)
{
    mDocumentPath = path;
}


QString SFBDocument::documentPath()
{
    return mDocumentPath;
}


QSize SFBDocument::calcTextureSizeForGlyphs(QVector<SFBGlyph>& glyphs, int fixedWidth = 0, int fixedHeight = 0)
{
    // Special case - if no auto axis then we do nothing
    if ( fixedWidth > 0 && fixedHeight > 0 ) {
        return QSize(fixedWidth, fixedHeight);
    }

    // Just use a trial and error system for now, there are only a few options to try
    QSize atlasSize;
    atlasSize.setWidth((fixedWidth > 0) ? fixedWidth : 128);
    atlasSize.setHeight((fixedHeight > 0) ? fixedHeight : 128);

    int maxWidth = 4096;
    int maxHeight = 4096;

    // Must use a local copy of the input settings so we can change the size
    SFBGenerationSettings generationSettings = mGenerationSettings;

    bool didFit = false;

    while( didFit == false )
    {
        generationSettings.width = atlasSize.width();
        generationSettings.height = atlasSize.height();
        didFit = layoutGlyphs(mSpriteFont, generationSettings, false);

        if ( didFit == false )
        {
            if ( fixedWidth > 0 )
            {
                atlasSize.setHeight(atlasSize.height() * 2);
            }
            else if ( fixedHeight > 0 )
            {
                atlasSize.setWidth(atlasSize.width() * 2);
            }
            else if ( (atlasSize.width() < maxWidth) && (atlasSize.width() <= atlasSize.height()) )
            {
                atlasSize.setWidth(atlasSize.width() * 2);
            }
            else if ( (atlasSize.height() < maxHeight) && (atlasSize.height() <= atlasSize.height()) )
            {
                atlasSize.setHeight(atlasSize.height() * 2);
            }
            else
            {
                break;
            }
        }
    }

    return atlasSize;
}


bool SFBDocument::generateGlyphs()
{
    //qDebug() << "Generating Glyphs: " << QTime::currentTime();
    mSpriteFont.glyphs.clear();

    // Let's see how long this process takes!
    QElapsedTimer timer;
    timer.start();

    // Variables for the process
    QFont glyphFont;

    if ( mInputSettings.fontStyle.isEmpty() )
    {
        glyphFont = QFont(mInputSettings.fontFamily, mInputSettings.fontSize);
    }
    else
    {
        glyphFont = mFontDatabase.font(mInputSettings.fontFamily, mInputSettings.fontStyle, mInputSettings.fontSize);
    }

    QVector<QChar> characters = mInputSettings.uniqueCharacters();

    for( int i = 0, iSize = characters.size(); i < iSize; ++i )
    {
        SFBGlyph glyph(glyphFont, characters[i]);
        mSpriteFont.glyphs.push_back(glyph);
    }

    // Calculate Glyph Size to fit all effects
    for( SFBGlyph & glyph : mSpriteFont.glyphs )
    {
        for( int j = 0; j < mEffects.size(); ++j )
        {
            if ( mEffects[j]->isEnabled() == false ) {
                continue;
            }

            glyph.expandSizeForEffect(mEffects[j].get());
        }
    }    

    // Prepare all glyphs concurrently
    SFBPrepareGlyphFunctor prepareGlyphFunctor;
    QtConcurrent::blockingMap(mSpriteFont.glyphs, prepareGlyphFunctor);

    // Apply Effects
    for( auto it = mEffects.begin(); it != mEffects.end(); ++it )
    {
        SFBEffect::Ptr effect = (*it);

        if ( effect->isEnabled() == false ) {
            continue;
        }

        effect->willApplyEffectToGlyphs(mSpriteFont.glyphs);

        SFBApplyEffectFunctor applyEffect(effect.get());
        QtConcurrent::blockingMap(mSpriteFont.glyphs, applyEffect);

        effect->didApplyEffectToGlyphs();
    }

    // Compose from background and foreground layers
    SFBComposeFinalGlyphFunctor composeGlyphFunctor;
    QtConcurrent::blockingMap(mSpriteFont.glyphs, composeGlyphFunctor);

    // Calculate kerning info in a really dodgy way!
    QPainter painter;
    glyphFont.setKerning(true);
    glyphFont.setHintingPreference(QFont::PreferNoHinting);
    QFontInfo fontInfo = QFontInfo(glyphFont);
    QFontMetricsF fontMetrics = QFontMetricsF(glyphFont);
    int kerningCount = 0;

    for( SFBGlyph & glyph : mSpriteFont.glyphs )
    {
        for( QChar & c : mInputSettings.characters )
        {
            if ( glyph.character == c ) {
                continue;
            }

            QString charPair = QString(glyph.character) + QString(c);
            float expectedWidth = fontMetrics.width(glyph.character) + fontMetrics.width(c);
            float actualWidth = fontMetrics.width(charPair);

            if ( fabsf(expectedWidth - actualWidth) > 1.0f ) {
                kerningCount++;
                SFBGlyph::KerningPair pair = SFBGlyph::KerningPair(c, (int)(actualWidth - expectedWidth));
                glyph.kerningPairs.append(pair);
            }
        }
    }

    //qDebug() << "Kerning Count: " << kerningCount;

    mSpriteFont.generationTime = (double)timer.nsecsElapsed() / 1000000000.0;
    return true;
}


bool SFBDocument::layoutGlyphsOnAtlas()
{
    mSpriteFont.doGlyphsFit = layoutGlyphs(mSpriteFont, mGenerationSettings, true);
    return true;
}


void SFBDocument::setNeedsSavingToFalse()
{
    mHasUnsavedData = false;

    for( auto it = mEffects.begin(); it != mEffects.end(); ++it )
    {
        (*it)->setNeedsSavingToFalse();
    }
}


bool SFBDocument::layoutGlyphs(SFBSpriteFont & spriteFont, const SFBGenerationSettings & settings, bool doPaint)
{
    int padding = settings.padding;
    QSize atlasSize(settings.width, settings.height);
    QPainter painter;

    //QVector<SFBGlyph> sortedGlyphs(spriteFont.glyphs);
    qSort(spriteFont.glyphs.begin(), spriteFont.glyphs.end(), [&] (SFBGlyph &g1, SFBGlyph &g2) {return g1.minSize.height() > g2.minSize.height();});

    if ( doPaint && (settings.width == 0 || settings.height == 0) )
    {
        atlasSize = calcTextureSizeForGlyphs(spriteFont.glyphs, settings.width, settings.height);
    }

    if ( doPaint )
    {
        spriteFont.textureAtlas = QImage(atlasSize.width(), atlasSize.height(), QImage::Format_ARGB32);
        spriteFont.textureAtlas.fill(settings.color);

        painter.begin(&(spriteFont.textureAtlas));
    }

    QRect paddedRect = QRect(0, 0, atlasSize.width(), atlasSize.height()).adjusted(padding, padding, -padding, -padding);
    QPoint cursor = QPoint(paddedRect.x(),paddedRect.y());
    int rowHeight = 0;
    bool didFit = true;

    // Cursor starts top left and glyphs are printed "down,right" from the cursor point.

    for(auto it = spriteFont.glyphs.begin(); it != spriteFont.glyphs.end(); ++it)
    {
        SFBGlyph &glyph = (*it);

        if ( cursor.x() + glyph.image.width() > paddedRect.right() )
        {
            cursor.setY(cursor.y() + rowHeight + settings.spacing);
            cursor.setX(paddedRect.x());
            rowHeight = 0;
        }

        QRect destRect = QRect(cursor.x(), cursor.y(), glyph.image.width(), glyph.image.height());

        if ( doPaint )
        {
            painter.drawImage(destRect, glyph.image);
            glyph.atlasRect = destRect;
        }

        cursor.setX(cursor.x() + glyph.image.width() + settings.spacing);

        if ( glyph.image.height() > rowHeight ) {
            rowHeight = glyph.image.height();
        }
    }

    cursor.setY(cursor.y() + rowHeight);

    if ( cursor.y() > paddedRect.bottom() )
    {
        didFit = false;
    }

    if ( doPaint )
    {
        painter.end();
    }

    return didFit;
}
