#ifndef GENERICPROJECTLOCATIONDIALOG_H
#define GENERICPROJECTLOCATIONDIALOG_H

#include <QDialog>

namespace Ui {
class GenericProjectLocationDialog;
}

class GenericProjectLocationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GenericProjectLocationDialog( QWidget *parent = 0 );
    ~GenericProjectLocationDialog();

    QString sourceLocation() const { return m_sourceLocation; }

public slots:
    virtual void accept();

private slots:
    void onSpecifyLocation();
    
private:
    Ui::GenericProjectLocationDialog *ui;
    QString m_sourceLocation;
};

#endif // GENERICPROJECTLOCATIONDIALOG_H
