#include "tthAnalysis/HiggsToTauTau/interface/Data_to_MC_CorrectionInterface_1l_2tau_trigger.h"

#include "tthAnalysis/HiggsToTauTau/interface/leptonTypes.h" // kElectron, kMuon
#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // kEra_2017
#include "tthAnalysis/HiggsToTauTau/interface/cmsException.h" // cmsException()
#include "tthAnalysis/HiggsToTauTau/interface/data_to_MC_corrections_auxFunctions.h" // aux::
#include "tthAnalysis/HiggsToTauTau/interface/sysUncertOptions.h" // TriggerSFsys, getTriggerSF_option()

#include "TauAnalysisTools/TauTriggerSFs/interface/TauTriggerSFs2017.h" // TauTriggerSFs2017

#include <TString.h> // Form()
#include <TMath.h> // TMath::Abs()

#include <boost/algorithm/string/predicate.hpp> // boost::ends_with()

#include <cassert> // assert()

Data_to_MC_CorrectionInterface_1l_2tau_trigger::Data_to_MC_CorrectionInterface_1l_2tau_trigger(const edm::ParameterSet & cfg)
  : effTrigger_1e1tau_tauLeg_(nullptr)
  , effTrigger_1m1tau_tauLeg_(nullptr)
  , era_str_(cfg.getParameter<std::string>("era"))
  , era_(get_era(era_str_))
  , hadTauSelection_(cfg.getParameter<std::string>("hadTauSelection"))
  , isDEBUG_(cfg.exists("isDEBUG") ? cfg.getParameter<bool>("isDEBUG") : false)
  , triggerSF_option_(getTriggerSF_option(cfg.getParameter<std::string>("central_or_shift")))
  , lepton_type_(-1)
  , lepton_pt_(0.)
  , lepton_eta_(0.)
  , hadTau1_genPdgId_(0)
  , hadTau1_pt_(0.)
  , hadTau1_eta_(0.)
  , hadTau1_phi_(0.)
  , hadTau1_decayMode_(0)
  , hadTau2_genPdgId_(0)
  , hadTau2_pt_(0.)
  , hadTau2_eta_(0.)
  , hadTau2_phi_(0.)
  , hadTau2_decayMode_(0)
{
  if(era_ == kEra_2016)
  {
    aux::loadTriggerEff_1e_2016(effTrigger_1e_data_, effTrigger_1e_mc_, inputFiles_);
    aux::loadTriggerEff_1e_1tau_lepLeg_2016(effTrigger_1e1tau_lepLeg_data_, effTrigger_1e1tau_lepLeg_mc_, inputFiles_);

    aux::loadTriggerEff_1m_2016(effTrigger_1m_data_, effTrigger_1m_mc_, inputFiles_);
    aux::loadTriggerEff_1m_1tau_lepLeg_2016(effTrigger_1m1tau_lepLeg_data_, effTrigger_1m1tau_lepLeg_mc_, inputFiles_);
  }
  else if(era_ == kEra_2017)
  {
    aux::loadTriggerEff_1e_2017(effTrigger_1e_data_, effTrigger_1e_mc_, inputFiles_);
    aux::loadTriggerEff_1e_1tau_lepLeg_2017(effTrigger_1e1tau_lepLeg_data_, effTrigger_1e1tau_lepLeg_mc_, inputFiles_);

    aux::loadTriggerEff_1m_2017(effTrigger_1m_data_, effTrigger_1m_mc_, inputFiles_);
    aux::loadTriggerEff_1m_1tau_lepLeg_2017(effTrigger_1m1tau_lepLeg_data_, effTrigger_1m1tau_lepLeg_mc_, inputFiles_);
  }
  else if(era_ == kEra_2018)
  {
    aux::loadTriggerEff_1e_2018(effTrigger_1e_data_, effTrigger_1e_mc_, inputFiles_);
    aux::loadTriggerEff_1e_1tau_lepLeg_2018(effTrigger_1e1tau_lepLeg_data_, effTrigger_1e1tau_lepLeg_mc_, inputFiles_);

    aux::loadTriggerEff_1m_2018(effTrigger_1m_data_, effTrigger_1m_mc_, inputFiles_);
    aux::loadTriggerEff_1m_1tau_lepLeg_2018(effTrigger_1m1tau_lepLeg_data_, effTrigger_1m1tau_lepLeg_mc_, inputFiles_);
  }
  else
  {
    throw cmsException(this) << "Invalid era = " << era_;
  }

  const LocalFileInPath inputFileName_tauLeg(Form(
    "TauAnalysisTools/TauTriggerSFs/data/tauTriggerEfficiencies%s.root", era_str_.data()
  ));
  const std::string hadTauSelection_TauTriggerSFs2017 = aux::getHadTauSelectionLabel(hadTauSelection_);
  const std::string wpType = "MVAv2";
  effTrigger_1m1tau_tauLeg_ = new TauTriggerSFs2017(
    inputFileName_tauLeg.fullPath(), "mutau", era_str_, hadTauSelection_TauTriggerSFs2017, wpType
  );
  effTrigger_1e1tau_tauLeg_ = new TauTriggerSFs2017(
    inputFileName_tauLeg.fullPath(), "etau", era_str_, hadTauSelection_TauTriggerSFs2017, wpType
  );
}

Data_to_MC_CorrectionInterface_1l_2tau_trigger::~Data_to_MC_CorrectionInterface_1l_2tau_trigger()
{
  aux::clearCollection(effTrigger_1e_data_);
  aux::clearCollection(effTrigger_1e_mc_);
  aux::clearCollection(effTrigger_1e1tau_lepLeg_data_);
  aux::clearCollection(effTrigger_1e1tau_lepLeg_mc_);

  aux::clearCollection(effTrigger_1m_data_);
  aux::clearCollection(effTrigger_1m_mc_);
  aux::clearCollection(effTrigger_1m1tau_lepLeg_data_);
  aux::clearCollection(effTrigger_1m1tau_lepLeg_mc_);

  delete effTrigger_1e1tau_tauLeg_;
  delete effTrigger_1m1tau_tauLeg_;
}

void
Data_to_MC_CorrectionInterface_1l_2tau_trigger::setTriggerBits(bool isTriggered_1e,
                                                               bool isTriggered_1e1tau,
                                                               bool isTriggered_1m,
                                                               bool isTriggered_1m1tau)
{
  isTriggered_1e_     = isTriggered_1e;
  isTriggered_1e1tau_ = isTriggered_1e1tau;
  isTriggered_1m_     = isTriggered_1m;
  isTriggered_1m1tau_ = isTriggered_1m1tau;
}

void
Data_to_MC_CorrectionInterface_1l_2tau_trigger::setLeptons(int lepton_type,
                                                           double lepton_pt,
                                                           double lepton_eta)
{
  lepton_type_ = lepton_type;
  lepton_pt_   = lepton_pt;
  lepton_eta_  = lepton_eta;
}

void
Data_to_MC_CorrectionInterface_1l_2tau_trigger::setHadTaus(double hadTau1_pt, double hadTau1_eta, double hadTau1_phi, int hadTau1_decayMode,
                                                           double hadTau2_pt, double hadTau2_eta, double hadTau2_phi, int hadTau2_decayMode)
{
  hadTau1_pt_        = hadTau1_pt;
  hadTau1_eta_       = hadTau1_eta;
  hadTau1_phi_       = hadTau1_phi;
  hadTau1_decayMode_ = hadTau1_decayMode;

  hadTau2_pt_        = hadTau2_pt;
  hadTau2_eta_       = hadTau2_eta;
  hadTau2_phi_       = hadTau2_phi;
  hadTau2_decayMode_ = hadTau2_decayMode;
}

double
Data_to_MC_CorrectionInterface_1l_2tau_trigger::getWeight_triggerEff() const
{
  assert(0);
}

double
Data_to_MC_CorrectionInterface_1l_2tau_trigger::getSF_triggerEff() const
{
  if(isDEBUG_)
  {
    std::cout << get_human_line(this, __func__, __LINE__) << '\n';
  }

  double eff_1l_data             = 0.;
  double eff_1l_mc               = 0.;
  double eff_1l1tau_lepLeg_data  = 0.;
  double eff_1l1tau_lepLeg_mc    = 0.;
  double eff_1l1tau_tauLeg1_data = 0.;
  double eff_1l1tau_tauLeg1_mc   = 0.;
  double eff_1l1tau_tauLeg2_data = 0.;
  double eff_1l1tau_tauLeg2_mc   = 0.;

  bool isTriggered_1l     = false;
  bool isTriggered_1l1tau = false;

  const auto getTriggerEfficiencyDataFunc = aux::getTriggerFuncData(triggerSF_option_);
  const auto getTriggerEfficiencyMCFunc   = aux::getTriggerFuncMC(triggerSF_option_);
  assert(getTriggerEfficiencyDataFunc);
  assert(getTriggerEfficiencyMCFunc);

  if(lepton_type_ == kElectron)
  {
    if(isDEBUG_)
    {
      std::cout << "electron: pT = " << lepton_pt_ << ", eta = " << lepton_eta_ << '\n';
    }
    eff_1l_data            = get_from_lut(effTrigger_1e_data_,            lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l_mc              = get_from_lut(effTrigger_1e_mc_,              lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_data = get_from_lut(effTrigger_1e1tau_lepLeg_data_, lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_mc   = get_from_lut(effTrigger_1e1tau_lepLeg_mc_,   lepton_pt_, lepton_eta_, isDEBUG_);

    eff_1l1tau_tauLeg1_data = 0.;
    eff_1l1tau_tauLeg1_mc = 0.;
    if(std::fabs(hadTau1_eta_) <= 2.1)
    {
      eff_1l1tau_tauLeg1_data = (effTrigger_1e1tau_tauLeg_->*getTriggerEfficiencyDataFunc)(hadTau1_pt_, hadTau1_eta_, hadTau1_phi_, hadTau1_decayMode_);
      eff_1l1tau_tauLeg1_mc   = (effTrigger_1e1tau_tauLeg_->*getTriggerEfficiencyMCFunc)  (hadTau1_pt_, hadTau1_eta_, hadTau1_phi_, hadTau1_decayMode_);
    }

    eff_1l1tau_tauLeg2_data = 0.;
    eff_1l1tau_tauLeg2_mc = 0.;
    if(std::fabs(hadTau2_eta_) <= 2.1)
    {
      eff_1l1tau_tauLeg2_data = (effTrigger_1e1tau_tauLeg_->*getTriggerEfficiencyDataFunc)(hadTau2_pt_, hadTau2_eta_, hadTau2_phi_, hadTau2_decayMode_);
      eff_1l1tau_tauLeg2_mc   = (effTrigger_1e1tau_tauLeg_->*getTriggerEfficiencyMCFunc)  (hadTau2_pt_, hadTau2_eta_, hadTau2_phi_, hadTau2_decayMode_);
    }

    isTriggered_1l     = isTriggered_1e_;
    isTriggered_1l1tau = isTriggered_1e1tau_;
  }
  else if(lepton_type_ == kMuon)
  {
    if(isDEBUG_)
    {
      std::cout << "muon: pT = " << lepton_pt_ << ", eta = " << lepton_eta_ << '\n';
    }
    eff_1l_data            = get_from_lut(effTrigger_1m_data_,            lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l_mc              = get_from_lut(effTrigger_1m_mc_,              lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_data = get_from_lut(effTrigger_1m1tau_lepLeg_data_, lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_mc   = get_from_lut(effTrigger_1m1tau_lepLeg_mc_,   lepton_pt_, lepton_eta_, isDEBUG_);

    eff_1l1tau_tauLeg1_data = 0.;
    eff_1l1tau_tauLeg1_mc = 0.;
    if(std::fabs(hadTau1_eta_) <= 2.1)
    {
      eff_1l1tau_tauLeg1_data = (effTrigger_1m1tau_tauLeg_->*getTriggerEfficiencyDataFunc)(hadTau1_pt_, hadTau1_eta_, hadTau1_phi_, hadTau1_decayMode_);
      eff_1l1tau_tauLeg1_mc   = (effTrigger_1m1tau_tauLeg_->*getTriggerEfficiencyMCFunc)  (hadTau1_pt_, hadTau1_eta_, hadTau1_phi_, hadTau1_decayMode_);
    }

    eff_1l1tau_tauLeg2_data = 0.;
    eff_1l1tau_tauLeg2_mc = 0.;
    if(std::fabs(hadTau2_eta_) <= 2.1)
    {
      eff_1l1tau_tauLeg2_data = (effTrigger_1m1tau_tauLeg_->*getTriggerEfficiencyDataFunc)(hadTau2_pt_, hadTau2_eta_, hadTau2_phi_, hadTau2_decayMode_);
      eff_1l1tau_tauLeg2_mc   = (effTrigger_1m1tau_tauLeg_->*getTriggerEfficiencyMCFunc)  (hadTau2_pt_, hadTau2_eta_, hadTau2_phi_, hadTau2_decayMode_);
    }

    isTriggered_1l     = isTriggered_1m_;
    isTriggered_1l1tau = isTriggered_1m1tau_;
  }
  else
  {
    assert(0);
  }

  const double eff_1l1tau_tauLegs_data = 1. - (1. - eff_1l1tau_tauLeg1_data) * (1. - eff_1l1tau_tauLeg2_data);
  const double eff_1l1tau_tauLegs_mc   = 1. - (1. - eff_1l1tau_tauLeg1_mc)   * (1. - eff_1l1tau_tauLeg2_mc);
  if(isDEBUG_)
  {
    std::cout << "hadTau (lead):    pT = " << hadTau1_pt_ << ", eta = " << hadTau1_eta_ << "\n"
                 "hadTau (sublead): pT = " << hadTau2_pt_ << ", eta = " << hadTau2_eta_ << "\n"
                 "eff (L):      data = " << eff_1l_data             << ", MC = " << eff_1l_mc             << "\n"
                 "eff (X_lep):  data = " << eff_1l1tau_lepLeg_data  << ", MC = " << eff_1l1tau_lepLeg_mc  << "\n"
                 "eff (X_tau):  data = " << eff_1l1tau_tauLegs_data << ", MC = " << eff_1l1tau_tauLegs_mc << "\n"
                 "eff (X_tau1): data = " << eff_1l1tau_tauLeg1_data << ", MC = " << eff_1l1tau_tauLeg1_mc << "\n"
                 "eff (X_tau2): data = " << eff_1l1tau_tauLeg2_data << ", MC = " << eff_1l1tau_tauLeg2_mc << '\n'
    ;
  }

  //-------------------------------------------------------------------------------------------------------------------
  // CV: data/MC corrections are agreed as discussed on HTT working mailing list
  //     (email from Alexei Raspereza on February 22nd 2017)
  double sf = 1.;
  if(isTriggered_1l && isTriggered_1l1tau)
  {
    // case 4: both single lepton trigger and lepton+tau cross trigger fire

    const double eff_data = std::min(eff_1l_data, eff_1l1tau_lepLeg_data) * eff_1l1tau_tauLegs_data;
    const double eff_mc   = std::min(eff_1l_mc,   eff_1l1tau_lepLeg_mc)   * eff_1l1tau_tauLegs_mc;

    sf = aux::compSF(eff_data, eff_mc);
    if(isDEBUG_)
    {
      std::cout << "case 4: both single lepton trigger and lepton+tau cross trigger fire\n"
                   " eff: data = " << eff_data << ", MC = " << eff_mc << " --> SF = " << sf << '\n';
    }
  }
  else if(isTriggered_1l1tau)
  {
    // case 3: lepton+tau cross trigger fires, single lepton trigger doesn't fire

    const double eff_data = std::max(1.e-2, (eff_1l1tau_lepLeg_data - eff_1l_data) * eff_1l1tau_tauLegs_data);
    const double eff_mc   = std::max(1.e-2, (eff_1l1tau_lepLeg_mc   - eff_1l_mc)   * eff_1l1tau_tauLegs_mc);

    sf = aux::compSF(eff_data, eff_mc);
    if(isDEBUG_)
    {
      std::cout << "case 3: lepton+tau cross trigger fires, single lepton trigger doesn't fire\n"
                   " eff: data = " << eff_data << ", MC = " << eff_mc << " --> SF = " << sf << '\n';
    }
  }
  else if(isTriggered_1l)
  {
    // case 2: single lepton trigger fires, lepton+tau cross trigger doesn't fire

    const double eff_data = std::max(1.e-2, eff_1l_data - eff_1l1tau_tauLegs_data * std::min(eff_1l_data, eff_1l1tau_lepLeg_data));
    const double eff_mc   = std::max(1.e-2, eff_1l_mc   - eff_1l1tau_tauLegs_mc   * std::min(eff_1l_mc,   eff_1l1tau_lepLeg_mc));

    sf = aux::compSF(eff_data, eff_mc);
    if(isDEBUG_)
    {
      std::cout << "case 2: single lepton trigger fires, lepton+tau cross trigger doesn't fire\n"
                   " eff: data = " << eff_data << ", MC = " << eff_mc << " --> SF = " << sf << '\n';
    }
  }
  else
  {
    // case 1: neither single lepton trigger nor lepton+tau cross trigger fires
    // (SF doesn't matter, as event does not pass event selection)

    sf = 0.;
    if(isDEBUG_)
    {
      std::cout << "case 1: neither single lepton trigger nor lepton+tau cross trigger fires\n"
                   "--> setting SF = " << sf << '\n';
    }
  } // isTriggered_*

  return sf;
}
