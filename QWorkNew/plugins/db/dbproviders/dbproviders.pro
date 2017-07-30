TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += Db2Provider \
           OracleProvider \
           QlalrOracleProvider \
           SqliteProvider \
           PostgresProvider \
           LinterProvider \
           CassandraProvider

win32: SUBDIRS += OracleRdbProvider
