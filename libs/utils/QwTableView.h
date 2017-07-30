#ifndef QWTABLEVIEW_H
#define QWTABLEVIEW_H

#include <QTableView>

#include "utils_global.h"

namespace Utils {

class QTCREATOR_UTILS_EXPORT QwTableView : public QTableView
{
    Q_OBJECT

public:
    QwTableView( const QString &clipboardSplitter = " ", QWidget *parent = 0 );

    void setModel ( QAbstractItemModel * model );

public slots:
    void headerClicked( int logicalIndex );
    void clipboardSplitterChanged( const QString &clipboardSplitter );

protected:
    void keyPressEvent( QKeyEvent * event );

private:
    void copy();

    QString m_clipboardSplitter;

};

}   // namespace Utils

#endif // QWTABLEVIEW_H
