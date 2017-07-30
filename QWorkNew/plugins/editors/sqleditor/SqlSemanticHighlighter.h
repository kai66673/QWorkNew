#ifndef SQLSEMANTICHIGHLIGHTER_H
#define SQLSEMANTICHIGHLIGHTER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "SqlSemanticInfo.h"
#include "SqlEditorPlugin.h"
#include "IParserEngine.h"

namespace Sql {

class TranslationUnitAST;

namespace Editor {

class SemanticHighlighter: public QThread
{
    Q_OBJECT

public:
    SemanticHighlighter(QObject *parent = 0 );
    virtual ~SemanticHighlighter();

    void abort();

    struct Source
    {
        QString fileName;
        QString code;
        int line;
        int column;
        int revision;
        bool force;
        bool ended;

        IParserEngine *parserEngine;

        Source()
            : line(0), column(0), revision(0), force(false), ended(false), parserEngine(0)
        { }

        Source( const QString &code,
                int line, int column,
                int revision, IParserEngine *engine = 0 )
            : code(code), line(line), column(column)
            , revision(revision), force(false), ended(false), parserEngine(engine)
        { }

        void clear()
        {
            code.clear();
            line = 0;
            column = 0;
            revision = 0;
            force = false;
            ended = true;
        }
    };

    SemanticInfo semanticInfo(const Source &source);

    void rehighlight(const Source &source);

Q_SIGNALS:
    void changed( const Sql::SemanticInfo &semanticInfo );

protected:
    virtual void run();

private:
    bool isOutdated();

private:
    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_done;
    Source m_source;
    SemanticInfo m_lastSemanticInfo;
};

} // namespace Editor
} // namespace Sql

#endif // SQLSEMANTICHIGHLIGHTER_H
