#ifndef CondCore_Utilities_PayloadInspectorPlot_h
#define CondCore_Utilities_PayloadInspectorPlot_h

#include "CondCore/CondDB/interface/Utils.h"
#include "CondCore/CondDB/interface/PayloadReader.h"
#include <iostream>

#include <string>
#include <tuple>
#include <vector>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>

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

    template <typename V> std::string serializeValue( const V& value ){
      // maybe something more clever than this... maybe boost::lexical_cast<std::string> ?
      std::stringstream ss;
      ss << value;
      return ss.str();
    }

    template <typename V> std::string serializeValue( const std::pair<V,V>& value ){
      // only for testing, the real thing has to be implemented
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
	PlotBase(),m_plotData(){
	m_plotAnnotations.type = type;
        m_plotAnnotations.title = title;
	m_plotAnnotations.xLabel = xLabel;
	m_plotAnnotations.yLabel = yLabel;
        m_plotAnnotations.payloadType = cond::demangledName( typeid(PayloadType) );
      }

      std::string serializeData(){
	return serialize( m_plotAnnotations, m_plotData);
      }

      std::string processData( const boost::python::list& iovs ) override {
	fill( iovs, m_plotData );
	return serializeData();
      }

      virtual bool fill( const boost::python::list& iovs, std::vector<std::tuple<X,Y> >& plotData  ) = 0;
    protected:
      std::vector<std::tuple<X,Y> > m_plotData;
    };

    template <typename PayloadType, typename X, typename Y, typename Z> class Plot2D : public PlotBase {
    public:
      Plot2D( const std::string& type, const std::string& title, const std::string xLabel, const std::string& yLabel, const std::string& zLabel ) :
        PlotBase(),m_plotData(){
        m_plotAnnotations.type = type;
        m_plotAnnotations.title = title;
        m_plotAnnotations.xLabel = xLabel;
        m_plotAnnotations.yLabel = yLabel;
        m_plotAnnotations.zLabel = zLabel;
        m_plotAnnotations.payloadType = cond::demangledName( typeid(PayloadType) );
      }

      std::string serializeData(){
	return serialize( m_plotAnnotations, m_plotData);
      }

      std::string processData( const boost::python::list& iovs ) override {
	fill( iovs, m_plotData );
	return serializeData();
      }

      virtual bool fill( const boost::python::list& iovs, std::vector<std::tuple<X,Y,Z> >& plotData ) = 0;
    protected:
      std::vector<std::tuple<X,Y,Z> > m_plotData;
    };

    template<typename PayloadType,typename Y> class TrendPlot : public Plot1D<PayloadType,unsigned long long,Y > {
    public:
      // the x axis label will be overwritten by the plot rendering application
      TrendPlot( const std::string& title, const std::string& yLabel ) :
	Plot1D<PayloadType,unsigned long long,Y>( "Trend", title, "-" , yLabel ){
      }

      bool fill( const boost::python::list& iovs, std::vector<std::tuple<unsigned long long,Y> >& plotData ) override {
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

      bool fill( const boost::python::list& iovs, std::vector<std::tuple<X,Y> >& plotData ) override {
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

    //
    template<typename PayloadType> class Histogram1D: public Plot1D<PayloadType,float,float > {
    public:
      typedef Plot1D<PayloadType,float,float > Base;
      // naive implementation, essentially provided as an example...
      Histogram1D( const std::string& title, const std::string& xLabel, size_t nbins, float min, float max ):
	Plot1D<PayloadType,float,float>( "Histo1D", title, xLabel , "entries" ),m_min(min),m_max(max){
	float binSize = (max-min)/nbins;
	if( binSize>0 ){
          m_binSize = binSize;
	  Base::m_plotData.resize( nbins );
	  for( size_t i=0;i<nbins;i++ ){
	    Base::m_plotData[i] = std::make_tuple( m_min+i*m_binSize, 0 );
	  }
	}
      }

      // to be used to fill the histogram!
      void fillWithValue( float value, float weight=1 ){
	// ignoring underflow/overflows ( they can be easily added - the total entries as well - in a more generic PlotAnnotation implementation )
        if( Base::m_plotData.size() && value < m_max && value >= m_min ){
	  size_t ibin = (value-m_min)/m_binSize;
	  std::get<1>(Base::m_plotData[ibin])+=weight;
	}
      }

      // this one can ( and in general should ) be overridden - the implementation should use fillWithValue
      virtual bool fill( const boost::python::list& iovs, std::vector<std::tuple<float,float> >& plotData ) override {
	cond::persistency::PayloadReader& reader = Base::databaseReader();
	for( int i=0; i< len( iovs ); i++ ) {
	  cond::Iov_t iov = boost::python::extract<cond::Iov_t>( iovs[i] );
	  std::shared_ptr<PayloadType> payload = reader.fetch<PayloadType>( iov.payloadId );
          if( payload.get() ){
	    float value = getFromPayload( *payload );
	    fillWithValue( value );
	  }
        }
	return true;
      }

      // implement this one if you use the default fill implementation, otherwise ignore it...
      virtual float getFromPayload( PayloadType& payload ) {
	return 0;
      }

    private:
      float m_binSize = 0; 
      float m_min;
      float m_max;
    };

    // 
    template<typename PayloadType> class Histogram2D: public Plot2D<PayloadType,float,float,float > {
    public:
      typedef Plot2D<PayloadType,float,float,float > Base;
      // naive implementation, essentially provided as an example...
      Histogram2D( const std::string& title, const std::string& xLabel, size_t nxbins, float xmin, float xmax, 
		                             const std::string& yLabel, size_t nybins, float ymin, float ymax  ):
	Plot2D<PayloadType,float,float,float>( "Histo2D", title, xLabel , yLabel, "entries" ),m_nxbins( nxbins), m_xmin(xmin),m_xmax(xmax),m_ymin(ymin),m_ymax(ymax){
	float xbinSize = (xmax-xmin)/nxbins;
        float ybinSize = (ymax-ymin)/nybins;
	if( xbinSize>0 && ybinSize>0){
          m_xbinSize = xbinSize;
          m_ybinSize = ybinSize;
	  Base::m_plotData.resize( nxbins*nybins );
	  for( size_t i=0;i<nybins;i++ ){
	    for( size_t j=0;j<nxbins;j++ ){
	      Base::m_plotData[i*nxbins+j] = std::make_tuple( m_xmin+i*m_xbinSize, m_ymin+j*m_ybinSize, 0 );
	    }
	  }
	}
      }

      // to be used to fill the histogram!
      void fillWithValue( float xvalue, float yvalue, float weight=1 ){
	// ignoring underflow/overflows ( they can be easily added - the total entries as well - in a more generic PlotAnnotation implementation )
	if( Base::m_plotData.size() && xvalue < m_xmax && xvalue >= m_xmin &&  yvalue < m_ymax && yvalue >= m_ymin ){
	  size_t ixbin = (xvalue-m_xmin)/m_xbinSize;
	  size_t iybin = (yvalue-m_ymin)/m_ybinSize;
	  std::get<2>(Base::m_plotData[iybin*m_nxbins+ixbin])+=weight;
	}
      }

      // this one can ( and in general should ) be overridden - the implementation should use fillWithValue
      virtual bool fill( const boost::python::list& iovs, std::vector<std::tuple<float,float,float> >& plotData ) override {
	cond::persistency::PayloadReader& reader = Base::databaseReader();
	for( int i=0; i< len( iovs ); i++ ) {
	  cond::Iov_t iov = boost::python::extract<cond::Iov_t>( iovs[i] );
	  std::shared_ptr<PayloadType> payload = reader.fetch<PayloadType>( iov.payloadId );
          if( payload.get() ){
	    std::tuple<float,float> value = getFromPayload( *payload );
	    fillWithValue( std::get<0>(value),std::get<1>(value) );
	  }
        }
	return true;
      }

      // implement this one if you use the default fill implementation, otherwise ignore it...
      virtual std::tuple<float,float> getFromPayload( PayloadType& payload ) {
        float x = 0;
	float y = 0;
	return std::make_tuple(x,y);
      }

    private:
      size_t m_nxbins;
      float m_xbinSize = 0; 
      float m_xmin;
      float m_xmax;
      float m_ybinSize = 0; 
      float m_ymin;
      float m_ymax;
    };

  }

}

#endif

