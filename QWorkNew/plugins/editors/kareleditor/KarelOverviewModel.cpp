#include <QDebug>

#include "KarelOverviewModel.h"
#include "KarelAST.h"
#include "KarelASTVisitor.h"
#include "KarelSymbolIcons.h"
#include "convenience.h"

namespace Karel {

class OverviewVisistor: protected ASTVisitor
{
public:
    OverviewVisistor(TranslationUnit *unit)
        : ASTVisitor(unit)
    { }

     void operator()(ModuleAST *ast, OverviewTreeItem *parentItem, QTextDocument *document) {
         m_parentItem = parentItem;
         m_document = document;

         if ( !ast->programName )
             return;

         const Token &tk = translationUnit()->tokenAt(ast->programName->t_identifier);
         OverviewTreeItem *programItem = new OverviewTreeItem(QString(tk.identifier->charsOrig()), tk.begin(), OverviewTreeItem::ProgramDeclaration, m_parentItem);
         m_parentItem->appandChild(programItem);

         m_parentItem = programItem;
         accept(ast->declarations);
         accept(ast->innerRoutines);
         if ( ast->t_begin ) {
             const Token &bodyTk = translationUnit()->tokenAt(ast->t_begin);
             OverviewTreeItem *bodyItem = new OverviewTreeItem(QStringLiteral("<Program Body>"), bodyTk.begin(), OverviewTreeItem::ProgramBody, m_parentItem);
             m_parentItem->appandChild(bodyItem);
         }

         m_parentItem = parentItem;
         accept(ast->outerRoutines);
     }

protected:
     bool visit(ConstDeclarationItemAST *ast) {
         if ( ast->constantName ) {
             const Token &tk = translationUnit()->tokenAt(ast->constantName->t_identifier);
             m_parentItem->appandChild(new OverviewTreeItem(QString(tk.identifier->charsOrig()), tk.begin(), OverviewTreeItem::ConstantDeclaration, m_parentItem));
         }
         return false;
     }

     bool visit(TypeDeclarationItemAST *ast) {
         if ( ast->typeName ) {
             const Token &tk = translationUnit()->tokenAt(ast->typeName->t_identifier);
             m_parentItem->appandChild(new OverviewTreeItem(QString(tk.identifier->charsOrig()), tk.begin(), OverviewTreeItem::TypeDeclaration, m_parentItem));
         }
         return false;
     }

     bool visit(VarDeclarationItemAST *ast) {
         for (VariableNameListAST *iter = ast->variables; iter; iter = iter->next) {
             if ( VariableNameAST *varName = iter->value ) {
                 const Token &tk = translationUnit()->tokenAt(varName->t_identifier);
                 m_parentItem->appandChild(new OverviewTreeItem(QString(tk.identifier->charsOrig()), tk.begin(), OverviewTreeItem::VariableDeclaration, m_parentItem));
             }
         }
         return false;
     }

     bool visit(RoutineDeclarationAST *ast) {
         if ( ast->definition && ast->definition->routineName ) {
             const Token &tk = translationUnit()->tokenAt(ast->definition->routineName->t_identifier);
             OverviewTreeItem *routineItem = new OverviewTreeItem(QString(tk.identifier->charsOrig()), tk.begin(), OverviewTreeItem::RoutineDeclaration, m_parentItem);
             m_parentItem->appandChild(routineItem);
             if ( ast->implementation ) {
                 if ( DirectRoutineImplemenationAST *impl = ast->implementation->asDirectRoutineImplemenation() ) {
                     OverviewTreeItem *oldParenItem = m_parentItem;
                     m_parentItem = routineItem;
                     accept(impl);
                     if ( impl->t_begin ) {
                         const Token &bTk = translationUnit()->tokenAt(impl->t_begin);
                         m_parentItem->appandChild(new OverviewTreeItem(QString("<") + QString(tk.identifier->charsOrig()) + QString(" Body>"),
                                                                        bTk.begin(), OverviewTreeItem::ProgramBody, m_parentItem));
                     }
                     m_parentItem = oldParenItem;
                 }
             }
         }
         return false;
     }

private:
     OverviewTreeItem *m_parentItem;
     QTextDocument *m_document;
};

OverviewTreeItem::OverviewTreeItem(const QString &name, int position, OverviewTreeItem::Kind kind, OverviewTreeItem *parent)
    : m_parent(parent)
    , m_name(name)
    , m_position(position)
    , m_kind(kind)
{ }

OverviewTreeItem::~OverviewTreeItem()
{ qDeleteAll(m_childrens); }

OverviewTreeItem *OverviewTreeItem::parent() const
{ return m_parent; }

QString OverviewTreeItem::name() const
{ return m_name; }

OverviewTreeItem::Kind OverviewTreeItem::kind() const
{ return m_kind; }

int OverviewTreeItem::position() const
{ return m_position; }

int OverviewTreeItem::childCount() const
{ return m_childrens.size(); }

OverviewTreeItem *OverviewTreeItem::childAt(int index)
{ return m_childrens[index]; }

void OverviewTreeItem::appandChild(OverviewTreeItem *child)
{ m_childrens.append(child); }

int OverviewTreeItem::row() const
{
    if ( m_parent )
        return m_parent->m_childrens.indexOf(const_cast<OverviewTreeItem *>(this));
    return 0;
}

OverviewModel::OverviewModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_editorRevision(0)
{
    m_rootItem = new OverviewTreeItem(QStringLiteral("<ROOT>"), 0, OverviewTreeItem::Root, 0);
    m_rootItem->appandChild(new OverviewTreeItem(QStringLiteral("<Select Item>"), 0, OverviewTreeItem::SelectHint, m_rootItem));
}

OverviewModel::~OverviewModel()
{ delete m_rootItem; }

QVariant OverviewModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
{ return QVariant(); }

QModelIndex OverviewModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !parent.isValid() )
        return createIndex(row, column, m_rootItem->childAt(row));

    OverviewTreeItem *parentItem = static_cast<OverviewTreeItem *>(parent.internalPointer());
    return createIndex(row, column, parentItem->childAt(row));
}

QModelIndex OverviewModel::parent(const QModelIndex &index) const
{
    if ( !index.isValid() )
        return QModelIndex();

    OverviewTreeItem *item = static_cast<OverviewTreeItem *>(index.internalPointer());
    OverviewTreeItem *parentItem = item->parent();

    if ( parentItem == m_rootItem )
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int OverviewModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_rootItem->childCount();

    OverviewTreeItem *parentItem = static_cast<OverviewTreeItem *>(parent.internalPointer());
    return parentItem->childCount();
}

int OverviewModel::columnCount(const QModelIndex & /*parent*/) const
{ return 1; }

QVariant OverviewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    OverviewTreeItem *item = static_cast<OverviewTreeItem *>(index.internalPointer());

    switch ( role ) {
        case Qt::DisplayRole:
            return item->name();

        case Qt::DecorationRole: {
            switch ( item->kind() ) {
                case OverviewTreeItem::ProgramDeclaration:
                    return QVariant();
                case OverviewTreeItem::ConstantDeclaration:
                    return KarelSymbolIcons::icon(KarelSymbolIcons::Constant);
                case OverviewTreeItem::TypeDeclaration:
                    return KarelSymbolIcons::icon(KarelSymbolIcons::UserType);
                case OverviewTreeItem::VariableDeclaration:
                    return KarelSymbolIcons::icon(KarelSymbolIcons::Variable);
                case OverviewTreeItem::RoutineDeclaration:
                    return KarelSymbolIcons::icon(KarelSymbolIcons::Function);
                default:
                    return QVariant();
            }
        }
    }

    return QVariant();
}

void OverviewModel::rebuild(Document::Ptr doc, QTextDocument *textDocument, unsigned revision)
{
    m_editorRevision = revision;

    beginResetModel();
    delete m_rootItem;
    m_rootItem = new OverviewTreeItem(QStringLiteral("<ROOT>"), 0, OverviewTreeItem::Root, 0);
    m_rootItem->appandChild(new OverviewTreeItem(QStringLiteral("<Select Item>"), 0, OverviewTreeItem::SelectHint, m_rootItem));
    if ( doc ) {
        if ( TranslationUnit *unit = doc->translationUnit() ) {
            if ( AST *ast = unit->ast() ) {
                if ( ModuleAST *moduleAst = ast->asModule() ) {
                    OverviewVisistor visitor(unit);
                    visitor(moduleAst, m_rootItem, textDocument);
                }
            }
        }
    }
    endResetModel();
}

OverviewTreeItem *OverviewModel::itemFromIndex(const QModelIndex &index)
{
    if ( !index.isValid() )
        return 0;

    return static_cast<OverviewTreeItem *>(index.internalPointer());
}

unsigned OverviewModel::editorRevision() const
{ return m_editorRevision; }

}   // namespace Karel
