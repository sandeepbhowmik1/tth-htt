#include "tthAnalysis/HiggsToTauTau/interface/EvtWeightRecorder.h" // EvtWeightRecorder

#include "tthAnalysis/HiggsToTauTau/interface/L1PreFiringWeightReader.h"
#include "tthAnalysis/HiggsToTauTau/interface/LHEInfoReader.h"
#include "tthAnalysis/HiggsToTauTau/interface/EventInfo.h"
#include "tthAnalysis/HiggsToTauTau/interface/Data_to_MC_CorrectionInterface_Base.h"
#include "tthAnalysis/HiggsToTauTau/interface/JetToTauFakeRateInterface.h"
#include "tthAnalysis/HiggsToTauTau/interface/LeptonFakeRateInterface.h"
#include "tthAnalysis/HiggsToTauTau/interface/EvtWeightManager.h"
#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h"
#include "tthAnalysis/HiggsToTauTau/interface/sysUncertOptions.h"
#include "tthAnalysis/HiggsToTauTau/interface/fakeBackgroundAuxFunctions.h"

#include <cassert> // assert()

EvtWeightRecorder::EvtWeightRecorder()
  : isMC_(false)
  , genWeight_(1.)
  , leptonSF_(1.)
  , chargeMisIdProb_(1.)
{}

EvtWeightRecorder::EvtWeightRecorder(const std::vector<std::string> & central_or_shifts,
                                     bool isMC)
  : isMC_(isMC)
  , genWeight_(1.)
  , leptonSF_(1.)
  , chargeMisIdProb_(1.)
  , central_or_shifts_(central_or_shifts)
{
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    checkOptionValidity(central_or_shift, isMC);
  }
}

double
EvtWeightRecorder::get(const std::string & central_or_shift) const
{
  // TODO OPTIMIZE
  return (isMC_ ? get_inclusive(central_or_shift) * get_data_to_MC_correction(central_or_shift) : 1.) *
         get_FR(central_or_shift) * chargeMisIdProb_
  ;
}

double
EvtWeightRecorder::get_inclusive(const std::string & central_or_shift) const
{
  // TODO OPTIMIZE
  return isMC_ ? genWeight_ * get_auxWeight(central_or_shift) * get_lumiScale(central_or_shift) *
                 get_nom_tH_weight(central_or_shift) * get_puWeight(central_or_shift) *
                 get_l1PreFiringWeight(central_or_shift) * get_lheScaleWeight(central_or_shift)
               : 1.
  ;
}

double
EvtWeightRecorder::get_genWeight() const
{
  return genWeight_;
}

double
EvtWeightRecorder::get_auxWeight(const std::string & central_or_shift) const
{
  if(isMC_ && auxWeight_.count(central_or_shift))
  {
    return auxWeight_.at(central_or_shift);
  }
  return 1.;
}

double
EvtWeightRecorder::get_lumiScale(const std::string & central_or_shift) const
{
  if(isMC_ && lumiScale_.count(central_or_shift))
  {
    return lumiScale_.at(central_or_shift);
  }
  return 1.;
}

double
EvtWeightRecorder::get_nom_tH_weight(const std::string & central_or_shift) const
{
  if(isMC_ && nom_tH_weight_.count(central_or_shift))
  {
    return nom_tH_weight_.at(central_or_shift);
  }
  return 1.;
}

double
EvtWeightRecorder::get_puWeight(const std::string & central_or_shift) const
{
  if(isMC_)
  {
    const PUsys puSys_option = central_or_shift.empty() ? PUsys::central : getPUsys_option(central_or_shift);
    if(weights_pu_.count(puSys_option))
    {
      return  weights_pu_.at(puSys_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_l1PreFiringWeight(const std::string & central_or_shift) const
{
  if(isMC_)
  {
    const L1PreFiringWeightSys l1PreFire_option = central_or_shift.empty() ?
      L1PreFiringWeightSys::nominal :
      getL1PreFiringWeightSys_option(central_or_shift)
    ;
    if(weights_l1PreFiring_.count(l1PreFire_option))
    {
      return weights_l1PreFiring_.at(l1PreFire_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_lheScaleWeight(const std::string & central_or_shift) const
{
  if(isMC_)
  {
    const int lheScale_option = central_or_shift.empty() ? kLHE_scale_central : getLHEscale_option(central_or_shift);
    if(weights_lheScale_.count(lheScale_option))
    {
      return weights_lheScale_.at(lheScale_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_btag(const std::string & central_or_shift) const
{
  if(isMC_)
  {
    const int jetBtagSF_option = central_or_shift.empty() ? kBtag_central : getBTagWeight_option(central_or_shift);
    if(weights_btag_.count(jetBtagSF_option))
    {
      return weights_btag_.at(jetBtagSF_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_sf_triggerEff(const std::string & central_or_shift) const
{
  if(isMC_)
  {
    const TriggerSFsys triggerSF_option = central_or_shift.empty() ? TriggerSFsys::central : getTriggerSF_option(central_or_shift);
    if(weights_leptonTriggerEff_.count(triggerSF_option))
    {
      return weights_leptonTriggerEff_.at(triggerSF_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_leptonSF() const
{
  return isMC_ ? leptonSF_ : 1.;
}

double
EvtWeightRecorder::get_chargeMisIdProb() const
{
  return chargeMisIdProb_;
}

double
EvtWeightRecorder::get_data_to_MC_correction(const std::string & central_or_shift) const
{
  return isMC_ ? get_sf_triggerEff(central_or_shift) * get_leptonSF() * get_tauSF(central_or_shift) : 1.;
}

double
EvtWeightRecorder::get_tauSF(const std::string & central_or_shift) const
{
  double tauSF_weight = 1.;
  if(isMC_)
  {
    const TauIDSFsys tauIDSF_option = central_or_shift.empty() ? TauIDSFsys::central : getTauIDSFsys_option(central_or_shift);
    if(weights_hadTauID_and_Iso_.count(tauIDSF_option))
    {
      tauSF_weight *= weights_hadTauID_and_Iso_.at(tauIDSF_option);
    }
    const FRet eToTauFakeRate_option = central_or_shift.empty() ? FRet::central : getEToTauFR_option(central_or_shift);
    if(weights_eToTauFakeRate_.count(eToTauFakeRate_option))
    {
      tauSF_weight *= weights_eToTauFakeRate_.at(eToTauFakeRate_option);
    }
    const FRmt muToTauFakeRate_option = central_or_shift.empty() ? FRmt::central : getMuToTauFR_option(central_or_shift);
    if(weights_muToTauFakeRate_.count(muToTauFakeRate_option))
    {
      tauSF_weight *= weights_muToTauFakeRate_.at(muToTauFakeRate_option);
    }
    const int jetToTauFakeRate_option = central_or_shift.empty() ? kFRjt_central : getJetToTauFR_option(central_or_shift);
    if(weights_SF_hadTau_lead_.count(jetToTauFakeRate_option))
    {
      tauSF_weight *= weights_SF_hadTau_lead_.at(jetToTauFakeRate_option);
    }
  }
  return tauSF_weight;
}

double
EvtWeightRecorder::get_FR(const std::string & central_or_shift) const
{
  const int jetToLeptonFakeRate_option = central_or_shift.empty() ? kFRl_central : getJetToLeptonFR_option(central_or_shift);
  const int jetToTauFakeRate_option = central_or_shift.empty() ? kFRjt_central : getJetToTauFR_option(central_or_shift);
  const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
  if(weights_FR_.count(weightKey))
  {
    return weights_FR_.at(weightKey);
  }
  return 1.;
}

void
EvtWeightRecorder::record_genWeight(double genWeight)
{
  assert(isMC_);
  genWeight_ = genWeight;
}

void
EvtWeightRecorder::record_auxWeight(const EvtWeightManager * const evtWeightManager)
{
  assert(isMC_);
  auxWeight_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    auxWeight_[central_or_shift] = evtWeightManager->has_central_or_shift(central_or_shift) ?
      evtWeightManager->getWeight(central_or_shift) :
      evtWeightManager->getWeight()
    ;
  }
}

void
EvtWeightRecorder::record_lumiScale(const edm::VParameterSet & lumiScales)
{
  assert(isMC_);
  lumiScale_.clear();
  for(const edm::ParameterSet & lumiScale: lumiScales)
  {
    const std::string central_or_shift = lumiScale.getParameter<std::string>("central_or_shift");
    const double nof_events = lumiScale.getParameter<double>("lumi");
    lumiScale_[central_or_shift] = nof_events;
  }
}

void
EvtWeightRecorder::record_nom_tH_weight(const EventInfo * const eventInfo)
{
  assert(isMC_);
  nom_tH_weight_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    nom_tH_weight_[central_or_shift] = eventInfo->has_central_or_shift(central_or_shift) ?
      eventInfo->genWeight_tH(central_or_shift) :
      eventInfo->genWeight_tH()
    ;
  }
}

void
EvtWeightRecorder::record_leptonSF(double weight)
{
  assert(isMC_);
  leptonSF_ *= weight;
}

void
EvtWeightRecorder::record_chargeMisIdProb(double weight)
{
  chargeMisIdProb_ = weight;
}

void
EvtWeightRecorder::record_l1PrefireWeight(const L1PreFiringWeightReader * const l1PreFiringWeightReader)
{
  assert(isMC_);
  weights_l1PreFiring_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const L1PreFiringWeightSys l1PreFire_option = getL1PreFiringWeightSys_option(central_or_shift);
    if(weights_l1PreFiring_.count(l1PreFire_option))
    {
      continue;
    }
    weights_l1PreFiring_[l1PreFire_option] = l1PreFiringWeightReader->getWeight(l1PreFire_option);
  }
}

void
EvtWeightRecorder::record_lheScaleWeight(const LHEInfoReader * const lheInfoReader)
{
  assert(isMC_);
  weights_lheScale_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int lheScale_option = getLHEscale_option(central_or_shift);
    if(weights_lheScale_.count(lheScale_option))
    {
      continue;
    }
    weights_lheScale_[lheScale_option] = lheInfoReader->getWeight_scale(lheScale_option);
  }
}

void
EvtWeightRecorder::record_puWeight(const EventInfo * const eventInfo)
{
  assert(isMC_);
  weights_pu_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const PUsys puSys_option = getPUsys_option(central_or_shift);
    if(weights_pu_.count(puSys_option))
    {
      continue;
    }
    switch(puSys_option)
    {
      case PUsys::central: weights_pu_[puSys_option] = eventInfo->pileupWeight;     break;
      case PUsys::up:      weights_pu_[puSys_option] = eventInfo->pileupWeightUp;   break;
      case PUsys::down:    weights_pu_[puSys_option] = eventInfo->pileupWeightDown; break;
      default: assert(0);
    }
  }
}

void
EvtWeightRecorder::record_btagWeight(const std::vector<const RecoJet *> & jets)
{
  assert(isMC_);
  weights_btag_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetBtagSF_option = getBTagWeight_option(central_or_shift);
    if(weights_btag_.count(jetBtagSF_option))
    {
      continue;
    }
    weights_btag_[jetBtagSF_option] = get_BtagWeight(jets, jetBtagSF_option);
  }
}

void
EvtWeightRecorder::record_leptonTriggerEff(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface)
{
  assert(isMC_);
  weights_leptonTriggerEff_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const TriggerSFsys triggerSF_option = getTriggerSF_option(central_or_shift);
    if(weights_leptonTriggerEff_.count(triggerSF_option))
    {
      continue;
    }
    weights_leptonTriggerEff_[triggerSF_option] = dataToMCcorrectionInterface->getSF_leptonTriggerEff(triggerSF_option);
  }
}

void
EvtWeightRecorder::record_hadTauID_and_Iso(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface)
{
  assert(isMC_);
  weights_hadTauID_and_Iso_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const TauIDSFsys tauIDSF_option = getTauIDSFsys_option(central_or_shift);
    if(weights_hadTauID_and_Iso_.count(tauIDSF_option))
    {
      continue;
    }
    weights_hadTauID_and_Iso_[tauIDSF_option] = dataToMCcorrectionInterface->getSF_hadTauID_and_Iso(tauIDSF_option);
  }
}

void
EvtWeightRecorder::record_eToTauFakeRate(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface)
{
  assert(isMC_);
  weights_eToTauFakeRate_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const FRet eToTauFakeRate_option = getEToTauFR_option(central_or_shift);
    if(weights_eToTauFakeRate_.count(eToTauFakeRate_option))
    {
      continue;
    }
    weights_eToTauFakeRate_[eToTauFakeRate_option] = dataToMCcorrectionInterface->getSF_eToTauFakeRate(eToTauFakeRate_option);
  }
}

void
EvtWeightRecorder::record_muToTauFakeRate(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface)
{
  assert(isMC_);
  weights_muToTauFakeRate_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const FRmt muToTauFakeRate_option = getMuToTauFR_option(central_or_shift);
    if(weights_muToTauFakeRate_.count(muToTauFakeRate_option))
    {
      continue;
    }
    weights_muToTauFakeRate_[muToTauFakeRate_option] = dataToMCcorrectionInterface->getSF_muToTauFakeRate(muToTauFakeRate_option);
  }
}

void
EvtWeightRecorder::record_jetToTau_FR_lead(const JetToTauFakeRateInterface * const jetToTauFakeRateInterface,
                                           const RecoHadTau * const hadTau_lead)
{
  assert(isMC_);
  const double hadTauPt_lead = hadTau_lead->pt();
  const double hadTauAbsEta_lead = hadTau_lead->absEta();
  weights_FR_hadTau_lead_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    if(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option))
    {
      continue;
    }
    weights_FR_hadTau_lead_[jetToTauFakeRate_option] = jetToTauFakeRateInterface->getWeight_lead(
      hadTauPt_lead, hadTauAbsEta_lead, jetToTauFakeRate_option
    );
  }
}

void
EvtWeightRecorder::record_jetToTau_SF_lead(const JetToTauFakeRateInterface * const jetToTauFakeRateInterface,
                                           const RecoHadTau * const hadTau_lead)
{
  assert(isMC_);
  const double hadTauPt_lead = hadTau_lead->pt();
  const double hadTauAbsEta_lead = hadTau_lead->absEta();
  weights_SF_hadTau_lead_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    if(weights_SF_hadTau_lead_.count(jetToTauFakeRate_option))
    {
      continue;
    }
    weights_SF_hadTau_lead_[jetToTauFakeRate_option] = jetToTauFakeRateInterface->getSF_lead(
      hadTauPt_lead, hadTauAbsEta_lead, jetToTauFakeRate_option
    );
  }
}

void
EvtWeightRecorder::record_jetToLepton_FR(const LeptonFakeRateInterface * const leptonFakeRateInterface,
                                         const RecoLepton * const lepton,
                                         std::map<int, double> & weights_FR_lepton)
{
  const int leptonPdgId = std::abs(lepton->pdgId());
  const double leptonPt = lepton->cone_pt();
  const double leptonAbsEta = lepton->absEta();
  assert(leptonPdgId == 11 || leptonPdgId == 13);
  weights_FR_lepton.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    if(weights_FR_lepton.count(jetToLeptonFakeRate_option))
    {
      continue;
    }
    if(leptonPdgId == 11)
    {
      int jetToLeptonFakeRate_option_e = jetToLeptonFakeRate_option;
      if(jetToLeptonFakeRate_option_e >= kFRm_shape_ptUp && jetToLeptonFakeRate_option_e <= kFRm_shape_eta_barrelDown)
      {
        jetToLeptonFakeRate_option_e = kFRl_central;
      }
      weights_FR_lepton[jetToLeptonFakeRate_option] = leptonFakeRateInterface->getWeight_e(
        leptonPt, leptonAbsEta, jetToLeptonFakeRate_option_e
      );
    }
    else if(leptonPdgId == 13)
    {
      int jetToLeptonFakeRate_option_m = jetToLeptonFakeRate_option;
      if(jetToLeptonFakeRate_option_m >= kFRe_shape_ptUp && jetToLeptonFakeRate_option_m <= kFRe_shape_eta_barrelDown)
      {
        jetToLeptonFakeRate_option_m = kFRl_central;
      }
      weights_FR_lepton[jetToLeptonFakeRate_option] = leptonFakeRateInterface->getWeight_mu(
        leptonPt, leptonAbsEta, jetToLeptonFakeRate_option_m
      );
    }
    else
    {
      throw cmsException(this, __func__, __LINE__) << "Invalid PDG ID: " << leptonPdgId;
    }
  }
}

void
EvtWeightRecorder::record_jetToLepton_FR_lead(const LeptonFakeRateInterface * const leptonFakeRateInterface,
                                              const RecoLepton * const lepton_lead)
{
  record_jetToLepton_FR(leptonFakeRateInterface, lepton_lead, weights_FR_lepton_lead_);
}

void
EvtWeightRecorder::record_jetToLepton_FR_sublead(const LeptonFakeRateInterface * const leptonFakeRateInterface,
                                                 const RecoLepton * const lepton_sublead)
{
  record_jetToLepton_FR(leptonFakeRateInterface, lepton_sublead, weights_FR_lepton_sublead_);
}

void
EvtWeightRecorder::compute_FR_2l1tau(bool passesTight_lepton_lead,
                                     bool passesTight_lepton_sublead,
                                     bool passesTight_hadTau)
{
  assert(! weights_FR_lepton_lead_.empty());
  assert(! weights_FR_lepton_sublead_.empty());
  assert(! weights_FR_hadTau_lead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_sublead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_3L(
      weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option),    passesTight_lepton_lead,
      weights_FR_lepton_sublead_.at(jetToLeptonFakeRate_option), passesTight_lepton_sublead,
      weights_FR_hadTau_lead_.at(jetToLeptonFakeRate_option),    passesTight_hadTau
    );
  }
}

void
EvtWeightRecorder::compute_FR_2l(bool passesTight_lepton_lead,
                                 bool passesTight_lepton_sublead)
{
  assert(! weights_FR_lepton_lead_.empty());
  assert(! weights_FR_lepton_sublead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_sublead_.count(jetToLeptonFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_2L(
      weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option),    passesTight_lepton_lead,
      weights_FR_lepton_sublead_.at(jetToLeptonFakeRate_option), passesTight_lepton_sublead
    );
  }
}

void
EvtWeightRecorder::compute_FR_1tau()
{
  assert(! weights_FR_lepton_lead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option));
    const std::string weightKey = jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_1L(weights_FR_lepton_lead_.at(jetToTauFakeRate_option));
  }
}
