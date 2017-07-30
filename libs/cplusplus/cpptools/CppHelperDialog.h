#ifndef CPPHELPERDIALOG_H
#define CPPHELPERDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>

#include "CppHelperItem.h"
#include "ModelManagerInterface.h"

namespace Ui {
class CppHelperDialog;
}

class CppRefsModel;

class CppRefDelegate: public QStyledItemDelegate
{
public:
    CppRefDelegate(CppRefsModel *model, QObject *parent = 0);

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

private:
    CppRefsModel *m_model;
};

class CppRefsModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    enum SearcRoles {
        SearchString = Qt::UserRole,
        MatchingPosition
    };

    CppRefsModel(const QString &filePath, const QString &searchString);

    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex &child ) const;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

    void refresh(const QString &key);
    const CppTools::CppHelperItem &item(int row);

private:
    void refreshItems(const QString &key);
    void fillQtHeaderPaths(const CppTools::ProjectPart::HeaderPaths &paths);

    QList<CppTools::CppHelperItem> m_items;
    QString m_filePath;
    QString m_searchString;
    CPlusPlus::CppModelManagerInterface *m_modelManager;

    CppTools::ProjectPart::HeaderPaths m_QtHeaderPaths;
};

class CppHelperDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CppHelperDialog(const QString &filePath, const QString &searchString, QWidget *parent = 0);
    virtual ~CppHelperDialog();

    const CppTools::CppHelperItem *selectedItem();

public slots:
    virtual void accept();
    virtual void reject();

protected:
    bool eventFilter( QObject *obj, QEvent *event );

private slots:
    void currentRowChanged( const QModelIndex &current, const QModelIndex &previous );
    void searchStringChanged(const QString &newKey);

private:
    void selectNextItem();
    void selectPreviousItem();

    Ui::CppHelperDialog *ui;
    CppRefsModel *m_model;
    const CppTools::CppHelperItem *m_selectedItem;

    bool m_blockingEventFilter;
};

#endif // CPPHELPERDIALOG_H
