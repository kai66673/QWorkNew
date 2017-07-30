/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef FORMTEMPLATEWIZARDPAGE_H
#define FORMTEMPLATEWIZARDPAGE_H

#include <QDesignerFormEditorInterface>

#include <jsonwizardpagefactory.h>

#include <wizardpage.h>

QT_BEGIN_NAMESPACE
class QDesignerNewFormWidgetInterface;
QT_END_NAMESPACE

namespace Designer {
namespace Internal {

class FormPageFactory : public ProjectExplorer::JsonWizardPageFactory
{
public:
    FormPageFactory(QDesignerFormEditorInterface *formEditor);

    Utils::WizardPage *create(ProjectExplorer::JsonWizard *wizard, Core::Id typeId, const QVariant &data);

    bool validateData(Core::Id typeId, const QVariant &data, QString *errorMessage);

private:
    QDesignerFormEditorInterface *m_formeditor;
};

// A wizard page embedding Qt Designer's QDesignerNewFormWidgetInterface
// widget.

// Sets FormContents property.

class FormTemplateWizardPage : public Utils::WizardPage
{
    Q_OBJECT

public:
    explicit FormTemplateWizardPage(QDesignerFormEditorInterface *formEditor, QWidget * parent = 0);

    virtual bool isComplete () const;
    virtual bool validatePage();

    QString templateContents() const { return  m_templateContents; }

    static QString stripNamespaces(const QString &className);

signals:
    void templateActivated();

private slots:
    void slotCurrentTemplateChanged(bool);

private:
    QString m_templateContents;
    QDesignerNewFormWidgetInterface *m_newFormWidget;
    bool m_templateSelected;
};

} // namespace Internal
} // namespace Designer

#endif // FORMTEMPLATEWIZARDPAGE_H
