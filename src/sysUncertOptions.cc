#include "tthAnalysis/HiggsToTauTau/interface/sysUncertOptions.h"

#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // kEra_*

#include <TString.h> // Form()

#include <boost/algorithm/string/predicate.hpp> // boost::algorithm::starts_with(), boost::algorithm::ends_with()

bool
isValidJESsource(int __attribute__((unused)) era,
                 int __attribute__((unused)) central_or_shift)
{
  // keeping for future compatibility
  return true;
}

int
getBTagWeight_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kBtag_central;
  if     (central_or_shift == "CMS_ttHl_btag_HFUp"        ) central_or_shift_int = kBtag_hfUp;
  else if(central_or_shift == "CMS_ttHl_btag_HFDown"      ) central_or_shift_int = kBtag_hfDown;
  else if(central_or_shift == "CMS_ttHl_btag_HFStats1Up"  ) central_or_shift_int = kBtag_hfStats1Up;
  else if(central_or_shift == "CMS_ttHl_btag_HFStats1Down") central_or_shift_int = kBtag_hfStats1Down;
  else if(central_or_shift == "CMS_ttHl_btag_HFStats2Up"  ) central_or_shift_int = kBtag_hfStats2Up;
  else if(central_or_shift == "CMS_ttHl_btag_HFStats2Down") central_or_shift_int = kBtag_hfStats2Down;
  else if(central_or_shift == "CMS_ttHl_btag_LFUp"        ) central_or_shift_int = kBtag_lfUp;
  else if(central_or_shift == "CMS_ttHl_btag_LFDown"      ) central_or_shift_int = kBtag_lfDown;
  else if(central_or_shift == "CMS_ttHl_btag_LFStats1Up"  ) central_or_shift_int = kBtag_lfStats1Up;
  else if(central_or_shift == "CMS_ttHl_btag_LFStats1Down") central_or_shift_int = kBtag_lfStats1Down;
  else if(central_or_shift == "CMS_ttHl_btag_LFStats2Up"  ) central_or_shift_int = kBtag_lfStats2Up;
  else if(central_or_shift == "CMS_ttHl_btag_LFStats2Down") central_or_shift_int = kBtag_lfStats2Down;
  else if(central_or_shift == "CMS_ttHl_btag_cErr1Up"     ) central_or_shift_int = kBtag_cErr1Up;
  else if(central_or_shift == "CMS_ttHl_btag_cErr1Down"   ) central_or_shift_int = kBtag_cErr1Down;
  else if(central_or_shift == "CMS_ttHl_btag_cErr2Up"     ) central_or_shift_int = kBtag_cErr2Up;
  else if(central_or_shift == "CMS_ttHl_btag_cErr2Down"   ) central_or_shift_int = kBtag_cErr2Down;
  else if(central_or_shift == "CMS_ttHl_JESUp"            ) central_or_shift_int = kBtag_jesUp;
  else if(central_or_shift == "CMS_ttHl_JESDown"          ) central_or_shift_int = kBtag_jesDown; 
  return central_or_shift_int;
}

int
getJet_option(const std::string & central_or_shift,
              bool isMC)
{
  int central_or_shift_int = isMC ? kJetMET_central : kJetMET_central_nonNominal;
  if     (central_or_shift == "CMS_ttHl_JESUp"                    ) central_or_shift_int = kJetMET_jesUp;
  else if(central_or_shift == "CMS_ttHl_JESDown"                  ) central_or_shift_int = kJetMET_jesDown;
  else if(central_or_shift == "CMS_ttHl_JERUp"                    ) central_or_shift_int = kJetMET_jerUp;
  else if(central_or_shift == "CMS_ttHl_JERDown"                  ) central_or_shift_int = kJetMET_jerDown;
  else if(central_or_shift == "CMS_ttHl_JESAbsoluteUp"            ) central_or_shift_int = kJetMET_jesAbsoluteUp;
  else if(central_or_shift == "CMS_ttHl_JESAbsoluteDown"          ) central_or_shift_int = kJetMET_jesAbsoluteDown;
  else if(central_or_shift == "CMS_ttHl_JESAbsolute_EraUp"        ) central_or_shift_int = kJetMET_jesAbsolute_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESAbsolute_EraDown"      ) central_or_shift_int = kJetMET_jesAbsolute_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1Up"               ) central_or_shift_int = kJetMET_jesBBEC1Up;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1Down"             ) central_or_shift_int = kJetMET_jesBBEC1Down;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1_EraUp"           ) central_or_shift_int = kJetMET_jesBBEC1_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1_EraDown"         ) central_or_shift_int = kJetMET_jesBBEC1_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESEC2Up"                 ) central_or_shift_int = kJetMET_jesEC2Up;
  else if(central_or_shift == "CMS_ttHl_JESEC2Down"               ) central_or_shift_int = kJetMET_jesEC2Down;
  else if(central_or_shift == "CMS_ttHl_JESEC2_EraUp"             ) central_or_shift_int = kJetMET_jesEC2_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESEC2_EraDown"           ) central_or_shift_int = kJetMET_jesEC2_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESFlavorQCDUp"           ) central_or_shift_int = kJetMET_jesFlavorQCDUp;
  else if(central_or_shift == "CMS_ttHl_JESFlavorQCDDown"         ) central_or_shift_int = kJetMET_jesFlavorQCDDown;
  else if(central_or_shift == "CMS_ttHl_JESHFUp"                  ) central_or_shift_int = kJetMET_jesHFUp;
  else if(central_or_shift == "CMS_ttHl_JESHFDown"                ) central_or_shift_int = kJetMET_jesHFDown;
  else if(central_or_shift == "CMS_ttHl_JESHF_EraUp"              ) central_or_shift_int = kJetMET_jesHF_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESHF_EraDown"            ) central_or_shift_int = kJetMET_jesHF_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESRelativeBalUp"         ) central_or_shift_int = kJetMET_jesRelativeBalUp;
  else if(central_or_shift == "CMS_ttHl_JESRelativeBalDown"       ) central_or_shift_int = kJetMET_jesRelativeBalDown;
  else if(central_or_shift == "CMS_ttHl_JESRelativeSample_EraUp"  ) central_or_shift_int = kJetMET_jesRelativeSample_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESRelativeSample_EraDown") central_or_shift_int = kJetMET_jesRelativeSample_EraDown;
  return central_or_shift_int;
}

int
getMET_option(const std::string & central_or_shift,
              bool isMC)
{
  int central_or_shift_int = isMC ? kJetMET_central : kJetMET_central_nonNominal;
  if     (central_or_shift == "CMS_ttHl_JESUp"                    ) central_or_shift_int = kJetMET_jesUp;
  else if(central_or_shift == "CMS_ttHl_JESDown"                  ) central_or_shift_int = kJetMET_jesDown;
  else if(central_or_shift == "CMS_ttHl_JERUp"                    ) central_or_shift_int = kJetMET_jerUp;
  else if(central_or_shift == "CMS_ttHl_JERDown"                  ) central_or_shift_int = kJetMET_jerDown;
  else if(central_or_shift == "CMS_ttHl_UnclusteredEnUp"          ) central_or_shift_int = kJetMET_UnclusteredEnUp;
  else if(central_or_shift == "CMS_ttHl_UnclusteredEnDown"        ) central_or_shift_int = kJetMET_UnclusteredEnDown;
  else if(central_or_shift == "CMS_ttHl_JESAbsoluteUp"            ) central_or_shift_int = kJetMET_jesAbsoluteUp;
  else if(central_or_shift == "CMS_ttHl_JESAbsoluteDown"          ) central_or_shift_int = kJetMET_jesAbsoluteDown;
  else if(central_or_shift == "CMS_ttHl_JESAbsolute_EraUp"        ) central_or_shift_int = kJetMET_jesAbsolute_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESAbsolute_EraDown"      ) central_or_shift_int = kJetMET_jesAbsolute_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1Up"               ) central_or_shift_int = kJetMET_jesBBEC1Up;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1Down"             ) central_or_shift_int = kJetMET_jesBBEC1Down;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1_EraUp"           ) central_or_shift_int = kJetMET_jesBBEC1_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1_EraDown"         ) central_or_shift_int = kJetMET_jesBBEC1_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESEC2Up"                 ) central_or_shift_int = kJetMET_jesEC2Up;
  else if(central_or_shift == "CMS_ttHl_JESEC2Down"               ) central_or_shift_int = kJetMET_jesEC2Down;
  else if(central_or_shift == "CMS_ttHl_JESEC2_EraUp"             ) central_or_shift_int = kJetMET_jesEC2_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESEC2_EraDown"           ) central_or_shift_int = kJetMET_jesEC2_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESFlavorQCDUp"           ) central_or_shift_int = kJetMET_jesFlavorQCDUp;
  else if(central_or_shift == "CMS_ttHl_JESFlavorQCDDown"         ) central_or_shift_int = kJetMET_jesFlavorQCDDown;
  else if(central_or_shift == "CMS_ttHl_JESHFUp"                  ) central_or_shift_int = kJetMET_jesHFUp;
  else if(central_or_shift == "CMS_ttHl_JESHFDown"                ) central_or_shift_int = kJetMET_jesHFDown;
  else if(central_or_shift == "CMS_ttHl_JESHF_EraUp"              ) central_or_shift_int = kJetMET_jesHF_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESHF_EraDown"            ) central_or_shift_int = kJetMET_jesHF_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESRelativeBalUp"         ) central_or_shift_int = kJetMET_jesRelativeBalUp;
  else if(central_or_shift == "CMS_ttHl_JESRelativeBalDown"       ) central_or_shift_int = kJetMET_jesRelativeBalDown;
  else if(central_or_shift == "CMS_ttHl_JESRelativeSample_EraUp"  ) central_or_shift_int = kJetMET_jesRelativeSample_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESRelativeSample_EraDown") central_or_shift_int = kJetMET_jesRelativeSample_EraDown;
  return central_or_shift_int;
}

METSyst
getMETsyst_option(const std::string & central_or_shift)
{
  METSyst central_or_shift_int = METSyst::central;
  if     (central_or_shift == "MET_RespUp")    central_or_shift_int = METSyst::respUp;
  else if(central_or_shift == "MET_RespDown")  central_or_shift_int = METSyst::respDown;
  else if(central_or_shift == "MET_ResolUp")   central_or_shift_int = METSyst::resolUp;
  else if(central_or_shift == "MET_ResolDown") central_or_shift_int = METSyst::resolDown;
  return central_or_shift_int;
}

int
getHadTauPt_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kHadTauPt_central;
  if     (central_or_shift == "CMS_ttHl_tauESUp"  ) central_or_shift_int = kHadTauPt_shiftUp;
  else if(central_or_shift == "CMS_ttHl_tauESDown") central_or_shift_int = kHadTauPt_shiftDown;
  return central_or_shift_int;
}

int
getJetToTauFR_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kFRjt_central;
  if     (central_or_shift == "CMS_ttHl_FRjt_normUp"   ) central_or_shift_int = kFRjt_normUp;
  else if(central_or_shift == "CMS_ttHl_FRjt_normDown" ) central_or_shift_int = kFRjt_normDown;
  else if(central_or_shift == "CMS_ttHl_FRjt_shapeUp"  ) central_or_shift_int = kFRjt_shapeUp;
  else if(central_or_shift == "CMS_ttHl_FRjt_shapeDown") central_or_shift_int = kFRjt_shapeDown;
  return central_or_shift_int;
}

FRet
getEToTauFR_option(const std::string & central_or_shift)
{
  FRet central_or_shift_int = FRet::central;
  if     (central_or_shift == "CMS_ttHl_FRet_shiftUp"  ) central_or_shift_int = FRet::shiftUp;
  else if(central_or_shift == "CMS_ttHl_FRet_shiftDown") central_or_shift_int = FRet::shiftDown;
  return central_or_shift_int;
}

FRmt
getMuToTauFR_option(const std::string & central_or_shift)
{
  FRmt central_or_shift_int = FRmt::central;
  if     (central_or_shift == "CMS_ttHl_FRmt_shiftUp"  ) central_or_shift_int = FRmt::shiftUp;
  else if(central_or_shift == "CMS_ttHl_FRmt_shiftDown") central_or_shift_int = FRmt::shiftDown;
  return central_or_shift_int;
}

LeptonIDSFsys
getLeptonIDSFsys_option(const std::string & central_or_shift)
{
  LeptonIDSFsys central_or_shift_int = LeptonIDSFsys::central;
  if     (central_or_shift == "CMS_ttHl_leptonIDSFUp"  ) central_or_shift_int = LeptonIDSFsys::shiftUp;
  else if(central_or_shift == "CMS_ttHl_leptonIDSFDown") central_or_shift_int = LeptonIDSFsys::shiftDown;
  return central_or_shift_int;
}

TauIDSFsys
getTauIDSFsys_option(const std::string & central_or_shift)
{
  TauIDSFsys central_or_shift_int = TauIDSFsys::central;
  if     (central_or_shift == "CMS_ttHl_tauIDSFUp"  ) central_or_shift_int = TauIDSFsys::shiftUp;
  else if(central_or_shift == "CMS_ttHl_tauIDSFDown") central_or_shift_int = TauIDSFsys::shiftDown;
  return central_or_shift_int;
}

TriggerSFsys
getTriggerSF_option(const std::string & central_or_shift)
{
  TriggerSFsys central_or_shift_int = TriggerSFsys::central;
  if     (central_or_shift == "CMS_ttHl_triggerUp"  ) central_or_shift_int = TriggerSFsys::shiftUp;
  else if(central_or_shift == "CMS_ttHl_triggerDown") central_or_shift_int = TriggerSFsys::shiftDown;
  return central_or_shift_int;
}

int
getLHEscale_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kLHE_scale_central;
  if(boost::starts_with(central_or_shift, "CMS_ttHl_thu_shape"))
  {
    if     (boost::ends_with(central_or_shift, "x1Down")) central_or_shift_int = kLHE_scale_xDown;
    else if(boost::ends_with(central_or_shift, "x1Up")  ) central_or_shift_int = kLHE_scale_xUp;
    else if(boost::ends_with(central_or_shift, "y1Down")) central_or_shift_int = kLHE_scale_yDown;
    else if(boost::ends_with(central_or_shift, "y1Up")  ) central_or_shift_int = kLHE_scale_yUp;
    else throw cmsException(__func__, __LINE__)
           << "Invalid option to LHE systematics: " << central_or_shift;
  }
  return central_or_shift_int;
}

int
getElectronPt_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kElectronPt_central;
  if     (central_or_shift == "CMS_ttHl_electronESBarrelUp"  ) central_or_shift_int = kElectronPt_scaleUp_barrel;
  else if(central_or_shift == "CMS_ttHl_electronESBarrelDown") central_or_shift_int = kElectronPt_scaleDown_barrel;
  else if(central_or_shift == "CMS_ttHl_electronESEndcapUp"  ) central_or_shift_int = kElectronPt_scaleUp_endcap;
  else if(central_or_shift == "CMS_ttHl_electronESEndcapDown") central_or_shift_int = kElectronPt_scaleDown_endcap;
  else if(central_or_shift == "CMS_ttHl_electronERUp"        ) central_or_shift_int = kElectronPt_resUp;
  else if(central_or_shift == "CMS_ttHl_electronERDown"      ) central_or_shift_int = kElectronPt_resDown;
  return central_or_shift_int;
}

int
getMuon_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kMuon_central;
  if     (central_or_shift == "CMS_ttHl_muonERUp"         ) central_or_shift_int = kMuon_ERUp;
  else if(central_or_shift == "CMS_ttHl_muonERDown"       ) central_or_shift_int = kMuon_ERDown;
  else if(central_or_shift == "CMS_ttHl_muonESBarrel1Up"  ) central_or_shift_int = kMuon_ESBarrel1Up;
  else if(central_or_shift == "CMS_ttHl_muonESBarrel1Down") central_or_shift_int = kMuon_ESBarrel1Down;
  else if(central_or_shift == "CMS_ttHl_muonESBarrel2Up"  ) central_or_shift_int = kMuon_ESBarrel2Up;
  else if(central_or_shift == "CMS_ttHl_muonESBarrel2Down") central_or_shift_int = kMuon_ESBarrel2Down;
  else if(central_or_shift == "CMS_ttHl_muonESEndcap1Up"  ) central_or_shift_int = kMuon_ESEndcap1Up;
  else if(central_or_shift == "CMS_ttHl_muonESEndcap1Down") central_or_shift_int = kMuon_ESEndcap1Down;
  else if(central_or_shift == "CMS_ttHl_muonESEndcap2Up"  ) central_or_shift_int = kMuon_ESEndcap2Up;
  else if(central_or_shift == "CMS_ttHl_muonESEndcap2Down") central_or_shift_int = kMuon_ESEndcap2Down;
  return central_or_shift_int;
}

int
getJetToLeptonFR_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kFRl_central;
  if     (central_or_shift == "CMS_ttHl_FRe_shape_ptUp"          ) central_or_shift_int = kFRe_shape_ptUp;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_ptDown"        ) central_or_shift_int = kFRe_shape_ptDown;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_normUp"        ) central_or_shift_int = kFRe_shape_normUp;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_normDown"      ) central_or_shift_int = kFRe_shape_normDown;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_eta_barrelUp"  ) central_or_shift_int = kFRe_shape_eta_barrelUp;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_eta_barrelDown") central_or_shift_int = kFRe_shape_eta_barrelDown;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_ptUp"          ) central_or_shift_int = kFRm_shape_ptUp;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_ptDown"        ) central_or_shift_int = kFRm_shape_ptDown;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_normUp"        ) central_or_shift_int = kFRm_shape_normUp;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_normDown"      ) central_or_shift_int = kFRm_shape_normDown;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_eta_barrelUp"  ) central_or_shift_int = kFRm_shape_eta_barrelUp;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_eta_barrelDown") central_or_shift_int = kFRm_shape_eta_barrelDown;
  return central_or_shift_int;
}

PUsys
getPUsys_option(const std::string & central_or_shift)
{
  PUsys central_or_shift_int = PUsys::central;
  if     (central_or_shift == "CMS_ttHl_pileupUp"  ) central_or_shift_int = PUsys::up;
  else if(central_or_shift == "CMS_ttHl_pileupDown") central_or_shift_int = PUsys::down;
  return central_or_shift_int;
}


L1PreFiringWeightSys
getL1PreFiringWeightSys_option(const std::string & central_or_shift)
{
  L1PreFiringWeightSys central_or_shift_int = L1PreFiringWeightSys::nominal;
  if     (central_or_shift == "CMS_ttHl_l1PreFireUp")   central_or_shift_int = L1PreFiringWeightSys::up;
  else if(central_or_shift == "CMS_ttHl_l1PreFireDown") central_or_shift_int = L1PreFiringWeightSys::down;
  return central_or_shift_int;
}

int
getDYMCReweighting_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kDYMCReweighting_central;
  if     (central_or_shift == "CMS_ttHl_DYMCReweightingUp"  ) central_or_shift_int = kDYMCReweighting_shiftUp;
  else if(central_or_shift == "CMS_ttHl_DYMCReweightingDown") central_or_shift_int = kDYMCReweighting_shiftDown;
  return central_or_shift_int;
}

int
getDYMCNormScaleFactors_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kDYMCNormScaleFactors_central;
  if     (central_or_shift == "CMS_ttHl_DYMCNormScaleFactorsUp"  ) central_or_shift_int = kDYMCNormScaleFactors_shiftUp;
  else if(central_or_shift == "CMS_ttHl_DYMCNormScaleFactorsDown") central_or_shift_int = kDYMCNormScaleFactors_shiftDown;
  return central_or_shift_int;
}

int
getTopPtReweighting_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kTopPtReweighting_central;
  if     (central_or_shift == "CMS_ttHl_topPtReweightingUp"  ) central_or_shift_int = kTopPtReweighting_shiftUp;
  else if(central_or_shift == "CMS_ttHl_topPtReweightingDown") central_or_shift_int = kTopPtReweighting_shiftDown;
  return central_or_shift_int;
}

MEMsys
getMEMsys_option(const std::string & central_or_shift)
{
  MEMsys central_or_shift_int = MEMsys::nominal;
  if(boost::starts_with(central_or_shift, "CMS_ttHl_MEM"))
  {
    if     (boost::ends_with(central_or_shift, "Up")  ) central_or_shift_int = MEMsys::up;
    else if(boost::ends_with(central_or_shift, "Down")) central_or_shift_int = MEMsys::down;
  }
  return central_or_shift_int;
}

void
checkOptionValidity(const std::string & central_or_shift,
                    bool isMC)
{
  if(! (central_or_shift.empty() || central_or_shift == "central"))
  {
    if(! isMC && getJetToLeptonFR_option(central_or_shift) != kFRl_central && getJetToTauFR_option(central_or_shift) != kFRjt_central)
    {
      throw cmsException(__func__, __LINE__) << "Non-empty or non-central systematics option passed to data";
    }
    if(isMC && ! (boost::ends_with(central_or_shift, "Up") || boost::ends_with(central_or_shift, "Down")))
    {
      throw cmsException(__func__, __LINE__) << "Non-central MC systematics option not ending with Up or Down";
    }
  }
}

std::string
getBranchName_bTagWeight(Btag btag,
                         const std::string & default_collectionName,
                         int central_or_shift)
{
  std::map<int, std::string> branchNames_bTagWeight;
  std::string discrName;
  switch(btag)
  {
    case Btag::kDeepJet: discrName = "deepjet"; break;
    case Btag::kDeepCSV: discrName = "deepcsv"; break;
    case Btag::kCSVv2:   discrName = "csvv2";   break;
  }
  branchNames_bTagWeight[kBtag_central] = Form("%s_btagSF_%s_shape", default_collectionName.data(), discrName.data());
  branchNames_bTagWeight[kBtag_hfUp]         = branchNames_bTagWeight[kBtag_central] + "_up_hf";
  branchNames_bTagWeight[kBtag_hfDown]       = branchNames_bTagWeight[kBtag_central] + "_down_hf";
  branchNames_bTagWeight[kBtag_hfStats1Up]   = branchNames_bTagWeight[kBtag_central] + "_up_hfstats1";
  branchNames_bTagWeight[kBtag_hfStats1Down] = branchNames_bTagWeight[kBtag_central] + "_down_hfstats1";
  branchNames_bTagWeight[kBtag_hfStats2Up]   = branchNames_bTagWeight[kBtag_central] + "_up_hfstats2";
  branchNames_bTagWeight[kBtag_hfStats2Down] = branchNames_bTagWeight[kBtag_central] + "_down_hfstats2";
  branchNames_bTagWeight[kBtag_lfUp]         = branchNames_bTagWeight[kBtag_central] + "_up_lf";
  branchNames_bTagWeight[kBtag_lfDown]       = branchNames_bTagWeight[kBtag_central] + "_down_lf";
  branchNames_bTagWeight[kBtag_lfStats1Up]   = branchNames_bTagWeight[kBtag_central] + "_up_lfstats1";
  branchNames_bTagWeight[kBtag_lfStats1Down] = branchNames_bTagWeight[kBtag_central] + "_down_lfstats1";
  branchNames_bTagWeight[kBtag_lfStats2Up]   = branchNames_bTagWeight[kBtag_central] + "_up_lfstats2";
  branchNames_bTagWeight[kBtag_lfStats2Down] = branchNames_bTagWeight[kBtag_central] + "_down_lfstats2";
  branchNames_bTagWeight[kBtag_cErr1Up]      = branchNames_bTagWeight[kBtag_central] + "_up_cferr1";
  branchNames_bTagWeight[kBtag_cErr1Down]    = branchNames_bTagWeight[kBtag_central] + "_down_cferr1";
  branchNames_bTagWeight[kBtag_cErr2Up]      = branchNames_bTagWeight[kBtag_central] + "_up_cferr2";
  branchNames_bTagWeight[kBtag_cErr2Down]    = branchNames_bTagWeight[kBtag_central] + "_down_cferr2";
  branchNames_bTagWeight[kBtag_jesUp]        = branchNames_bTagWeight[kBtag_central] + "_up_jes";
  branchNames_bTagWeight[kBtag_jesDown]      = branchNames_bTagWeight[kBtag_central] + "_down_jes";
  assert(branchNames_bTagWeight.count(central_or_shift));
  return branchNames_bTagWeight.at(central_or_shift);
}

std::string
getBranchName_jetMET(const std::string & default_branchName,
                        int era,
                        int central_or_shift,
                        bool isPt)
{
  static std::map<int, std::string> branchNames_sys;
  const bool isJet = default_branchName == "Jet";
  const bool isMET = default_branchName == "MET";
  const std::string era_str = [era]() -> std::string {
    switch(era)
    {
      case kEra_2016 : return "2016";
      case kEra_2017 : return "2017";
      case kEra_2018 : return "2018";
      default: throw cmsException(__func__, __LINE__) << "Invalid era: " << era;
    }
  }();
  if(! isJet && ! isMET)
  {
    throw cmsException(__func__, __LINE__) << "Invalid branch name provided: " << default_branchName;
  }
  branchNames_sys[kJetMET_central_nonNominal] = Form(
    "%s_%s", default_branchName.data(), isPt ? "pt" : (isJet ? "mass" : "phi")
  );
  branchNames_sys[kJetMET_central]                   = branchNames_sys[kJetMET_central_nonNominal] + "_nom";
  branchNames_sys[kJetMET_jesUp]                     = branchNames_sys[kJetMET_central_nonNominal] + "_jesTotalUp";
  branchNames_sys[kJetMET_jesDown]                   = branchNames_sys[kJetMET_central_nonNominal] + "_jesTotalDown";
  //
  branchNames_sys[kJetMET_jesAbsoluteUp]             = branchNames_sys[kJetMET_central_nonNominal] + "_jesAbsoluteUp";
  branchNames_sys[kJetMET_jesAbsoluteDown]           = branchNames_sys[kJetMET_central_nonNominal] + "_jesAbsoluteDown";
  branchNames_sys[kJetMET_jesAbsolute_EraUp]         = branchNames_sys[kJetMET_central_nonNominal] + Form("_jesAbsolute_%sUp",    era_str.data());
  branchNames_sys[kJetMET_jesAbsolute_EraDown]       = branchNames_sys[kJetMET_central_nonNominal] + Form("_jesAbsolute_%sDown", era_str.data());
  branchNames_sys[kJetMET_jesBBEC1Up]                = branchNames_sys[kJetMET_central_nonNominal] + "_jesBBEC1Up";
  branchNames_sys[kJetMET_jesBBEC1Down]              = branchNames_sys[kJetMET_central_nonNominal] + "_jesBBEC1Down";
  branchNames_sys[kJetMET_jesBBEC1_EraUp]            = branchNames_sys[kJetMET_central_nonNominal] + Form("_jesBBEC1_%sUp",   era_str.data());
  branchNames_sys[kJetMET_jesBBEC1_EraDown]          = branchNames_sys[kJetMET_central_nonNominal] + Form("_jesBBEC1_%sDown", era_str.data());
  branchNames_sys[kJetMET_jesEC2Up]                  = branchNames_sys[kJetMET_central_nonNominal] + "_jesEC2Up";
  branchNames_sys[kJetMET_jesEC2Down]                = branchNames_sys[kJetMET_central_nonNominal] + "_jesEC2Down";
  branchNames_sys[kJetMET_jesEC2_EraUp]              = branchNames_sys[kJetMET_central_nonNominal] + Form("_jesEC2_%sUp",   era_str.data());
  branchNames_sys[kJetMET_jesEC2_EraDown]            = branchNames_sys[kJetMET_central_nonNominal] + Form("_jesEC2_%sDown", era_str.data());
  branchNames_sys[kJetMET_jesFlavorQCDUp]            = branchNames_sys[kJetMET_central_nonNominal] + "_jesFlavorQCDUp";
  branchNames_sys[kJetMET_jesFlavorQCDDown]          = branchNames_sys[kJetMET_central_nonNominal] + "_jesFlavorQCDDown";
  branchNames_sys[kJetMET_jesHFUp]                   = branchNames_sys[kJetMET_central_nonNominal] + "_jesHFUp";
  branchNames_sys[kJetMET_jesHFDown]                 = branchNames_sys[kJetMET_central_nonNominal] + "_jesHFDown";
  branchNames_sys[kJetMET_jesHF_EraUp]               = branchNames_sys[kJetMET_central_nonNominal] + Form("_jesHF_%sUp",   era_str.data());
  branchNames_sys[kJetMET_jesHF_EraDown]             = branchNames_sys[kJetMET_central_nonNominal] + Form("_jesHF_%sDown", era_str.data());
  branchNames_sys[kJetMET_jesRelativeBalUp]          = branchNames_sys[kJetMET_central_nonNominal] + "_jesRelativeBalUp";
  branchNames_sys[kJetMET_jesRelativeBalDown]        = branchNames_sys[kJetMET_central_nonNominal] + "_jesRelativeBalDown";
  branchNames_sys[kJetMET_jesRelativeSample_EraUp]   = branchNames_sys[kJetMET_central_nonNominal] + Form("_jesRelativeSample_%sUp",   era_str.data());
  branchNames_sys[kJetMET_jesRelativeSample_EraDown] = branchNames_sys[kJetMET_central_nonNominal] + Form("_jesRelativeSample_%sDown", era_str.data());
  //
  branchNames_sys[kJetMET_jerUp]                     = branchNames_sys[kJetMET_central_nonNominal] + "_jerUp";
  branchNames_sys[kJetMET_jerDown]                   = branchNames_sys[kJetMET_central_nonNominal] + "_jerDown";
  branchNames_sys[kJetMET_UnclusteredEnUp]           = branchNames_sys[kJetMET_central_nonNominal] + "_unclustEnUp";
  branchNames_sys[kJetMET_UnclusteredEnDown]         = branchNames_sys[kJetMET_central_nonNominal] + "_unclustEnDown";
  //
  assert(branchNames_sys.count(central_or_shift));
  assert(isValidJESsource(era, central_or_shift));
  assert((isJet && central_or_shift != kJetMET_UnclusteredEnUp && central_or_shift != kJetMET_UnclusteredEnDown) || ! isJet);
  return branchNames_sys.at(central_or_shift);
}

std::string
getBranchName_pileup(PUsys puSys_option)
{
  switch(puSys_option)
  {
    case PUsys::central: return "puWeight";
    case PUsys::up:      return "puWeightUp";
    case PUsys::down:    return "puWeightDown";
    default: throw cmsException(__func__, __LINE__) << "Invalid option = " << static_cast<int>(puSys_option);
  }
}
