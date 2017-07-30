#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

#include "DbLoader.h"

namespace Ui {
    class ConnectDialog;
}

class IDatabaseLoaderPlugin;

class ConnectDialog : public QDialog {
    Q_OBJECT
public:
    ConnectDialog( const QList<Database::IDbLoader *> &providers, QWidget *parent = 0 );
    ~ConnectDialog();

    int getConnectionType() const;
    QStringList getConnectionParameters() const { return m_connParameters; }

public slots:
    void tryAccept();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ConnectDialog *ui;
    QStringList m_connParameters;
};

#endif // CONNECTDIALOG_H
