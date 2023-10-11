#ifndef DESIGNERDOCKTITLE_H
#define DESIGNERDOCKTITLE_H

#include <QWidget>

namespace Ui {
class DesignerDockTitle;
}

class DesignerDockTitle : public QWidget
{
    Q_OBJECT

public:
    explicit DesignerDockTitle(QWidget *parent = 0);
    ~DesignerDockTitle();

private:
    Ui::DesignerDockTitle *ui;
};

#endif // DESIGNERDOCKTITLE_H
