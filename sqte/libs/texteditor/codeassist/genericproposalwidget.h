/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#ifndef GENERICPROPOSALWIDGET_H
#define GENERICPROPOSALWIDGET_H

#include "iassistproposalwidget.h"

#include <texteditor_global.h>

#include <qtcoverride.h>

namespace TextEditor {

class GenericProposalWidgetPrivate;
class IGenericProposalModel;

class TEXTEDITOR_EXPORT GenericProposalWidget : public IAssistProposalWidget
{
    Q_OBJECT
    friend class GenericProposalWidgetPrivate;

public:
    GenericProposalWidget();
    ~GenericProposalWidget();

    void setAssistant(CodeAssistant *assistant) QTC_OVERRIDE;
    void setReason(AssistReason reason) QTC_OVERRIDE;
    void setKind(AssistKind kind) QTC_OVERRIDE;
    void setUnderlyingWidget(const QWidget *underlyingWidget) QTC_OVERRIDE;
    void setModel(IAssistProposalModel *model) QTC_OVERRIDE;
    void setDisplayRect(const QRect &rect) QTC_OVERRIDE;
    void setIsSynchronized(bool isSync) QTC_OVERRIDE;

    void showProposal(const QString &prefix) QTC_OVERRIDE;
    void updateProposal(const QString &prefix) QTC_OVERRIDE;
    void closeProposal() QTC_OVERRIDE;

private:
    bool updateAndCheck(const QString &prefix);
    void notifyActivation(int index);
protected:
    void abort();

private slots:
    void updatePositionAndSize();
    void turnOffAutoWidth();
    void turnOnAutoWidth();

protected:
    bool eventFilter(QObject *o, QEvent *e);
    bool activateCurrentProposalItem();
    IGenericProposalModel *model();

private:
    GenericProposalWidgetPrivate *d;
};

} // TextEditor

#endif // GENERICPROPOSALWIDGET_H
