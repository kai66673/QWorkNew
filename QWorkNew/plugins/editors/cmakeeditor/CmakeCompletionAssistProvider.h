#ifndef CMAKECOMPLETIONASSISTPROVIDER_H
#define CMAKECOMPLETIONASSISTPROVIDER_H

#include <QStringList>

#include <completionassistprovider.h>

namespace Cmake {

class CmakeCompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
    Q_OBJECT

public:
    CmakeCompletionAssistProvider();
    ~CmakeCompletionAssistProvider();
    void init();

    bool supportsEditor(const Core::Context &context) const Q_DECL_OVERRIDE;
    TextEditor::IAssistProcessor *createProcessor() const Q_DECL_OVERRIDE;
    QStringList variables() const;
    QStringList functions() const;

private:
    QStringList m_variables;
    QStringList m_functions;
};

}   // namespace Cmake

#endif // CMAKECOMPLETIONASSISTPROVIDER_H
