#include "SFBFileWriter.h"

//
// Constants
//
const QString SFBFileWriter::kSFBRootKey = QString("SpriteFont");
const QString SFBFileWriter::kSFBEffectsKey = QString("Effects");


SFBFileWriter::SFBFileWriter(QString path) :
    mPath(path)
{

}


bool SFBFileWriter::writeDocument(SFBDocument *document)
{
    QFile outFile(mPath);

    if ( !outFile.open(QIODevice::WriteOnly) ) {
        mLastError = "Could not open file '" + mPath + "' for writing.";
        return false;
    }

    QXmlStreamWriter writer;
    writer.setDevice(&outFile);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement(SFBFileWriter::kSFBRootKey);       // <SpriteFont>

    document->getInputSettings().writeToXmlStream(writer);
    document->getGenerationSettings().writeToXmlStream(writer);
    document->getExportSettings().writeToXmlStream(writer);

    writer.writeStartElement(kSFBEffectsKey);                   // <Effects>
    int effectCount = document->getEffectCount();

    for( int i = 0; i < effectCount; ++i )
    {
        SFBEffect * effect = document->getEffectAtIndex(i).get();
        effect->writeToXmlStream(writer);
    }

    writer.writeEndElement();          // </Effects>
    writer.writeEndElement();          // </SpriteFont>
    writer.writeEndDocument();

    document->setDocumentPath(mPath);
    document->setNeedsSavingToFalse();
    return true;
}


QString SFBFileWriter::lastError()
{
    return mLastError;
}
