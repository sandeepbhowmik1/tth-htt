#include "tthAnalysis/HiggsToTauTau/interface/EvtHistManager_ttZctrl.h"

#include "FWCore/Utilities/interface/Exception.h" // cms::Exception

#include "tthAnalysis/HiggsToTauTau/interface/histogramAuxFunctions.h" // fillWithOverFlow, fillWithOverFlow2d, getLogWeight
#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // kEra_2015, kEra_2016

#include <TMath.h>

#include <assert.h> // assert

EvtHistManager_ttZctrl::EvtHistManager_ttZctrl(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  std::string era_string = cfg.getParameter<std::string>("era");
  era_ = -1;
  if      ( era_string == "2015" ) era_ = kEra_2015;
  else if ( era_string == "2016" ) era_ = kEra_2016;
  else throw cms::Exception("EvtHistManager_ttZctrl") 
    << "Invalid Configuration parameter 'era' = " << era_string << " !!\n";
}

void EvtHistManager_ttZctrl::bookHistograms(TFileDirectory& dir)
{
  histogram_numElectrons_ = book1D(dir, "numElectrons", "numElectrons", 5, -0.5, +4.5);
  histogram_numMuons_ = book1D(dir, "numMuons", "numMuons", 5, -0.5, +4.5);
  histogram_numLeptons_ = book1D(dir, "numLeptons", "numLeptons", 5, -0.5, +4.5);
  histogram_numHadTaus_ = book1D(dir, "numHadTaus", "numHadTaus", 5, -0.5, +4.5);
  histogram_numJets_ = book1D(dir, "numJets", "numJets", 20, -0.5, +19.5);
  histogram_numBJets_loose_ = book1D(dir, "numBJets_loose", "numBJets_loose", 10, -0.5, +9.5);
  histogram_numBJets_medium_ = book1D(dir, "numBJets_medium", "numBJets_medium", 10, -0.5, +9.5);

  histogram_mvaOutput_2lss_ttV_ = book1D(dir, "mvaOutput_2lss_ttV", "mvaOutput_2lss_ttV", 40, -1., +1.);
  histogram_mvaOutput_2lss_ttbar_ = book1D(dir, "mvaOutput_2lss_ttbar", "mvaOutput_2lss_ttbar", 40, -1., +1.);
  if      ( era_ == kEra_2015 ) histogram_mvaDiscr_2lss_ = book1D(dir, "mvaDiscr_2lss", "mvaDiscr_2lss", 6, 0.5, 6.5);
  else if ( era_ == kEra_2016 ) histogram_mvaDiscr_2lss_ = book1D(dir, "mvaDiscr_2lss", "mvaDiscr_2lss", 7, 0.5, 7.5);
  else assert(0);

  histogram_mvaOutput_3l_ttV_ = book1D(dir, "mvaOutput_3l_ttV", "mvaOutput_3l_ttV", 40, -1., +1.);
  histogram_mvaOutput_3l_ttbar_  = book1D(dir, "mvaOutput_3l_ttbar", "mvaOutput_3l_ttbar", 40, -1., +1.);
  if      ( era_ == kEra_2015 ) histogram_mvaDiscr_3l_  = book1D(dir, "mvaDiscr_3l", "mvaDiscr_3l", 3, 0.5, 3.5);
  else if ( era_ == kEra_2016 ) histogram_mvaDiscr_3l_  = book1D(dir, "mvaDiscr_3l", "mvaDiscr_3l", 5, 0.5, 5.5);
  else assert(0);

  histogram_mvaOutput_2lss_1tau_ttV_ = book1D(dir, "mvaOutput_2lss_1tau_ttV", "mvaOutput_2lss_1tau_ttV", 40, -1., +1.);
  histogram_mvaOutput_2lss_1tau_ttbar_ = book1D(dir, "mvaOutput_2lss_1tau_ttbar", "mvaOutput_2lss_1tau_ttbar", 40, -1., +1.);

  histogram_mLL_ = book1D(dir, "mLL", "mLL", 30, 60., 120.);

  histogram_mT_ = book1D(dir, "mT", "mT", 40, 0., 200.);

  histogram_sumLeptonCharge_ = book1D(dir, "sumLeptonCharge", "sumLeptonCharge", 7, -3., +3.);

  histogram_EventCounter_ = book1D(dir, "EventCounter", "EventCounter", 1, -0.5, +0.5);
}

void EvtHistManager_ttZctrl::fillHistograms(int numElectrons, int numMuons, int numHadTaus, int numJets, int numBJets_loose, int numBJets_medium, 
					    double mvaOutput_2lss_ttV, double mvaOutput_2lss_ttbar, double mvaDiscr_2lss,
					    double mvaOutput_3l_ttV, double mvaOutput_3l_ttbar, double mvaDiscr_3l,
					    double mvaOutput_2lss_1tau_ttV, double mvaOutput_2lss_1tau_ttbar,
					    double mLL, double mT, int sumLeptonCharge, double evtWeight)
{
  double evtWeightErr = 0.;

  fillWithOverFlow(histogram_numElectrons_, numElectrons, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_numMuons_, numMuons, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_numLeptons_, numElectrons + numMuons, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_numHadTaus_, numHadTaus, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_numJets_, numJets, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_numBJets_loose_, numBJets_loose, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_numBJets_medium_, numBJets_medium, evtWeight, evtWeightErr);

  fillWithOverFlow(histogram_mvaOutput_2lss_ttV_, mvaOutput_2lss_ttV, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaOutput_2lss_ttbar_, mvaOutput_2lss_ttbar, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaDiscr_2lss_, mvaDiscr_2lss, evtWeight, evtWeightErr);

  fillWithOverFlow(histogram_mvaOutput_3l_ttV_, mvaOutput_3l_ttV, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaOutput_3l_ttbar_, mvaOutput_3l_ttbar, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaDiscr_3l_, mvaDiscr_3l, evtWeight, evtWeightErr);

  fillWithOverFlow(histogram_mvaOutput_2lss_1tau_ttV_, mvaOutput_2lss_1tau_ttV, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaOutput_2lss_1tau_ttbar_, mvaOutput_2lss_1tau_ttbar, evtWeight, evtWeightErr);

  fillWithOverFlow(histogram_mLL_, mLL, evtWeight, evtWeightErr);

  fillWithOverFlow(histogram_mT_, mT, evtWeight, evtWeightErr);

  fillWithOverFlow(histogram_sumLeptonCharge_, sumLeptonCharge, evtWeight, evtWeightErr);

  fillWithOverFlow(histogram_EventCounter_, 0., evtWeight, evtWeightErr);
}
