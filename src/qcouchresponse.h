#ifndef QCOUCHRESPONSE_H
#define QCOUCHRESPONSE_H

#include "qcouchquery.h"

class QCouchResponse
{
private:
    QByteArray data;
    QCouchQuery *query;
    QCouchQueryStatus status;
    QString dbName;

public:
    explicit QCouchResponse(QCouchQuery* _req, QCouchQueryStatus status, QByteArray _data) {
        data = _data;
        query = _req;
        if(_req != nullptr)
            dbName = _req->getDatabase();
    }

    QString getDatabase(){
        return dbName;
    }
    void setDatabase(const QString str){
        dbName = str;
    }

    QByteArray getData(){
        return data;
    }

    QCouchQuery* getQuery(){
        return query;
    }

    QCouchQueryStatus getStatus() {
        return status;
    }
    void setStatus(QCouchQueryStatus _s) {
        status = _s;
    }
};

#endif // QCOUCHRESPONSE_H
