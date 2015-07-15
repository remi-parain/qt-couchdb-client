#include "qcouchserver.h"
#include <QDebug>

QCouchServer::QCouchServer() {
    this->url = "localhost";
    this->port = 5984;
    this->credential = "";
}
QCouchServer::QCouchServer( QString _u,  int _p,  QString _l , QString _pwd ) {
    this->url = _u;
    this->port = _p;
    this->setCredential(_l,_pwd);
}

QString QCouchServer::getUrl() { return this->url; }
void QCouchServer::setUrl(const QString _u) { url = _u; }

int  QCouchServer::getPort() { return this->port; }
void QCouchServer::setPort(const int _p) { port = _p; }


void QCouchServer::setCredential(const QString _login, const QString _pwd) {
    credential = QByteArray(QString("%1:%2").arg(_login, _pwd).toLatin1()).toBase64();
}

QByteArray QCouchServer::getCredential() {
    return credential;
}


QString QCouchServer::getServerBaseURL() {
     return QString("http://%1:%2").arg(this->url, QString::number(port));
}


bool QCouchServer::hasCredential() {
    return (this->credential != "" );
}

