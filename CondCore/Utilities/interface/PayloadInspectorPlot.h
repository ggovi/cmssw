#ifndef CondCore_Utilities_PayloadInspectorPlot_h
#define CondCore_Utilities_PayloadInspectorPlot_h

#include "CondCore/CondDB/interface/Utils.h"
#include "CondCore/CondDB/interface/PayloadReader.h"
#include <iostream>

#include <string>
#include <tuple>
#include <vector>
#include <boost/python/list.hpp>
namespace cond {

  namespace payloadInspector {

    struct PlotAnnotations {
      std::string type = "";
      std::string title = "";
      std::string payloadType = "";
      std::string info = "";
      std::string xLabel = "";
      std::string yLabel = "";
      std::string zLabel = "";
    };

    template <typename V> std::string serializeValue( V value ){
      // maybe something more clever than this...
      std::stringstream ss;
      ss << value;
      return ss.str();
    }

    template <typename V> std::string serializeValue( std::pair<V,V> value ){
      std::stringstream ss;
      ss << value.first <<"+-"<<value.second;
      return ss.str();
    }

    // Plot Serialize functions 
    template <typename X,typename Y> std::string serialize( const PlotAnnotations& annotations, const std::vector<std::tuple<X,Y> >& data ){
      // only for testing, the real thing has to be implemented                                                                                                                                                                    
      std::stringstream ss;
      ss<<"Type: "<<annotations.type<<" title: "<<annotations.title<<std::endl;
      for ( auto d : data ){
        ss <<" x: "<<serializeValue(std::get<0>(d))<<" y: "<<serializeValue(std::get<1>(d))<<std::endl;
      }
      return ss.str();
    }

    template <typename X,typename Y,typename Z> std::string serialize( const PlotAnnotations& annotations, const std::vector<std::tuple<X,Y,Z> >& data ){
      // only for testing, to be implemented                                                                                                                                                                    
      std::stringstream ss;
      ss<<"Type: "<<annotations.type<<" title: "<<annotations.title<<std::endl;
      for ( auto d : data ){
        ss <<" x: "<<serializeValue(std::get<0>(d))<<" y: "<<serializeValue(std::get<1>(d))<<" z: "<<serializeValue(std::get<2>(d))<<std::endl;
      }
      return ss.str();
    }

    class PlotBase {
    public:
      PlotBase();

      // return the type-name of the objects we handle, so the PayloadInspector can find corresponding tags
      std::string payloadType() const;

      std::string title() const; 

      std::string type() const;

      std::string data() const;

      bool process( const std::string& connectionString, const boost::python::list& iovs );

      virtual std::string processData( const boost::python::list& iovs );

      cond::persistency::PayloadReader& databaseReader();

    protected:

      PlotAnnotations m_plotAnnotations;

    private:

     cond::persistency::PayloadReader m_dbReader;

      std::string m_data = "";
    };

    // Concrete plot-types implementations
    template <typename PayloadType, typename X, typename Y> class Plot1D : public PlotBase {
    public:
      Plot1D( const std::string& type, const std::string& title, const std::string xLabel, const std::string& yLabel ) : 
	PlotBase(){
	m_plotAnnotations.type = type;
        m_plotAnnotations.title = title;
	m_plotAnnotations.xLabel = xLabel;
	m_plotAnnotations.yLabel = yLabel;
        m_plotAnnotations.payloadType = cond::demangledName( typeid(PayloadType) );
      }

      std::string processData( const boost::python::list& iovs ){
	fill( iovs, m_plotData );
	return serialize( m_plotAnnotations, m_plotData );
      }

      virtual bool fill( const boost::python::list& iovs, std::vector<std::tuple<X,Y> >& plotData  ) = 0;
    private:
      std::vector<std::tuple<X,Y> > m_plotData;
    };

    template <typename PayloadType, typename X, typename Y, typename Z> class Plot2D : public PlotBase {
    public:
      Plot2D( const std::string& type, const std::string& title, const std::string xLabel, const std::string& yLabel ) :
        PlotBase(){
        m_plotAnnotations.type = type;
        m_plotAnnotations.title = title;
        m_plotAnnotations.xLabel = xLabel;
        m_plotAnnotations.yLabel = yLabel;
        m_plotAnnotations.payloadType = cond::demangledName( typeid(PayloadType) );
      }

      std::string processData( const boost::python::list& iovs ){
	fill( iovs, m_plotData );
	return serialize( m_plotAnnotations, m_plotData);
      }

      virtual bool fill( const boost::python::list& iovs, std::vector<std::tuple<X,Y,Z> >& plotData ) = 0;
    private:
      std::vector<std::tuple<X,Y,Z> > m_plotData;
    };

    template<typename PayloadType,typename Y> class TrendPlot : public Plot1D<PayloadType,unsigned long long,Y > {
    public:
      // the x axis label will be overwritten by the plot rendering application
      TrendPlot( const std::string& title, const std::string& yLabel ) :
	Plot1D<PayloadType,unsigned long long,Y>( "Trend", title, "-" , yLabel ){
      }

      bool fill( const boost::python::list& iovs, std::vector<std::tuple<unsigned long long,Y> >& plotData ){
	cond::persistency::PayloadReader& reader = Plot1D<PayloadType,unsigned long long,Y>::databaseReader();
	for( int i=0; i< len( iovs ); i++ ) {
	  cond::Iov_t iov = boost::python::extract<cond::Iov_t>( iovs[i] );
	  std::shared_ptr<PayloadType> payload = reader.fetch<PayloadType>( iov.payloadId );
          if( payload.get() ){
	    Y value = getFromPayload( *payload );
	    plotData.push_back( std::make_tuple(iov.since,value));
	  }
	}
	return true;
      }

      virtual Y getFromPayload( PayloadType& payload ) = 0; 

    };

    template<typename PayloadType, typename X, typename Y> class ScatterPlot : public Plot1D<PayloadType,X,Y > {
    public:
      // the x axis label will be overwritten by the plot rendering application
      ScatterPlot( const std::string& title, const std::string& xLabel, const std::string& yLabel ) :
	Plot1D<PayloadType,X,Y>( "Scatter", title, xLabel , yLabel ){
      }

      bool fill( const boost::python::list& iovs, std::vector<std::tuple<X,Y> >& plotData ){
	cond::persistency::PayloadReader& reader = Plot1D<PayloadType,X,Y>::databaseReader();
	for( int i=0; i< len( iovs ); i++ ) {
	  cond::Iov_t iov = boost::python::extract<cond::Iov_t>( iovs[i] );
	  std::shared_ptr<PayloadType> payload = reader.fetch<PayloadType>( iov.payloadId );
          if( payload.get() ){
	    std::tuple<X,Y> value = getFromPayload( *payload );
	    plotData.push_back( value );
	  }
	}
	return true;
      }

      virtual std::tuple<X,Y> getFromPayload( PayloadType& payload ) = 0; 

    };


  }

}

#endif

