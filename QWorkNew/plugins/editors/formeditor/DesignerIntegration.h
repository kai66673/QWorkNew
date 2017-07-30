#ifndef DESIGNERINTEGRATION_H
#define DESIGNERINTEGRATION_H

#include <QObject>
#include <QDesignerIntegration>

#include "FormEditorPlugin.h"

class DesignerIntegration : public QDesignerIntegration
{
public:
    DesignerIntegration(QDesignerFormEditorInterface *core, FormEditorPlugin *formPlugin, QObject *parent = 0);
    virtual ~DesignerIntegration();

public slots:
    void updateSelection();

private:
    FormEditorPlugin *m_formPlugin;
};

#endif // DESIGNERINTEGRATION_H
