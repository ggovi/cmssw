#ifndef BEAMSPOTONLINESOURCEHANDLER_H
#define BEAMSPOTONLINESOURCEHANDLER_H

#include <string>

#include "CondCore/PopCon/interface/PopConSourceHandler.h"
#include "CondFormats/BeamSpotObjects/interface/BeamSpotObjects.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"

class BeamSpotOnlinePopConSourceHandler : public popcon::PopConSourceHandler<BeamSpotObjects> {
public:
  BeamSpotOnlinePopConSourceHandler(const edm::ParameterSet& pset);
  ~BeamSpotOnlinePopConSourceHandler() override;
  void getNewObjects() override;
  std::string id() const override;

private:
  bool m_debug;
  std::string m_name;
  unsigned int m_maxAge;
  unsigned int m_runNumber;
  std::string m_sourcePayloadTag;
  std::unique_ptr<BeamSpotObjects> m_payload;
};

#endif
