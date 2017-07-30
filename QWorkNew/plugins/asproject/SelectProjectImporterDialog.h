#ifndef SELECTPROJECTIMPORTERDIALOG_H
#define SELECTPROJECTIMPORTERDIALOG_H

#include <QDialog>

#include "AsofProjectImportManager.h"

namespace Ui {
class SelectProjectImporterDialog;
}

class SelectProjectImporterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectProjectImporterDialog( AsofProjectImportManager *importManager, QWidget *parent = 0 );
    ~SelectProjectImporterDialog();

    IAsofProjectImporter *selectedImporter() const { return m_importer; }

public slots:
    virtual void accept();
    
private:
    Ui::SelectProjectImporterDialog *ui;
    AsofProjectImportManager *m_importManager;
    IAsofProjectImporter *m_importer;
};

#endif // SELECTPROJECTIMPORTERDIALOG_H
