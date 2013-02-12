
#include "medDatabaseEditItemDialog.h"

#include <medDataIndex.h>
#include <medAbstractDatabaseItem.h>

#include <QtGui>


class medDatabaseEditItemDialogPrivate
{
public:

    QList<QVariant> values;

    QList<QString> attributes;

    QRadioButton *persistentRadioBt;
};


medDatabaseEditItemDialog::medDatabaseEditItemDialog(QList<QString> attributes, QList<QVariant> dataList, QWidget *parent,  bool displayPersistency, bool persistent): QDialog(parent,
            Qt::Dialog | Qt::WindowCloseButtonHint), d (new medDatabaseEditItemDialogPrivate)

{
    QVBoxLayout *dialogLayout = new QVBoxLayout;
    QHBoxLayout *radioBtLayout = new QHBoxLayout;
    QFormLayout *formLayout = new QFormLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    QPushButton *okButton = new QPushButton(tr("OK"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    dialogLayout->addLayout(radioBtLayout);
    dialogLayout->addLayout(formLayout);
    dialogLayout->addStretch();
    dialogLayout->addLayout(buttonLayout);

    if(displayPersistency)
    {
        d->persistentRadioBt = new QRadioButton(tr("Persistent"));
        QRadioButton *nonPersistentRadioBt = new QRadioButton(tr("Non Persistent"));
        radioBtLayout->addWidget(d->persistentRadioBt);
        radioBtLayout->addWidget(nonPersistentRadioBt);
        d->persistentRadioBt->setChecked(persistent);
        nonPersistentRadioBt->setChecked(!persistent);
    }

    int i=0;

    d->attributes = attributes;
    d->values = dataList;

    foreach(QString attrib, attributes)
    {
        QVariant data = dataList[i];
        i++;

        if(attrib.isEmpty())
            continue;
        
        QLineEdit *textEdit;

        switch(data.type())
        {
         //TODO: add other type QDateTime, int, ...
        case QVariant::String:
            textEdit = new QLineEdit(this);
            textEdit->setObjectName(attrib);
            textEdit->setText(data.toString());
            formLayout->addRow(attrib, textEdit);
            connect(textEdit, SIGNAL(textChanged(const QString &)), this, SLOT(setValue(const QString &)));
            break;  
        default:
            break;

        }
    }

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(validate()));

    setLayout(dialogLayout);
    setModal(true);
}

medDatabaseEditItemDialog::~medDatabaseEditItemDialog()
{
    delete d;
    d = NULL;
}


void medDatabaseEditItemDialog::setValue(const QString & text)
{
    QWidget *currentWidget = QApplication::focusWidget();
    QString objectName = currentWidget->objectName();

    if(!objectName.isEmpty())
    {
        int index = d->attributes.indexOf(objectName);

        if(index>-1 && index<d->values.length())
        {
            d->values[index] = QVariant(text);
        }
    }
}

void medDatabaseEditItemDialog::cancel()
{
    this->reject();
}


void medDatabaseEditItemDialog::validate()
{
    this->accept();
}

QVariant medDatabaseEditItemDialog::value(QString attribute)
{
    QVariant res;
    int index = d->attributes.indexOf(attribute);

    if(index>-1 && index<d->values.length()+1)
        res = d->values[index];

    if(res.isNull())
        res=QVariant("");

    return res;
}


bool medDatabaseEditItemDialog::isPersistent()
{
    if(d->persistentRadioBt)
        return d->persistentRadioBt->isChecked();
    else return false;

}



