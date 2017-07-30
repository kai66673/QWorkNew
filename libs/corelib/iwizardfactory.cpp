/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "iwizardfactory.h"
#include "WizardManager.h"

#include <utils/algorithm.h>
#include <utils/qtcassert.h>

#include <QStringList>

/*!
    \class Core::IWizardFactory
    \mainclass

    \brief The class IWizardFactory is the base class for all wizard factories
    (for example shown in \gui {File | New}).

    The wizard interface is a very thin abstraction for the \gui{New...} wizards.
    Basically it defines what to show to the user in the wizard selection dialogs,
    and a hook that is called if the user selects the wizard.

    Wizards can then perform any operations they like, including showing dialogs and
    creating files. Often it is not necessary to create your own wizard from scratch,
    instead use one of the predefined wizards and adapt it to your needs.

    To make your wizard known to the system, add your IWizardFactory instance to the
    plugin manager's object pool in your plugin's initialize function:
    \code
        bool MyPlugin::initialize(const QStringList &arguments, QString *errorString)
        {
            // ... do setup
            addAutoReleasedObject(new MyWizardFactory);
            // ... do more setup
        }
    \endcode
    \sa Core::BaseFileWizard
    \sa Core::StandardFileWizard
*/

/*!
    \enum Core::IWizardFactory::WizardKind
    Used to specify what kind of objects the wizard creates. This information is used
    to show e.g. only wizards that create projects when selecting a \gui{New Project}
    menu item.
    \value FileWizard
        The wizard creates one or more files.
    \value ClassWizard
        The wizard creates a new class (e.g. source+header files).
    \value ProjectWizard
        The wizard creates a new project.
*/

/*!
    \fn IWizardFactory::IWizardFactory(QObject *parent)
    \internal
*/

/*!
    \fn IWizardFactory::~IWizardFactory()
    \internal
*/

/*!
    \fn Kind IWizardFactory::kind() const
    Returns what kind of objects are created by the wizard.
    \sa Kind
*/

/*!
    \fn QIcon IWizardFactory::icon() const
    Returns an icon to show in the wizard selection dialog.
*/

/*!
    \fn QString IWizardFactory::description() const
    Returns a translated description to show when this wizard is selected
    in the dialog.
*/

/*!
    \fn QString IWizardFactory::displayName() const
    Returns the translated name of the wizard, how it should appear in the
    dialog.
*/

/*!
    \fn QString IWizardFactory::id() const
    Returns an arbitrary id that is used for sorting within the category.
*/


/*!
    \fn QString IWizardFactory::category() const
    Returns a category ID to add the wizard to.
*/

/*!
    \fn QString IWizardFactory::displayCategory() const
    Returns the translated string of the category, how it should appear
    in the dialog.
*/

/*!
    \fn void IWizardFactory::runWizard(const QString &path,
                                      QWidget *parent,
                                      const QString &platform,
                                      const QVariantMap &variables)

    This function is executed when the wizard has been selected by the user
    for execution. Any dialogs the wizard opens should use the given \a parent.
    The \a path argument is a suggestion for the location where files should be
    created. The wizard should fill this in its path selection elements as a
    default path.
*/

using namespace Core;

/* A utility to find all wizards supporting a view mode and matching a predicate */
template <class Predicate>
    QList<IWizardFactory*> findWizardFactories(Predicate predicate)
{
    const QList<IWizardFactory*> allFactories = IWizardFactory::allWizardFactories();
    QList<IWizardFactory*> rc;
    const QList<IWizardFactory*>::const_iterator cend = allFactories.constEnd();
    for (QList<IWizardFactory*>::const_iterator it = allFactories.constBegin(); it != cend; ++it)
        if (predicate(*(*it)))
            rc.push_back(*it);
    return rc;
}

QList<IWizardFactory*> IWizardFactory::allWizardFactories()
{
    return WizardManager::allWizardFactories();
}

// Utility to find all registered wizards of a certain kind

class WizardKindPredicate {
public:
    WizardKindPredicate(IWizardFactory::WizardKind kind) : m_kind(kind) {}
    bool operator()(const IWizardFactory &w) const { return w.kind() == m_kind; }
private:
    const IWizardFactory::WizardKind m_kind;
};

QList<IWizardFactory*> IWizardFactory::wizardFactoriesOfKind(WizardKind kind)
{
    return findWizardFactories(WizardKindPredicate(kind));
}

bool IWizardFactory::isAvailable(const QString &platformName) const
{
    FeatureSet availableFeatures = pluginFeatures();

    for (int i = 0; i < WizardManager::featureProviderCount(); i++)
        availableFeatures |= WizardManager::featureProviderAt(i)->availableFeatures(platformName);

    return availableFeatures.contains(requiredFeatures());
}

QStringList IWizardFactory::supportedPlatforms() const
{
    QStringList stringList;

    foreach (const QString &platform, allAvailablePlatforms()) {
        if (isAvailable(platform))
            stringList.append(platform);
    }

    return stringList;
}

QStringList IWizardFactory::allAvailablePlatforms()
{
    QStringList platforms;

    for (int i = 0; i < WizardManager::featureProviderCount(); i++)
        platforms.append(WizardManager::featureProviderAt(i)->availablePlatforms());

    return platforms;
}

QString IWizardFactory::displayNameForPlatform(const QString &string)
{
    for (int i = 0; i < WizardManager::featureProviderCount(); i++) {
        QString displayName = WizardManager::featureProviderAt(i)->displayNameForPlatform(string);
        if (!displayName.isEmpty())
            return displayName;
    }

    return QString();
}

FeatureSet IWizardFactory::pluginFeatures() const
{
    static FeatureSet plugins;
    if (plugins.isEmpty()) {
        QStringList list = QStringList() << "Plugin.CppEditor" << "Plugin.QtSupport" << "Plugin.CMakeProjectManager";
        plugins = FeatureSet::fromStringList(list);
    }
    return plugins;
}
