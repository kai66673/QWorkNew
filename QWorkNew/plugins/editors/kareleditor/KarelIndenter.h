#ifndef KARELINDENTER_H
#define KARELINDENTER_H

#include <indenter.h>

namespace Karel {

class Indenter : public TextEditor::Indenter
{
public:
    Indenter();
    virtual ~Indenter();

    bool isElectricCharacter(const QChar &ch) const;
    void indentBlock(QTextDocument *document,
                     const QTextBlock &block,
                     const QChar &typedChar,
                     const TextEditor::TabSettings &settings);

private:
    void indentSpace(const QTextBlock &block,
                     const TextEditor::TabSettings &settings);
    void indentDirective(const QTextBlock &block,
                         const TextEditor::TabSettings &settings);
    void indentEnter(const QTextBlock &block,
                     const TextEditor::TabSettings &settings, bool firstStage);
    int findIdentetion(const QString &startText, const QTextBlock &block, const TextEditor::TabSettings &settings);
};

}   // namespace Karel

#endif // KARELINDENTER_H
