#ifndef KARELSYMBOLICONS_H
#define KARELSYMBOLICONS_H

#include <QIcon>

namespace Karel {

class KarelSymbolIcons
{
public:
    enum IconKind {
        Directive = 0,
        Constant,
        UserType,
        Keyword,
        Variable,
        Field,
        Function,
        KindCount
    };

    static QIcon icon(IconKind kind);
    static void init();

private:
    static QIcon m_icons[KindCount];
};

}   // namespace Karel

#endif // KARELSYMBOLICONS_H
