/*=========================================================================

medInria

Copyright (c) INRIA 2013. All rights reserved.
See LICENSE.txt for details.
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

=========================================================================*/

#include "medDiffusionWorkspace.h"

#include <dtkCore/dtkSmartPointer.h>
#include <medAbstractDiffusionProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>

#include <medAbstractLayeredView.h>
#include <medAbstractImageData.h>
#include <medAbstractDiffusionModelImageData.h>
#include <medDataManager.h>

#include <medViewContainer.h>
#include <medViewContainerManager.h>
#include <medTabbedViewContainers.h>
#include <medToolBox.h>
#include <medToolBoxFactory.h>

#include <medRunnableProcess.h>
#include <medJobManager.h>
#include <medMessageController.h>

class medDiffusionWorkspacePrivate
{
public:

    medToolBox *fiberBundlingToolBox;
    medViewContainer * diffusionContainer;

    medDiffusionSelectorToolBox *diffusionEstimationToolBox;
    medDiffusionSelectorToolBox *diffusionScalarMapsToolBox;
    medDiffusionSelectorToolBox *diffusionTractographyToolBox;

    dtkSmartPointer <medAbstractDiffusionProcess> currentProcess;
    bool processRunning;
};

medDiffusionWorkspace::medDiffusionWorkspace(QWidget *parent) : medAbstractWorkspace(parent), d(new medDiffusionWorkspacePrivate)
{
    d->diffusionContainer = 0;

    // -- Bundling toolbox --
    d->fiberBundlingToolBox = medToolBoxFactory::instance()->createToolBox("medFiberBundlingToolBox", parent);

    // -- Estimation toolbox --
    d->diffusionEstimationToolBox = new medDiffusionSelectorToolBox(parent,medDiffusionSelectorToolBox::Estimation);
    d->diffusionEstimationToolBox->setTitle("Model Estimation");

    connect(d->diffusionEstimationToolBox, SIGNAL(addToolBox(medToolBox *)),
            this, SLOT(addToolBox(medToolBox *)));
    connect(d->diffusionEstimationToolBox, SIGNAL(removeToolBox(medToolBox *)),
            this, SLOT(removeToolBox(medToolBox *)));

    // -- Scalar maps toolbox --
    d->diffusionScalarMapsToolBox = new medDiffusionSelectorToolBox(parent,medDiffusionSelectorToolBox::ScalarMaps);
    d->diffusionScalarMapsToolBox->setTitle("Diffusion Scalar Maps");

    connect(d->diffusionScalarMapsToolBox, SIGNAL(addToolBox(medToolBox *)),
            this, SLOT(addToolBox(medToolBox *)));
    connect(d->diffusionScalarMapsToolBox, SIGNAL(removeToolBox(medToolBox *)),
            this, SLOT(removeToolBox(medToolBox *)));

    // -- Tractography toolbox --
    d->diffusionTractographyToolBox = new medDiffusionSelectorToolBox(parent,medDiffusionSelectorToolBox::Tractography);
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

    d->processRunning = false;
}

medDiffusionWorkspace::~medDiffusionWorkspace()
{
    delete d;
    d = NULL;
}

QString medDiffusionWorkspace::identifier() const {
    return "Diffusion";
}

QString medDiffusionWorkspace::description() const {
    return "Diffusion";
}

void medDiffusionWorkspace::setupViewContainerStack()
{
    //the stack has been instantiated in constructor
    if ( ! this->stackedViewContainers()->count())
    {
        d->diffusionContainer = this->stackedViewContainers()->addContainerInTab(identifier());

        d->diffusionContainer->setUserClosable(true);
        d->diffusionContainer->setUserSplittable(false);
        d->diffusionContainer->setMultiLayered(true);
        
        connect (d->diffusionContainer,SIGNAL(viewContentChanged()), this, SLOT(updateToolBoxesInputs()));
        connect(this->stackedViewContainers(),SIGNAL(containersSelectedChanged()),this,SLOT(changeCurrentContainer()));
    }
}

void medDiffusionWorkspace::runProcess(QString processName, QString category)
{
    if (d->processRunning)
        return;

    medRunnableProcess *runProcess = new medRunnableProcess;

    medDiffusionSelectorToolBox * originToolbox = dynamic_cast <medDiffusionSelectorToolBox *> (this->sender());

    if (!originToolbox)
        return;

    d->currentProcess = dynamic_cast <medAbstractDiffusionProcess *> (dtkAbstractProcessFactory::instance()->create(processName));
    originToolbox->setProcessParameters(d->currentProcess);
    
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

void medDiffusionWorkspace::cancelProcess()
{
    d->currentProcess->cancel();
    this->resetRunningFlags();
}

void medDiffusionWorkspace::getOutput()
{
    this->stackedViewContainers()->setEnabled(true);
    dtkSmartPointer <medAbstractData> outputData = dynamic_cast <medAbstractData *> (d->currentProcess->output());

    if (!outputData)
        return;

    d->diffusionContainer->addData(outputData);

    QString uuid = QUuid::createUuid().toString();
    medDataManager::instance()->importNonPersistent (outputData, uuid);

    this->resetRunningFlags();
}

void medDiffusionWorkspace::resetRunningFlags()
{
    d->processRunning = false;
    this->stackedViewContainers()->setEnabled(true);

    d->diffusionEstimationToolBox->resetButtons();
    d->diffusionTractographyToolBox->resetButtons();
}

void medDiffusionWorkspace::changeCurrentContainer()
{
    QList <QUuid> containersSelectedList = this->stackedViewContainers()->containersSelected();
    if (!containersSelectedList.size())
        return;

    // This cannot happen while a process is running, container stack is disabled at that point

    // For security, disconnect current connections
    if (d->diffusionContainer)
        disconnect (d->diffusionContainer,SIGNAL(viewContentChanged()), this, SLOT(updateToolBoxesInputs()));

    // Now connect new container
    d->diffusionContainer = medViewContainerManager::instance()->container(containersSelectedList.first());
    if (d->diffusionContainer)
    {
        d->diffusionContainer->setUserClosable(true);
        d->diffusionContainer->setUserSplittable(false);
        d->diffusionContainer->setMultiLayered(true);

        connect (d->diffusionContainer,SIGNAL(viewContentChanged()), this, SLOT(updateToolBoxesInputs()));
    }

    this->resetToolBoxesInputs();
}

void medDiffusionWorkspace::updateToolBoxesInputs()
{
    medAbstractLayeredView *medView = dynamic_cast <medAbstractLayeredView *> (d->diffusionContainer->view());

    if (!medView)
        return;
    
    unsigned int layersCount = medView->layersCount();
    for (unsigned int i = 0;i < layersCount;++i)
        this->addToolBoxInput(medView->layerData(i));
}

void medDiffusionWorkspace::addToolBoxInput(medAbstractData *data)
{
    dtkSmartPointer <medAbstractImageData> medData = dynamic_cast <medAbstractImageData *> (data);
    if (!medData)
        return;

    if (medData->Dimension() == 4)
        d->diffusionEstimationToolBox->addInputImage(medData);

    if (medData->dataCategory() == medData::DIFFUSION_MODEL)
    {
        d->diffusionScalarMapsToolBox->addInputImage(medData);
        d->diffusionTractographyToolBox->addInputImage(medData);
    }
}

void medDiffusionWorkspace::resetToolBoxesInputs()
{
    d->diffusionEstimationToolBox->clearInputs();
    d->diffusionScalarMapsToolBox->clearInputs();
    d->diffusionTractographyToolBox->clearInputs();
    
    this->updateToolBoxesInputs();
}

bool medDiffusionWorkspace::isUsable()
{
    medToolBoxFactory * tbFactory = medToolBoxFactory::instance();
    bool workspaceUsable = (tbFactory->toolBoxesFromCategory("diffusion-estimation").size()!=0)||
                           (tbFactory->toolBoxesFromCategory("diffusion-scalar-maps").size()!=0)||
                           (tbFactory->toolBoxesFromCategory("diffusion-tractography").size()!=0);

    return workspaceUsable;
}
