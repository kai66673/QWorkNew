#include <QDebug>

#include <TranslationUnit.h>
#include <AST.h>

#include "GrammarRulesModel.h"

namespace Grammar {

GrammarRulesModel::GrammarRulesModel (QObject *parent )
    : QAbstractListModel(parent)
    , m_rules(0)
{
}

int GrammarRulesModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    if ( m_rules )
        return m_rules->size() + 1;
    return 1;
}

QVariant GrammarRulesModel::data( const QModelIndex &index, int role ) const
{
    if ( role == Qt::DisplayRole ) {
        if ( index.row() == 0 )
            return tr("<No Rule Selected>");
        return m_rules->at(index.row() - 1).first;
    }

    else if ( role == LineNumberRole ) {
        if ( index.row() == 0 )
            return -1;
        return m_rules->at(index.row() - 1).second;
    }

    return QVariant();
}

CPlusPlus::Document::Ptr GrammarRulesModel::document() const
{
    return m_doc;
}

void GrammarRulesModel::rebuild( CPlusPlus::Document::Ptr doc )
{
    beginResetModel();
    m_rules = doc->grammarRules();
    m_doc = doc;
    endResetModel();
}

}   // namespace Grammar
