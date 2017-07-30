#ifndef SNIPPETSTABLEMODEL_H
#define SNIPPETSTABLEMODEL_H

#include <QAbstractTableModel>

#include "snippetscollection.h"
#include "snippet.h"
#include "texteditor_global.h"

namespace TextEditor {
namespace Internal {

class TEXTEDITOR_EXPORT SnippetsTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SnippetsTableModel(QObject *parent);
    virtual ~SnippetsTableModel() {}

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual Qt::ItemFlags flags(const QModelIndex &modelIndex) const;
    virtual QVariant data(const QModelIndex &modelIndex, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &modelIndex, const QVariant &value,
                         int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    QList<QString> groupIds() const;
    void load(const QString &groupId);

    QModelIndex createSnippet();
    QModelIndex insertSnippet(const Snippet &snippet);
    void removeSnippet(const QModelIndex &modelIndex);
    const Snippet &snippetAt(const QModelIndex &modelIndex) const;
    void setSnippetContent(const QModelIndex &modelIndex, const QString &content);
    void revertBuitInSnippet(const QModelIndex &modelIndex);
    void restoreRemovedBuiltInSnippets();

private:
    void replaceSnippet(const Snippet &snippet, const QModelIndex &modelIndex);
    static bool isValidTrigger(const QString &s);

    SnippetsCollection* m_collection;
    QString m_activeGroupId;
};

}   // Internal
}   // TextEditor

#endif // SNIPPETSTABLEMODEL_H
