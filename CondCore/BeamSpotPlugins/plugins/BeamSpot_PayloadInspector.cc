#include "CondCore/Utilities/interface/PayloadInspectorModule.h"
#include "CondCore/Utilities/interface/PayloadInspectorPlot.h"
#include "CondCore/CondDB/interface/Time.h"

#include "CondFormats/BeamSpotObjects/interface/BeamSpotObjects.h"

#include <memory>
#include <sstream>

namespace {

  class BeamSpotPlot_x : public cond::payloadInspector::TrendPlot<BeamSpotObjects,std::pair<double,double> > {
  public:
    BeamSpotPlot_x(): cond::payloadInspector::TrendPlot<BeamSpotObjects,std::pair<double,double> >( "x vs run number", "x"){
    }

    std::pair<double,double> getFromPayload( BeamSpotObjects& payload ){
      return std::make_pair(payload.GetX(),payload.GetXError());
    }
  };

  class BeamSpotPlot_y : public cond::payloadInspector::TrendPlot<BeamSpotObjects,std::pair<double,double> >{
  public:
    BeamSpotPlot_y(): cond::payloadInspector::TrendPlot<BeamSpotObjects,std::pair<double,double> >( "y vs run number", "y"){
    }

    std::pair<double,double> getFromPayload( BeamSpotObjects& payload ){
      return std::make_pair(payload.GetY(),payload.GetYError());
    }
  };

  class BeamSpotPlot_xy : public cond::payloadInspector::ScatterPlot<BeamSpotObjects,double,double>{
  public:
    BeamSpotPlot_xy(): cond::payloadInspector::ScatterPlot<BeamSpotObjects,double,double>("BeamSpot x vs y","x","y" ){
    }

    std::tuple<double,double> getFromPayload( BeamSpotObjects& payload ){
      return std::make_tuple( payload.GetX(), payload.GetY() );
    }
  };


}

PAYLOAD_INSPECTOR_MODULE( BeamSpot ){
  PAYLOAD_INSPECTOR_CLASS( BeamSpotPlot_x );
  PAYLOAD_INSPECTOR_CLASS( BeamSpotPlot_y );
  PAYLOAD_INSPECTOR_CLASS( BeamSpotPlot_xy );
}
