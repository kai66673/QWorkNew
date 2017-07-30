#ifndef __IDB_FORWARD_H

#define __IDB_FORWARD_H

namespace Database {

class BaseConnectionInfo;

class IDbLoader;
class ItemProperties;

class IDbObject;
class IDbRootObject;

class DbSchemaList;
  class DbSchema;
    class DbObjectList;
      class DbTableList;
        class DbTable;
          class DbTableColumnList;
            class DbTableColumn;
          class DbTableTriggerList;
            class DbTableTrigger;
          class DbTableIndexList;
            class DbTableIndex;
          class DbTableConstraintList;
            class DbTableConstraint;
      class DbViewList;
        class DbView;
          class DbViewColumnList;
            class DbViewColumn;
      class DbIndexList;
        class DbIndex;
      class DbTriggerList;
        class DbTrigger;
      class DbConstraintList;
        class DbConstraint;

} // Database

#endif // __IDB_FORWARD_H

