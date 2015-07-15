#ifndef QCOUCHCST
#define QCOUCHCST

enum QCouchOperation
    {
        ListDatabases,
        CreateDatabase,
        DeleteDatabase,
        ListDocuments,
        RetrieveDocument,
        UpdateDocument,
        DeleteDocument/*,
        SyncDatabase,
        RetrieveViewDocuments*/  // query a view and get whole document (infos + additional data if _include_doc=true)
    };

enum QCouchQueryStatus
    {
        Sucess,
        Error
};
#endif // QCOUCHCST

