#include "KarelSymbolIcons.h"

namespace Karel {

QIcon KarelSymbolIcons::m_icons[KarelSymbolIcons::KindCount];

QIcon KarelSymbolIcons::icon(KarelSymbolIcons::IconKind kind)
{
    switch (kind) {
        case Directive:
        case Constant:
        case UserType:
        case Keyword:
        case Variable:
        case Field:
        case Function:
            return m_icons[kind];
    }

    return QIcon();
}

void KarelSymbolIcons::init()
{
    m_icons[Directive]  = QIcon(":/kareleditor/images/percent.png");
    m_icons[Constant]   = QIcon(":/kareleditor/images/constant.png");
    m_icons[UserType]   = QIcon(":/kareleditor/images/type.png");
    m_icons[Keyword]    = QIcon(":/kareleditor/images/keyword.png");
    m_icons[Variable]   = QIcon(":/kareleditor/images/variable.png");
    m_icons[Field]      = QIcon(":/kareleditor/images/field.png");
    m_icons[Function]   = QIcon(":/kareleditor/images/function.png");
}

}   // namespace Karel
