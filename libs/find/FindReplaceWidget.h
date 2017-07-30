#ifndef QFINDREPLACEWIDGET_H
#define QFINDREPLACEWIDGET_H

#include <QWidget>
#include <QtCore/QTimer>
#include <QStringListModel>

#include "currentdocumentfind.h"
#include "textfindconstants.h"

#include "CommandManager.h"

namespace Ui {
    class FindReplaceWidget;
}

class FindReplaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FindReplaceWidget(QWidget *parent = 0);
    ~FindReplaceWidget();

    void applySettings();

    void setFindText( const QString &findText );
    QString findText() const;

    QList<QAction *> searchActions() const;

protected:
    bool eventFilter( QObject *obj, QEvent *e );

public slots:
    void startSearch();
    void startSearchOnDocumentChanged();

private slots:
    void onCaseSensetiveToggled( bool checked );
    void onWholeWordsOnlyToggled( bool checked );
    void onUseRegularExpressionsToggled( bool checked );
    void onFindIn();

    void findFlagsChanged();

    void invokeClearResults();
    void invokeFindNext();
    void invokeFindPrev();
    void invokeFindStep();
    void invokeReplNext();
    void invokeReplPrev();
    void invokeReplStep();
    void invokeReplaceAll();

    void invokeResetIncrementalSearch();
    void invokeFindIncremental();
    void invokeFindEnter();

private:
    Find::FindFlags effectiveFindFlags();
    void setFindFlag( Find::FindFlags flag, bool enabled );
    bool hasFindFlag( Find::FindFlags flag );

    Ui::FindReplaceWidget *ui;
    Find::Internal::CurrentDocumentFind *m_currentDocumentFind;
    Find::FindFlags m_findFlags;

    QTimer m_findIncrementalTimer;
    QTimer m_findStepTimer;

    Core::Command *m_cmdFindNext;
    Core::Command *m_cmdFindPrev;
    Core::Command *m_cmdReplNext;
    Core::Command *m_cmdReplPrev;
    Core::Command *m_cmdFindIn;
    Core::Command *m_cmdReplaceAll;

    QAction *m_actSeparator;

    bool m_firstSearch;

    // Find Completions
    void initFindCompletions();
    void updateCompletions( const QString &text );
    void saveCompletions();
    QStringList m_findCompletions;
    QStringListModel *m_findModel;

    QString m_orgName;
    QString m_appName;
};

#endif // QFINDREPLACEWIDGET_H
