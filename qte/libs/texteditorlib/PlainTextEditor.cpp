/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
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

#include "PlainTextEditor.h"
#include "tabsettings.h"
#include "texteditorconstants.h"
#include "texteditorsettings.h"
#include "basetextdocument.h"
#include "highlightdefinition.h"
#include "highlighter.h"
#include "highlighterexception.h"
#include "highlightersettings.h"
#include "manager.h"
#include "context.h"
#include "normalindenter.h"
#include "fontsettings.h"

#include <coreconstants.h>
#include <mimedatabase.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QFileInfo>

#include <QtCore/QDebug>

#include "Core.h"
#include "IDocument.h"

using namespace TextEditor;
using namespace TextEditor::Internal;

PlainTextEditor::PlainTextEditor(PlainTextEditorWidget *editor)
  : BaseTextEditor(editor)
{
    setContext(Core::Context(Core::Constants::K_DEFAULT_TEXT_EDITOR_ID,
                             TextEditor::Constants::C_TEXTEDITOR));
}

PlainTextEditorWidget::PlainTextEditorWidget(QWidget *parent)
  : TextEditorWidget(parent)
  , m_isMissingSyntaxDefinition(false)
  , m_ignoreMissingSyntaxDefinition(false)
{
    setRevisionsVisible(true);
    setMarksVisible(true);
    setRequestMarkEnabled(false);
    setLineSeparatorsAllowed(true);
    setIndenter(new NormalIndenter); // Currently only "normal" indentation is supported.

    setMimeType(QLatin1String(TextEditor::Constants::C_TEXTEDITOR_MIMETYPE_TEXT));
    setDisplayName(tr(Core::Constants::K_DEFAULT_TEXT_EDITOR_DISPLAY_NAME));

    m_commentDefinition.clearCommentStyles();

    connect(file(), SIGNAL(changed()), this, SLOT(configure()));
    connect(Manager::instance(), SIGNAL(mimeTypesRegistered()), this, SLOT(configure()));
}

PlainTextEditorWidget::~PlainTextEditorWidget()
{ }

Core::IEditor *PlainTextEditor::duplicate(QWidget * /*parent*/)
{
    return 0;
}

QString PlainTextEditor::id() const
{
    return QLatin1String(Core::Constants::K_DEFAULT_TEXT_EDITOR_ID);
}

void PlainTextEditorWidget::unCommentSelection()
{
    Utils::unCommentSelection(this, m_commentDefinition);
}

void PlainTextEditorWidget::onSettingsChanged()
{
    TextEditorWidget::onSettingsChanged();
    if ( Highlighter *highlighter = static_cast<Highlighter *>(baseTextDocument()->syntaxHighlighter()) ) {
        highlighter->onSettingsChanged();
        highlighter->rehighlight();
    }
}

void PlainTextEditorWidget::setFontSettings(const FontSettings &fs)
{
    TextEditorWidget::setFontSettings(fs);

    if (baseTextDocument()->syntaxHighlighter()) {
        Highlighter *highlighter =
            static_cast<Highlighter *>(baseTextDocument()->syntaxHighlighter());

        highlighter->onSettingsChanged();

        highlighter->rehighlight();
    }
}

void PlainTextEditorWidget::setTabSettings(const TextEditor::TabSettings &ts)
{
    TextEditorWidget::setTabSettings(ts);

    if (baseTextDocument()->syntaxHighlighter()) {
        Highlighter *highlighter =
            static_cast<Highlighter *>(baseTextDocument()->syntaxHighlighter());
        highlighter->setTabSettings(ts);
    }
}

void PlainTextEditorWidget::configure()
{
    Core::MimeType mimeType;
    if ( file() )
        mimeType = Core::Storage::mainWindow()->mimeDatabase()->findByFile(file()->fileName());
    configure(mimeType);
}

void PlainTextEditorWidget::configure(const QString &mimeType)
{
    configure(Core::Storage::mainWindow()->mimeDatabase()->findByType(mimeType));
}

void PlainTextEditorWidget::configure(const Core::MimeType &mimeType)
{
    Highlighter *highlighter = new Highlighter();
    baseTextDocument()->setSyntaxHighlighter(highlighter);

    setCodeFoldingSupported(false);

    if (!mimeType.isNull()) {
        m_isMissingSyntaxDefinition = true;

        const QString &type = mimeType.type();
        setMimeType(type);

        QString definitionId = Manager::instance()->definitionIdByMimeType(type);
        if (definitionId.isEmpty())
            definitionId = findDefinitionId(mimeType, true);

        if (!definitionId.isEmpty()) {
            m_isMissingSyntaxDefinition = false;
            const QSharedPointer<HighlightDefinition> &definition =
                Manager::instance()->definition(definitionId);
            if (!definition.isNull()) {
                highlighter->setDefaultContext(definition->initialContext());

                m_commentDefinition.setAfterWhiteSpaces(definition->isCommentAfterWhiteSpaces());
                m_commentDefinition.setSingleLine(definition->singleLineComment());
                m_commentDefinition.setMultiLineStart(definition->multiLineCommentStart());
                m_commentDefinition.setMultiLineEnd(definition->multiLineCommentEnd());

                setCodeFoldingSupported(true);
                highlighter->onSettingsChanged();
                highlighter->rehighlight();
            }
//        } else if (file()) {
//            const QString &fileName = file()->fileName();
//            if (TextEditorSettings::instance()->highlighterSettings().isIgnoredFilePattern(fileName))
//                m_isMissingSyntaxDefinition = false;
        }
    }

//    setFontSettings(TextEditorSettings::instance()->fontSettings());

    emit configured(editor());
}

bool PlainTextEditorWidget::isMissingSyntaxDefinition() const
{
    return m_isMissingSyntaxDefinition;
}

bool PlainTextEditorWidget::ignoreMissingSyntaxDefinition() const
{
    return m_ignoreMissingSyntaxDefinition;
}

QString PlainTextEditorWidget::findDefinitionId(const Core::MimeType &mimeType,
                                          bool considerParents) const
{
    QString definitionId = Manager::instance()->definitionIdByAnyMimeType(mimeType.aliases());
    if (definitionId.isEmpty() && considerParents) {
        definitionId = Manager::instance()->definitionIdByAnyMimeType(mimeType.subClassesOf());
        if (definitionId.isEmpty()) {
            foreach (const QString &parent, mimeType.subClassesOf()) {
                const Core::MimeType &parentMimeType =
                    Core::Storage::mainWindow()->mimeDatabase()->findByType(parent);
                definitionId = findDefinitionId(parentMimeType, considerParents);
            }
        }
    }
    return definitionId;
}

void PlainTextEditorWidget::acceptMissingSyntaxDefinitionInfo()
{
//    Core::ICore::instance()->showOptionsDialog(Constants::TEXT_EDITOR_SETTINGS_CATEGORY,
//                                               Constants::TEXT_EDITOR_HIGHLIGHTER_SETTINGS);
}

void PlainTextEditorWidget::ignoreMissingSyntaxDefinitionInfo()
{
    m_ignoreMissingSyntaxDefinition = true;
}
