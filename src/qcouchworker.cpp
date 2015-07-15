#include "qcouchworker.h"
#include "qcouchutils.h"

QCouchWorker::QCouchWorker(QNetworkAccessManager *nam,QObject *parent) : QObject(parent)
{
    if(nam) {
        network = nam;
        shouldReleaseNamOnDel = false;
    } else {
        network = new QNetworkAccessManager(this);
        shouldReleaseNamOnDel = true;
    }
}

QCouchWorker::~QCouchWorker() {
    if(shouldReleaseNamOnDel) {
        delete network;
    }
}


void QCouchWorker::processQuery(QCouchQuery*query) {
    qDebug() << "QCouchWorker::processQuery()";
    qDebug() << "INVOKED URL" << " " << query->getRequest().url().toString();
    QNetworkReply * reply;
    switch(query->getCouchOperation()) {
        case ListDatabases:
            reply = network->get(query->getRequest());
            break;
        case CreateDatabase:
            reply = network->put(query->getRequest(),query->getBody());
            break;
        case DeleteDatabase:
            reply = network->deleteResource(query->getRequest());
            break;
        case ListDocuments:
            reply = network->get(query->getRequest());
            break;
        case UpdateDocument:
            reply = network->put(query->getRequest(),query->getBody());
            break;
        case RetrieveDocument:
            reply = network->get(query->getRequest());
            break;
        case DeleteDocument:
            reply = network->deleteResource(query->getRequest());
            break;
        default :
            break;
    }
    connect(reply, SIGNAL(finished()), this, SLOT(onQueryFinished()));
    this->ongoingQueries[reply] = query;
}

QCouchQuery* QCouchWorker::newQuery(QCouchServer* _s) {
    if(!_s) {
        return new QCouchQuery(&defaultServer,this);
    } else
        return new QCouchQuery(_s,this);
}

void QCouchWorker::setDefaultServer(QCouchServer _s) {
    defaultServer = _s;
}

QCouchResponse QCouchWorker::getResponseFromReply(QNetworkReply* _reply) {
    QByteArray data;
    QCouchQuery *query = this->ongoingQueries[_reply];
    bool hasError = false;
    if(_reply->error() == QNetworkReply::NoError) {
        data = _reply->readAll();
    }else {
            emit query->onError(_reply->errorString());
            qWarning() << _reply->errorString();
            hasError = true;
    }
    this->ongoingQueries.remove(_reply);
    _reply->deleteLater();

    return QCouchResponse(query, (hasError)? QCouchQueryStatus::Error : QCouchQueryStatus::Sucess, data);
}


void QCouchWorker::onQueryFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QCouchResponse resp= this->getResponseFromReply(reply);
    switch(resp.getQuery()->getCouchOperation()) {
        case ListDatabases:
            handleDatabaseListingResponse(&resp);
            break;
        case CreateDatabase:
            handleDatabaseCreateResponse(&resp);
            break;
        case DeleteDatabase:
            handleDatabaseDeleteResponse(&resp);
            break;
        case ListDocuments:
            handleDocumentsRetrievalResponse(&resp);
            break;
        case UpdateDocument:
            handleDocumentUpdateResponse(&resp);
            break;
        case RetrieveDocument:
            handleDocumentRetrievalResponse(&resp);
            break;
        case DeleteDocument:
            handleDocumentDeletionResponse(&resp);
            break;
        default :
            break;
    }

    if(resp.getQuery()->isAutodelete())
       delete resp.getQuery();
}

/*
    void onDatabaseListing(const QVariant&);
    void onDocumentRetrieval(const QVariant&);
    void onDocumentsRetrieval(const QVariant&);
    void onDocumentUpdate(const QVariant&);
    void onDocumentDeletion(const QVariant&);
    void onDatabaseCreate(const QString&);
    void onDatabaseDelete(const QString&);
*/
void QCouchWorker::handleDatabaseListingResponse(QCouchResponse *_resp) {
    qDebug() << "QCouchWorker::onDatabaseListingFinished";
    if(_resp->getData() != nullptr) {
        const QVariant dbs = QCouchUtils::parseJSONString( _resp->getData() );
        QStringList result = dbs.toStringList();
       emit _resp->getQuery()->onSuccess(result);
       emit onDatabaseListing(result);
    }
}

void QCouchWorker::handleDocumentRetrievalResponse(QCouchResponse *_resp){
    qDebug() << "QCouchWorker::onDocumentRetrievalFinished";

    if(_resp->getData() != nullptr) {
        const QVariant docAsVariant = QCouchUtils::parseJSONString( _resp->getData() );
        emit _resp->getQuery()->onSuccess(docAsVariant);
        emit onDocumentRetrieval(docAsVariant.value<CouchDBDocument>());
    }
}

void QCouchWorker::handleDocumentsRetrievalResponse(QCouchResponse *_resp){
    qDebug() << "QCouchWorker::onDocumentsRetrievalFinished";

    if(_resp->getData() != nullptr) {
        const QVariant docsAsVariant = QCouchUtils::parseJSONString( _resp->getData() );
        const CouchDBDocumentList docList = QCouchUtils::variantMapToDocList( docsAsVariant, _resp->getDatabase() );
        emit _resp->getQuery()->onSuccess(QVariant::fromValue(docList));
        emit onDocumentsRetrieval(docList);
    }
}

void QCouchWorker::handleDocumentUpdateResponse(QCouchResponse *_resp){
    qDebug() << "QCouchWorker::onDocumentUpdateFinished";
    /*    // json level error handling looks like this:
    // "{"error":"conflict","reason":"Document update conflict."}
    // "{"ok":true,"id":"428004fb5c5a8db558a98f7527e2b9d10","rev":"1-967a00dff5e02add41819138abb3284d"}
    const QVariantMap map = d->parseJSONString( reply->readAll() ).toMap();
    if ( map.contains("error") ) {
        const QString err = map["reason"].toString();
        qWarning() << err;
        emit documentUpdated( false, err );
        return;
    }
    Q_ASSERT( map.contains("ok") && map["ok"].toBool() == true );
    emit documentUpdated( true );*/

}
void QCouchWorker::handleDocumentDeletionResponse(QCouchResponse *_resp){
    qDebug() << "QCouchWorker::onDocumentDeletionFinished";
    qDebug() << _resp->getData();
    /*// json level error handling looks like this:
    // "{"error":"conflict","reason":"Document update conflict."}
    // "{"ok":true,"id":"428004fb5c5a8db558a98f7527e2b9d10","rev":"1-967a00dff5e02add41819138abb3284d"}
    const QVariantMap map = d->parseJSONString( reply->readAll() ).toMap();
    if (map.contains("error")) {
        const QString err = map["reason"].toString();
        qWarning() << err;
        emit documentDeleted(false, err);
        return;
    }
    Q_ASSERT( map.contains("ok") && map["ok"].toBool() == true );
    emit documentDeleted(true);
*/
}

void QCouchWorker::handleDatabaseCreateResponse(QCouchResponse *_resp){
    qDebug() << "QCouchWorker::onDatabaseCreateFinished";
    this->handleResponseForDbOperation(_resp);
    emit onDatabaseCreate(_resp->getQuery()->getDatabase());
}
void QCouchWorker::handleDatabaseDeleteResponse(QCouchResponse *_resp){
    qDebug() << "QCouchWorker::onDatabaseDeleteFinished";
    this->handleResponseForDbOperation(_resp);
    if(_resp->getStatus() == QCouchQueryStatus::Sucess)
        emit onDatabaseDelete(_resp->getQuery()->getDatabase());
}

//utils
void QCouchWorker::handleResponseForDbOperation(QCouchResponse *_resp) {
    if( _resp->getStatus() != QCouchQueryStatus::Error && _resp->getData() != nullptr) {
        QVariant v = QCouchUtils::parseJSONString(_resp->getData());
        if(!QCouchUtils::isQueryOk(v)) {
            _resp->setStatus(QCouchQueryStatus::Error);
        }
    }

        switch(_resp->getStatus()) {
        case Error:
            emit _resp->getQuery()->onError(_resp->getData());
            break;
        case Sucess :
            emit _resp->getQuery()->onSuccess(QVariant());
            break;
        }

}
