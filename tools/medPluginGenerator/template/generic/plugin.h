// /////////////////////////////////////////////////////////////////
// Generated by medPluginGenerator
// /////////////////////////////////////////////////////////////////

#ifndef %2PLUGIN_H
#define %2PLUGIN_H

#include <dtkCore/dtkPlugin.h>

#include "%1PluginExport.h"

class %2PLUGIN_EXPORT %1PluginPrivate;

class %2PLUGIN_EXPORT %1Plugin : public dtkPlugin
{
    Q_OBJECT
    Q_INTERFACES(dtkPlugin)

public:
     %1Plugin(QObject *parent = 0);
    ~%1Plugin(void);

    virtual bool initialize(void);
    virtual bool uninitialize(void);

    virtual QString name(void) const;
    virtual QString identifier(void) const;
    virtual QString description(void) const;
    virtual QString contact(void) const;
    virtual QString version (void) const;
    virtual QStringList authors(void) const;
    virtual QStringList contributors(void) const;
    virtual QStringList dependencies(void) const;

    virtual QStringList tags(void) const;
    virtual QStringList types(void) const;

private:
     %1PluginPrivate *d;
};

#endif
