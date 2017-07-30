#include "ICodeModel.h"
#include "CodeModelManager.h"

ICodeModel::ICodeModel(const QString &id, QObject *parent)
    : QObject(parent)
    , m_id(id)
{
    m_registered = CodeModelManager::registerCodeModel(this, &m_registerErrorMessage);
}

ICodeModel::~ICodeModel()
{ }
