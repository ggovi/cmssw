#include "CondCore/Utilities/interface/PayloadInspectorModule.h"
#include "CondCore/Utilities/interface/PayloadInspector.h"
#include "CondFormats/Common/interface/BasicPayload.h"
#include "CondCore/CondDB/interface/Time.h"
#include "CondCore/CondDB/interface/PayloadReader.h"
#include <memory>
#include <sstream>

namespace {

  class BasicPayload_data0 : public cond::payloadInspector::TrendPlot<cond::BasicPayload,float> {
  public:
    BasicPayload_data0() : cond::payloadInspector::TrendPlot<cond::BasicPayload,float>( "Example Trend", "data0"){
    }

    float getFromPayload( cond::BasicPayload& payload ){
      return payload.m_data0;
    }
  };

  class BasicPayload_data1 : public cond::payloadInspector::TrendPlot<cond::BasicPayload,float> {
  public:
    BasicPayload_data1() : cond::payloadInspector::TrendPlot<cond::BasicPayload,float>( "Example Scatter", "data1"){
    }

    float getFromPayload( cond::BasicPayload& payload ){
      return payload.m_data1;
    }
  };

  class BasicPayload_data2 : public cond::payloadInspector::Histogram1D<cond::BasicPayload> {
  public:
    BasicPayload_data2() : cond::payloadInspector::Histogram1D<cond::BasicPayload>( "Example Histo1d", "x",10,0,10){
      Base::setSingleIov( true );
    }

    bool fill( const boost::python::list& iovs, std::vector<std::tuple<float,float> >& plotData ){
      for( int i=0; i< len( iovs ); i++ ) {
	cond::Iov_t iov = boost::python::extract<cond::Iov_t>( iovs[i] );
	std::shared_ptr<cond::BasicPayload> payload = Base::fetchPayload( iov.payloadId );
	if( payload.get() ){
	  for( size_t j=0;j<100;j++ ) {
	    fillWithValue( j, payload->m_vec[j] );
	  }
	}
      }
      return true;
    }
  };

  class BasicPayload_data3 : public cond::payloadInspector::Histogram2D<cond::BasicPayload> {
  public:
    BasicPayload_data3() : cond::payloadInspector::Histogram2D<cond::BasicPayload>( "Example Histo2d", "x",10,0,10,"y",10,0,10){
      Base::setSingleIov( true );
    }

    bool fill( const boost::python::list& iovs, std::vector<std::tuple<float,float,float> >& plotData ){
      for( int i=0; i< len( iovs ); i++ ) {
	cond::Iov_t iov = boost::python::extract<cond::Iov_t>( iovs[i] );
	std::shared_ptr<cond::BasicPayload> payload = Base::fetchPayload( iov.payloadId );
	if( payload.get() ){
          for( size_t i=0;i<10;i++ )
	    for( size_t j=0;j<10;j++ ) {
	      fillWithValue( j, i, payload->m_vec[i*10+j] );
	    }
	}
      }
      return true;
    }
  };


}

PAYLOAD_INSPECTOR_MODULE( BasicPayload ){
  PAYLOAD_INSPECTOR_CLASS( BasicPayload_data0 );
  PAYLOAD_INSPECTOR_CLASS( BasicPayload_data1 );
  PAYLOAD_INSPECTOR_CLASS( BasicPayload_data2 );
  PAYLOAD_INSPECTOR_CLASS( BasicPayload_data3 );
}
