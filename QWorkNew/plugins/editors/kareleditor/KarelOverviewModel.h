#ifndef KARELOVERVIEWMODEL_H
#define KARELOVERVIEWMODEL_H

#include <QAbstractItemModel>

#include "KarelDocument.h"
#include "Control.h"

namespace Karel {

class OverviewTreeItem
{
public:
    enum Kind {
        Root = 0,
        SelectHint,
        ProgramDeclaration,
        ConstantDeclaration,
        TypeDeclaration,
        VariableDeclaration,
        RoutineDeclaration,
        ProgramBody
    };

    OverviewTreeItem(const QString &name, int position, Kind kind, OverviewTreeItem *parent);
    virtual ~OverviewTreeItem();

    OverviewTreeItem *parent() const;
    QString name() const;
    Kind kind() const;
    int position() const;

    int childCount() const;
    OverviewTreeItem *childAt(int index);
    void appandChild(OverviewTreeItem *child);
    int row() const;


private:
    QList<OverviewTreeItem *> m_childrens;
    OverviewTreeItem *m_parent;

    QString m_name;
    int m_position;
    Kind m_kind;
};

class OverviewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit OverviewModel(QObject *parent = 0);
    virtual ~OverviewModel();

    // Header:
    QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void rebuild(Document::Ptr doc, QTextDocument *textDocument, unsigned revision);
    OverviewTreeItem *itemFromIndex(const QModelIndex &index);

    unsigned editorRevision() const;

private:
    OverviewTreeItem *m_rootItem;
    unsigned m_editorRevision;
};

}   // namespace Karel

#endif // KARELOVERVIEWMODEL_H
