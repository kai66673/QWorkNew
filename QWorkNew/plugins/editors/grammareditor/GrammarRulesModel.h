#ifndef GRAMMARRULESMODEL_H
#define GRAMMARRULESMODEL_H

#include <QAbstractListModel>

#include <CppDocument.h>

namespace Grammar {

class GrammarRulesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Role { LineNumberRole = Qt::UserRole + 1 };

    explicit GrammarRulesModel( QObject *parent = 0 );

    virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

    CPlusPlus::Document::Ptr document() const;

public slots:
    void rebuild( CPlusPlus::Document::Ptr doc );

private:
    CPlusPlus::Document::GrammarRules *m_rules;
    CPlusPlus::Document::Ptr m_doc;
};

}   // namespace Grammar

#endif // GRAMMARRULESMODEL_H
