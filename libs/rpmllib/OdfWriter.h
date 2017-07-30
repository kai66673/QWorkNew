#ifndef ODFWRITER_H
#define ODFWRITER_H

#include <QtCore/QXmlStreamWriter>
#include <QtCore/qset.h>
#include <QtCore/qstack.h>

#include <QTextList>
#include <QTextDocument>

#include "ReportData.h"

class QTextDocumentPrivate;
class QTextCursor;
class QTextBlock;
class QIODevice;
class QXmlStreamWriter;
class QTextOdfWriterPrivate;
class QTextBlockFormat;
class QTextCharFormat;
class QTextListFormat;
class QTextFrameFormat;
class QTextTableCellFormat;
class QTextFrame;
class QTextFragment;

namespace RPML {

class OutputStrategy;

class OdfWriter {
public:
    OdfWriter(const ReportData *m_reportData, QIODevice *device);
    bool writeAll();

    void setCodec(QTextCodec *codec) { m_codec = codec; }
    void setCreateArchive(bool on) { m_createArchive = on; }
    bool createArchive() const { return m_createArchive; }

    void writeBlock(QXmlStreamWriter &writer, const QTextBlock &block);
    void writeFormats(QXmlStreamWriter &writer, QSet<int> formatIds) const;
    void writeBlockFormat(QXmlStreamWriter &writer, QTextBlockFormat format, int formatIndex) const;
    void writeCharacterFormat(QXmlStreamWriter &writer, QTextCharFormat format, int formatIndex) const;
    void writeListFormat(QXmlStreamWriter &writer, QTextListFormat format, int formatIndex) const;
    void writeFrameFormat(QXmlStreamWriter &writer, QTextFrameFormat format, int formatIndex) const;
    void writeTableFormat(QXmlStreamWriter &writer, QTextTableFormat format, int formatIndex) const;
    void writeTableCellFormat(QXmlStreamWriter &writer, QTextTableCellFormat format, int formatIndex) const;
    void writeFrame(QXmlStreamWriter &writer, const QTextFrame *frame);
    void writeInlineCharacter(QXmlStreamWriter &writer, const QTextFragment &fragment) const;

    const QString officeNS, textNS, styleNS, foNS, tableNS, drawNS, xlinkNS, svgNS, numberNS;

private:
    void fillDocumentFormats( QSet<int> &formats );

    QTextDocument *m_document;
    const ReportData *m_reportData;
    QIODevice *m_device;
    QString m_styleNamePrefix;
    bool m_replaceFields;

    OutputStrategy *m_strategy;
    QTextCodec *m_codec;
    bool m_createArchive;

    QStack<QTextList *> m_listStack;
};

}   // namespace RPML

#endif // ODFWRITER_H
