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

    // Metadata dictionary
    struct PlotAnnotations {
      static constexpr const char* const PLOT_TYPE_K = "type";
      static constexpr const char* const TITLE_K = "title";
      static constexpr const char* const PAYLOAD_TYPE_K = "payload_type";
      static constexpr const char* const INFO_K = "info";
      static constexpr const char* const XAXIS_K = "x_label";
      static constexpr const char* const YAXIS_K = "y_label";
      static constexpr const char* const ZAXIS_K = "z_label";
      PlotAnnotations();
      std::string get( const std::string& key ) const;
      std::map<std::string,std::string> m;
      bool singleIov = false;
    };

    // Serialize functions 
    template <typename V> std::string serializeValue( const std::string& entryLabel, const V& value ){
      // prototype
      std::stringstream ss;
      ss << "\""<<entryLabel<<"\":\""<<value<<"\"";
      return ss.str();
    }

    template <typename V> std::string serializeValue( const std::string& entryLabel, const std::pair<V,V>& value ){
      // prototype
      std::stringstream ss;
      ss << "\""<<entryLabel<<"\":\""<<value.first<<"\", \""<<entryLabel<<"_err\":\""<<value.second<<"\"";
      return ss.str();
    }

    std::string serializeAnnotations( const PlotAnnotations& annotations ){
      std::stringstream ss;
      ss <<"\"annotations\": {";
      bool first = true;
      for( auto a: annotations.m ){
        if ( !first ) ss <<",";
	ss << "\""<<a.first<<"\":\"" << a.second <<"\"";
        first = false;
      } 
      ss <<"}";
      return ss.str();
    }

    template <typename X,typename Y> std::string serialize( const PlotAnnotations& annotations, const std::vector<std::tuple<X,Y> >& data ){
      // prototype implementation...
      std::stringstream ss;
      ss << "{";
      ss << serializeAnnotations( annotations );
      ss <<",";
      ss << "\"data\": [";
      bool first = true;
      for ( auto d : data ){
        if( !first ) ss <<",";
        ss <<" { "<<serializeValue("x",std::get<0>(d))<<", "<<serializeValue( "y",std::get<1>( d))<<" }";
        first = false;
      }
      ss << "]";
      ss << "}";
      return ss.str();
    }

    template <typename X,typename Y,typename Z> std::string serialize( const PlotAnnotations& annotations, const std::vector<std::tuple<X,Y,Z> >& data ){
      // prototype implementation...
      std::stringstream ss;
      ss << "{";
      ss << serializeAnnotations( annotations );
      ss <<",";
      ss << "\"data\": [";
      bool first = true;
      for ( auto d : data ){
        if( !first ) ss <<",";
        ss <<" { "<<serializeValue("x",std::get<0>(d))<<", "<<serializeValue( "y",std::get<1>( d))<<", "<<serializeValue( "z",std::get<2>( d))<<" }";
        first = false;
      }
      ss << "]";
      ss << "}";
      return ss.str();
    }

    // Base class, factorizing the functions exposed in the python interface
    class PlotBase {
    public:
      PlotBase();

      // required in the browser to find corresponding tags
      std::string payloadType() const;

      // required in the browser
      std::string title() const; 

      // required in the browser
      std::string type() const;

      // required in the browser
      bool isSingleIov() const;

      // returns the json file with the plot data
      std::string data() const;

      // triggers the processing producing the plot
      bool process( const std::string& connectionString, const boost::python::list& iovs );

      // not exposed in python:
      // called internally in process()
      virtual std::string processData( const boost::python::list& iovs );

      // to be set in order to limit the iov selection ( and processing ) to 1 iov
      void setSingleIov( bool flag );

      // access to the fetch function of the configured reader, to be used in the processData implementations
      template <typename PayloadType> std::shared_ptr<PayloadType> fetchPayload( const cond::Hash& payloadHash ){
	return m_dbReader.fetch<PayloadType>( payloadHash );
      }

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
	m_plotAnnotations.m[PlotAnnotations::PLOT_TYPE_K] = type;
        m_plotAnnotations.m[PlotAnnotations::TITLE_K] = title;
	m_plotAnnotations.m[PlotAnnotations::XAXIS_K] = xLabel;
	m_plotAnnotations.m[PlotAnnotations::YAXIS_K] = yLabel;
        m_plotAnnotations.m[PlotAnnotations::PAYLOAD_TYPE_K] = cond::demangledName( typeid(PayloadType) );
      }

      std::string serializeData(){
	return serialize( m_plotAnnotations, m_plotData);
      }

      std::string processData( const boost::python::list& iovs ) override {
	fill( iovs, m_plotData );
	return serializeData();
      }

      std::shared_ptr<PayloadType> fetchPayload( const cond::Hash& payloadHash ){
      	return PlotBase::fetchPayload<PayloadType>( payloadHash );
      }

      virtual bool fill( const boost::python::list& iovs, std::vector<std::tuple<X,Y> >& plotData  ) = 0;
    protected:
      std::vector<std::tuple<X,Y> > m_plotData;
    };

    template <typename PayloadType, typename X, typename Y, typename Z> class Plot2D : public PlotBase {
    public:
      Plot2D( const std::string& type, const std::string& title, const std::string xLabel, const std::string& yLabel, const std::string& zLabel ) :
        PlotBase(),m_plotData(){
        m_plotAnnotations.m[PlotAnnotations::PLOT_TYPE_K] = type;
        m_plotAnnotations.m[PlotAnnotations::TITLE_K] = title;
        m_plotAnnotations.m[PlotAnnotations::XAXIS_K] = xLabel;
        m_plotAnnotations.m[PlotAnnotations::YAXIS_K] = yLabel;
        m_plotAnnotations.m[PlotAnnotations::ZAXIS_K] = zLabel;
        m_plotAnnotations.m[PlotAnnotations::PAYLOAD_TYPE_K] = cond::demangledName( typeid(PayloadType) );
      }

      std::string serializeData(){
	return serialize( m_plotAnnotations, m_plotData);
      }

      std::string processData( const boost::python::list& iovs ) override {
	fill( iovs, m_plotData );
	return serializeData();
      }

      std::shared_ptr<PayloadType> fetchPayload( const cond::Hash& payloadHash ){
      	return PlotBase::fetchPayload<PayloadType>( payloadHash );
      }

      virtual bool fill( const boost::python::list& iovs, std::vector<std::tuple<X,Y,Z> >& plotData ) = 0;
    protected:
      std::vector<std::tuple<X,Y,Z> > m_plotData;
    };

    template<typename PayloadType,typename Y> class TrendPlot : public Plot1D<PayloadType,unsigned long long,Y > {
    public:
      typedef Plot1D<PayloadType,unsigned long long,Y > Base;
      // the x axis label will be overwritten by the plot rendering application
      TrendPlot( const std::string& title, const std::string& yLabel ) : 
	Base( "Trend", title, "-" , yLabel ){
      }

      bool fill( const boost::python::list& iovs, std::vector<std::tuple<unsigned long long,Y> >& plotData ) override {
	for( int i=0; i< len( iovs ); i++ ) {
	  cond::Iov_t iov = boost::python::extract<cond::Iov_t>( iovs[i] );
	  std::shared_ptr<PayloadType> payload = Base::fetchPayload( iov.payloadId );
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
      typedef Plot1D<PayloadType,X,Y > Base;
      // the x axis label will be overwritten by the plot rendering application
      ScatterPlot( const std::string& title, const std::string& xLabel, const std::string& yLabel ) :
	Base( "Scatter", title, xLabel , yLabel ){
      }

      bool fill( const boost::python::list& iovs, std::vector<std::tuple<X,Y> >& plotData ) override {
	for( int i=0; i< len( iovs ); i++ ) {
	  cond::Iov_t iov = boost::python::extract<cond::Iov_t>( iovs[i] );
	  std::shared_ptr<PayloadType> payload = Base::fetchPayload( iov.payloadId );
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
	Base( "Histo1D", title, xLabel , "entries" ),m_min(min),m_max(max){
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
	for( int i=0; i< len( iovs ); i++ ) {
	  cond::Iov_t iov = boost::python::extract<cond::Iov_t>( iovs[i] );
	  std::shared_ptr<PayloadType> payload = Base::fetchPayload( iov.payloadId );
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
	Base( "Histo2D", title, xLabel , yLabel, "entries" ),m_nxbins( nxbins), m_xmin(xmin),m_xmax(xmax),m_ymin(ymin),m_ymax(ymax){
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
	for( int i=0; i< len( iovs ); i++ ) {
	  cond::Iov_t iov = boost::python::extract<cond::Iov_t>( iovs[i] );
	  std::shared_ptr<PayloadType> payload = Base::fetchPayload( iov.payloadId );
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

