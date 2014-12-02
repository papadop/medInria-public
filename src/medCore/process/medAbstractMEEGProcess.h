/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medCoreExport.h>
#include <medAbstractProcess.h>

class medAbstractData;

class MEDCORE_EXPORT medAbstractMEEGProcess: public medAbstractProcess {
public:

    medAbstractMEEGProcess(medAbstractProcess*parent=NULL): medAbstractProcess(parent) { }
    virtual ~medAbstractMEEGProcess() { }

    virtual void setInputImage(medAbstractData* data) = 0;
};
