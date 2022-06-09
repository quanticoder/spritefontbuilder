#include "SFBFontExporterBMFont.h"

#include "../SFBDocument.h"

SFBFontExporterBMFont::SFBFontExporterBMFont()
{

}


bool SFBFontExporterBMFont::exportFont(SFBDocument *doc)
{
    QString path = doc->getExportSettings().path;
    path = fullPathFromExportPath(doc, path);
    path = path.replace(".fnt", "", Qt::CaseInsensitive);

    if ( doc == nullptr || doc->getSpriteFont().isNull ) {
        mLastError = "Null document or document with null sprite font provided for exporting.";
        return false;
    }

    bool result = writeDocumentToPath(doc, path);

    if ( !result ) {
        return result;
    }

    if ( doc->getExportSettings().retinaCopy )
    {
        SFBDocument::Ptr retinaDoc = doc->clone();
        retinaDoc->scale(2.0f);
        retinaDoc->generateSpriteFont();

        result = writeDocumentToPath(retinaDoc.get(), path.append("@2x"));
    }

    return result;
}


bool SFBFontExporterBMFont::writeDocumentToPath(SFBDocument * doc, const QString &path)
{
    if ( path.isNull() ) {
        mLastError = "Invalid path provided.";
        return false;
    }

    QString fntPath = path;

    if ( fntPath.right(4).compare(".fnt") != 0 ) {
        fntPath = fntPath.append(".fnt");
    }

    SFBInputSettings inSettings = doc->getInputSettings();
    SFBGenerationSettings genSettings = doc->getGenerationSettings();
    SFBSpriteFont spriteFont = doc->getSpriteFont();

    QFontMetrics fontMetrics(QFont(inSettings.fontFamily, inSettings.fontSize));
    QString padding = QString::number(genSettings.padding);
    QString spacing = QString::number(genSettings.spacing);

    QString pngPath = fntPath;
    pngPath.replace(".fnt", ".png", Qt::CaseInsensitive);

    QStringList lines;

    QStringList info;
    info << "info";
    info << QString("font=\"%1\"").arg(inSettings.fontFamily);
    info << QString("size=%1").arg(inSettings.fontSize);
    info << QString("bold=%1").arg(0);
    info << QString("italic=%1").arg(0);
    info << QString("charset=\"\"");
    info << QString("unicode=%1").arg(0);
    info << QString("stretchH=%1").arg(100);
    info << QString("smooth=%1").arg(1);
    info << QString("aa=%1").arg(1);
    info << QString("padding=%1,%2,%3,%4").arg(padding, padding, padding, padding);
    info << QString("spacing=%1,%2").arg(spacing, spacing);
    lines << info.join(" ");

    QStringList common;
    common << "common";
    common << QString("lineHeight=%1").arg(fontMetrics.lineSpacing());
    common << QString("base=%1").arg(fontMetrics.ascent());
    common << QString("scaleW=%1").arg(spriteFont.textureAtlas.width());
    common << QString("scaleH=%1").arg(spriteFont.textureAtlas.height());
    common << QString("pages=%1").arg(1);
    common << QString("packed=%1").arg(0);
    lines << common.join(" ");

    QStringList page;
    page << "page";
    page << QString("id=%1").arg(0);
    page << QString("file=\"%1\"").arg(QFileInfo(pngPath).fileName());
    lines << page.join(" ");

    QStringList chars;
    chars << "chars";
    chars << QString("count=%1").arg(spriteFont.glyphs.size() + 1);
    lines << chars.join(" ");

    bool hasSpace = false;

    for( SFBGlyph & glyph : spriteFont.glyphs )
    {
        if ( glyph.character == ' ' ) {
            hasSpace = true;
        }

        QRect boundingRect = fontMetrics.boundingRect(glyph.character); // Used to determine y-offset
        //qDebug() << glyph.character << ": " << boundingRect;

        QStringList c;
        c << "char";
        c << QString("id=%1").arg(glyph.character.unicode());
        c << QString("x=%1").arg(glyph.atlasRect.x());
        c << QString("y=%1").arg(glyph.atlasRect.y());
        c << QString("width=%1").arg(glyph.atlasRect.width());
        c << QString("height=%1").arg(glyph.atlasRect.height());
        c << QString("xoffset=%1").arg((int)(boundingRect.x() + glyph.effectPadding.left));
        c << QString("yoffset=%1").arg((int)(fontMetrics.height() + boundingRect.y() - fontMetrics.descent() - glyph.effectPadding.bottom));
        c << QString("xadvance=%1").arg((int)(fontMetrics.width(glyph.character)));
        c << QString("page=%1").arg(0);
        c << QString("chnl=%1").arg(0);
        c << QString("letter=\"%1\"").arg(glyph.character);

        lines << c.join(" ");
    }

    // Space character should always exist
    if ( !hasSpace )
    {
        QStringList spaceChar;
        spaceChar << "char";
        spaceChar << QString("id=%1").arg((short)QChar(' ').toLatin1());
        spaceChar << QString("x=%1").arg(0);
        spaceChar << QString("y=%1").arg(0);
        spaceChar << QString("width=%1").arg(0);
        spaceChar << QString("height=%1").arg(0);
        spaceChar << QString("xoffset=%1").arg(0);
        spaceChar << QString("yoffset=%1").arg(0);
        spaceChar << QString("xadvance=%1").arg(fontMetrics.width(' '));
        spaceChar << QString("page=%1").arg(0);
        spaceChar << QString("chnl=%1").arg(0);
        spaceChar << QString("letter=\"%1\"").arg("space");
        lines << spaceChar.join(" ");
    }

    // Kerning Pairs
    QStringList kerningLines;

    for( SFBGlyph & glyph : spriteFont.glyphs )
    {
        for( SFBGlyph::KerningPair & kerning : glyph.kerningPairs )
        {
            QStringList k;
            k << "kerning";
            k << QString("first=%1").arg((short)glyph.character.toLatin1());
            k << QString("second=%1").arg((short)kerning.first.toLatin1());
            k << QString("amount=%1").arg(kerning.second);

            kerningLines << k.join(" ");
        }
    }

    lines << QString("kernings count=%1").arg(kerningLines.count());
    lines.append(kerningLines);

    // Write to File

    QFile file(fntPath);

    if ( !file.open(QIODevice::WriteOnly) )
    {
        return false;
    }

    QTextStream textStream(&file);
    textStream << lines.join("\n");
    file.close();

    // Write PNG File
    bool result = doc->getSpriteFont().textureAtlas.save(pngPath, "PNG");

    if ( result == false ) {
        mLastError = "Save failed.";
    }

    return result;
}
