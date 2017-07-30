#ifndef QNAVIGATIONMANAGER_H
#define QNAVIGATIONMANAGER_H

#include <QObject>
#include <QtCore/QPointer>
#include <QVariant>

namespace Core {
    class IFile;
    class IEditor;
}

class IDocument;

struct EditLocation {
    QPointer<Core::IFile> file;
    QString fileName;
    QString id;

    IDocument *document;
    QVariant state;
};

class NavigationManager : public QObject
{
    Q_OBJECT

public:
    explicit NavigationManager( QObject *parent = 0 );
    void init();

    void setNavigationHistoryBlocked( bool onoff = true );
    void addCurrentPositionToNavigationHistory(Core::IEditor *editor = 0, const QByteArray &saveState = QByteArray());
    void cutForwardNavigationHistory();

    inline bool canGoForward() const { return m_currentNavigationHistoryPosition < m_navigationHistory.size()-1; }
    inline bool canGoBack() const { return m_currentNavigationHistoryPosition > 0; }

signals:

public slots:
    void goBackInNavigationHistory();
    void goForwardInNavigationHistory();
    void onDocumentClosed( IDocument *document );

private:
    void updateCurrentPositionInNavigationHistory();

    QList<EditLocation> m_navigationHistory;
    int m_currentNavigationHistoryPosition;
    bool m_blocked;
};

#endif // QNAVIGATIONMANAGER_H
