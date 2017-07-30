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
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include <QCoreApplication>

#include "cppvirtualfunctionassistprovider.h"

#include "texteditor.h"
#include "cppeditorconstants.h"
#include "cppvirtualfunctionproposalitem.h"

#include <Icons.h>
#include <Overview.h>

#include <cpptools/functionutils.h>
#include <cpptools/symbolfinder.h>
#include <TypeHierarchyBuilder.h>

#include <codeassist/basicproposalitemlistmodel.h>
#include <codeassist/genericproposal.h>
#include <codeassist/genericproposalwidget.h>
#include <codeassist/iassistinterface.h>
#include <codeassist/iassistprocessor.h>
#include <codeassist/iassistproposal.h>
#include <texteditorconstants.h>

#include <qtcassert.h>
#include <qtcoverride.h>

using namespace CPlusPlus;
using namespace CppEditor::Internal;
using namespace CppTools;
using namespace TextEditor;

/// Activate current item with the same shortcut that is configured for Follow Symbol Under Cursor.
/// This is limited to single-key shortcuts without modifiers.
class VirtualFunctionProposalWidget : public GenericProposalWidget
{
public:
    VirtualFunctionProposalWidget()
    { }

protected:
    bool eventFilter(QObject *o, QEvent *e)
    { return GenericProposalWidget::eventFilter(o, e); }

    void showProposal(const QString &prefix) QTC_OVERRIDE
    {
        IGenericProposalModel *proposalModel = model();
        if (proposalModel && proposalModel->size() == 1) {
            emit proposalItemActivated(proposalModel->proposalItem(0));
            deleteLater();
            return;
        }
        GenericProposalWidget::showProposal(prefix);
    }
};

class VirtualFunctionProposal : public GenericProposal
{
public:
    VirtualFunctionProposal(int cursorPos, IGenericProposalModel *model)
        : GenericProposal(cursorPos, model)
    {}

    bool isFragile() const QTC_OVERRIDE { return true; }

    IAssistProposalWidget *createWidget() const QTC_OVERRIDE
    { return new VirtualFunctionProposalWidget(); }
};

static Core::ILink *linkToSymbol(CPlusPlus::Symbol *symbol)
{
    if (!symbol)
        return 0;

    const QString fileName = QString::fromUtf8(symbol->fileName(),
                                               symbol->fileNameLength());
    unsigned line = symbol->line();
    unsigned column = symbol->column();

    if (column)
        --column;

    if (symbol->isGenerated())
        column = 0;

    return new Core::TextTargetLink(0, 0, fileName, line, column);
}


class VirtualFunctionsAssistProcessor : public IAssistProcessor
{
public:
    VirtualFunctionsAssistProcessor(const VirtualFunctionAssistProvider::Parameters &params)
        : m_params(params)
    {}

    IAssistProposal *immediateProposal(const TextEditor::IAssistInterface *) QTC_OVERRIDE
    {
        QTC_ASSERT(m_params.function, return 0);

        BasicProposalItem *hintItem = new VirtualFunctionProposalItem(0);
        hintItem->setText(QCoreApplication::translate("VirtualFunctionsAssistProcessor",
                                                      "...searching overrides"));
        hintItem->setOrder(-1000);

        QList<BasicProposalItem *> items;
        items << itemFromFunction(m_params.function);
        items << hintItem;
        return new VirtualFunctionProposal(m_params.cursorPosition,
                                           new BasicProposalItemListModel(items));
    }

    IAssistProposal *perform(const IAssistInterface *) QTC_OVERRIDE
    {
        QTC_ASSERT(m_params.function, return 0);
        QTC_ASSERT(m_params.staticClass, return 0);
        QTC_ASSERT(!m_params.snapshot.isEmpty(), return 0);

        Class *functionsClass = m_finder.findMatchingClassDeclaration(m_params.function,
                                                                      m_params.snapshot);
        if (!functionsClass)
            return 0;

        const QList<Function *> overrides = FunctionUtils::overrides(
            m_params.function, functionsClass, m_params.staticClass, m_params.snapshot);
        if (overrides.isEmpty())
            return 0;

        QList<BasicProposalItem *> items;
        foreach (Function *func, overrides)
            items << itemFromFunction(func);
        items.first()->setOrder(1000); // Ensure top position for function of static type

        return new VirtualFunctionProposal(m_params.cursorPosition,
                                           new BasicProposalItemListModel(items));
    }

private:
    Function *maybeDefinitionFor(Function *func) const
    {
        if (Function *definition = m_finder.findMatchingDefinition(func, m_params.snapshot))
            return definition;
        return func;
    }

    BasicProposalItem *itemFromFunction(Function *func) const
    {
        const Core::ILink *link = linkToSymbol(maybeDefinitionFor(func));
        QString text = m_overview.prettyName(LookupContext::fullyQualifiedName(func));
        if (func->isPureVirtual())
            text += QLatin1String(" = 0");

        BasicProposalItem *item = new VirtualFunctionProposalItem(link);
        item->setText(text);
        item->setIcon(m_icons.iconForSymbol(func));

        return item;
    }

    VirtualFunctionAssistProvider::Parameters m_params;
    Overview m_overview;
    Icons m_icons;
    mutable CppTools::SymbolFinder m_finder;
};

VirtualFunctionAssistProvider::VirtualFunctionAssistProvider()
{
}

bool VirtualFunctionAssistProvider::configure(const Parameters &parameters)
{
    m_params = parameters;
    return true;
}

bool VirtualFunctionAssistProvider::isAsynchronous() const
{
    return true;
}

bool VirtualFunctionAssistProvider::supportsEditor(const Core::Context &context) const
{
    return context.contains(CppEditor::Constants::CPPEDITOR_ID);
}

IAssistProcessor *VirtualFunctionAssistProvider::createProcessor() const
{
    return new VirtualFunctionsAssistProcessor(m_params);
}
