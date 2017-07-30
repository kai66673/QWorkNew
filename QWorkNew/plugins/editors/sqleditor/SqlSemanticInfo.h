#ifndef SQLSEMANTICINFO_H
#define SQLSEMANTICINFO_H

#include <QList>

#include "SqlDocument.h"
#include "AST.h"
#include "ASTfwd.h"
#include "ASTVisitor.h"
#include "sqlchecknamesvisitor.h"
#include "SqlNameUse.h"
#include "IParserEngine.h"

namespace Sql {

class SemanticInfo
{
public:
    SemanticInfo();

    unsigned revision;
    QList<CheckName::Use> names;
    Document::Ptr doc;
    QList<Utils::DiagnosticMessage> diagnosticMessages;
    IParserEngine *m_parserEngine;
};

} // namespace Sql

#endif // SQLSEMANTICINFO_H
