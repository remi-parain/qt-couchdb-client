#ifndef QCOUCHQUERY_H
#define QCOUCHQUERY_H

#include <QObject>
#include <QNetworkRequest>
#include "qcouchcst.h"
#include "qcouchserver.h"
#include "qcouchdocinfo.h"

class QCouchWorker;

class QCouchQuery : public QObject
{
    Q_OBJECT
private:
    QCouchWorker *m_worker;
    QCouchServer *m_server;
    QNetworkRequest m_request;
    QCouchOperation m_operation;
    QString m_dbName;
    QByteArray m_body;

    bool autoDelete;
    bool releaseServer = false;
    bool releaseWorker = false;


    void configureDbRequest(const QString& db);

public:
    explicit QCouchQuery(QCouchServer* _server=0, QCouchWorker* _worker =0, QObject *parent = 0);
    ~QCouchQuery();

    QCouchQuery* server(QCouchServer *server=0);

    QCouchQuery* requestDatabaseList();
    QCouchQuery* requestDatabaseCreation( const QString& db );
    QCouchQuery* requestDatabaseDeletion( const QString& db );
    QCouchQuery* requestDocumentListing( const QString& db );
    QCouchQuery* requestDocument( const QCouchDocInfo& info );
    QCouchQuery* updateDocument( const QCouchDocInfo& info, const QVariant &v );
    QCouchQuery* deleteDocument(const QCouchDocInfo& info);


    //QCouchQuery* syncTo(QCouchServer *server=0);

    //accessors
    QCouchOperation getCouchOperation();
    QNetworkRequest getRequest();
    QByteArray getBody();
    QString getDatabase();
    bool isAutodelete();

signals:
    void onSuccess(QVariant);
    void onError(QString);

public slots:
    void execute(bool autoDelete=false);
};

#endif // QCOUCHQUERY_H
