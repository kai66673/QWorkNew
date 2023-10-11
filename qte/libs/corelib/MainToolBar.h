#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QToolBar>

#include "StyleManager.h"
#include "corelib_global.h"

namespace Core {

class CORELIB_EXPORT MainToolBar: public QToolBar, public Utils::StyleManagedWidget
{
    Q_OBJECT

public:
    explicit MainToolBar(const QString &title, QWidget *parent = 0);
};

}   // namespace Core

#endif // MAINTOOLBAR_H
