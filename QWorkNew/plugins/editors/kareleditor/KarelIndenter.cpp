#include <QTextBlock>

#include "KarelIndenter.h"

#include <tabsettings.h>

namespace Karel {

static const int TAB_SIZE = 4;

Indenter::Indenter()
    : TextEditor::Indenter()
{ }

Indenter::~Indenter()
{ }

bool Indenter::isElectricCharacter(const QChar &ch) const
{
    switch (ch.toLatin1()) {
        case ' ':
        case '%':
            return true;
    }

    return false;
}

void Indenter::indentBlock(QTextDocument *document, const QTextBlock &block, const QChar &typedChar, const TextEditor::TabSettings &settings)
{
    Q_UNUSED(document)

    switch (typedChar.toLatin1()) {
        case ' ':
            indentSpace(block, settings);
            break;
        case '%':
            indentDirective(block, settings);
            break;
        default:
            indentEnter(block.previous(), settings, true);
            indentEnter(block, settings, false);
            break;
    }
}

void Indenter::indentSpace(const QTextBlock &block, const TextEditor::TabSettings &settings)
{
    QString currentLine = block.text().trimmed();
    if ( !currentLine.compare("end", Qt::CaseInsensitive) ) {
        int indentation = findIdentetion(QStringLiteral("begin"), block.previous(), settings);
        if ( indentation != -1 )
            settings.indentLine(block, indentation);
    }
}

void Indenter::indentDirective(const QTextBlock &block, const TextEditor::TabSettings &settings)
{
    QString currentLineTrimmed = block.text().trimmed();
    if ( currentLineTrimmed[0] == QLatin1Char('%') )
        settings.indentLine(block, 0);
}

void Indenter::indentEnter(const QTextBlock &block, const TextEditor::TabSettings &settings, bool firstStage)
{
    if ( !block.isValid() )
        return;

    QString currentLineTrimmed = block.text().trimmed();
    if ( !currentLineTrimmed.compare(QStringLiteral("endif"), Qt::CaseInsensitive) ||
         !currentLineTrimmed.compare(QStringLiteral("else"), Qt::CaseInsensitive) ) {
        int indentation = findIdentetion(QStringLiteral("if"), block.previous(), settings);
        if ( indentation != -1 )
            settings.indentLine(block, indentation);
        return;
    } else if ( !currentLineTrimmed.compare(QStringLiteral("endwhile"), Qt::CaseInsensitive) ) {
        int indentation = findIdentetion(QStringLiteral("while"), block.previous(), settings);
        if ( indentation != -1 )
            settings.indentLine(block, indentation);
        return;
    } else if ( !currentLineTrimmed.compare(QStringLiteral("endfor"), Qt::CaseInsensitive) ) {
        int indentation = findIdentetion(QStringLiteral("for"), block.previous(), settings);
        if ( indentation != -1 )
            settings.indentLine(block, indentation);
        return;
    } else if ( !currentLineTrimmed.compare(QStringLiteral("endselect"), Qt::CaseInsensitive) ) {
        int indentation = findIdentetion(QStringLiteral("select"), block.previous(), settings);
        if ( indentation != -1 )
            settings.indentLine(block, indentation);
        return;
    } else if ( !currentLineTrimmed.compare(QStringLiteral("endusing"), Qt::CaseInsensitive) ) {
        int indentation = findIdentetion(QStringLiteral("using"), block.previous(), settings);
        if ( indentation != -1 )
            settings.indentLine(block, indentation);
        return;
    }

    if ( currentLineTrimmed.startsWith(QStringLiteral("end"), Qt::CaseInsensitive) ) {
        if ( currentLineTrimmed.length() == 3 || currentLineTrimmed.at(3).isSpace() ) {
            int indentation = findIdentetion(QStringLiteral("begin"), block.previous(), settings);
            if ( indentation != -1 )
                settings.indentLine(block, indentation);
            return;
        }
    }

    if ( firstStage )
        return;

    QTextBlock previousBlock = block.previous();

    while (true) {
        if ( !previousBlock.isValid() ) {
            settings.indentLine(block, 0);
            return;
        }

        QString previousLine = previousBlock.text();
        QString previousLineTrimmed = previousLine.trimmed();
        if ( !previousLineTrimmed.isEmpty() ) {
            int indentation = settings.indentationColumn(previousLine);
            if ( previousLineTrimmed.endsWith("begin", Qt::CaseInsensitive) ) {
                int index = previousLineTrimmed.length() - 6;
                if ( index < 0 || previousLineTrimmed.at(index).isSpace() )
                    indentation += TAB_SIZE;
            } else if ( previousLineTrimmed.endsWith("do", Qt::CaseInsensitive) ) {
                int index = previousLineTrimmed.length() - 3;
                if ( index < 0 || previousLineTrimmed.at(index).isSpace() )
                    indentation += TAB_SIZE;
            } else if ( previousLineTrimmed.endsWith("then", Qt::CaseInsensitive) ) {
                int index = previousLineTrimmed.length() - 5;
                if ( index < 0 || previousLineTrimmed.at(index).isSpace() )
                    indentation += TAB_SIZE;
            } else if ( !previousLineTrimmed.compare("else", Qt::CaseInsensitive) ) {
                indentation += TAB_SIZE;
            }
            settings.indentLine(block, indentation);
            return;
        }

        previousBlock = previousBlock.previous();
    }
}

int Indenter::findIdentetion(const QString &startText, const QTextBlock &block, const TextEditor::TabSettings &settings)
{
    QTextBlock currentBlock = block;
    while (currentBlock.isValid()) {
        QString currentLineTrimmed = currentBlock.text().trimmed();
        if ( currentLineTrimmed.startsWith(startText, Qt::CaseInsensitive) ) {
            if ( currentLineTrimmed.length() == startText.length() ||
                 currentLineTrimmed.at(startText.length()).isSpace() )
                return settings.indentationColumn(currentBlock.text());
        }
        currentBlock = currentBlock.previous();
    }
    return -1;
}

}   // namespace Karel
