#ifndef INPUTPARAMETERSDIALOG_H
#define INPUTPARAMETERSDIALOG_H

#include <QDialog>
#include <QCheckBox>

#include "ReportInputParameters.h"

class InputParametersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InputParametersDialog( QList<RPML::ReportInputParameter *> *paramteters, const QString &title, QWidget *parent = 0 );

    QList<RPML::InputParameterValue> values() const { return m_values; }

signals:

private slots:
    void onOk();

private:
    QList<RPML::ReportInputParameter *> *m_paramteters;
    QList<QWidget *> m_editors;
    QList<QCheckBox *> m_switchers;

    bool checkValues();
    QList<RPML::InputParameterValue> m_values;

};

#endif // INPUTPARAMETERSDIALOG_H
