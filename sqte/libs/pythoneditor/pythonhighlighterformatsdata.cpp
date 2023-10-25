#include "pythonhighlighterformatsdata.h"

namespace PythonEditor {

PythonHighlighterFormatData::PythonHighlighterFormatData() {
    m_formats.append( new HighlighterFormat( QObject::tr("Number"),
                                             false,
                                             false,
                                             QColor(  0,   0, 224),
                                             QColor() ) );

    m_formats.append( new HighlighterFormat( QObject::tr("String"),
                                             false,
                                             false,
                                             QColor(  0,   0, 128),
                                             QColor() ) );

    m_formats.append( new HighlighterFormat( QObject::tr("Keyword"),
                                             true,
                                             false,
                                             QColor(  0,   0,   0),
                                             QColor() ) );

    m_formats.append( new HighlighterFormat( QObject::tr("Type"),
                                             true,
                                             false,
                                             QColor(224,   0,   0),
                                             QColor() ) );

    m_formats.append( new HighlighterFormat( QObject::tr("Class Field"),
                                             true,
                                             false,
                                             QColor(224,  96,   0),
                                             QColor() ) );

    m_formats.append( new HighlighterFormat( QObject::tr("Magic Attribute"),
                                             true,
                                             true,
                                             QColor(  0, 224,  24),
                                             QColor() ) );

    m_formats.append( new HighlighterFormat( QObject::tr("Operator"),
                                             false,
                                             false,
                                             QColor(160, 64, 0),
                                             QColor() ) );

    m_formats.append( new HighlighterFormat( QObject::tr("Comment"),
                                             false,
                                             false,
                                             QColor(224,   0,   0),
                                             QColor() ) );

    m_formats.append( new HighlighterFormat( QObject::tr("Doxygen"),
                                             true,
                                             false,
                                             QColor(165,   0,   0),
                                             QColor() ) );

    m_formats.append( new HighlighterFormat( QObject::tr("Identifier"),
                                             false,
                                             false,
                                             QColor(44, 62, 80),
                                             QColor() ) );

    m_formats.append( new HighlighterFormat( QObject::tr("Whitespace"),
                                             true,
                                             false,
                                             QColor(  0,   0, 165),
                                             QColor() ) );

    m_formats.append( new HighlighterFormat( QObject::tr("Imported Module"),
                                             true,
                                             false,
                                             QColor(  0, 165, 165),
                                             QColor() ) );
}

QString PythonHighlighterFormatData::languageId() const { return QObject::tr("Python"); }

}   // namespace PythonEditor
