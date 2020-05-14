#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "CondCore/CondDB/interface/ConnectionPool.h"
#include "CondFormats/Common/interface/TimeConversions.h"
#include "CondTools/BeamSpot/interface/BeamSpotOnlinePopConSourceHandler.h"
//#include <memory>
//#include <sstream>
//#include <utility>
//#include <vector>
//#include <cmath>

//#include <chrono>   

BeamSpotOnlinePopConSourceHandler::BeamSpotOnlinePopConSourceHandler(edm::ParameterSet const& pset)
    : m_debug(pset.getUntrackedParameter<bool>("debug", false)),
      m_name(pset.getUntrackedParameter<std::string>("name", "BeamSpotOnlineSourceHandler")),
      m_maxAge(pset.getUntrackedParameter<unsigned int>("maxAge", 86400)),
      m_runNumber(pset.getUntrackedParameter<unsigned int>("runNumber", 1)),
      m_sourcePayloadTag( pset.getUntrackedParameter<std::string>("sourcePayloadTag","")) {
}

BeamSpotOnlinePopConSourceHandler::~BeamSpotOnlinePopConSourceHandler() {}

bool checkPayloadAge( const BeamSpotObjects&, unsigned int maxAge ){
  // here we need to get the creation time of the payload
  // unsigned long long creationTime = payload.getCreationTime();
  //const auto timeNow = std::chrono::system_clock::now();
  //auto nowSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(timeNow.time_since_epoch()).count();
  // unsigned int age = nowSinceEpoch - creationTime;
  unsigned int age = 86399;
  return age < maxAge;
}

std::unique_ptr<BeamSpotObjects> makeDummyPayload(){
  // implement here
  std::unique_ptr<BeamSpotObjects> ret;
  ret.reset( new BeamSpotObjects() );
  return ret;
}

void BeamSpotOnlinePopConSourceHandler::getNewObjects() {

  bool addNewPayload = false;
  if (tagInfo().name.empty()) {
    edm::LogInfo(m_name) << "New tag " << tagInfo().name << "; from " << m_name << "::getNewObjects";
    addNewPayload = true;
  } else {
    edm::LogInfo(m_name) << "got info for tag " << tagInfo().name
                         << ", last object valid since " << tagInfo().lastInterval.since 
                         << "; from " << m_name << "::getNewObjects";
    if( !checkPayloadAge( *lastPayload(), m_maxAge ) ){
      addNewPayload = true;
    }
  }

  if( addNewPayload ){
    if( !m_sourcePayloadTag.empty() ){
      edm::LogInfo(m_name) << "Reading target payload from tag " << m_sourcePayloadTag;
      auto session = dbSession();
      session.transaction().start(true);
      auto lastIov = session.readIov( m_sourcePayloadTag ).getLast();
      m_payload = session.fetchPayload<BeamSpotObjects>( lastIov.payloadId );
      session.transaction().commit();
    }  else {
      m_payload = makeDummyPayload();
    }
 
    cond::Time_t targetTime = cond::time::lumiTime( m_runNumber, 1 );
    m_to_transfer.push_back( std::make_pair( m_payload.get(), targetTime ) );

    edm::LogInfo(m_name) << "Payload added with IOV since " << targetTime;
  } else {
    edm::LogInfo(m_name) << "Nothing to do, last payload satisfies maximum age requirement.";
  }
}

std::string BeamSpotOnlinePopConSourceHandler::id() const { return m_name; }
