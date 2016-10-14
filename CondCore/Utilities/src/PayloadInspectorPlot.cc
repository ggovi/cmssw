#include "CondCore/Utilities/interface/PayloadInspectorPlot.h"
#include <sstream>
#include <iostream>

namespace cond {

  namespace payloadInspector {

    PlotAnnotations::PlotAnnotations():m(){}

    std::string PlotAnnotations::get( const std::string& key ) const{
      std::string ret("");
      auto im = m.find( key );
      if( im != m.end() ) ret = im->second;
      return ret;
    } 

    PlotBase::PlotBase():
        m_plotAnnotations(),m_data(""){
    }

    std::string PlotBase::payloadType() const {
      return m_plotAnnotations.get(PlotAnnotations::PAYLOAD_TYPE_K);
    }

    std::string PlotBase::title() const {
      return m_plotAnnotations.get(PlotAnnotations::TITLE_K);
    }

    std::string PlotBase::type() const {
      return m_plotAnnotations.get(PlotAnnotations::PLOT_TYPE_K);
    }

    bool PlotBase::isSingleIov() const {
      return m_plotAnnotations.singleIov;
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

    void PlotBase::setSingleIov( bool flag ) {
      m_plotAnnotations.singleIov = flag;
    }

  }

}
