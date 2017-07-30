#ifndef REPORTSELECTINPUTPARAMETERDIALOG_H
#define REPORTSELECTINPUTPARAMETERDIALOG_H

#include <QDialog>

#include "ReportInputParameters.h"

namespace Ui {
class ReportSelectInputParameterDialog;
}

class ReportSelectInputParameterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ReportSelectInputParameterDialog( QList<RPML::ReportInputParameter *> *parameters, QWidget *parent = 0 );
    ~ReportSelectInputParameterDialog();

    inline int index() const { return m_index; }
    
private slots:
    void on_okButton_clicked();

private:
    Ui::ReportSelectInputParameterDialog *ui;
    QList<RPML::ReportInputParameter *> *m_parameters;
    int m_index;
};

#endif // REPORTSELECTINPUTPARAMETERDIALOG_H
