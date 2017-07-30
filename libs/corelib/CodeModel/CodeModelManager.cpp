#include "CodeModelManager.h"

CodeModelManager *CodeModelManager::m_instance = 0;

CodeModelManager::CodeModelManager(QObject *parent)
    : QObject(parent)
{
    m_instance = this;
}

CodeModelManager::~CodeModelManager()
{
    qDeleteAll(m_codeModels.values());
}

CodeModelManager *CodeModelManager::instance()
{ return m_instance; }

ICodeModel *CodeModelManager::codeModel(const QString &id)
{
    QMap<QString, ICodeModel *>::const_iterator it = m_instance->m_codeModels.find(id);
    return (it == m_instance->m_codeModels.constEnd() ? 0 : it.value());
}

bool CodeModelManager::registerCodeModel(ICodeModel *codeModel, QString *errorMessage)
{
    bool result;

    QMap<QString, ICodeModel *>::const_iterator it = m_instance->m_codeModels.find(codeModel->id());
    result = it == m_instance->m_codeModels.constEnd();
    if ( result )
        m_instance->m_codeModels[codeModel->id()] = codeModel;
    else
        *errorMessage = tr("Code Model with Id \'%1\' already registered").arg(codeModel->id());

    return result;
}
