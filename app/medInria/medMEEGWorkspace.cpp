/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medMEEGWorkspace.h"

#include <dtkCore/dtkSmartPointer.h>
#include <medAbstractMEEGProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>

#include <medAbstractLayeredView.h>
#include <medAbstractImageData.h>
//#include <medAbstractMEEGModelImageData.h>
#include <medDataManager.h>

#include <medViewContainer.h>
#include <medViewContainerManager.h>
#include <medTabbedViewContainers.h>
#include <medToolBox.h>
#include <medToolBoxFactory.h>

#include <medRunnableProcess.h>
#include <medJobManager.h>
#include <medMessageController.h>

class medMEEGWorkspacePrivate {
public:

#if 0
    medToolBox *fiberBundlingToolBox;
    QPointer<medViewContainer> diffusionContainer;

    medMEEGSelectorToolBox *diffusionEstimationToolBox;
    medMEEGSelectorToolBox *diffusionScalarMapsToolBox;
    medMEEGSelectorToolBox *diffusionTractographyToolBox;
#endif

    dtkSmartPointer <medAbstractMEEGProcess> currentProcess;
    bool processRunning;
};

medMEEGWorkspace::medMEEGWorkspace(QWidget *parent): medAbstractWorkspace(parent),d(new medMEEGWorkspacePrivate) {

#if 0
    d->diffusionContainer = 0;

    // -- Bundling toolbox --
    d->fiberBundlingToolBox = medToolBoxFactory::instance()->createToolBox("medFiberBundlingToolBox", parent);

    // -- Estimation toolbox --
    d->diffusionEstimationToolBox = new medMEEGSelectorToolBox(parent,medMEEGSelectorToolBox::Estimation);
    d->diffusionEstimationToolBox->setTitle("Model Estimation");

    connect(d->diffusionEstimationToolBox, SIGNAL(addToolBox(medToolBox *)),
            this, SLOT(addToolBox(medToolBox *)));
    connect(d->diffusionEstimationToolBox, SIGNAL(removeToolBox(medToolBox *)),
            this, SLOT(removeToolBox(medToolBox *)));

    // -- Scalar maps toolbox --
    d->diffusionScalarMapsToolBox = new medMEEGSelectorToolBox(parent,medMEEGSelectorToolBox::ScalarMaps);
    d->diffusionScalarMapsToolBox->setTitle("MEEG Scalar Maps");

    connect(d->diffusionScalarMapsToolBox, SIGNAL(addToolBox(medToolBox *)),
            this, SLOT(addToolBox(medToolBox *)));
    connect(d->diffusionScalarMapsToolBox, SIGNAL(removeToolBox(medToolBox *)),
            this, SLOT(removeToolBox(medToolBox *)));

    // -- Tractography toolbox --
    d->diffusionTractographyToolBox = new medMEEGSelectorToolBox(parent,medMEEGSelectorToolBox::Tractography);
    d->diffusionTractographyToolBox->setTitle("Tractography");

    connect(d->diffusionTractographyToolBox, SIGNAL(addToolBox(medToolBox *)),
            this, SLOT(addToolBox(medToolBox *)));
    connect(d->diffusionTractographyToolBox, SIGNAL(removeToolBox(medToolBox *)),
            this, SLOT(removeToolBox(medToolBox *)));

    // -- View toolboxes --

    QList<QString> toolboxNames = medToolBoxFactory::instance()->toolBoxesFromCategory("view");
    foreach(QString toolbox, toolboxNames)
    {
       addToolBox( medToolBoxFactory::instance()->createToolBox(toolbox, parent) );
    }

    connect(d->diffusionEstimationToolBox, SIGNAL(processRequested(QString, QString)), this, SLOT(runProcess(QString, QString)));
    connect(d->diffusionScalarMapsToolBox, SIGNAL(processRequested(QString, QString)), this, SLOT(runProcess(QString, QString)));
    connect(d->diffusionTractographyToolBox, SIGNAL(processRequested(QString, QString)), this, SLOT(runProcess(QString, QString)));

    connect(d->diffusionEstimationToolBox, SIGNAL(processCancelled()), this, SLOT(cancelProcess()));
    connect(d->diffusionTractographyToolBox, SIGNAL(processCancelled()), this, SLOT(cancelProcess()));

    this->addToolBox( d->diffusionEstimationToolBox );
    this->addToolBox( d->diffusionScalarMapsToolBox );
    this->addToolBox( d->diffusionTractographyToolBox );
#endif

    d->processRunning = false;
}

medMEEGWorkspace::~medMEEGWorkspace()
{
    delete d;
    d = NULL;
}

void medMEEGWorkspace::setupViewContainerStack() {

    //the stack has been instantiated in constructor

    if (!this->stackedViewContainers()->count()) {
        #if 0
        d->diffusionContainer = this->stackedViewContainers()->addContainerInTab(this->name());

        d->diffusionContainer->setClosingMode(medViewContainer::CLOSE_CONTAINER);
        d->diffusionContainer->setUserSplittable(false);
        d->diffusionContainer->setMultiLayered(true);

        connect (d->diffusionContainer,SIGNAL(viewContentChanged()), this, SLOT(updateToolBoxesInputs()));
        connect(this->stackedViewContainers(),SIGNAL(containersSelectedChanged()),this,SLOT(changeCurrentContainer()));
        #endif
    }
}

void medMEEGWorkspace::runProcess(QString processName,QString category) {
    if (d->processRunning)
        return;

    medRunnableProcess *runProcess = new medRunnableProcess;

    #if 0
    medMEEGSelectorToolBox * originToolbox = dynamic_cast <medMEEGSelectorToolBox *> (this->sender());

    if (!originToolbox)
        return;
    #endif

    d->currentProcess = dynamic_cast <medAbstractMEEGProcess *> (dtkAbstractProcessFactory::instance()->create(processName));
    #if 0
    originToolbox->setProcessParameters(d->currentProcess);
    #endif

    runProcess->setProcess(d->currentProcess);

    d->processRunning = true;
    this->stackedViewContainers()->setEnabled(false);

    medJobManager::instance()->registerJobItem(runProcess);
    connect(runProcess, SIGNAL(success(QObject*)), this, SLOT(getOutput()));
    connect(runProcess, SIGNAL(failure(QObject*)), this, SLOT(resetRunningFlags()));

    medMessageProgress *messageProgress = medMessageController::instance()->showProgress(category);

    messageProgress->setProgress(0);
    connect(runProcess, SIGNAL(progressed(int)), messageProgress, SLOT(setProgress(int)));
    connect(runProcess, SIGNAL(success(QObject*)), messageProgress, SLOT(success()));
    connect(runProcess, SIGNAL(failure(QObject*)), messageProgress, SLOT(failure()));

    QThreadPool::globalInstance()->start(dynamic_cast<QRunnable*>(runProcess));
}

void medMEEGWorkspace::cancelProcess() {
    d->currentProcess->cancel();
    this->resetRunningFlags();
}

void medMEEGWorkspace::getOutput() {
    this->stackedViewContainers()->setEnabled(true);
    medAbstractData *outputData = dynamic_cast<medAbstractData*>(d->currentProcess->output());

    if (!outputData)
        return;

    #if 0
    d->diffusionContainer->addData(outputData);
    #endif
    medDataManager::instance()->importData(outputData);

    this->resetRunningFlags();
}

void medMEEGWorkspace::resetRunningFlags() {
    d->processRunning = false;
    this->stackedViewContainers()->setEnabled(true);

#if 0
    d->diffusionEstimationToolBox->resetButtons();
    d->diffusionTractographyToolBox->resetButtons();
#endif
}

void medMEEGWorkspace::changeCurrentContainer() {

    QList <QUuid> containersSelectedList = this->stackedViewContainers()->containersSelected();
    if (!containersSelectedList.size())
        return;

    // This cannot happen while a process is running, container stack is disabled at that point

#if 0
    // For security, disconnect current connections
    if (!d->diffusionContainer.isNull())
        disconnect (d->diffusionContainer,SIGNAL(viewContentChanged()), this, SLOT(updateToolBoxesInputs()));

    // Now connect new container
    d->diffusionContainer = medViewContainerManager::instance()->container(containersSelectedList.first());
    if (!d->diffusionContainer.isNull())
    {
        d->diffusionContainer->setClosingMode(medViewContainer::CLOSE_CONTAINER);
        d->diffusionContainer->setUserSplittable(false);
        d->diffusionContainer->setMultiLayered(true);

        connect (d->diffusionContainer,SIGNAL(viewContentChanged()), this, SLOT(updateToolBoxesInputs()));
    }
#endif

    this->resetToolBoxesInputs();
}

void medMEEGWorkspace::updateToolBoxesInputs() {

#if 0
    medAbstractLayeredView *medView = dynamic_cast <medAbstractLayeredView *> (d->diffusionContainer->view());

    if (!medView)
        return;

    unsigned int layersCount = medView->layersCount();
    for (unsigned int i = 0;i < layersCount;++i)
        this->addToolBoxInput(medView->layerData(i));
#endif
}

void medMEEGWorkspace::addToolBoxInput(medAbstractData *data) {
    medAbstractImageData *medData = dynamic_cast <medAbstractImageData *> (data);
    if (!medData)
        return;

    #if 0
    if (medData->Dimension() == 4)
        d->diffusionEstimationToolBox->addInputImage(medData);

    if (dynamic_cast<medAbstractMEEGModelImageData*>(medData)!=0) {
        d->diffusionScalarMapsToolBox->addInputImage(medData);
        d->diffusionTractographyToolBox->addInputImage(medData);
    }
    #endif
}

void medMEEGWorkspace::resetToolBoxesInputs() {
    #if 0
    d->diffusionEstimationToolBox->clearInputs();
    d->diffusionScalarMapsToolBox->clearInputs();
    d->diffusionTractographyToolBox->clearInputs();
    #endif

    this->updateToolBoxesInputs();
}

bool medMEEGWorkspace::isUsable()
{
    medToolBoxFactory* tbFactory = medToolBoxFactory::instance();
    #if 0
    bool workspaceUsable = (tbFactory->toolBoxesFromCategory("diffusion-estimation").size()!=0)||
                           (tbFactory->toolBoxesFromCategory("diffusion-scalar-maps").size()!=0)||
                           (tbFactory->toolBoxesFromCategory("diffusion-tractography").size()!=0);

    return workspaceUsable;
    #else
    return true;
    #endif
}
