#ifndef DSPCONFIGDIALOG_H
#define DSPCONFIGDIALOG_H

#include <QDialog>
#include <QSet>

namespace Ui {
class DspConfigDialog;
}

class DspConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DspConfigDialog( const QSet<QString> &cfgSet, QWidget *parent = 0 );
    ~DspConfigDialog();

    QString configurationName() const { return m_cfgName; }

public slots:
    virtual void accept();
    
private:
    Ui::DspConfigDialog *ui;
    QString m_cfgName;
};

#endif // DSPCONFIGDIALOG_H
