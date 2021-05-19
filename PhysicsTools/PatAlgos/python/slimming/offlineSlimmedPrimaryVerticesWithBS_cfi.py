import FWCore.ParameterSet.Config as cms

offlineSlimmedPrimaryVerticesWithBS = cms.EDProducer("PATVertexSlimmer",
    src = cms.InputTag("offlinePrimaryVerticesWithBS"),
#this works also on input file produced with CMSSW<=740pre8, needed for testing or for 72X AOD processing with 74X
    score = cms.InputTag("primaryVertexAssociationWithBS","original"),
# this need new input file with scores made at RECO time (PR #8102, #8101), enable before MINIAOD prod in 74x
#   score = cms.InputTag("offlinePrimaryVertices"),
)
