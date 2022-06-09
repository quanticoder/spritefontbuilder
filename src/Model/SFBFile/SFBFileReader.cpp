#include "SFBFileReader.h"

#include <QDomDocument>
#include "SFBFileWriter.h"

SFBFileReader::SFBFileReader()
{

}


SFBDocument::Ptr SFBFileReader::loadDocument(const QString &path)
{
    mDocument = SFBDocument::Ptr(new SFBDocument());

    QFile file(path);
    QDomDocument document("sfbDocument");

    if ( !document.setContent(&file) ) {
        mLastError = "Failed to load XML from file at path: " + path;
        return SFBDocument::Ptr(nullptr);
    }

    QDomElement docNode = document.documentElement();

    if ( docNode.isNull() || docNode.tagName().compare(SFBFileWriter::kSFBRootKey) != 0 ) {
        mLastError = "Failed to find node: " + SFBFileWriter::kSFBRootKey;
        return SFBDocument::Ptr(nullptr);
    }

    QDomNodeList nodes = docNode.childNodes();

    for( int i = 0; i < nodes.count(); ++i )
    {
        QDomElement element = nodes.at(i).toElement();
        QString tagName = element.tagName();
        //qDebug() << tagName;

        if ( tagName.compare(SFBInputSettings::kRootKey, Qt::CaseInsensitive) == 0 )
        {
            mDocument->setInputSettings(SFBInputSettings(element));
        }
        else if ( tagName.compare(SFBGenerationSettings::kRootKey, Qt::CaseInsensitive) == 0 )
        {
            mDocument->setGenerationSettings(SFBGenerationSettings(element));
        }
        else if ( tagName.compare(SFBExportSettings::kRootKey, Qt::CaseInsensitive) == 0 )
        {
            mDocument->setExportSettings(SFBExportSettings(element));
        }
        else if ( tagName.compare(SFBFileWriter::kSFBEffectsKey, Qt::CaseInsensitive) == 0 )
        {
            QDomNodeList effectNodes = element.childNodes();

            for( int j = 0; j < effectNodes.count(); ++j )
            {
                QDomElement effectElement = effectNodes.at(j).toElement();

                if ( effectElement.isNull() || effectElement.tagName().compare(SFBEffect::kRootKey) != 0 ) {
                    continue;
                }

                SFBEffect::Ptr effect = SFBEffect::CreateEffectFromXmlNode(effectElement);

                if ( effect != nullptr )
                {
                    mDocument->addEffect(effect);
                }
            }
        }
    }

    mDocument->setDocumentPath(path);
    mDocument->setNeedsSavingToFalse();
    return mDocument;
}



