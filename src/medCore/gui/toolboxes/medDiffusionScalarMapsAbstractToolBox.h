/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medDiffusionAbstractToolBox.h>

class MEDCORE_EXPORT medDiffusionScalarMapsAbstractToolBox: public medDiffusionAbstractToolBox {

    Q_OBJECT

public:

    medDiffusionScalarMapsAbstractToolBox(QWidget *parent = 0): medDiffusionAbstractToolBox(parent) { }
    virtual ~medDiffusionScalarMapsAbstractToolBox() { }
    
signals:

    void processStartRequested();
};
