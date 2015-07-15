#ifndef QCOUCHSERVER_H
#define QCOUCHSERVER_H

#include <QString>

class QCouchServer
{
private :
    QString url;
    int  port;
    QByteArray credential;

public:
    QCouchServer();
    QCouchServer( QString _u,  int _p,  QString _l = nullptr, QString _pwd = nullptr) ;

    QString getUrl();
    void setUrl(const QString _u);

    int  getPort();
    void setPort(const int _p);

    QByteArray getCredential();
    void setCredential(const QString _login, const QString _pwd);

    QString getServerBaseURL();
    bool hasCredential();

};

#endif // QCOUCHSERVER_H
