#ifndef IPYTHONMODULE_H
#define IPYTHONMODULE_H

#include <QObject>

#include "corelib_global.h"

namespace Python {

class CORELIB_EXPORT PythonModuleFunction
{
public:
    PythonModuleFunction( const QString &name, const QString &description )
        : m_name(name)
        , m_description(description)
    {}

    QString name() const { return m_name; }
    QString description() const { return m_description; }

private:
    QString m_name;
    QString m_description;
};

class CORELIB_EXPORT BuiltinPythonScript
{
public:
    BuiltinPythonScript( const QString &scriptFile )
        : m_scriptFile(scriptFile)
    {}

    QString scriptFile() const { return m_scriptFile; }

private:
    QString m_scriptFile;
};

class CORELIB_EXPORT IPythonModule: public QObject
{
public:
    IPythonModule( const QString &moduleName, const QString &objName,
                   const QString &description, QObject *parent = 0 )
        : QObject(parent)
        , m_moduleName(moduleName)
        , m_objectName(objName)
        , m_description(description)
    {}

    virtual QList<PythonModuleFunction *> functions() const  = 0;
    virtual QString sourceCodeForAssist() const { return QStringLiteral(""); }

    QString moduleName() const { return m_moduleName; }
    QString objName() const { return m_objectName; }
    QString description() const { return m_description; }

    virtual QMetaObject *metaObjectForFunction( const QString &/*functionName*/ ) const { return 0; }

private:
    QString m_moduleName;
    QString m_objectName;
    QString m_description;
};

}   // namespace Python

#endif // IPYTHONMODULE_H
