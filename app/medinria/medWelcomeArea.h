/* medWelcomeArea.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Oct  5 10:47:56 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Wed Jun 30 13:07:35 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 7
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MEDWELCOMEAREA_H
#define MEDWELCOMEAREA_H

#include <QtCore>
#include <QtGui>

class medWelcomeAreaPrivate;

class medWelcomeArea : public QWidget
{
    Q_OBJECT

public:
     medWelcomeArea(QWidget *parent = 0);
    ~medWelcomeArea(void);

    void setup(QStatusBar *status);
    void setdw(QStatusBar *status);

signals:
    void switchToBrowserArea(void);
    void switchToViewerArea(void);
    void switchToDocumentationArea(void);

protected slots:
    void linkClicked(const QUrl &url);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    medWelcomeAreaPrivate *d;
};

#endif // MEDWELCOMEAREA_H