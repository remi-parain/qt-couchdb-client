#ifndef QCOUCHWORKER_H
#define QCOUCHWORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QUrlQuery>


#include "qcouchcst.h"
#include "qcouchquery.h"
#include "qcouchresponse.h"
#include "qcouchdocinfo.h"

class QCouchWorker : public QObject
{
    Q_OBJECT

    //auto instanciated QNetworkAccessManager
    bool shouldReleaseNamOnDel = false;

    QCouchServer defaultServer;
    QNetworkAccessManager *network;
    QHash<QNetworkReply*, QCouchQuery* > ongoingQueries;
    //QMap<QString, CouchDBQtChangeNotifier*> notifiers;

private:
    QCouchResponse getResponseFromReply(QNetworkReply* _reply);
    void handleQueryTermination(QCouchQuery* _q);

    //utils
    void handleResponseForDbOperation(QCouchResponse *_resp);

public:
    explicit QCouchWorker(QNetworkAccessManager* nam=0, QObject *parent = 0);
    ~QCouchWorker();

    void processQuery(QCouchQuery*query);
    QCouchQuery* newQuery(QCouchServer *server= 0);
    void setDefaultServer(QCouchServer server);

signals :
    void onDatabaseListing(const QVariant&);
    void onDocumentRetrieval(const CouchDBDocument&);
    void onDocumentsRetrieval(const CouchDBDocumentList&);
    void onDocumentUpdate(const QVariant&);
    void onDocumentDeletion(const QVariant&);
    void onDatabaseCreate(const QString&);
    void onDatabaseDelete(const QString&);

private slots:
    void onQueryFinished();
    void handleDatabaseListingResponse(QCouchResponse *_resp);
    void handleDocumentRetrievalResponse(QCouchResponse *_resp);
    void handleDocumentsRetrievalResponse(QCouchResponse *_resp);
    void handleDocumentUpdateResponse(QCouchResponse *_resp);
    void handleDocumentDeletionResponse(QCouchResponse *_resp);
    void handleDatabaseCreateResponse(QCouchResponse *_resp);
    void handleDatabaseDeleteResponse(QCouchResponse *_resp);
};

#endif // QCOUCHWORKER_H
