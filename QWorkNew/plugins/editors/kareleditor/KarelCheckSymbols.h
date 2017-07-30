#ifndef KARELCHECKSYMBOLS_H
#define KARELCHECKSYMBOLS_H

#include <QtCore/QFuture>
#ifndef HAVE_QT5
#include <QtCore/QtConcurrentRun>
#else
#include <QtConcurrent/QtConcurrentRun>
#endif

#include "KarelASTVisitor.h"
#include "KarelSemanticInfo.h"

namespace Karel {

class CheckSymbols: protected ASTVisitor, public QRunnable,
        public QFutureInterface<SemanticInfo::Use>
{
public:
    virtual ~CheckSymbols();

    typedef SemanticInfo::Use Result;
    typedef SemanticInfo::Use::Kind Kind;

    virtual void run();

    typedef QFuture<Result> Future;

    Future start()
    {
        this->setRunnable(this);
        this->reportStarted();
        Future future = this->future();
        QThreadPool::globalInstance()->start(this, QThread::LowestPriority);
        return future;
    }

    static Future go(Document::Ptr doc);

    static QMap<int, QVector<Result> > chunks(const QFuture<Result> &future, int from, int to)
    {
        QMap<int, QVector<Result> > chunks;

        for (int i = from; i < to; ++i) {
            const Result use = future.resultAt(i);
            const int blockNumber = use.line;
            chunks[blockNumber].append(use);
        }

        return chunks;
    }

protected:
    CheckSymbols(Document::Ptr doc);

    virtual bool preVisit(AST *);

    bool visit(ModuleAST *ast);
    bool visit(RoutineDeclarationAST *ast);
    bool visit(ConstDeclarationItemAST *ast);
    bool visit(TypeDeclarationItemAST *ast);
    bool visit(FieldDefinitionAST *ast);
    bool visit(ConstantNameAST *ast);
    bool visit(TypeNameAST *ast);
    bool visit(VariableNameAST *ast);
    bool visit(ProgramNameAST *ast);
    bool visit(RoutineNameAST *ast);

    bool visit(FakeVariableStatementAST *ast);
    bool visit(UsingStatementAST *ast);
    bool visit(PrimaryIdAST *ast);
    bool visit(ArrayIdAST *ast);

    void proceedStatementList(unsigned beginLocation);

    void flush();
    void addUse(unsigned tokenIndex, Kind kind);

private:
    Document::Ptr _doc;
    QVector<Result> _usages;
    int _chunkSize;
    const Scope *_scope;
    const Type *_type;
    const Type *_nulltype;
};

}   // namespace Karel

#endif // KARELCHECKSYMBOLS_H
