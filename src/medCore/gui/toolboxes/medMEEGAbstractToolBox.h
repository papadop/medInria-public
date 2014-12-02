/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include "medToolBox.h"
#include "medCoreExport.h"

class medAbstractMEEGProcess;
class dtkPlugin;

class MEDCORE_EXPORT medMEEGAbstractToolBox : public medToolBox {
public:

    medMEEGAbstractToolBox(QWidget *parent = 0): medToolBox(parent) { }
    virtual ~medMEEGAbstractToolBox() { }

    /**
     * @brief Returns the plugin this custom Toolbox comes from.
     *
     * Used to fill the aboutPlugin button in the parent toolbox.
     *
     */
    virtual dtkPlugin *plugin()const = 0;
    
    virtual QString processName() = 0;
    virtual void setProcessParameters(medAbstractMEEGProcess *process) = 0;
};
