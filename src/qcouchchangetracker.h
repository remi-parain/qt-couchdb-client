#ifndef COUCHDBQTCHANGENOTIFIER_H
#define COUCHDBQTCHANGENOTIFIER_H

#include <QObject>
#include "qcouchserver.h"
#include <QTimer>
#include <QMap>
#include <QNetworkReply>
class QVariant;
class QString;



class QCouchChangeTracker : public QObject
{
  Q_OBJECT
  public:
    QCouchChangeTracker( const QString& db, QCouchServer * server = nullptr );
    ~QCouchChangeTracker();

    void launch();
    void setParam(QString, QString);

  signals:
    void notification( const QString& db, const QList<QString>& v );

  private slots:
    void slotNotification( QNetworkReply* reply);
    void readChange();
    void start();

  private:
    QString db;
    QNetworkAccessManager * nam;
    QCouchServer * server;
    QNetworkReply *new_reply;
    QTimer* m_retry;
    QMap<QString,QString> parameters;

    bool cleanServerOnQuit=false;

};

#endif // COUCHDBQTCHANGENOTIFIER_H
