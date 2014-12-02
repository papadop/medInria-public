/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once


#include <QtCore>

#include <medAbstractWorkspace.h>
//#include <medMEEGSelectorToolBox.h>

class medAbstractData;
class medMEEGWorkspacePrivate;

class medMEEGWorkspace : public medAbstractWorkspace {

    Q_OBJECT
    MED_WORKSPACE_INTERFACE("MEEG",
                            "MEEG Forward Problem Builder.")

public:

     medMEEGWorkspace(QWidget* parent = 0);
    ~medMEEGWorkspace();

    static bool isUsable();
    void setupViewContainerStack();
    
public slots:

    void runProcess(QString processName, QString category);
    void getOutput();
    void cancelProcess();
    void resetRunningFlags();
    
    //Handle new data in central view, connect them to toolboxes

    void resetToolBoxesInputs();
    void updateToolBoxesInputs();
    void addToolBoxInput(medAbstractData *data);

    void changeCurrentContainer();

private:

    medMEEGWorkspacePrivate* d;
};
