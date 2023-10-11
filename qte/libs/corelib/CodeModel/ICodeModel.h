#ifndef ICODEMODEL_H
#define ICODEMODEL_H

#include <QObject>
#include <QSet>
#include <QString>
#include <QFuture>

#include "corelib_global.h"
#include "abstracteditorsupport.h"

namespace CodeModel {
class AbstractEditorSupport;
}

class CORELIB_EXPORT ICodeModel : public QObject
{
    Q_OBJECT

public:
    explicit ICodeModel(const QString &id, QObject *parent = 0);
    virtual ~ICodeModel();

    QString id() const { return m_id; }
    bool isRegistered() const { return m_registered; }
    QString registerErrorMessage() const { return m_registerErrorMessage; }

    virtual QFuture<void> updateSourceFiles(const QStringList &sourceFiles) = 0;
    virtual void addExtraEditorSupport(CodeModel::AbstractEditorSupport *editorSupport) = 0;
    virtual void removeExtraEditorSupport(CodeModel::AbstractEditorSupport *editorSupport) = 0;

private:
    QString m_id;
    QString m_registerErrorMessage;
    bool m_registered;
};

#endif // ICODEMODEL_H
