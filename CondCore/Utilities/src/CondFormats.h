#ifndef Utilities_CondFormats_h
#define Utilities_CondFormats_h
//
#include "CondFormats/HLTObjects/interface/AlCaRecoTriggerBits.h"
#include "CondFormats/Alignment/interface/AlignmentErrors.h"
#include "CondFormats/Alignment/interface/AlignmentErrorsExtended.h"
#include "CondFormats/Alignment/interface/AlignmentSurfaceDeformations.h"
#include "CondFormats/Alignment/interface/Alignments.h"
#include "CondFormats/BeamSpotObjects/interface/BeamSpotObjects.h"
#include "CondFormats/CastorObjects/interface/CastorElectronicsMap.h"
#include "CondFormats/CastorObjects/interface/CastorSaturationCorrs.h"
#include "CondFormats/HIObjects/interface/CentralityTable.h"
#include "CondFormats/HIObjects/interface/RPFlatParams.h"
#include "CondFormats/GeometryObjects/interface/CSCRecoDigiParameters.h"
#include "CondFormats/CSCObjects/interface/CSCBadChambers.h"
#include "CondFormats/CSCObjects/interface/CSCBadStrips.h"
#include "CondFormats/CSCObjects/interface/CSCBadWires.h"
#include "CondFormats/CSCObjects/interface/CSCChamberIndex.h"
#include "CondFormats/CSCObjects/interface/CSCChamberMap.h"
#include "CondFormats/CSCObjects/interface/CSCChamberTimeCorrections.h"
#include "CondFormats/CSCObjects/interface/CSCCrateMap.h"
#include "CondFormats/CSCObjects/interface/CSCDBChipSpeedCorrection.h"
#include "CondFormats/CSCObjects/interface/CSCDBGains.h"
#include "CondFormats/CSCObjects/interface/CSCDBGasGainCorrection.h"
#include "CondFormats/CSCObjects/interface/CSCDBNoiseMatrix.h"
#include "CondFormats/CSCObjects/interface/CSCDBPedestals.h"
#include "CondFormats/CSCObjects/interface/CSCDDUMap.h"
#include "CondFormats/CSCObjects/interface/CSCL1TPParameters.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelDAQMapping.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelAnalysisMask.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelGainCalibrations.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSRPAlignmentCorrectionsData.h"
#include "CondFormats/CTPPSReadoutObjects/interface/PPSTimingCalibration.h"
#include "CondFormats/CTPPSReadoutObjects/interface/LHCOpticalFunctionsSetCollection.h"
#include "CondFormats/DTObjects/interface/DTCCBConfig.h"
#include "CondFormats/DTObjects/interface/DTDeadFlag.h"
#include "CondFormats/DTObjects/interface/DTHVStatus.h"
#include "CondFormats/DTObjects/interface/DTKeyedConfig.h"
#include "CondFormats/DTObjects/interface/DTLVStatus.h"
#include "CondFormats/DTObjects/interface/DTMtime.h"
#include "CondFormats/DTObjects/interface/DTReadOutMapping.h"
#include "CondFormats/DTObjects/interface/DTStatusFlag.h"
#include "CondFormats/DTObjects/interface/DTT0.h"
#include "CondFormats/DTObjects/interface/DTTPGParameters.h"
#include "CondFormats/DTObjects/interface/DTTtrig.h"
#include "CondFormats/DTObjects/interface/DTRecoConditions.h"
#include "CondFormats/DTObjects/interface/DTRecoUncertainties.h"
#include "CondFormats/ESObjects/interface/ESEEIntercalibConstants.h"
#include "CondFormats/ESObjects/interface/ESGain.h"
#include "CondFormats/ESObjects/interface/ESMIPToGeVConstant.h"
#include "CondFormats/ESObjects/interface/ESMissingEnergyCalibration.h"
#include "CondFormats/ESObjects/interface/ESRecHitRatioCuts.h"
#include "CondFormats/ESObjects/interface/ESThresholds.h"
#include "CondFormats/ESObjects/interface/ESTimeSampleWeights.h"
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"
#include "CondFormats/EcalObjects/interface/EcalFunctionParameters.h"
#include "CondFormats/EcalObjects/interface/EcalLaserAPDPNRatios.h"
#include "CondFormats/EcalObjects/interface/EcalSRSettings.h"
#include "CondFormats/EcalObjects/interface/EcalSampleMask.h"
#include "CondFormats/EcalObjects/interface/EcalTBWeights.h"
#include "CondFormats/EcalObjects/interface/EcalTimeBiasCorrections.h"
#include "CondFormats/EcalObjects/interface/EcalTimeOffsetConstant.h"
#include "CondFormats/EcalObjects/interface/EcalTimeBiasCorrections.h"
#include "CondFormats/EcalObjects/interface/EcalTimeDependentCorrections.h"
#include "CondFormats/EcalObjects/interface/EcalSamplesCorrelation.h"
#include "CondFormats/EcalObjects/interface/EcalPulseShapes.h"
#include "CondFormats/EcalObjects/interface/EcalPulseCovariances.h"
#include "CondFormats/EcalObjects/interface/EcalPulseSymmCovariances.h"
#include "CondFormats/EgammaObjects/interface/GBRForest.h"
#include "CondFormats/EgammaObjects/interface/GBRForestD.h"
#include "CondFormats/HcalObjects/interface/AbsOOTPileupCorrection.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"
#include "CondFormats/HcalObjects/interface/OOTPileupCorrectionBuffer.h"
#include "CondFormats/HcalObjects/interface/StorableDoubleMap.h"
#include "CondFormats/HcalObjects/interface/HcalInterpolatedPulseColl.h"
#include "CondFormats/HcalObjects/interface/HBHENegativeEFilter.h"
#include "CondFormats/HcalObjects/interface/HFPhase1PMTParams.h"
#include "CondFormats/HcalObjects/interface/HcalFrontEndMap.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetResolutionObject.h"
#include "CondFormats/JetMETObjects/interface/METCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/MEtXYcorrectParameters.h"
#include "CondFormats/JetMETObjects/interface/QGLikelihoodObject.h"
#include "CondFormats/L1TObjects/interface/L1CaloEcalScale.h"
#include "CondFormats/L1TObjects/interface/L1CaloEtScale.h"
#include "CondFormats/L1TObjects/interface/L1CaloHcalScale.h"
#include "CondFormats/L1TObjects/interface/L1GctChannelMask.h"
#include "CondFormats/L1TObjects/interface/L1MuCSCPtLut.h"
#include "CondFormats/L1TObjects/interface/L1MuCSCTFAlignment.h"
#include "CondFormats/L1TObjects/interface/L1MuCSCTFConfiguration.h"
#include "CondFormats/L1TObjects/interface/L1MuDTEtaPatternLut.h"
#include "CondFormats/L1TObjects/interface/L1MuDTExtLut.h"
#include "CondFormats/L1TObjects/interface/L1MuDTPhiLut.h"
#include "CondFormats/L1TObjects/interface/L1MuDTPtaLut.h"
#include "CondFormats/L1TObjects/interface/L1MuDTQualPatternLut.h"
#include "CondFormats/L1TObjects/interface/L1MuDTTFMasks.h"
#include "CondFormats/L1TObjects/interface/L1MuDTTFParameters.h"
#include "CondFormats/L1TObjects/interface/L1MuGMTChannelMask.h"
#include "CondFormats/L1TObjects/interface/L1MuGMTParameters.h"
#include "CondFormats/L1TObjects/interface/L1MuGMTScales.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerPtScale.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerScales.h"
#include "CondFormats/Luminosity/interface/LumiSectionData.h"
#include "CondFormats/RPCObjects/interface/L1RPCHwConfig.h"
#include "CondFormats/RPCObjects/interface/RPCClusterSize.h"
#include "CondFormats/RPCObjects/interface/RPCStripNoises.h"
#include "CondFormats/L1TObjects/interface/L1RCTParameters.h"
#include "CondFormats/L1TObjects/interface/L1RPCConeDefinition.h"
#include "CondFormats/PhysicsToolsObjects/interface/MVAComputer.h"
#include "CondFormats/GeometryObjects/interface/PCaloGeometry.h"
#include "CondFormats/GeometryObjects/interface/HcalParameters.h"
#include "CondFormats/PhysicsToolsObjects/interface/PerformancePayloadFromTable.h"
#include "CondFormats/PhysicsToolsObjects/interface/PerformanceWorkingPoint.h"
#include "CondFormats/PhysicsToolsObjects/interface/PhysicsTGraphPayload.h"
#include "CondFormats/PhysicsToolsObjects/interface/PhysicsTFormulaPayload.h"
#include "CondFormats/GeometryObjects/interface/PGeometricDet.h"
#include "CondFormats/GeometryObjects/interface/PGeometricDetExtra.h"
#include "CondFormats/GeometryObjects/interface/PTrackerParameters.h"
#include "CondFormats/GeometryObjects/interface/PHGCalParameters.h"
#include "CondFormats/RPCObjects/interface/RPCEMap.h"
#include "CondFormats/RPCObjects/interface/RPCObFebmap.h"
#include "CondFormats/RPCObjects/interface/RPCObGas.h"
#include "CondFormats/RPCObjects/interface/RPCObCond.h"
#include "CondFormats/RPCObjects/interface/RPCObGasMix.h"
#include "CondFormats/RPCObjects/interface/RPCObPVSSmap.h"
#include "CondFormats/RPCObjects/interface/RPCObUXC.h"
#include "CondFormats/RPCObjects/interface/RPCLBLinkMap.h"
#include "CondFormats/RPCObjects/interface/RPCDCCLinkMap.h"
#include "CondFormats/RPCObjects/interface/RPCAMCLinkMap.h"
#include "CondFormats/GeometryObjects/interface/RecoIdealGeometry.h"
#include "CondFormats/RecoMuonObjects/interface/DYTThrObject.h"
#include "CondFormats/RecoMuonObjects/interface/MuScleFitDBobject.h"
#include "CondFormats/RecoMuonObjects/interface/DYTParamObject.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/RunInfo/interface/LHCInfo.h"
#include "CondFormats/RunInfo/interface/MixingModuleConfig.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelCalibConfiguration.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelCPEGenericErrorParm.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelGainCalibrationForHLT.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelGainCalibrationOffline.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelLorentzAngle.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelDynamicInefficiency.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelQuality.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFEDChannelContainer.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelQualityProbabilities.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelTemplateDBObject.h"
#include "CondFormats/SiPixelObjects/interface/SiPixel2DTemplateDBObject.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelGenErrorDBObject.h"
#include "CondFormats/SiStripObjects/interface/SiStripApvGain.h"
#include "CondFormats/SiStripObjects/interface/SiStripBadStrip.h"
#include "CondFormats/SiStripObjects/interface/SiStripFedCabling.h"
#include "CondFormats/SiStripObjects/interface/SiStripThreshold.h"
#include "CondFormats/SiStripObjects/interface/SiStripBackPlaneCorrection.h"
#include "CondFormats/CSCObjects/interface/CSCDBCrosstalk.h"
#include "CondFormats/CSCObjects/interface/CSCDBL1TPParameters.h"
#include "CondFormats/CastorObjects/interface/CastorChannelQuality.h"
#include "CondFormats/CastorObjects/interface/CastorGainWidths.h"
#include "CondFormats/CastorObjects/interface/CastorGains.h"
#include "CondFormats/CastorObjects/interface/CastorPedestalWidths.h"
#include "CondFormats/CastorObjects/interface/CastorPedestals.h"
#include "CondFormats/CastorObjects/interface/CastorQIEData.h"
#include "CondFormats/CastorObjects/interface/CastorRecoParams.h"
#include "CondFormats/Common/interface/DropBoxMetadata.h"
#include "CondFormats/ESObjects/interface/ESChannelStatus.h"
#include "CondFormats/ESObjects/interface/ESCondObjectContainer.h"
#include "CondFormats/ESObjects/interface/ESPedestals.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/EcalObjects/interface/EcalClusterEnergyCorrectionObjectSpecificParameters.h"
#include "CondFormats/EcalObjects/interface/EcalDAQTowerStatus.h"
#include "CondFormats/EcalObjects/interface/EcalDCSTowerStatus.h"
#include "CondFormats/EcalObjects/interface/EcalDQMChannelStatus.h"
#include "CondFormats/EcalObjects/interface/EcalDQMTowerStatus.h"
#include "CondFormats/EcalObjects/interface/EcalGainRatios.h"
#include "CondFormats/EcalObjects/interface/EcalMappingElectronics.h"
#include "CondFormats/EcalObjects/interface/EcalPedestals.h"
#include "CondFormats/EcalObjects/interface/EcalTPGCrystalStatus.h"
#include "CondFormats/EcalObjects/interface/EcalTPGFineGrainEBGroup.h"
#include "CondFormats/EcalObjects/interface/EcalTPGFineGrainEBIdMap.h"
#include "CondFormats/EcalObjects/interface/EcalTPGFineGrainStripEE.h"
#include "CondFormats/EcalObjects/interface/EcalTPGFineGrainTowerEE.h"
#include "CondFormats/EcalObjects/interface/EcalTPGLinearizationConst.h"
#include "CondFormats/EcalObjects/interface/EcalTPGLutGroup.h"
#include "CondFormats/EcalObjects/interface/EcalTPGLutIdMap.h"
#include "CondFormats/EcalObjects/interface/EcalTPGPedestals.h"
#include "CondFormats/EcalObjects/interface/EcalTPGPhysicsConst.h"
#include "CondFormats/EcalObjects/interface/EcalTPGSlidingWindow.h"
#include "CondFormats/EcalObjects/interface/EcalTPGSpike.h"
#include "CondFormats/EcalObjects/interface/EcalTPGStripStatus.h"
#include "CondFormats/EcalObjects/interface/EcalTPGTowerStatus.h"
#include "CondFormats/EcalObjects/interface/EcalTPGWeightGroup.h"
#include "CondFormats/EcalObjects/interface/EcalTPGWeightIdMap.h"
#include "CondFormats/EcalObjects/interface/EcalWeightXtalGroups.h"
#include "CondFormats/EcalObjects/interface/EcalSimPulseShape.h"
#include "CondFormats/Common/interface/FileBlob.h"
//#include "CondFormats/GeometryObjects/interface/GeometryFile.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/HcalObjects/interface/HcalDcsValues.h"
#include "CondFormats/HcalObjects/interface/HcalFlagHFDigiTimeParams.h"
#include "CondFormats/HcalObjects/interface/HcalGains.h"
#include "CondFormats/HcalObjects/interface/HcalGainWidths.h"
#include "CondFormats/HcalObjects/interface/HcalL1TriggerObjects.h"
#include "CondFormats/HcalObjects/interface/HcalLUTCorrs.h"
#include "CondFormats/HcalObjects/interface/HcalLongRecoParams.h"
#include "CondFormats/HcalObjects/interface/HcalZDCLowGainFractions.h"
#include "CondFormats/HcalObjects/interface/HcalLutMetadata.h"
#include "CondFormats/HcalObjects/interface/HcalMCParams.h"
#include "CondFormats/HcalObjects/interface/HcalPFCorrs.h"
#include "CondFormats/HcalObjects/interface/HcalPedestalWidths.h"
#include "CondFormats/HcalObjects/interface/HcalPedestals.h"
#include "CondFormats/HcalObjects/interface/HcalQIEData.h"
#include "CondFormats/HcalObjects/interface/HcalQIETypes.h"
#include "CondFormats/HcalObjects/interface/HcalRecoParams.h"
#include "CondFormats/HcalObjects/interface/HcalRespCorrs.h"
#include "CondFormats/HcalObjects/interface/HcalSiPMParameters.h"
#include "CondFormats/HcalObjects/interface/HcalSiPMCharacteristics.h"
#include "CondFormats/HcalObjects/interface/HcalTimeCorrs.h"
#include "CondFormats/HcalObjects/interface/HcalTPParameters.h"
#include "CondFormats/HcalObjects/interface/HcalTPChannelParameters.h"
#include "CondFormats/HcalObjects/interface/HcalZSThresholds.h"
#include "CondFormats/L1TObjects/interface/L1CaloGeometry.h"
#include "CondFormats/L1TObjects/interface/L1GctJetFinderParams.h"
#include "CondFormats/L1TObjects/interface/L1GtBoardMaps.h"
#include "CondFormats/L1TObjects/interface/L1GtParameters.h"
#include "CondFormats/L1TObjects/interface/L1GtPrescaleFactors.h"
#include "CondFormats/L1TObjects/interface/L1GtPsbSetup.h"
#include "CondFormats/L1TObjects/interface/L1GtStableParameters.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMask.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/L1TObjects/interface/L1RCTChannelMask.h"
#include "CondFormats/L1TObjects/interface/L1RCTNoisyChannelMask.h"
#include "CondFormats/L1TObjects/interface/L1RPCBxOrConfig.h"
#include "CondFormats/L1TObjects/interface/L1RPCConfig.h"
#include "CondFormats/L1TObjects/interface/L1RPCHsbConfig.h"
#include "CondFormats/L1TObjects/interface/CaloConfig.h"
#include "CondFormats/L1TObjects/interface/CaloParams.h"
#include "CondFormats/L1TObjects/interface/L1TMuonBarrelParams.h"
#include "CondFormats/L1TObjects/interface/L1TMuonEndCapForest.h"
#include "CondFormats/L1TObjects/interface/L1TMuonEndCapParams.h"
#include "CondFormats/L1TObjects/interface/L1TMuonGlobalParams.h"
#include "CondFormats/L1TObjects/interface/L1TMuonOverlapParams.h"
#include "CondFormats/L1TObjects/interface/L1TUtmAlgorithm.h"
#include "CondFormats/L1TObjects/interface/L1TUtmBin.h"
#include "CondFormats/L1TObjects/interface/L1TUtmCondition.h"
#include "CondFormats/L1TObjects/interface/L1TUtmCut.h"
#include "CondFormats/L1TObjects/interface/L1TUtmCutValue.h"
#include "CondFormats/L1TObjects/interface/L1TUtmObject.h"
#include "CondFormats/L1TObjects/interface/L1TUtmScale.h"
#include "CondFormats/L1TObjects/interface/L1TUtmTriggerMenu.h"
#include "CondFormats/L1TObjects/interface/L1TGlobalParameters.h"
#include "CondFormats/L1TObjects/interface/L1TGlobalPrescalesVetos.h"
#include "CondFormats/L1TObjects/interface/L1TriggerKey.h"
#include "CondFormats/L1TObjects/interface/L1TriggerKeyExt.h"
#include "CondFormats/L1TObjects/interface/L1TriggerKeyList.h"
#include "CondFormats/L1TObjects/interface/L1TriggerKeyListExt.h"
#include "CondFormats/PhysicsToolsObjects/interface/Histogram3D.h"
#include "CondFormats/PhysicsToolsObjects/interface/PerformancePayload.h"
#include "CondFormats/PhysicsToolsObjects/interface/PerformancePayloadFromTFormula.h"
#include "CondFormats/PhysicsToolsObjects/interface/PerformancePayloadFromBinnedTFormula.h"
#include "CondFormats/SiStripObjects/interface/SiStripConfObject.h"
#include "CondFormats/SiStripObjects/interface/SiStripLatency.h"
#include "CondFormats/SiStripObjects/interface/SiStripLorentzAngle.h"
#include "CondFormats/SiStripObjects/interface/SiStripNoises.h"
#include "CondFormats/SiStripObjects/interface/SiStripPedestals.h"
#include "CondFormats/BTauObjects/interface/TrackProbabilityCalibration.h"
#include "CondFormats/MFObjects/interface/MagFieldConfig.h"
#include "CondFormats/PCLConfig/interface/AlignPCLThresholds.h"
#include "CondFormats/SiPhase2TrackerObjects/interface/TrackerDetToDTCELinkCablingMap.h"
#include "CondFormats/SiPhase2TrackerObjects/interface/DTCELinkId.h"
#include "CondFormats/Common/interface/BaseKeyed.h"

#include "CondFormats/External/interface/DetID.h"
#include "CondFormats/External/interface/EcalDetID.h"
#include "CondFormats/External/interface/SMatrix.h"
#include "CondFormats/External/interface/L1GtLogicParser.h"
#include "CondFormats/External/interface/Timestamp.h"
#include "CondFormats/External/interface/HLTPrescaleTable.h"

#endif
