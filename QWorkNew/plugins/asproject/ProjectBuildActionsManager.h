#ifndef PROJECTBUILDACTIONSMANAGER_H
#define PROJECTBUILDACTIONSMANAGER_H

#include <QStackedWidget>
#include <QPlainTextEdit>
#include <QListView>
#include <QStyledItemDelegate>
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>
#include <QRegExp>
#include <QFutureInterface>

#include <utils/qtcprocess.h>
#include <utils/IssuesView.h>

class ProjectBuildAction;

class MsvcParser
{
public:
    MsvcParser( Utils::IssuesView *view );
    ~MsvcParser();

    void stdOutput(const QString &line);
    void stdError(const QString &line);
    void flush();

private:
    void sendQueuedTask();
    bool processCompileLine(const QString &line);

    Utils::IssuesView *m_issuesView;

    QRegExp m_compileRegExp;
    QRegExp m_additionalInfoRegExp;

    Utils::Issue m_lastIssue;
};

class ProjectBuildOutputWindow : public QPlainTextEdit
{
    Q_OBJECT

public:
    ProjectBuildOutputWindow( QWidget *parent = 0 );
    ~ProjectBuildOutputWindow();

    void scrollToBottom();
    void appendText( const QString &text, const QTextCharFormat &format, int maxLineCount );

private:
    QString doNewlineEnfocement( const QString &out );
    bool isScrollbarAtBottom() const;

    bool m_enforceNewline;
    bool m_scrollToBottom;
};

class ProjectBuildActionsManager : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ProjectBuildActionsManager( QTabBar *buildTabBar, QWidget *parent = 0 );
    ~ProjectBuildActionsManager();

    enum TextFormat {
        Normal = 0,
        Message,
        Error,
        FatalError
    };

    Utils::Environment *environment() { return &m_environment; }
    
signals:
    void buildActionStarted();
    void buildActionStartFailed();
    void buildActionFinished();
    
public slots:
    void startBuildAction( const ProjectBuildAction &buildAction );
    void cancelBuildAction();
    void toggleMode();
    void toggleHideWarnings( bool onoff );

    void clearEnvironment();
    void refreshEnvironment();

private slots:
    void processReadyReadStdOutput();
    void processReadyReadStdError();
    void slotProcessFinished(int, QProcess::ExitStatus);
    void onCurrentBuildIssueChanged( const QModelIndex &index );

private:
    ProjectBuildOutputWindow *m_buildActionOutput;
    Utils::IssuesView *m_issues;
    bool m_dirty;
    Utils::QtcProcess *m_process;
    MsvcParser *m_parser;
    QString m_lastBuildWorkDir;
    Utils::Environment m_environment;

    void appendText( const QString &text, TextFormat format );
    void processStarted( const QString &commandFileName );
    void processStartupFailed( const QString &commandFileName );
    void processFinished( int exitCode, QProcess::ExitStatus status );

    QFutureInterface<void> *m_futureInterface;
    QTabBar *m_buildTabBar;
};

#endif // PROJECTBUILDACTIONSMANAGER_H
