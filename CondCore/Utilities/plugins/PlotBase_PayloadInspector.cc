#include "CondCore/Utilities/interface/PayloadInspectorPlot.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE( pluginPlotBase_PayloadInspector ) {
  boost::python::class_< cond::payloadInspector::PlotBase > ( "PayloadInspectorPlot" )
  .def("process",&cond::payloadInspector::PlotBase::process )
  .def("payloadType",&cond::payloadInspector::PlotBase::payloadType )
  .def("type",&cond::payloadInspector::PlotBase::type )
  .def("title",&cond::payloadInspector::PlotBase::title );
} 
