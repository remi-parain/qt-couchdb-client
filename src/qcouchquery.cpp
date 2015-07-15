#include "qcouchquery.h"
#include "qcouchworker.h"
#include "qcouchutils.h"

QCouchQuery::QCouchQuery(QCouchServer *_server, QCouchWorker* _worker, QObject *parent) : QObject(parent)
{

        if(_worker) {
            m_worker = _worker;
        } else {
            m_worker = new QCouchWorker();
            releaseWorker = true;
        }


    if(_server) {
         m_server = _server;
    } else {
        //default server is localhost in admin party
        m_server = new QCouchServer();
        releaseServer= true;
    }
}

 QCouchQuery::~QCouchQuery() {

     if(releaseServer)
         delete m_server;
      if(releaseWorker)
         delete m_worker;

 }

void QCouchQuery::execute(bool _autoDel) {
    autoDelete = _autoDel;
    if(m_server->hasCredential()) {
        m_request.setRawHeader("Authorization", "Basic "+m_server->getCredential());
    }
    this->m_worker->processQuery(this);
}

QCouchQuery* QCouchQuery::server(QCouchServer *_server) {
    if(_server) {
        if(releaseServer) {
            delete m_server;
            releaseServer = false;
        }
        m_server = _server;
    }
    return this;
}
QString QCouchQuery::getDatabase() {
    return m_dbName;
}

QCouchOperation QCouchQuery::getCouchOperation() {
    return this->m_operation;
}

QNetworkRequest QCouchQuery::getRequest(){
    return m_request;
}

bool QCouchQuery::isAutodelete() {
    return autoDelete;
}

QByteArray QCouchQuery::getBody() {
    return m_body;
}


QCouchQuery* QCouchQuery::requestDatabaseList() {
    m_operation = QCouchOperation::ListDatabases;
    m_request.setUrl(QString("%1/_all_dbs").arg(m_server->getServerBaseURL()));
    return this;
}

QCouchQuery* QCouchQuery::requestDatabaseCreation( const QString& db ){
    m_operation = QCouchOperation::CreateDatabase;
    this->configureDbRequest(db);
    return this;
}

QCouchQuery* QCouchQuery::requestDatabaseDeletion( const QString& db ){
    m_operation = QCouchOperation::DeleteDatabase;
    this->configureDbRequest(db);
    return this;
}

QCouchQuery* QCouchQuery::requestDocumentListing( const QString& db ){
    m_operation = QCouchOperation::ListDocuments;
    m_dbName = db;
    m_request.setUrl(QString("%1/%2/_all_docs").arg(m_server->getServerBaseURL(),db));
    return this;
}

QCouchQuery* QCouchQuery::requestDocument( const QCouchDocInfo& info ){
    if ( info.id().isEmpty() ) {
           return this;
    }
    m_operation = QCouchOperation::RetrieveDocument;
    m_request.setUrl(QString("%1/%2/%3").arg(m_server->getServerBaseURL(),info.database(), info.id()));
    return this;
}

QCouchQuery* QCouchQuery::updateDocument( const QCouchDocInfo& info, const QVariant &v ){
    const QString str = QCouchUtils::serializeToJSONString( v );
    if ( str.isNull() ) {
        //emit documentUpdated( false, tr("Error parsing JSON document") );
        return this;
    }
    m_operation = QCouchOperation::UpdateDocument;
    m_request.setUrl(QString("%1/%2/%3").arg(m_server->getServerBaseURL(),info.database(), info.id()));
    m_body = str.toUtf8();
    return this;
}

QCouchQuery* QCouchQuery::deleteDocument(const QCouchDocInfo& info){
    m_operation = QCouchOperation::DeleteDocument;
    m_request.setUrl(QString("%1/%2/%3?rev=%4").arg(m_server->getServerBaseURL(),info.database(), info.id(),info.revision() ));
    return this;
}

void QCouchQuery::configureDbRequest(const QString& db) {
    this->m_dbName = db;
    m_request.setUrl(QString("%1/%2").arg(m_server->getServerBaseURL(),db));
    m_body = QString("{}").toUtf8();
}
