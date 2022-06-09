#ifndef SFBFILEWRITER_H
#define SFBFILEWRITER_H

#include <QtCore>
#include <QtXml>

#include "../SFBDocument.h"

class SFBFileWriter
{
public:
    static const QString kSFBRootKey;
    static const QString kSFBEffectsKey;

public:
    SFBFileWriter(QString path);

    bool writeDocument(SFBDocument * document);

    QString lastError();

private:
    QString mPath;
    QString mLastError;
};

#endif // SFBFILEWRITER_H
