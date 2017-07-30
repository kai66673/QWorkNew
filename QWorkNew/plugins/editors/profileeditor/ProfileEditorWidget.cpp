#include <QTextBlock>
#include <QDir>

#include "ProfileEditorWidget.h"
#include "ProfileEditor.h"
#include "texteditorconstants.h"
#include "basetextdocument.h"

namespace ProEditor {

static bool isValidFileNameChar(const QChar &c)
{
    return c.isLetterOrNumber()
            || c == QLatin1Char('.')
            || c == QLatin1Char('_')
            || c == QLatin1Char('-')
            || c == QLatin1Char('/')
            || c == QLatin1Char('\\');
}

ProfileEditorWidget::ProfileEditorWidget(IDocument *document, QWidget *parent)
    : TextEditor::TextEditorWidget(document, parent)
{
    m_commentDefinition.setMultiLineStart(QString());
    m_commentDefinition.setMultiLineEnd(QString());
    m_commentDefinition.setSingleLine(QLatin1String("#"));

    ProFileCompletionAssistProvider *pcap = new ProFileCompletionAssistProvider;
    TextEditor::Keywords keywords(pcap->variables(), pcap->functions(), QMap<QString, QStringList>());

    m_highlighter = new ProfileHighlighter(keywords);
    baseTextDocument()->setSyntaxHighlighter(m_highlighter);
    setCompletionProvider(pcap);
}

ProfileEditorWidget::~ProfileEditorWidget()
{ }

void ProfileEditorWidget::unCommentSelection()
{ Utils::unCommentSelection(this, m_commentDefinition); }

void ProfileEditorWidget::onSettingsChanged()
{
    TextEditor::TextEditorWidget::onSettingsChanged();
    m_highlighter->onFormatSettingsChanged();
}

TextEditor::BaseTextEditor *ProfileEditorWidget::createEditor()
{ return new ProfileEditor(this); }

Core::ILink *ProfileEditorWidget::findLinkAt(const QTextCursor &cursor, bool /*resolveTarget*/)
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

    // remove trailing '\' since it can be line continuation char
    if (buffer.at(buffer.size() - 1) == QLatin1Char('\\')) {
        buffer.chop(1);
        endPos--;
    }

    // if the buffer starts with $$PWD accept it
    if (buffer.startsWith(QLatin1String("PWD/")) ||
            buffer.startsWith(QLatin1String("PWD\\"))) {
        if (beginPos > 0 && block.mid(beginPos - 1, 2) == QLatin1String("$$")) {
            beginPos -=2;
            buffer = buffer.mid(4);
        }
    }

    QFileInfo fp(m_document->filePath());
    QDir dir(fp.absolutePath());
    QString fileName = dir.filePath(buffer);
    QFileInfo fi(fileName);
    if (fi.exists()) {
        if (fi.isDir()) {
            QDir subDir(fi.absoluteFilePath());
            QString subProject = subDir.filePath(subDir.dirName() + QLatin1String(".pro"));
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

}   // namespace Profile
