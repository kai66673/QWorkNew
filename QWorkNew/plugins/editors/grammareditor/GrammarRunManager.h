#ifndef GRAMMARRUNMANAGER_H
#define GRAMMARRUNMANAGER_H

#include <QStackedWidget>
#include <QPlainTextEdit>
#include <QFutureInterface>

#include "qtcprocess.h"
#include "IssuesView.h"

class GrammarRunParser
{
public:
    GrammarRunParser( Utils::IssuesView *view, const QString &filePath );
    ~GrammarRunParser();

    void stdOutput( const QString &line );
    void stdError( const QString &line );

private:
    void sendQueuedTask();
    void processCompileLine( const QString &line );

    Utils::IssuesView *m_issuesView;
    Utils::Issue m_lastIssue;

    QRegExp m_posRegExp;
    QString m_filePath;
};

class GrammarRunOutputWindow : public QPlainTextEdit
{
    Q_OBJECT

public:
    GrammarRunOutputWindow( QWidget *parent = 0 );
    ~GrammarRunOutputWindow();

    void scrollToBottom();
    void appendText( const QString &text, const QTextCharFormat &format, int maxLineCount );

private:
    QString doNewlineEnfocement( const QString &out );
    bool isScrollbarAtBottom() const;

    bool m_enforceNewline;
    bool m_scrollToBottom;
};

class GrammarRunManager: public QStackedWidget
{
    Q_OBJECT

public:
    enum TextFormat {
        Normal = 0,
        Message,
        Error,
        FatalError
    };

    explicit GrammarRunManager( QTabBar *buildTabBar, QWidget *parent = 0 );
    virtual ~GrammarRunManager();

signals:
    void buildActionStarted();
    void buildActionStartFailed();
    void buildActionFinished();

public slots:
    void execute();
    void cancelBuildAction();
    void toggleHideWarnings( bool onoff );

private slots:
    void processReadyReadStdOutput();
    void processReadyReadStdError();
    void slotProcessFinished(int, QProcess::ExitStatus);
    void onCurrentBuildIssueChanged( const QModelIndex &index );

private:
    void executeInternal( const QString &fileName );

    QTabBar *m_buildTabBar;
    GrammarRunOutputWindow *m_buildActionOutput;
    Utils::IssuesView *m_issues;
    bool m_dirty;
    bool m_blockedByAutoRunOnSave;

    void appendText( const QString &text, TextFormat format );
    void processStarted( const QString &commandFileName );
    void processStartupFailed( const QString &commandFileName );
    void processFinished( int exitCode, QProcess::ExitStatus status );

    Utils::QtcProcess *m_process;
    GrammarRunParser *m_parser;
    QFutureInterface<void> *m_futureInterface;
};

#endif // GRAMMARRUNMANAGER_H
