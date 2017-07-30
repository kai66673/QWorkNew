#ifndef CODEMODELMANAGER_H
#define CODEMODELMANAGER_H

#include <QObject>
#include <QMap>

#include "corelib_global.h"
#include "ICodeModel.h"

class CORELIB_EXPORT CodeModelManager : public QObject
{
    Q_OBJECT

public:
    explicit CodeModelManager(QObject *parent = 0);
    virtual ~CodeModelManager();

    static CodeModelManager *instance();
    static ICodeModel *codeModel(const QString &id);
    static bool registerCodeModel(ICodeModel *codeModel, QString *errorMessage);

signals:

public slots:

private:
    static CodeModelManager *m_instance;

    QMap<QString, ICodeModel *> m_codeModels;

};

#endif // CODEMODELMANAGER_H
