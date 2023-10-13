/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

/**
 * @brief The Highlighter class pre-highlights Python source using simple scanner.
 *
 * Highlighter doesn't highlight user types (classes and enumerations), syntax
 * and semantic errors, unnecessary code, etc. It's implements only
 * basic highlight mechanism.
 *
 * Main highlight procedure is highlightBlock().
 */

#include "pythonhighlighter.h"
#include "lexical/pythonscanner.h"
#include "pythoneditorconstants.h"
#include "highlighterformatsmanager.h"

#include <basetextdocumentlayout.h>
#include <fontsettings.h>
#include <texteditorconstants.h>

namespace PythonEditor {

using namespace PythonEditor::Internal;

/**
 * @class PyEditor::Highlighter
 * @brief Handles incremental lexical highlighting, but not semantic
 *
 * Incremental lexical highlighting works every time when any character typed
 * or some text inserted (i.e. copied & pasted).
 * Each line keeps associated scanner state - integer number. This state is the
 * scanner context for next line. For example, 3 quotes begin a multiline
 * string, and each line up to next 3 quotes has state 'MultiLineString'.
 *
 * @code
 *  def __init__:               # Normal
 *      self.__doc__ = """      # MultiLineString (next line is inside)
 *                     banana   # MultiLineString
 *                     """      # Normal
 * @endcode
 */

/// New instance created when opening any document in editor
PythonHighlighter::PythonHighlighter(QTextDocument *document) :
    TextEditor::SyntaxHighlighter(document)
{
}

/// Instance destroyed when one of documents closed from editor
PythonHighlighter::~PythonHighlighter()
{ }

void PythonHighlighter::onFormatSettingsChanged( bool forceRehilight )
{
    HighlighterFormatData *formatData = HighlighterFormatsManager::highlighterFormatData(tr("Python"));

    for (int i = 0; i < Internal::Format_FormatsAmount; i++)
        formatData->fillTextCharFormat(i, m_formats[i]);

    if ( forceRehilight )
        rehighlight();
}

/**
 * @brief Highlighter::highlightBlock highlights single line of Python code
 * @param text is single line without EOLN symbol. Access to all block data
 * can be obtained through inherited currentBlock() method.
 *
 * This method receives state (int number) from previously highlighted block,
 * scans block using received state and sets initial highlighting for current
 * block. At the end, it saves internal state in current block.
 */
void PythonHighlighter::highlightBlock(const QString &text)
{
    int initialState = previousBlockState();
    setCurrentBlockState(highlightLine(text, initialState));
}

/**
 * @return True if this keyword is acceptable at start of import line
 */
static inline
bool isImportKeyword(const QString &keyword)
{
    return (keyword == QLatin1String("import")
            || keyword == QLatin1String("from"));
}

/**
 * @brief Highlight line of code, returns new block state
 * @param text Source code to highlight
 * @param initialState Initial state of scanner, retrieved from previous block
 * @return Final state of scanner, should be saved with current block
 */
int PythonHighlighter::highlightLine(const QString &text, int initialState)
{
    int initialBraceDepth = 0;
    int prevSpacesCount = 0;
    if (initialState == -1)
        initialState = 0;
    else {
        initialBraceDepth = (initialState & 0xFFF0) >> 4;
        prevSpacesCount = initialState >> 16;
    }

    bool isEmptyOrCommentLine = true;
    int spacesCount = 0;

    TextEditor::Parentheses parentheses;
    parentheses.reserve(20); // assume wizard level ;-)

    Scanner scanner(text.constData(), text.size(), &parentheses);
    scanner.setState(initialState & 0x07);
    scanner.setBraceDepth(initialBraceDepth);

    FormatToken tk;
    bool hasOnlyWhitespace = true;
    while ((tk = scanner.read()).format() != Format_EndOfBlock) {
        Format format = tk.format();

        switch ( format ) {
            case Format_Whitespace:
                if ( isEmptyOrCommentLine )
                    spacesCount = tk.length();
            case Format_Comment:
            case Format_Doxygen:
                break;
            default:
                isEmptyOrCommentLine = false;
        }

        if (format == Format_Keyword) {
            QString value = scanner.value(tk);
            if (isImportKeyword(value) && hasOnlyWhitespace) {
                setFormat(tk.begin(), tk.length(), m_formats[format]);
                highlightImport(scanner);
                break;
            }
        }

        setFormat(tk.begin(), tk.length(), m_formats[format]);
        if (format != Format_Whitespace)
            hasOnlyWhitespace = false;
    }

    int scannerState = scanner.state();
    int braceDepth = scanner.braceDepth();
    if ( initialBraceDepth || (initialState & 0x07) )
        spacesCount = prevSpacesCount;
    if (  isEmptyOrCommentLine )
        spacesCount = 0;

    int bit4 = ( isEmptyOrCommentLine ? 0x08 : 0 );
    int currentState = (spacesCount << 16) | (braceDepth << 4) | bit4 | scannerState;

    if ( !isEmptyOrCommentLine ) {
        prevSpacesCount = 0;
        int foldIndent = 0;
        QTextBlock block = currentBlock().previous();
        while (true) {
            if ( !block.isValid() )
                break;
            int pState = block.userState();
            if ( !(pState & 0x08) ) {
                prevSpacesCount = (pState >> 16);
                if ( spacesCount >= prevSpacesCount ) {
                    foldIndent = TextEditor::BaseTextDocumentLayout::foldingIndent(block);
                    if ( spacesCount > prevSpacesCount )
                        foldIndent++;
                    break;
                }
            }
            block = block.previous();
        }
        TextEditor::BaseTextDocumentLayout::setFoldingIndent(currentBlock(), foldIndent);

        { // Set Folding for previous empty lines
            QTextBlock block = currentBlock().previous();
            while (true) {
                if ( !block.isValid() )
                    break;
                int prevBit4 = (block.userState() & 0x08);
                if ( !prevBit4 )
                    break;
                TextEditor::BaseTextDocumentLayout::setFoldingIndent(block, foldIndent);
                block = block.previous();
            }
        }
    }

    TextEditor::BaseTextDocumentLayout::setParentheses(currentBlock(), parentheses);

    return currentState;
}

/**
 * @brief Highlights rest of line as import directive
 */
void PythonHighlighter::highlightImport(Scanner &scanner)
{
    FormatToken tk;
    while ((tk = scanner.read()).format() != Format_EndOfBlock) {
        Format format = tk.format();
        if (tk.format() == Format_Identifier)
            format = Format_ImportedModule;
        setFormat(tk.begin(), tk.length(), m_formats[format]);
    }
}

} // namespace PythonEditor
