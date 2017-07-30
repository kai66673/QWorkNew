#include "KarelTypeResolver.h"

namespace Karel {

const Type *TypeResolver::operator ()(const QString &text, const QTextBlock &block)
{
    m_accessItems.push(AccessItem());

    m_bracketDepth = 0;

    switch (resolveText(text)) {
        case Failed:
            return 0;
        case Finished:
            return resolveType();
    }

    // Ready
    QTextBlock currentBlock(block);
    while (true) {
        currentBlock = currentBlock.previous();
        if ( !currentBlock.isValid() )
            return 0;
        switch (resolveText(currentBlock.text())) {
            case Failed:
                return 0;
            case Finished:
                return resolveType();
        }
        // Ready
    }

    return 0;
}

TypeResolver::ResolveStatus TypeResolver::resolveText(const QString &text)
{
    SimpleLexer lex;
    const QList<Token> &tokens = lex(text, true);
    for (int i = tokens.size() - 2; i >= 0; i--) {
        const Token &tk = tokens.at(i);
        if ( tk.keyid() == T_BEGIN )
            return m_accessItems.top().kind == VariableIdentifier ? Finished : Failed;
        if ( tk.isComment() )
            continue;
        unsigned tokenKind = tk.kind();
        if ( m_bracketDepth ) {
            if ( tokenKind ==T_LBRACKET )
                m_bracketDepth--;
            else if ( tokenKind == T_RBRACKET )
                m_bracketDepth++;
            continue;
        } else {
            AccessKind previousAccessKind = m_accessItems.top().kind;
            if ( tokenKind == T_IDENTIFIER ) {
                if ( previousAccessKind == Dot || previousAccessKind == Brackets ) {
                    m_accessItems.push(AccessItem(VariableIdentifier, text.mid(tk.begin(), tk.end() - tk.begin())));
                } else return Finished;
            } else if ( tokenKind ==T_RBRACKET ) {
                if ( previousAccessKind == Dot || previousAccessKind == Brackets ) {
                    m_accessItems.push(AccessItem(Brackets));
                    m_bracketDepth++;
                } else return Finished;
            } else if ( tokenKind == T_DOT ) {
                if ( previousAccessKind == VariableIdentifier ) {
                    m_accessItems.push(AccessItem(Dot));
                } else return Failed;
            } else {
                return previousAccessKind == VariableIdentifier ? Finished : Failed;
            }
        }
    }
    return Ready;
}

const Type *TypeResolver::resolveType()
{
    const Type *result = 0;
    AccessItem item = m_accessItems.pop();

    if ( item.kind != VariableIdentifier )
        return 0;

    QByteArray bid = item.id.toLatin1();
    Identifier id(bid.constData(), bid.length());

    const Declaration *varDeclaration = m_scope->lookupVariable(&id);
    if ( varDeclaration  )
        result = varDeclaration->type();
    else
        result = m_scope->lookupUsingFieldType(&id);

    if ( !result )
        return 0;

    while ( !m_accessItems.isEmpty() ) {
        AccessItem item = m_accessItems.pop();
        switch (item.kind) {
            case VariableIdentifier:
                return 0;
            case Dot: {
                if ( m_accessItems.isEmpty() )
                    return result;
                AccessItem field = m_accessItems.pop();
                if ( field.kind != VariableIdentifier )
                    return 0;
                QByteArray fbid = field.id.toLatin1();
                Identifier fid(fbid.constData(), fbid.length());
                result = result->fieldType(&fid);
                if ( !result )
                    return 0;
                break;
            }
            case Brackets:
                result = result->arrayItemtype();
                if ( !result )
                    return 0;
                break;
        }
    }

    return 0;
}

}   // namespace Karel
