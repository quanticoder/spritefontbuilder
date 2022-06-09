#ifndef SFBFILEREADER_H
#define SFBFILEREADER_H

#include "SFBDocumentElement.h"
#include "../SFBDocument.h"
#include "../SFBTypes.h"
#include "../Effects/SFBEffect.h"


class SFBFileReader
{
public:
    SFBFileReader();

    /** Load an SFB document object from the XML file at the given path. */
    SFBDocument::Ptr loadDocument(const QString &path);

    /** Get the last error that occured. */
    QString lastError();

private:
    SFBDocument::Ptr mDocument;
    QString mLastError;
};

#endif // SFBFILEREADER_H
