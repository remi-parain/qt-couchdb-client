#ifndef QCOUCHUTILS_H
#define QCOUCHUTILS_H

#include "qcouchdocinfo.h"
class QCouchUtils
{
public:
    QCouchUtils();
    static QVariant parseJSONString( const QByteArray &json );
    static CouchDBDocumentInfoList variantMapToDocInfoList( const QVariant &map, const QString &db );
    static CouchDBDocumentList variantMapToDocList( const QVariant& vmap, const QString& db );
    static QString serializeToJSONString( const QVariant &v );
    static bool isQueryOk( const QVariant &v  );
};

#endif // QCOUCHUTILS_H
