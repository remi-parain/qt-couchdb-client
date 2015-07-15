#include "qcouchutils.h"

#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHash>


QCouchUtils::QCouchUtils() {}

QVariant QCouchUtils::parseJSONString( const QByteArray &json ) {
    return QJsonDocument::fromJson(json).toVariant();
}
QString QCouchUtils::serializeToJSONString( const QVariant &v ) {
    return (QString)QJsonDocument::fromVariant(v).toJson();
}

CouchDBDocumentInfoList QCouchUtils::variantMapToDocInfoList( const QVariant &vmap, const QString &db ) {
    CouchDBDocumentInfoList list;
    CouchDBDocumentList doclist = QCouchUtils::variantMapToDocList(vmap,db);
    QPair<QCouchDocInfo, QVariantMap> p;
    foreach(  p, doclist ) {
        list << p.first;
    }
    return list;
}

CouchDBDocumentList QCouchUtils::variantMapToDocList( const QVariant& vmap, const QString& db ) {
    CouchDBDocumentList list;
   QVariantMap map = vmap.toMap();
    bool ok = false;

    const int rows = map["total_rows"].toInt( &ok );
    if ( !ok ) {
        return list;
    }

    const int offset = map["offset"].toInt( &ok );
    if ( !ok ) {
        return list;
    }

    qDebug() << rows << offset;


    QVariantList rowlist = map["rows"].toList();

    foreach( QVariant v, rowlist ) {
        QVariantMap m = v.toMap();
        const QString id = m["id"].toString();
        if ( !id.isEmpty() ) {
            QCouchDocInfo info;
            info.setDatabase( db );
            info.setId( id );
            QVariantMap mm = m["value"].toMap();
            const QString rev = mm["rev"].toString();
            if(!rev.isEmpty()) {
                info.setRevision(rev);
            }
            mm.remove("rev");
            list << QPair<QCouchDocInfo, QVariantMap>(info, mm);
        }
    }
    return list;
}

bool QCouchUtils::isQueryOk(const QVariant& vmap ) {
     return vmap.toMap()["ok"].toBool();
}
