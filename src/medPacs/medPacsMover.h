/*=========================================================================

 MedInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once


#include <medJobItem.h>
#include "medPacsExport.h"
#include "medMoveCommandItem.h"

class medPacsMoverPrivate;


class MEDPACS_EXPORT medPacsMover : public medJobItem
{
    Q_OBJECT

public:
     medPacsMover(const QVector<medMoveCommandItem>& cmdList);
    ~medPacsMover();

    void run();

    void doQueuedMove();

signals:
    void import(QString);
   
public slots:
    void onCancel(QObject*);

private slots:
    void progressForward(int);

private:
    medPacsMoverPrivate *d;
};


