#include <QTextCursor>

#include "KarelDocumentCaseFixHandler.h"
#include "KarelLexer.h"
#include "SettingsManager.h"
#include "karel_constants.h"

namespace Karel {

static void fixCase(const Token &tk, QTextCursor &tc, int caseType)
{
    tc.setPosition(tk.begin());
    tc.setPosition(tk.end(), QTextCursor::KeepAnchor);

    QString strToReplace = tc.selectedText();

    switch ( caseType ) {
    case 0:
        strToReplace = strToReplace.toUpper();
        break;
    case 1:
        strToReplace = strToReplace.toLower();
        break;
    case 2:
        strToReplace = strToReplace.toLower();
        if ( strToReplace[0].isLetterOrNumber() || strToReplace[0] == QLatin1Char('_') )
            strToReplace[0] = strToReplace[0].toUpper();
        else
            strToReplace[1] = strToReplace[1].toUpper();
        break;
    }

    tc.insertText(strToReplace);
}

bool KarelDocumentCaseFixHandler::fixOnSave(QTextDocument *document) const
{
    const QMap<QString, QVariant> &valuesMap = SettingsManager::instance()->valuesMap(Constants::C_KAREL_EDITOR_SETTINGS_ID);

    if ( !valuesMap["ApplyOnSave"].toBool() )
        return true;

    bool applyToBuiltings   = valuesMap["ApplyToBuiltings"].toBool();
    bool applyToKeywords    = valuesMap["ApplyToKeywords"].toBool();
    bool applyToTypes       = valuesMap["ApplyToTypes"].toBool();
    bool applyToDirectives  = valuesMap["ApplyToDirectives"].toBool();
    bool applyToSystems     = valuesMap["ApplyToSystems"].toBool();

    if ( !applyToBuiltings && !applyToKeywords && !applyToTypes &&
         !applyToDirectives && !applyToSystems )
        return true;

    int builtingsCaseType   = valuesMap["BuiltingsCaseType"].toInt();
    int keywordsCaseType    = valuesMap["KeywordsCaseType"].toInt();
    int typesCaseType       = valuesMap["TypesCaseType"].toInt();
    int directivesCaseType  = valuesMap["DirectivesCaseType"].toInt();
    int systemsCaseType     = valuesMap["SystemsCaseType"].toInt();

    if ( builtingsCaseType < 0 || builtingsCaseType > 2 )
        builtingsCaseType = 0;
    if ( keywordsCaseType < 0 || keywordsCaseType > 2 )
        keywordsCaseType = 0;
    if ( typesCaseType < 0 || typesCaseType > 2 )
        typesCaseType = 0;
    if ( directivesCaseType < 0 || directivesCaseType > 2 )
        directivesCaseType = 0;
    if ( systemsCaseType < 0 || systemsCaseType > 2 )
        systemsCaseType = 0;

    const QByteArray bytes = document->toPlainText().toLatin1();
    const char *firstChar = bytes.constData();
    const char *lastChar = firstChar + bytes.size();

    Lexer lex(firstChar, lastChar);
    lex.setScanKeywords(true);

    QTextCursor tc(document);

    while (true) {
        Token tk;
        lex(&tk);
        if ( tk.is(T_EOF_SYMBOL) )
            break;
        if ( tk.isKeyword() ) {
            switch (tk.keywordType()) {
            case 1:
                if ( applyToBuiltings )
                    fixCase(tk, tc, builtingsCaseType);
                break;
            case 2:
                if ( applyToKeywords )
                    fixCase(tk, tc, keywordsCaseType);
                break;
            case 3:
                if ( applyToTypes )
                    fixCase(tk, tc, typesCaseType);
                break;
            case 4:
                if ( applyToDirectives )
                    fixCase(tk, tc, directivesCaseType);
                break;
            case 5:
                if ( applyToSystems )
                    fixCase(tk, tc, systemsCaseType);
                break;
            }
        }
    }

    return true;
}

}   // namespace Karel
