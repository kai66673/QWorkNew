#include <QtPlugin>
#include <QPluginLoader>
#include <QFileDialog>
#include <QApplication>

#include "Core.h"
#include "FavoriteFactoriesManager.h"
#include "IDocument.h"
#include "SelectFavoriteFactoryDialog.h"
#include "LogManager.h"

class OpenFileFavorite: public IFavorite
{
public:
    OpenFileFavorite( FavoriteData *data )
        : IFavorite(data)
    { }

    virtual void execute() const {
        DocumentManager::getInstance()->openDocument(data()->description());
    }
    virtual QString iconName() const { return QLatin1String(":/images/fav_open_file.png"); }
};

FavoriteFactoriesManager::FavoriteFactoriesManager()
{
}

FavoriteFactoriesManager::~FavoriteFactoriesManager()
{
    qDeleteAll(m_factories.values());
}

IFavoriteFactory *FavoriteFactoriesManager::operator []( const QString &id )
{
    QMap<QString, IFavoriteFactory *>::iterator it = m_factories.find(id);
    if ( it != m_factories.end() )
        return *it;
    return 0;
}

void FavoriteFactoriesManager::loadFactoryPlugins()
{
    OpenFileFavoriteFactory *openFactory = new OpenFileFavoriteFactory();
    m_factories[openFactory->id()] = openFactory;

    SelectOpenFileFavoriteFactory *selectOpenFactory = new SelectOpenFileFavoriteFactory();
    m_factories[selectOpenFactory->id()] = selectOpenFactory;

    QDir pluginsDir;
    pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("favorites");

    Core::LogManager::addMessage( Core::LogMessage( QObject::tr("\"Favorite Factories\" Plugins Loading Started..."),
                                                    QObject::tr("Plugins"),
                                                    Core::LogMessage::Information,
                                                    QColor(255, 255, 192) ) );

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if ( plugin ) {
            IFavoriteFactory *iLoader = qobject_cast<IFavoriteFactory *>(plugin);
            if ( iLoader ) {
                if ( m_factories.find(iLoader->id()) == m_factories.end() ) {
                    m_factories[iLoader->id()] = iLoader;
                    Core::LogManager::addMessage( Core::LogMessage( QObject::tr("  \"Favorite Factories\" Plugin \"%1\" Loaded.").arg(fileName),
                                                                    QObject::tr("Plugins"),
                                                                    Core::LogMessage::Information,
                                                                    QColor(255, 255, 192) ) );
                }
                else {
                    Core::LogManager::addMessage( Core::LogMessage( QObject::tr("  \"Favorite Factories\" Plugin \"%1\" Not Loaded. Id \"%2\" already Registered").arg(fileName).arg(iLoader->id()),
                                                                    QObject::tr("Plugins"),
                                                                    Core::LogMessage::Information,
                                                                    QColor(255, 255, 192) ) );
                }
            }
        }
    }

    Core::LogManager::addMessage( Core::LogMessage( QObject::tr("\"Favorite Factories\" Plugins Loaded."),
                                                    QObject::tr("Plugins"),
                                                    Core::LogMessage::Information,
                                                    QColor(255, 255, 192) ) );
}

static bool caseInsensitiveLessThan( const IFavoriteFactory *s1, const IFavoriteFactory *s2 )
{
    return s1->description().toLower() < s2->description().toLower();
}

IFavoriteFactory *FavoriteFactoriesManager::selectFavoriteFactory() const
{
    QList<IFavoriteFactory *> factories = m_factories.values();
    if ( factories.empty() )
        return 0;
    if ( factories.size() == 1 )
        return factories[0];

    qSort(factories.begin(), factories.end(), caseInsensitiveLessThan);

    SelectFavoriteFactoryDialog selFactoryDialog(&factories, Core::Storage::mainWindow()->widget());
    if ( selFactoryDialog.exec() == QDialog::Accepted )
        return selFactoryDialog.selectedFactory();

    return 0;
}


IFavorite *OpenFileFavoriteFactory::doCreateFavorite( FavoriteData *data ) const
{
    return new OpenFileFavorite(data);
}

FavoriteData *OpenFileFavoriteFactory::doCreateFavoriteData() const
{
    if ( IDocument *document = DocumentManager::getInstance()->currentDocument() ) {
        if ( !document->isNew() ) {
            return new FavoriteData(id(), document->title(), document->filePath(), 0);
        }
        else {
            Core::Storage::warning(QObject::tr("New Document Should be Saved"));
        }
    }
    else {
        Core::Storage::warning(QObject::tr("No Document Opened"));
    }
    return 0;
}


IFavorite *SelectOpenFileFavoriteFactory::doCreateFavorite( FavoriteData *data ) const
{
    return new OpenFileFavorite(data);
}

FavoriteData *SelectOpenFileFavoriteFactory::doCreateFavoriteData() const
{
    QString filePath = QFileDialog::getOpenFileName(Core::Storage::mainWindow()->widget());
    if ( filePath.isEmpty() )
        return 0;
    QFileInfo fi(filePath);
    QString fileName = fi.fileName();
    return new FavoriteData(id(), fileName, filePath, 0);
}
