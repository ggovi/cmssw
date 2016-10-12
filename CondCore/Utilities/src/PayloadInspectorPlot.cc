#include "CondCore/Utilities/interface/PayloadInspectorPlot.h"
#include <sstream>
#include <iostream>

namespace cond {

  namespace payloadInspector {

    PlotBase::PlotBase():
        m_plotAnnotations(),m_data(""){
    }

    std::string PlotBase::payloadType() const {
      return m_plotAnnotations.payloadType;
    }

    std::string PlotBase::title() const {
      return m_plotAnnotations.title;
    }

    std::string PlotBase::type() const {
      return m_plotAnnotations.type;
    }

    std::string PlotBase::data() const {
      return m_data;
    }

    bool PlotBase::process( const std::string& connectionString, const boost::python::list& iovs ){
      m_dbReader.open( connectionString ); 
      m_data = processData( iovs );
      m_dbReader.close();
      // fixme...
      return true;
    }

    std::string PlotBase::processData( const boost::python::list& ){
      return ""; 
    }

    cond::persistency::PayloadReader& PlotBase::databaseReader(){ 
      return m_dbReader;
    }

  }

}
