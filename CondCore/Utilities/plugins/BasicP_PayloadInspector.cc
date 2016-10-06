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
    BasicPayloadPlot_data0() : cond::payloadInspector::TrendPlot<cond::BasicPayload,float>( "Example", "data0"){
    }

    float getFromPayload( cond::BasicPayload& payload ){
      return payload.m_data0;
    }
  };

  class BasicPayloadPlot_data1 : public cond::payloadInspector::TrendPlot<cond::BasicPayload,float> {
  public:
    BasicPayloadPlot_data1() : cond::payloadInspector::TrendPlot<cond::BasicPayload,float>( "Example", "data1"){
    }

    float getFromPayload( cond::BasicPayload& payload ){
      return payload.m_data1;
    }
  };

}

PAYLOAD_INSPECTOR_MODULE( BasicPayload ){
  PAYLOAD_INSPECTOR_CLASS( BasicPayloadPlot_data0 );
  PAYLOAD_INSPECTOR_CLASS( BasicPayloadPlot_data1 );
}
