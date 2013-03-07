// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////
#include "itkDataShImageReaderPlugin.h"

#include <dtkLog/dtkLog.h>

#include <dtkItkLogForwarder/itkLogForwarder.h>

#include "itkNrrdDataShImageReader.h"
#include "itkNiftiDataShImageReader.h"
#include "itkMetaDataShImageReader.h"

// /////////////////////////////////////////////////////////////////
// itkDataShImageReaderPluginPrivate
// /////////////////////////////////////////////////////////////////

class itkDataShImageReaderPluginPrivate
{
public:
  itkDataShImageReaderPluginPrivate()
  {
      forwarder = itk::LogForwarder::New();
  }
  ~itkDataShImageReaderPluginPrivate()
  {

  }
  // Class variables go here.
  itk::LogForwarder::Pointer forwarder;
};

// /////////////////////////////////////////////////////////////////
// itkDataShImageReaderPlugin
// /////////////////////////////////////////////////////////////////

itkDataShImageReaderPlugin::itkDataShImageReaderPlugin(QObject *parent) : dtkPlugin(parent), d(new itkDataShImageReaderPluginPrivate)
{

}

itkDataShImageReaderPlugin::~itkDataShImageReaderPlugin(void)
{
    delete d;

    d = NULL;
}

bool itkDataShImageReaderPlugin::initialize(void)
{
    if(!itkMetaDataShImageReader::registered())  dtkWarn() << "Unable to register itkMetaDataShImageReader type";
    if(!itkNiftiDataShImageReader::registered()) dtkWarn() << "Unable to register itkNiftiDataShImageReader type";
    if(!itkNrrdDataShImageReader::registered())  dtkWarn() << "Unable to register itkNrrdDataShImageReader type";

    return true;
}

bool itkDataShImageReaderPlugin::uninitialize(void)
{
    return true;
}

QString itkDataShImageReaderPlugin::name(void) const
{
    return "itkDataShImageReaderPlugin";
}

QString itkDataShImageReaderPlugin::description(void) const
{
    return "ITK Sh data image readers";
}

QStringList itkDataShImageReaderPlugin::tags(void) const
{
    return QStringList() << "itk" << "data" << "Sh" << "image" << "reader";
}

QStringList itkDataShImageReaderPlugin::types(void) const
{
    return QStringList() << "itkMetaDataShImageReader"
                         << "itkNiftiDataShImageReader"
                         << "itkNrrdDataShImageReader";
}

Q_EXPORT_PLUGIN2(itkDataShImageReaderPlugin, itkDataShImageReaderPlugin)