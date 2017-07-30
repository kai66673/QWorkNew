#ifndef QSEARCHENGINE_H
#define QSEARCHENGINE_H

#include "ISettingsFactory.h"
#include "SettingsManager.h"
#include "QwDockWidget.h"
#include "CommandManager.h"
#include "find_global.h"

#include <filesearch.h>

#include <QObject>
#include <QList>
#include <QAbstractTableModel>
#include <QIcon>
#include <QAction>

class SearchResultTabWidget;

QT_BEGIN_NAMESPACE
class QTabWidget;
class QStackedWidget;
class QLineEdit;
class QVBoxLayout;
QT_END_NAMESPACE

namespace Find {
    class SearchResultWindow;
    class SearchResultItem;
}

namespace TextEditor {
    class SimpleBaseFileFind;
}

class FindReplaceWidget;
class IFindScope;
class BaseTextSearchParamsWidget;
class ISearchModePlugin;

class SearchModeAction : public QAction {
public:
    SearchModeAction( const QString &text, int searchModeIndex, ISearchModePlugin *searchModePlugin, QObject *parent = 0 );
    int getSearchModeIndex() const { return m_searchModeIndex; }
    ISearchModePlugin *searchModePlugin() { return m_searchModePlugin; }

private:
    int m_searchModeIndex;
    ISearchModePlugin *m_searchModePlugin;
};

class ISearchModePlugin : public QObject, public ISettingsFactory
{
public:
    virtual QString name() const = 0;
    virtual QString menuText() const = 0;
    virtual QKeySequence shortcut() const = 0;
    virtual QString toolTip() const = 0;
    virtual QWidget *createWidget( QWidget *parent ) = 0;
    virtual void startSearchText( const QString &/*searchText*/ ) {}
    virtual bool isSearchChanged( const QString &/*searchText*/ ) { return false; }

    virtual QList<QAction *> actions() const = 0;
    virtual QString iconName() const = 0;
};

class BaseSearchModeSettings : public ISettings
{
public:
    BaseSearchModeSettings( FindReplaceWidget *searchWidget );

    virtual QString iconName() const { return QString(); }
    virtual QString title() const { return QString(); }
    virtual ISettingsTabPage *createSettingsTabPage( QWidget *parent = 0 ) { Q_UNUSED(parent); return NULL; }
    virtual QString groupName() const { return QStringLiteral("Base_Find_Setings"); }
    virtual void doApplySettings();

private:
    FindReplaceWidget *m_searchWidget;
};

class BaseSearchModePlugin : public ISearchModePlugin
{
public:
    BaseSearchModePlugin();
    virtual ~BaseSearchModePlugin();

    virtual QString name() const { return QStringLiteral("SMP_Base"); }
    virtual QString menuText() const { return tr("&Find/Replace"); }
    virtual QKeySequence shortcut() const { return QKeySequence(tr("Ctrl+F")); }
    virtual QString toolTip() const { return tr("Find Text in Current Document"); }
    virtual QWidget *createWidget( QWidget *parent );
    virtual void startSearchText( const QString &searchText );
    virtual bool isSearchChanged( const QString &searchText );

    virtual QList<QAction *> actions() const;
    virtual QString iconName() const { return QStringLiteral(":/images/find_text.png"); }

    void registerFindScope( IFindScope *findIn ) { m_findScopeList.append(findIn); }

    int findScopeCount() const { return m_findScopeList.size(); }
    IFindScope *findScopeAt( int index ) { return m_findScopeList[index]; }

protected:
    virtual QStringList settingsKeyList() const { return QStringList() << "Find_Base"; }
    virtual ISettings *createSettings( const QString &settingsKey ) const { Q_UNUSED(settingsKey); return new BaseSearchModeSettings(m_searchWidget); }

private:
    FindReplaceWidget *m_searchWidget;
    QList <IFindScope *> m_findScopeList;
};

class QTestSearchModePlugin : public ISearchModePlugin
{
public:
    virtual QString name() const { return QStringLiteral("SMP_Test"); }
    virtual QString menuText() const { return tr("&Test"); }
    virtual QKeySequence shortcut() const { return QKeySequence(tr("Ctrl+Alt+T")); }
    virtual QString toolTip() const { return tr("For Switch Search Mode Test"); }
    virtual QWidget *createWidget( QWidget *parent );

    virtual QList<QAction *> actions() const { return QList<QAction *>(); }
    virtual QString iconName() const { return QStringLiteral(":/images/find_test.png"); }

protected:
    virtual QStringList settingsKeyList() const { return QStringList(); }
    virtual ISettings *createSettings( const QString &settingsKey ) const { Q_UNUSED(settingsKey); return NULL; }
};

class FIND_EXPORT ISearchProcess
{
public:
    virtual ~ISearchProcess() {}
    virtual void cancelSearch() = 0;
};

class FIND_EXPORT SearchEngine : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SearchEngine)

public:
    SearchEngine( QMenu *searchMenu, QObject *parent = 0 );
    virtual ~SearchEngine();

    Find::SearchResultWindow *newSearchResultWindow( const QString &title, const QIcon &icon, ISearchProcess *search );
    void setCurrentSearchProgressWidget( QWidget *progressWidget );

    static SearchEngine *getInstance() { return searchEngine; }

    BaseSearchModePlugin *getBaseSearchMode() const
    {
        if ( searchModePlugins.size() )
            return static_cast<BaseSearchModePlugin *>(searchModePlugins[0]);
        return 0;
    }

private slots:
    void onTabClose( int tabIndex );
    void onAllTabClose();
    void onSearch( bool bShow );

    void onPrevSearchResult();
    void onNextSearchResult();
    void onExpandSearchResults( bool checked );

    void onAnySearchProgressFinished();

private:
    static SearchEngine *searchEngine;

    void baseSearchModeInit();
    void loadSearchModePlugins();
    void addSearchMode( ISearchModePlugin *searchModePlugin );

    QwCustomDockWidget    *dockWidget;
    SearchResultTabWidget *searchResultTabWidget;

    QList <ISearchModePlugin *> searchModePlugins;
    QList <SearchModeAction *> searchModeActions;
    bool m_toggleSearchActionsBlocked;

    QwTabCornerToolBar *m_cornerToolBar;
    Core::Command *m_cmdNext;
    Core::Command *m_cmdPrev;
    Core::Command *m_cmdExpand;
    Core::Command *m_cmdCloseAll;

    QList<Find::SearchResultWindow *> m_searchWindows;

    QMenu *m_searchActionsMenu;
    QMenu *m_searchMenu;
    ISearchModePlugin *m_lastSearchModePlugin;
    QList<QWidget *> m_searchModeWidgets;
    QVBoxLayout *m_mainLayout;
};

class IFindScope : public QObject
{
public:
    virtual QString name() const = 0;
    virtual QWidget *createFindParamsWidget( QWidget *parent = 0 ) = 0;
    virtual bool isSearchEnabled() const = 0;
    virtual bool isSearchAndReplaceEnabled() const = 0;
    virtual void startSearch( const QString &searchText ) = 0;
    virtual void startSearchAndReplace( const QString &searchText ) = 0;

    virtual bool prepareToSearch() const = 0;
    virtual bool prepareToSearchAndReplace() const { return prepareToSearch(); }
};

class FIND_EXPORT BaseTextFindScope : public IFindScope
{
    Q_OBJECT

public:
    BaseTextFindScope();
    virtual ~BaseTextFindScope();

    virtual QWidget *createFindParamsWidget( QWidget *parent = 0 );
    virtual void startSearch( const QString &searchText );
    virtual void startSearchAndReplace( const QString &searchText );

protected:
    virtual Utils::FileIterator *files() = 0;

    BaseTextSearchParamsWidget *m_searchParamsWidget;

private slots:
    void openEditor( const Find::SearchResultItem &item );
};

class CurrentTextDocumentScope : public BaseTextFindScope
{
public:
    CurrentTextDocumentScope()
        : BaseTextFindScope()
    {}

    virtual QString name() const { return QObject::tr("Current Document"); }
    virtual bool isSearchEnabled() const;
    virtual bool isSearchAndReplaceEnabled() const;
    virtual bool prepareToSearch() const;

protected:
    virtual Utils::FileIterator *files();
};

class OpenedTextDocumentsScope : public BaseTextFindScope
{
public:
    OpenedTextDocumentsScope()
        : BaseTextFindScope()
    {}

    virtual QString name() const { return QObject::tr("Opened Documents"); }
    virtual bool isSearchEnabled() const;
    virtual bool isSearchAndReplaceEnabled() const;
    virtual bool prepareToSearch() const;

protected:
    virtual Utils::FileIterator *files();
};

class SystemDirDocumentsScope : public BaseTextFindScope
{
    Q_OBJECT

public:
    SystemDirDocumentsScope()
        : BaseTextFindScope()
        , m_filePatternLineEdit(0)
        , m_rootDirLineEdit(0)
        , m_selectRootDirPushButton(0)
    {}

    virtual QString name() const { return QObject::tr("In Directory"); }
    virtual bool isSearchEnabled() const { return true; }
    virtual bool isSearchAndReplaceEnabled() const { return true; }
    virtual bool prepareToSearch() const;

    virtual QWidget *createFindParamsWidget( QWidget *parent = 0 );

protected:
    virtual Utils::FileIterator *files();

private slots:
    void onSelectRootDir();

private:
    QStringList fileNameFilters() const;

    QLineEdit   *m_filePatternLineEdit;
    QLineEdit   *m_rootDirLineEdit;
    QPushButton *m_selectRootDirPushButton;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ISearchModePlugin, "QWorkBase.ISearchModePlugin/1.0")
QT_END_NAMESPACE

#endif // QSEARCHENGINE_H
