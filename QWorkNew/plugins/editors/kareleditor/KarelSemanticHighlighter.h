#ifndef KARELSEMANTICHIGHLIGHTER_H
#define KARELSEMANTICHIGHLIGHTER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "KarelSemanticInfo.h"

namespace Karel {

class SemanticHighlighter: public QThread
{
    Q_OBJECT

public:
    SemanticHighlighter(QObject *parent = 0);
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

        Source()
            : line(0), column(0), revision(0), force(false), ended(false)
        { }

        Source( const QString &code,
                int line, int column,
                int revision )
            : code(code), line(line), column(column)
            , revision(revision), force(false), ended(false)
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
    void changed(const Karel::SemanticInfo &semanticInfo);

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

}   // namespace Karel

#endif // KARELSEMANTICHIGHLIGHTER_H
