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

#ifndef ABSTRACTEDITORSUPPORT_H
#define ABSTRACTEDITORSUPPORT_H

#include "corelib_global.h"
#include "ICodeModel.h"

#include <QString>
#include <QObject>

class ICodeModel;

namespace CodeModel {

class CORELIB_EXPORT AbstractEditorSupport : public QObject
{
    Q_OBJECT
public:
    explicit AbstractEditorSupport(ICodeModel *codemodel);
    virtual ~AbstractEditorSupport();

    /// \returns the contents, encoded as UTF-8
    virtual QByteArray contents() const = 0;
    virtual QString fileName() const = 0;

    void updateDocument();
    unsigned revision() const { return m_revision; }

    static QString licenseTemplate(const QString &file = QString(), const QString &className = QString());

private:
    ICodeModel *m_codemodel;
    unsigned m_revision;
};

} // namespace CodeModel

#endif // ABSTRACTEDITORSUPPORT_H
