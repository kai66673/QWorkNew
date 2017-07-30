#ifndef SQLHIGHLIGHTER_H
#define SQLHIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
class QSettings;
QT_END_NAMESPACE

class SqlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    SqlHighlighter(QTextDocument *parent = 0);
    virtual ~SqlHighlighter();

public:
    void saveSettings();

protected:
    void highlightBlock(const QString &text);

private:
    QSettings* _config;

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> _highlightingRules;

    QRegExp _commentStartExpression;
    QRegExp _commentEndExpression;

    QTextCharFormat _numberFormat;
    QTextCharFormat _keywordFormat;
    QVariantList _keywords;
    QTextCharFormat _operatorFormat;
    QVariantList _operators;
    QTextCharFormat _dataTypeFormat;
    QVariantList _dataTypes;
    QTextCharFormat _functionFormat;
    QVariantList _functions;
    QTextCharFormat _quotationFormat;
    QTextCharFormat _singleLineCommentFormat;
    QTextCharFormat _multiLineCommentFormat;
};

#endif