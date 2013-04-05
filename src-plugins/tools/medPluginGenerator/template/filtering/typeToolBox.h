// /////////////////////////////////////////////////////////////////
// Generated by medPluginGenerator
// /////////////////////////////////////////////////////////////////

#pragma once

#include <medFilteringAbstractToolBox.h>
#include "%1PluginExport.h"

class %1ToolBoxPrivate;

class %2PLUGIN_EXPORT %1ToolBox : public medFilteringAbstractToolBox
{
    Q_OBJECT
    
public:
    %1ToolBox(QWidget *parent = 0);
    ~%1ToolBox();
    
    dtkAbstractData *processOutput();
    
    static bool registered();
    
signals:
    void success();
    void failure();
    
    public slots:
    void run();
    
private:
    %1ToolBoxPrivate *d;
};

