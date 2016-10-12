#include "CondCore/Utilities/interface/PayloadInspectorModule.h"
#include "CondCore/Utilities/interface/PayloadInspectorPlot.h"
#include "CondFormats/Common/interface/BasicPayload.h"
#include "CondCore/CondDB/interface/Time.h"
#include "CondCore/CondDB/interface/PayloadReader.h"
#include <memory>
#include <sstream>

namespace {

  class BasicPayloadPlot_data0 : public cond::payloadInspector::TrendPlot<cond::BasicPayload,float> {
  public:
    BasicPayloadPlot_data0() : cond::payloadInspector::TrendPlot<cond::BasicPayload,float>( "Example Trend", "data0"){
    }

    float getFromPayload( cond::BasicPayload& payload ){
      return payload.m_data0;
    }
  };

  class BasicPayloadPlot_data1 : public cond::payloadInspector::TrendPlot<cond::BasicPayload,float> {
  public:
    BasicPayloadPlot_data1() : cond::payloadInspector::TrendPlot<cond::BasicPayload,float>( "Example Scatter", "data1"){
    }

    float getFromPayload( cond::BasicPayload& payload ){
      return payload.m_data1;
    }
  };

  class BasicPayloadPlot_data2 : public cond::payloadInspector::Histogram2D<cond::BasicPayload> {
  public:
    BasicPayloadPlot_data2() : cond::payloadInspector::Histogram2D<cond::BasicPayload>( "Example Histo2d", "x",10,0,10,"y",10,0,10){
    }

    bool fill( const boost::python::list& iovs, std::vector<std::tuple<float,float,float> >& plotData ){
      cond::persistency::PayloadReader& reader = Base::databaseReader();
      for( int i=0; i< len( iovs ); i++ ) {
	cond::Iov_t iov = boost::python::extract<cond::Iov_t>( iovs[i] );
	std::shared_ptr<cond::BasicPayload> payload = reader.fetch<cond::BasicPayload>( iov.payloadId );
	if( payload.get() ){
          for( size_t i=0;i<10;i++ )
	    for( size_t j=0;j<10;j++ ) {
	      std::cout <<"## filling x="<<j<<" y="<<i<<" z="<<payload->m_vec[i*10+j]<<std::endl;
	      fillWithValue( j, i, payload->m_vec[i*10+j] );
	    }
	}
      }
      return true;
    }

  };


}

PAYLOAD_INSPECTOR_MODULE( BasicPayload ){
  PAYLOAD_INSPECTOR_CLASS( BasicPayloadPlot_data0 );
  PAYLOAD_INSPECTOR_CLASS( BasicPayloadPlot_data1 );
  PAYLOAD_INSPECTOR_CLASS( BasicPayloadPlot_data2 );
}
