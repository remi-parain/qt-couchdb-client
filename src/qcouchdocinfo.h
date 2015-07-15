#ifndef COUCHDBDOCUMENTINFO_H
#define COUCHDBDOCUMENTINFO_H

#include <QList>
#include <QString>
#include <QPair>
#include <QVariant>
#include <QDataStream>


class QCouchDocInfo
{
public:
    QCouchDocInfo();
    QCouchDocInfo( const QCouchDocInfo& );
    QCouchDocInfo operator=( QCouchDocInfo );
    QString id() const;
    void setId(const QString &id);
    QString revision() const;
    void setRevision(const QString &revision);
    QString database() const;
    void setDatabase( const QString& );

private:
    void swap( QCouchDocInfo& );
    class Private;
    Private * const d;
};

QDebug operator<<(QDebug debug, const QCouchDocInfo &f);


typedef QList<QCouchDocInfo> CouchDBDocumentInfoList;
typedef QPair<QCouchDocInfo, QVariantMap> CouchDBDocument;
typedef QList< CouchDBDocument > CouchDBDocumentList;

Q_DECLARE_METATYPE(QCouchDocInfo)
Q_DECLARE_METATYPE(CouchDBDocumentInfoList)
Q_DECLARE_METATYPE(CouchDBDocument)
Q_DECLARE_METATYPE(CouchDBDocumentList)

#endif // COUCHDBDOCUMENTINFO_H
