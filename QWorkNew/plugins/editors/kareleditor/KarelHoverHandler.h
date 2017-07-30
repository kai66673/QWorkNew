#ifndef KARELHOVERHANDLER_H
#define KARELHOVERHANDLER_H

#include <basehoverhandler.h>

namespace Karel {
namespace Editor {

class KarelHoverHandler: public TextEditor::BaseHoverHandler
{
    Q_OBJECT

public:
    KarelHoverHandler(QObject *parent = 0);
    virtual ~KarelHoverHandler();

private:
    virtual bool acceptEditor( Core::IEditor *editor );
    virtual void identifyMatch( TextEditor::ITextEditor *editor, int pos );
    virtual void decorateToolTip();
};

}   // namespace Editor
}   // namespace Karel

#endif // KARELHOVERHANDLER_H
