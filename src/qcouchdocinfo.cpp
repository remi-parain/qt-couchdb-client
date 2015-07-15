#include "qcouchdocinfo.h"
#include <QDebug>

class QCouchDocInfo::Private
{
public:
    Private()
    {
    }

    QString dbname;
    QString docid;
    QString docrev;
};

QCouchDocInfo::QCouchDocInfo()
    :d( new Private )
{
}

QCouchDocInfo::QCouchDocInfo( const QCouchDocInfo& other )
    :d( new Private( *other.d ) )
{
}

QCouchDocInfo QCouchDocInfo::operator= ( QCouchDocInfo other ){
    if( this != &other ) {
        other.swap( *this );
    }

    return *this;
}

void QCouchDocInfo::swap( QCouchDocInfo& other )
{
    *other.d = *d;
}

void QCouchDocInfo::setId( const QString& id )
{
    d->docid = id;
}

QString QCouchDocInfo::id() const
{
    return d->docid;
}

void QCouchDocInfo::setRevision(const QString &revision)
{
    d->docrev = revision;
}

QString QCouchDocInfo::revision() const
{
    return d->docrev;
}


void QCouchDocInfo::setDatabase( const QString& db )
{
    d->dbname = db;
}

QString QCouchDocInfo::database() const
{
    return d->dbname;
}

QDebug operator<<(QDebug debug, const QCouchDocInfo &f) {
      debug<< "doc " << f.id();
      debug<< "(rev= " << f.revision();
      debug<< " ,db= "<< f.database();
      debug<< " )";
      return debug.space();
}
