#ifndef SNIPPETSDOCKWIDGET_H
#define SNIPPETSDOCKWIDGET_H

#include <QMainWindow>
#include <QComboBox>

#include "QwDockWidget.h"

class SnippetsSettingsPagePrivate;

class SnippetsDockWidget : public QwCustomDockWidget
{
public:
    SnippetsDockWidget(QMainWindow *mainWindow, QComboBox *groupsComboBox,
                       QAction *actAdd, QAction *actDelete);

    void synchronize();

private:
    QComboBox *m_groupsComboBox;
    QAction *m_actAdd;
    QAction *m_actDelete;

    SnippetsSettingsPagePrivate *m_data;
};

#endif // SNIPPETSDOCKWIDGET_H
