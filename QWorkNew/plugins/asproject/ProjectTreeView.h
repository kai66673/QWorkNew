#ifndef QPROJECTTREEVIEW_H
#define QPROJECTTREEVIEW_H

#include <QTreeView>

#include "ProjectData.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
class QModelIndex;
QT_END_NAMESPACE

class ProjectTreeView : public QTreeView
{
    Q_OBJECT

public:
    ProjectTreeView( QWidget *parent = 0 );

    ProjectTreeItem *addDocument( ProjectTreeItem *parent, const QString &filePath );

protected:
    void contextMenuEvent( QContextMenuEvent * event );
    void mouseDoubleClickEvent ( QMouseEvent * event );
    void keyPressEvent( QKeyEvent *event );

signals:
    void editProjectProperties();

public slots:
    void onBuildActionStarted();
    void onBuildActionFinished();

private slots:
    void onAddFolder();
    void onEditFolder();
    void onDelFolder();
    void onAddFile();
    void onDelFile();
    void onOpenFile();
    void onEditProjectProperties();
    void onConfigureProjectBuildActions();
    void onProjectSynhronize();

private:
    QAction *actAddFolder;
    QAction *actEditFolder;
    QAction *actDelFolder;
    QAction *actAddFile;
    QAction *actDelFile;
    QAction *actOpenFile;
    QAction *actProjectProperties;
    QAction *actProjectConfigBuildActions;
    QAction *actProjectSynhronize;

    QMenu *fileMenu;
    QMenu *folderMenu;
};

#endif // QPROJECTTREEVIEW_H
