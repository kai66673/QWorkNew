/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (info@qt.nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "basefilefind.h"

#include "Core.h"
#include "IDocument.h"
#include "itexteditor.h"
#include "ITextDocument.h"
#include <progressmanager.h>
#include <progressview.h>
#include <futureprogress.h>
#include <textfindconstants.h>
#include <find/searchresultwindow.h>
#include <utils/stylehelper.h>

#include <QtCore/QDebug>
#include <QtCore/QDirIterator>
#include <QtCore/QSettings>
#include <QFileDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTextBlock>
#include <QTextCursor>

using namespace Utils;
using namespace Find;
using namespace TextEditor;

BaseFileFind::BaseFileFind(SearchResultWindow *resultWindow)
  : m_resultWindow(resultWindow),
    m_isSearching(false),
    m_resultLabel(0),
    m_filterCombo(0)
{
    m_watcher.setPendingResultsLimit(1);
    connect(&m_watcher, SIGNAL(resultReadyAt(int)), this, SLOT(displayResult(int)));
    connect(&m_watcher, SIGNAL(finished()), this, SLOT(searchFinished()));
}

bool BaseFileFind::isEnabled() const
{
    return !m_isSearching;
}

bool BaseFileFind::canCancel() const
{
    return m_isSearching;
}

void BaseFileFind::cancel()
{
    disconnect(&m_watcher, SIGNAL(resultReadyAt(int)), this, SLOT(displayResult(int)));
    disconnect(&m_watcher, SIGNAL(finished()), this, SLOT(searchFinished()));

    m_watcher.cancel();
}

QStringList BaseFileFind::fileNameFilters() const
{
    QStringList filters;
    if (m_filterCombo && !m_filterCombo->currentText().isEmpty()) {
        const QStringList parts = m_filterCombo->currentText().split(QLatin1Char(','));
        foreach (const QString &part, parts) {
            const QString filter = part.trimmed();
            if (!filter.isEmpty()) {
                filters << filter;
            }
        }
    }
    return filters;
}

SearchResult *BaseFileFind::findAll( const QString &txt, Find::FindFlags findFlags )
{
    m_isSearching = true;
    emit changed();
    if (m_filterCombo)
        updateComboEntries(m_filterCombo, true);
    m_watcher.setFuture(QFuture<FileSearchResultList>());
    SearchResult *result = m_resultWindow->startNewSearch();
//    connect(result, SIGNAL(activated(Find::SearchResultItem)), this, SLOT(openEditor(Find::SearchResultItem)));
    m_resultWindow->popup(true);
    if (findFlags & Find::FindRegularExpression) {
        m_watcher.setFuture( Utils::findInFilesRegExp( txt, files(),
                                                       textDocumentFlagsForFindFlags(findFlags),
                                                       DocumentManager::getInstance()->openedEditorsContents() ) );
    } else {
        m_watcher.setFuture( Utils::findInFiles( txt, files(),
                                                 textDocumentFlagsForFindFlags(findFlags),
                                                 DocumentManager::getInstance()->openedEditorsContents() ) );
    }

    QString taskTitle = tr("Searching text \"%1\"").arg(txt);
    Core::FutureProgress *progress =
        Core::Storage::mainWindow()->addTask( m_watcher.future(), taskTitle,
                                              Constants::TASK_SEARCH, false );
    // progress->setWidget(createProgressWidget());
    SearchEngine::getInstance()->setCurrentSearchProgressWidget(progress);
    connect(progress, SIGNAL(clicked()), m_resultWindow, SLOT(popup()));
    connect(progress, SIGNAL(removeMe()), SearchEngine::getInstance(), SLOT(onAnySearchProgressFinished()));

    return result;
}

void BaseFileFind::replaceAll(const QString &txt, Find::FindFlags findFlags)
{
    m_isSearching = true;
    emit changed();
    if (m_filterCombo)
        updateComboEntries(m_filterCombo, true);
    m_watcher.setFuture(QFuture<FileSearchResultList>());
    SearchResult *result = m_resultWindow->startNewSearch(SearchResultWindow::SearchAndReplace);
    connect(result, SIGNAL(activated(Find::SearchResultItem)), this, SLOT(openEditor(Find::SearchResultItem)));
    connect(result, SIGNAL(replaceButtonClicked(QString,QList<Find::SearchResultItem>)),
            this, SLOT(doReplace(QString,QList<Find::SearchResultItem>)));
    m_resultWindow->popup(true);
    if (findFlags & Find::FindRegularExpression) {
        m_watcher.setFuture(Utils::findInFilesRegExp(txt, files(),
            textDocumentFlagsForFindFlags(findFlags), ITextEditor::openedTextEditorsContents()));
    } else {
        m_watcher.setFuture(Utils::findInFiles(txt, files(),
            textDocumentFlagsForFindFlags(findFlags), ITextEditor::openedTextEditorsContents()));
    }
    QString taskTitle = tr("Searching text \"%1\"").arg(txt);
    Core::FutureProgress *progress =
        Core::Storage::mainWindow()->addTask( m_watcher.future(), taskTitle, Constants::TASK_SEARCH );
    progress->setWidget(createProgressWidget());
    connect(progress, SIGNAL(clicked()), m_resultWindow, SLOT(popup()));
}

void BaseFileFind::doReplace(const QString &text,
                               const QList<Find::SearchResultItem> &items)
{
    QStringList files = replaceAll(text, items);
//    Core::FileManager *fileManager = Core::ICore::instance()->fileManager();
    if (!files.isEmpty()) {
//        fileManager->notifyFilesChangedInternally(files);
        m_resultWindow->hide();
    }
}

void BaseFileFind::displayResult(int index) {
    Utils::FileSearchResultList results = m_watcher.future().resultAt(index);
    QList<Find::SearchResultItem> items; // this conversion is stupid...
    foreach (const Utils::FileSearchResult &result, results) {
        Find::SearchResultItem item;
        item.path = QStringList() << QDir::toNativeSeparators(result.fileName);
        item.lineNumber = result.lineNumber;
        item.text = result.matchingLine;
        item.textMarkLength = result.matchLength;
        item.textMarkPos = result.matchStart;
        item.useTextEditorFont = true;
        item.userData = result.regexpCapturedTexts;
        items << item;
    }
    m_resultWindow->addResults(items, Find::SearchResultWindow::AddOrdered);
    if (m_resultLabel)
        m_resultLabel->setText(tr("%1 found").arg(m_resultWindow->numberOfResults()));
}

void BaseFileFind::searchFinished()
{
    m_resultWindow->finishSearch();
    m_isSearching = false;
    m_resultLabel = 0;
    emit changed();
    emit searchTextFinished();
}

QWidget *BaseFileFind::createProgressWidget()
{
    m_resultLabel = new QLabel;
    m_resultLabel->setAlignment(Qt::AlignCenter);
    // ### TODO this setup should be done by style
    QFont f = m_resultLabel->font();
    f.setBold(true);
    f.setPointSizeF(StyleHelper::sidebarFontSize());
    m_resultLabel->setFont(f);
    m_resultLabel->setPalette(StyleHelper::sidebarFontPalette(m_resultLabel->palette()));
    m_resultLabel->setText(tr("%1 found").arg(m_resultWindow->numberOfResults()));
    return m_resultLabel;
}

QWidget *BaseFileFind::createPatternWidget()
{
    QString filterToolTip = tr("List of comma separated wildcard filters");
    m_filterCombo = new QComboBox;
    m_filterCombo->setEditable(true);
    m_filterCombo->setModel(&m_filterStrings);
    m_filterCombo->setMaxCount(10);
    m_filterCombo->setMinimumContentsLength(10);
    m_filterCombo->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    m_filterCombo->setInsertPolicy(QComboBox::InsertAtBottom);
    m_filterCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_filterCombo->setToolTip(filterToolTip);
    syncComboWithSettings(m_filterCombo, m_filterSetting);
    return m_filterCombo;
}

void BaseFileFind::writeCommonSettings(QSettings *settings)
{
    settings->setValue("filters", m_filterStrings.stringList());
    if (m_filterCombo)
        settings->setValue("currentFilter", m_filterCombo->currentText());
}

void BaseFileFind::readCommonSettings(QSettings *settings, const QString &defaultFilter)
{
    QStringList filters = settings->value("filters").toStringList();
    m_filterSetting = settings->value("currentFilter").toString();
    if (filters.isEmpty())
        filters << defaultFilter;
    if (m_filterSetting.isEmpty())
        m_filterSetting = filters.first();
    m_filterStrings.setStringList(filters);
    if (m_filterCombo)
        syncComboWithSettings(m_filterCombo, m_filterSetting);
}

void BaseFileFind::syncComboWithSettings(QComboBox *combo, const QString &setting)
{
    if (!combo)
        return;
    int index = combo->findText(setting);
    if (index < 0)
        combo->setEditText(setting);
    else
        combo->setCurrentIndex(index);
}

void BaseFileFind::updateComboEntries(QComboBox *combo, bool onTop)
{
    int index = combo->findText(combo->currentText());
    if (index < 0) {
        if (onTop) {
            combo->insertItem(0, combo->currentText());
        } else {
            combo->addItem(combo->currentText());
        }
        combo->setCurrentIndex(combo->findText(combo->currentText()));
    }
}

void BaseFileFind::openEditor(const Find::SearchResultItem &item)
{
    if (item.path.size() > 0) {
        DocumentManager::getInstance()->openDocument( QDir::fromNativeSeparators(item.path.first()),
                                                      new TextDocumentCursor(item.lineNumber - 1, item.textMarkPos) );
    } else {
        DocumentManager::getInstance()->openDocument(item.text);
    }
}

// #pragma mark Static methods

static void applyChanges(QTextDocument *doc, const QString &text, const QList<Find::SearchResultItem> &items)
{
    QList<QPair<QTextCursor, QString> > changes;

    foreach (const Find::SearchResultItem &item, items) {
        const int blockNumber = item.lineNumber - 1;
        QTextCursor tc(doc->findBlockByNumber(blockNumber));

        const int cursorPosition = tc.position() + item.textMarkPos;

        int cursorIndex = 0;
        for (; cursorIndex < changes.size(); ++cursorIndex) {
            const QTextCursor &otherTc = changes.at(cursorIndex).first;

            if (otherTc.position() == cursorPosition)
                break;
        }

        if (cursorIndex != changes.size())
            continue; // skip this change.

        tc.setPosition(cursorPosition);
        tc.setPosition(tc.position() + item.textMarkLength,
                       QTextCursor::KeepAnchor);
        QString substitutionText;
        if (item.userData.canConvert<QStringList>() && !item.userData.toStringList().isEmpty())
            substitutionText = Utils::expandRegExpReplacement(text, item.userData.toStringList());
        else
            substitutionText = text;
        changes.append(QPair<QTextCursor, QString>(tc, substitutionText));
    }

    for (int i = 0; i < changes.size(); ++i) {
        QPair<QTextCursor, QString> &cursor = changes[i];
        cursor.first.insertText(cursor.second);
    }
}

QStringList BaseFileFind::replaceAll(const QString &text,
                               const QList<Find::SearchResultItem> &items)
{
    if (items.isEmpty())
        return QStringList();

    QHash<QString, QList<Find::SearchResultItem> > changes;

    foreach (const Find::SearchResultItem &item, items)
        changes[QDir::fromNativeSeparators(item.path.first())].append(item);

    QHashIterator<QString, QList<Find::SearchResultItem> > it(changes);
    while (it.hasNext()) {
        it.next();

        const QString fileName = it.key();
        const QList<Find::SearchResultItem> changeItems = it.value();

        if ( IDocument *document = DocumentManager::getInstance()->openDocument( fileName,
                                                                                 new TextDocumentCursor( changeItems[0].lineNumber - 1,
                                                                                                           changeItems[0].textMarkPos) ) ) {
            if ( ITextEditor *textEditor = qobject_cast<ITextEditor *>(document->editor()) ) {
                textEditor->waitForEditorSupport();
                QTextCursor tc = textEditor->textCursor();
                tc.beginEditBlock();
                applyChanges(textEditor->document(), text, changeItems);
                tc.endEditBlock();
            }
        }

//        if ( Core::IEditor *editor = DocumentManager::getInstance()->openEditorAt(fileName, changeItems[0].lineNumber - 1, changeItems[0].textMarkPos) ) {
//            if ( ITextEditor *textEditor = qobject_cast<ITextEditor *>(editor) ) {
//                textEditor->waitForEditorSupport();
//                QTextCursor tc = textEditor->textCursor();
//                tc.beginEditBlock();
//                applyChanges(textEditor->document(), text, changeItems);
//                tc.endEditBlock();
//            }
//        }
    }

    return changes.keys();
}
