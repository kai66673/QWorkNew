#include <QDir>

#include "CmakeEditorWidget.h"
#include "CmakeEditor.h"
#include "CmakeCompletionAssistProvider.h"
#include "highlighter.h"
#include "basetextdocument.h"

namespace Cmake {

static bool isValidFileNameChar(const QChar &c)
{
    return c.isLetterOrNumber()
            || c == QLatin1Char('.')
            || c == QLatin1Char('_')
            || c == QLatin1Char('-')
            || c == QLatin1Char('/')
            || c == QLatin1Char('\\');
}

CmakeEditorWidget::CmakeEditorWidget(IDocument *document, QWidget *parent)
    : TextEditor::PlainTextEditorWidget(document, parent)
{
    setCompletionProvider(new CmakeCompletionAssistProvider);
}

CmakeEditorWidget::~CmakeEditorWidget()
{ }

TextEditor::BaseTextEditor *CmakeEditorWidget::createEditor()
{ return new CmakeEditor(this); }

Core::ILink *CmakeEditorWidget::findLinkAt(const QTextCursor &cursor, bool /*resolveTarget*/)
{
    Core::ILink *link = 0;

    unsigned lineNumber = 0, positionInBlock = 0;
    convertPosition(cursor.position(), &lineNumber, &positionInBlock);

    const QString block = cursor.block().text();

    // check if the current position is commented out
    const int hashPos = block.indexOf(QLatin1Char('#'));
    if (hashPos >= 0 && hashPos < (int)positionInBlock)
        return link;

    // find the beginning of a filename
    QString buffer;
    int beginPos = positionInBlock - 1;
    while (beginPos >= 0) {
        QChar c = block.at(beginPos);
        if (isValidFileNameChar(c)) {
            buffer.prepend(c);
            beginPos--;
        } else {
            break;
        }
    }

    // find the end of a filename
    int endPos = positionInBlock;
    while (endPos < block.count()) {
        QChar c = block.at(endPos);
        if (isValidFileNameChar(c)) {
            buffer.append(c);
            endPos++;
        } else {
            break;
        }
    }

    if (buffer.isEmpty())
        return link;

    // TODO: Resolve variables

    QFileInfo fp(m_document->filePath());
    QDir dir(fp.absolutePath());
    QString fileName = dir.filePath(buffer);
    QFileInfo fi(fileName);
    if (fi.exists()) {
        if (fi.isDir()) {
            QDir subDir(fi.absoluteFilePath());
            QString subProject = subDir.filePath(QLatin1String("CMakeLists.txt"));
            if (QFileInfo::exists(subProject))
                fileName = subProject;
            else
                return link;
        }
        link = new Core::TextTargetLink(cursor.position() - positionInBlock + beginPos + 1,
                                        cursor.position() - positionInBlock + endPos,
                                        QDir::cleanPath(fileName),
                                        0, 0);
    }
    return link;
}

}   // namespace CmakeEditor
