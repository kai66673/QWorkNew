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
#ifndef CPLUSPLUS_SIMPLELEXER_H
#define CPLUSPLUS_SIMPLELEXER_H

#include <CPlusPlusForwardDeclarations.h>

#include <QString>
#include <QList>
#include <QTextDocument>

namespace CPlusPlus {

class SimpleLexer;
class Token;

class CPLUSPLUS_EXPORT SimpleLexer
{
public:
    SimpleLexer();
    ~SimpleLexer();

    bool skipComments() const;
    void setSkipComments(bool skipComments);

    bool qtMocRunEnabled() const;
    void setQtMocRunEnabled(bool enabled);

    bool objCEnabled() const;
    void setObjCEnabled(bool onoff);

    bool singleLineCommentEnabled() const;
    void setSingleLineCommentEnabled( bool onoff );

    bool extraExecSqlEnabled() const;
    void setExtraExecSqlEnabled( bool onoff );

    bool endedJoined() const;

    QList<Token> operator()(const QString &text, int state = 0, int sqlState = 0);

    int state() const
    { return _lastState; }

    int sqlState() const
    { return _lastSqlState; }

    static int tokenAt(const QList<Token> &tokens, unsigned offset);
    static Token tokenAt(const QString &text,
                         unsigned offset,
                         int state,
                         bool qtMocRunEnabled = false);

    static bool inBracesOrIncomment( const QString &text, unsigned offset, int state );
    static int skipFirstComment(QTextDocument *doc);

    static int tokenBefore(const QList<Token> &tokens, unsigned offset);

private:
    int _lastState;
    int _lastSqlState;
    bool _skipComments: 1;
    bool _qtMocRunEnabled: 1;
    bool _objCEnabled: 1;
    bool _endedJoined: 1;
    bool _singleLineCommentsEnabled: 1;
    bool _extraExecSqlEnabled: 1;
};

} // namespace CPlusPlus

#endif // CPLUSPLUS_SIMPLELEXER_H
