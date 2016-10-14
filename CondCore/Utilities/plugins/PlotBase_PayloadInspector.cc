#include "CondCore/Utilities/interface/PayloadInspector.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE( pluginPlotBase_PayloadInspector ) {
  boost::python::class_< cond::payloadInspector::PlotBase > ( "PlotBase" )
  .def("process",&cond::payloadInspector::PlotBase::process )
  .def("payloadType",&cond::payloadInspector::PlotBase::payloadType )
  .def("type",&cond::payloadInspector::PlotBase::type )
  .def("title",&cond::payloadInspector::PlotBase::title )
  .def("isSingleIov",&cond::payloadInspector::PlotBase::isSingleIov )
  .def("data",&cond::payloadInspector::PlotBase::data );
} 
