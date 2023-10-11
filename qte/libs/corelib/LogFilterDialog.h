#ifndef LOGFILTERDIALOG_H
#define LOGFILTERDIALOG_H

#include <QDialog>

#include "LogManager.h"

namespace Ui {
class LogFilterDialog;
}

class LogFilterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LogFilterDialog( Core::LogFilter *filter, QSet<QString> &registeredTags, QWidget *parent = 0 );
    ~LogFilterDialog();

private slots:
    void onAccept();
    
private:
    Ui::LogFilterDialog *ui;
    Core::LogFilter *m_filter;
};

#endif // LOGFILTERDIALOG_H
