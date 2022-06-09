#ifndef SFBDOCUMENT_H
#define SFBDOCUMENT_H

#include <QObject>
#include "SFBTypes.h"
#include "SFBGlyph.h"
#include "SFBSpriteFont.h"
#include "Effects/Effects.h"
#include "SFBExportSettings.h"


struct SFBPrepareGlyphFunctor
{
    void operator()(SFBGlyph& glyph);
};


struct SFBComposeFinalGlyphFunctor
{
    void operator()(SFBGlyph& glyph);
};


struct SFBApplyEffectFunctor
{
    SFBApplyEffectFunctor(SFBEffect* effect) :
        mEffect(effect) { }

    void operator()(SFBGlyph& glyph);

    private:
        SFBEffect* mEffect;
};


class SFBDocument : public QObject
{
    friend class SFBFileWriter;
    friend class SFBFileReader;

    Q_OBJECT

public:
    typedef std::shared_ptr<SFBDocument> Ptr;


public:
    explicit SFBDocument(QObject *parent = 0);

    void initDefaultDocument();

    SFBDocument::Ptr clone();

    // Texture Atlas Generation
public:
    const SFBSpriteFont & getSpriteFont();

    SFBInputSettings getInputSettings();
    void setInputSettings(const SFBInputSettings &settings);

    SFBGenerationSettings getGenerationSettings();
    void setGenerationSettings(const SFBGenerationSettings &settings);

    SFBExportSettings getExportSettings();
    void setExportSettings(const SFBExportSettings &settings);

    int getEffectCount();
    SFBEffect::Ptr getEffectAtIndex(int index);
    void addEffect(SFBEffect::Ptr effect);
    void insertEffect(int index, SFBEffect::Ptr effect);
    void removeEffect(SFBEffect::Ptr effect);
    void removeEffect(int index);

    /** Scale the font size and the relevant settings on each effect. */
    void scale(float scale);

    /** Does this sprite font have unsaved data that? */
    bool needsSaving();

    /** Force (re-)generation of the current sprite font, even if the current sprite font is not dirty. */
    void generateSpriteFont();

    /** Set document path. */
    void setDocumentPath(const QString &path);

    /** Get the current document's path. */
    QString documentPath();


protected:

    /** Calculate the required texture size for the given glyphs and the provided fixed width/height values. */
    QSize calcTextureSizeForGlyphs(QVector<SFBGlyph>& glyphs, int fixedWidth, int fixedHeight);

    /** Generate internal glyphs vector from the given settings. */
    bool generateGlyphs();

    /** Layout the current set of glyphs on a new atlas. Returns true if the glyphs fit, false otherwise. */
    bool layoutGlyphsOnAtlas();

    /** Called by file readers / writers to say that this document has been saved. */
    void setNeedsSavingToFalse();


private:

    /**
     * Do the work of laying out the glyphs. This is separate so we can share the routine between calcTextureSize and the layout function.
     * @return Did the glyphs fit onto atlas?
     */
    bool layoutGlyphs(SFBSpriteFont & spriteFont, const SFBGenerationSettings & settings, bool doPaint);


    // Signals
signals:
    void spriteFontUpdated(const SFBSpriteFont& spriteFont);


public slots:


    // Member Variables
private:
    QFontDatabase mFontDatabase;
    SFBSpriteFont mSpriteFont;
    bool mIsDirty;
    bool mHasUnsavedData;
    QString mDocumentPath;

    SFBInputSettings mInputSettings;
    SFBGenerationSettings mGenerationSettings;
    SFBExportSettings mExportSettings;

    QVector<SFBEffect::Ptr> mEffects;
};

#endif // SFBDOCUMENT_H
