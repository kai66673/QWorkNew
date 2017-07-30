#include  "RpmlProcessor.h"

namespace RPML {

RpmlProcessor::RpmlProcessor( QTextDocument *textDocument, QObject *parent )
    : QObject(parent)
    , m_textDocument(textDocument)
{

}

}   // namespace RPML
