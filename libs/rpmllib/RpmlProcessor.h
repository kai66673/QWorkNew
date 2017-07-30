#ifndef RPMLPROCESSOR_H
#define RPMLPROCESSOR_H

#include <QObject>
#include <QTextDocument>

namespace RPML {

class RpmlProcessor: public QObject
{
    Q_OBJECT

public:
    RpmlProcessor( QTextDocument *textDocument, QObject *parent = 0 );

private:
    QTextDocument *m_textDocument;
};

}   // namespace RPML

#endif  // RPMLPROCESSOR_H
