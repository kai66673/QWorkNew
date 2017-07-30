#include "KarelSemanticHighlighter.h"

namespace Karel {

SemanticHighlighter::SemanticHighlighter(QObject *parent)
    : QThread(parent)
    , m_done(false)
{ }

SemanticHighlighter::~SemanticHighlighter()
{ }

void SemanticHighlighter::abort()
{
    QMutexLocker locker(&m_mutex);
    m_done = true;
    m_condition.wakeOne();
}

SemanticInfo SemanticHighlighter::semanticInfo(const SemanticHighlighter::Source &source)
{
    m_mutex.lock();
    const int revision = m_lastSemanticInfo.revision;
    m_mutex.unlock();

    Document::Ptr doc;
    QList<Utils::DiagnosticMessage> diagnosticMessages;

    if ( !source.force && revision == source.revision ) {
        m_mutex.lock();
        doc = m_lastSemanticInfo.doc;
        diagnosticMessages = m_lastSemanticInfo.diagnosticMessages;
        m_mutex.unlock();
    }

    if ( !doc ) {
        doc = Document::create(source.fileName);
        doc->setSource(source.code.toLatin1());
        doc->check();
    }

    SemanticInfo semanticInfo;
    semanticInfo.revision = source.revision;
    semanticInfo.doc = doc;
    semanticInfo.diagnosticMessages = diagnosticMessages;

    return semanticInfo;
}

void SemanticHighlighter::rehighlight(const SemanticHighlighter::Source &source)
{
    QMutexLocker locker(&m_mutex);
    m_source = source;
    m_condition.wakeOne();
}

void SemanticHighlighter::run()
{
    setPriority(QThread::LowestPriority);

    forever {
        m_mutex.lock();

        while ( !( m_done || !m_source.ended) )
            m_condition.wait(&m_mutex);

        const bool done = m_done;
        const Source source = m_source;
        m_source.clear();

        m_mutex.unlock();

        if ( done )
            break;

        const SemanticInfo info = semanticInfo(source);

        if ( !isOutdated() ) {
            m_mutex.lock();
            m_lastSemanticInfo = info;
            m_mutex.unlock();

            emit changed(info);
        }
    }
}

bool SemanticHighlighter::isOutdated()
{
    QMutexLocker locker(&m_mutex);
    const bool outdated = !m_source.ended || m_done;
    return outdated;
}

}   // namespace Karel
