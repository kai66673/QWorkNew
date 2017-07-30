#ifndef SQLHOVERHANDLER_H
#define SQLHOVERHANDLER_H

#include <basehoverhandler.h>

namespace Core {
    class IEditor;
}

namespace Sql {
namespace Editor {

class SqlHoverHandler : public TextEditor::BaseHoverHandler
{
    Q_OBJECT

public:
    SqlHoverHandler( QObject *parent = 0 );
    virtual ~SqlHoverHandler();
    
private:
    virtual bool acceptEditor( Core::IEditor *editor );
    virtual void identifyMatch( TextEditor::ITextEditor *editor, int pos );
    virtual void decorateToolTip();
};

} // namespace Editor
} // namespace Sql

#endif // SQLHOVERHANDLER_H
