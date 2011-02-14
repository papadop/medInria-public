#include "medToolBoxSourceData.h"

class medToolBoxSourceDataPrivate
{
public:
	medToolBox *parent;
};

medToolBoxSourceData::medToolBoxSourceData(QWidget *parent) : medToolBox(parent), d(new medToolBoxSourceDataPrivate)
{
	d->parent = parent;
}

medToolBoxSourceData::~medToolBoxSourceData(void)
{
	if (d)
		delete d;
	
	d = NULL;
}

medToolBox *medToolBoxSourceData::parent(void)
{
	return d->parent;
}
