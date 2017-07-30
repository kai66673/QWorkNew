#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
    class NewProjectDialog;
}

class NewEditProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewEditProjectDialog( const QString &name, const QString &description, const QString &location,
                                   const QStringList &includes, const QStringList &systemIncludes, const QStringList &defines,
                                   const QString &precompiledHeader,
                                   QWidget *parent = 0 );
    ~NewEditProjectDialog();

    QString projectName() const;
    QString projectDescription() const;
    QString projectLocation() const;

    QStringList includePaths() const;
    QStringList systemIncludePaths() const;
    QStringList defines() const;
    QString     precompiledHeader() const;

private slots:
    void tryAccept();
    void choiseLocation();

    void addIncludePath();
    void delIncludePath();
    void addSystemIncludePath();
    void delSystemIncludePath();
    void importSystemIncludePath();
    void addDefine();
    void delDefine();

private:
    Ui::NewProjectDialog *ui;
    bool m_editMode;
};

#endif // NEWPROJECTDIALOG_H
