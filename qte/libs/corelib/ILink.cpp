#include "ILink.h"
#include "IDocument.h"
#include "ITextDocument.h"

namespace Core {

ILink::ILink( int begin, int end )
    : m_begin(begin)
    , m_end(end)
{
}

bool ILink::isEqual( ILink *other ) const
{
    if ( !other )
        return false;

    return ( m_begin == other->m_begin && m_end == other->m_end );
}

TextTargetLink::TextTargetLink( int begin, int end, const QString &fileName, int line, int column )
    : ILink(begin, end)
    , m_fileName(fileName)
    , m_line(line)
    , m_column(column)
{
}

bool TextTargetLink::open() const
{
    if ( m_fileName.isEmpty() ) {
        if ( IDocument *document = DocumentManager::getInstance()->currentDocument() ) {
            TextDocumentCursor position(m_line, m_column);
            document->setCursor(&position);
            return true;
        }
        return false;
    }

    return DocumentManager::getInstance()->openDocument(m_fileName, new TextDocumentCursor(m_line - 1, m_column));
}

}   // namespace Core
