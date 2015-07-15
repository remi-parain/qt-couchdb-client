#include "qcouchchangetracker.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include <QDebug>
#include <QVariant>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QUrlQuery>



QCouchChangeTracker::QCouchChangeTracker( const QString& db , QCouchServer * _server )
{
  this->db = db;
  if(_server)
      this->server = _server;
  else {
      this->server = new QCouchServer();
      this->cleanServerOnQuit = true;
  }

  this->nam = new QNetworkAccessManager(this);

  connect(this->nam, SIGNAL(finished(QNetworkReply*)),  this, SLOT(slotNotification(QNetworkReply *)));
  this->m_retry = new QTimer(this);
  connect(this->m_retry, SIGNAL(timeout()), this, SLOT(start()));
  this->m_retry->setInterval(500);
  this->m_retry->setSingleShot(true);
}

void QCouchChangeTracker::start() {
    QUrlQuery urlq;
    QMapIterator<QString, QString> i(parameters);
    while (i.hasNext()) {
        i.next();
        urlq.addQueryItem(i.key(), i.value());

    }
    QNetworkRequest request;
    QUrl url = QUrl(QString("%1/%2/_changes").arg(this->server->getServerBaseURL(), this->db));
    url.setQuery(urlq);
    request.setUrl(url);
    if(this->server->hasCredential()) {
        request.setRawHeader("Authorization", "Basic "+this->server->getCredential());
    }
    this->new_reply = this->nam->get(request);
    connect(this->new_reply, SIGNAL(readyRead()),  this, SLOT(readChange()));
}

void QCouchChangeTracker::readChange() {
    QList<QString> changes_list;
    QTextStream in(this->new_reply->readAll());
        while (!in.atEnd()) {
            QJsonDocument json_line =  QJsonDocument::fromJson(in.readLine().toUtf8());
            if(json_line.object().contains("id"))
                changes_list.append(json_line.object().find("id").value().toString());
            else
                if(json_line.object().contains("last_seq"))
                    qDebug() << json_line.object().find("last_seq").value().toInt();
        }

    emit notification( this->db, changes_list);

}


void QCouchChangeTracker::slotNotification( QNetworkReply *reply  )
{
    // Check the network reply for errors.
     QNetworkReply::NetworkError netError = reply->error();
       if (netError != QNetworkReply::NoError)
       {
            qWarning() << "ERROR";
            switch(netError)
                   {
                       case QNetworkReply::ContentNotFoundError:
                           qWarning() << "The content was not found on the server";
                           break;

                       case QNetworkReply::HostNotFoundError:
                           qWarning() <<"The server was not found";
                           break;

                       default:
                          qWarning() <<reply->errorString();
                           break;
                   }

       }
  reply->deleteLater();
  this->m_retry->start();
}

QCouchChangeTracker::~QCouchChangeTracker() {
    this->m_retry->stop();
    this->new_reply->abort();

    delete  this->nam;
    delete this->new_reply;
    delete this->m_retry;
    if(this->cleanServerOnQuit) {
        delete this->server;
    }
}

void QCouchChangeTracker::launch() {
    this->m_retry->start();
}

void QCouchChangeTracker::setParam(QString key, QString val) {
    parameters.insert(key,val);
}
