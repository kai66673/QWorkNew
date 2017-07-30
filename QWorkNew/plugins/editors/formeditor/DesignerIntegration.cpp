#include "DesignerIntegration.h"
#include "ModeManager.h"

DesignerIntegration::DesignerIntegration(QDesignerFormEditorInterface *core, FormEditorPlugin *formPlugin, QObject *parent)
    : QDesignerIntegration(core, parent)
    , m_formPlugin(formPlugin)
{ }

DesignerIntegration::~DesignerIntegration()
{ }

void DesignerIntegration::updateSelection()
{
    QDesignerIntegration::updateSelection();
}

