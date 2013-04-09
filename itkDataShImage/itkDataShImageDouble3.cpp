// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#include "itkDataShImageDouble3.h"

#include <dtkCore/dtkAbstractDataFactory.h>

#include <itkImage.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkImageFileWriter.h>
#include <itkVectorImage.h>
#include <itkVector.h>

// /////////////////////////////////////////////////////////////////
// itkDataShImagePrivate
// /////////////////////////////////////////////////////////////////

class itkDataShImageDouble3Private
{
public:
    typedef itk::VectorImage<double, 3> ShImageType;
    typedef ShImageType::PixelType    ShType;

    ShImageType::Pointer shs;

    QImage        thumbnail;
    QList<QImage> thumbnails;
};

// /////////////////////////////////////////////////////////////////
// itkDataShImage
// /////////////////////////////////////////////////////////////////

itkDataShImageDouble3::itkDataShImageDouble3(void) : medAbstractDataTypedImage<3,double>(), d(new itkDataShImageDouble3Private)
{
    d->shs = 0;
    d->thumbnail = QImage(":/itkDataShImage/icons/shs.png");
    d->thumbnails << d->thumbnail;
}

itkDataShImageDouble3::~itkDataShImageDouble3(void)
{
    delete d;
    d = 0;
}

bool itkDataShImageDouble3::registered(void)
{
    return dtkAbstractDataFactory::instance()->registerDataType("itkDataShImageDouble3", createItkDataShImageDouble3);
}

QString itkDataShImageDouble3::description(void) const
{
    return tr("itk spherical harmonic 3d image data (double)");
}
QString itkDataShImageDouble3::identifier(void) const
{
    return "itkDataShImageDouble3";
}

void *itkDataShImageDouble3::data(void)
{
    return d->shs.GetPointer();
}

void *itkDataShImageDouble3::output(void)
{
    return d->shs.GetPointer();
}

void itkDataShImageDouble3::setData(void *data)
{
    typedef itkDataShImageDouble3Private::ShImageType ShImageType;

    if( ShImageType* shs = dynamic_cast<ShImageType*>( (itk::Object*)(data) ) ){
        d->shs = shs;
    }
    else
      qDebug() << "Cannot cast pointer to correct Sh double type";
}

int itkDataShImageDouble3::xDimension (void)
{
    if (!d->shs.IsNull())
      return d->shs->GetLargestPossibleRegion().GetSize()[0];
    return -1;
}

int itkDataShImageDouble3::yDimension (void)
{
    if (!d->shs.IsNull())
        return d->shs->GetLargestPossibleRegion().GetSize()[1];
    return -1;
}

int itkDataShImageDouble3::zDimension (void)
{
    if (!d->shs.IsNull())
        return d->shs->GetLargestPossibleRegion().GetSize()[2];
    return -1;
}

QImage& itkDataShImageDouble3::thumbnail  (void) const
{
    // TODO: TEMPORARY black image just to allow drag and drop
    return d->thumbnail;
}

QList<QImage>& itkDataShImageDouble3::thumbnails (void) const
{
    return d->thumbnails;
}

// /////////////////////////////////////////////////////////////////
// Type instantiation
// /////////////////////////////////////////////////////////////////

dtkAbstractData *createItkDataShImageDouble3(void)
{
    return new itkDataShImageDouble3;
}