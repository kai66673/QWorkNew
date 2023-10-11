#ifndef DOCUMENTTOOLBAR_H
#define DOCUMENTTOOLBAR_H

#include <QToolBar>

#include "StyleManager.h"
#include "corelib_global.h"

class CORELIB_EXPORT DocumentToolBar: public QToolBar, public Utils::StyleManagedWidget
{
    Q_OBJECT

public:
    DocumentToolBar(QWidget *parent = 0);
};

#endif // DOCUMENTTOOLBAR_H
