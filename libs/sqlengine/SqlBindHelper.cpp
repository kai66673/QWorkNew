#include <QDebug>

#include "SqlBindHelper.h"
#include "SqlDefineBindingsDialog.h"

namespace Sql {

SqlBindHelper::SqlBindHelper()
{
}

bool SqlBindHelper::defineBindValues( const QSet<QString> &bindNames, QMap<QString, SqlBindValue> &bindings )
{
    bindings.clear();
    if ( bindNames.size() == 0 )
        return true;

    SqlDefineBindingsDialog defBindDialog(bindNames, bindingsHistory);
    if ( defBindDialog.exec() != QDialog::Accepted )
        return false;

    defBindDialog.getBindings(bindings, bindingsHistory);
    return true;
}

} // Sql
