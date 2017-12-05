#include "tthAnalysis/HiggsToTauTau/interface/EvtHistManager_1l_2tau.h"

#include "tthAnalysis/HiggsToTauTau/interface/histogramAuxFunctions.h"

#include <TMath.h>

EvtHistManager_1l_2tau::EvtHistManager_1l_2tau(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{}

void EvtHistManager_1l_2tau::bookHistograms(TFileDirectory& dir)
{
  histogram_numElectrons_ = book1D(dir, "numElectrons", "numElectrons", 5, -0.5, +4.5);
  histogram_numMuons_ = book1D(dir, "numMuons", "numMuons", 5, -0.5, +4.5);
  histogram_numHadTaus_ = book1D(dir, "numHadTaus", "numHadTaus", 5, -0.5, +4.5);
  histogram_numJets_ = book1D(dir, "numJets", "numJets", 20, -0.5, +19.5);
  histogram_numBJets_loose_ = book1D(dir, "numBJets_loose", "numBJets_loose", 10, -0.5, +9.5);
  histogram_numBJets_medium_ = book1D(dir, "numBJets_medium", "numBJets_medium", 10, -0.5, +9.5);

  histogram_numBJets_loose_vs_numJets_ = book2D(dir, "numBJets_loose_vs_numJets", "numBJets_loose_vs_numJets", 8, -0.5, +7.5, 6, -0.5, +5.5);
  histogram_numBJets_medium_vs_numJets_ = book2D(dir, "numBJets_medium_vs_numJets", "numBJets_medium_vs_numJets", 8, -0.5, +7.5, 6, -0.5, +5.5);

  histogram_mvaOutput_1l_2tau_ttbar_ = book1D(dir, "mvaOutput_1l_2tau_ttbar", "mvaOutput_1l_2tau_ttbar", 500, 0., +1.);
  histogram_mvaOutput_1l_2tau_ttbar_HTTWithKinFit_MVAonly_ = book1D(dir, "mvaOutput_1l_2tau_ttbar_HTTWithKinFit_MVAonly", "mvaOutput_1l_2tau_ttbar_HTTWithKinFit_MVAonly", 500, 0., +1.);
  histogram_mvaOutput_1l_2tau_ttbar_HTTWithKinFit_ = book1D(dir, "mvaOutput_1l_2tau_ttbar_HTTWithKinFit", "mvaOutput_1l_2tau_ttbar_HTTWithKinFit", 500, 0., +1.);
  histogram_mvaOutput_1l_2tau_ttbar_Old_ = book1D(dir, "mvaOutput_1l_2tau_ttbar_Old", "mvaOutput_1l_2tau_ttbar_Old", 500, -1., +1.);
  histogram_mvaOutput_1l_2tau_ttbar_HTTLepID_ = book1D(dir, "mvaOutput_1l_2tau_ttbar_HTTLepID", "mvaOutput_1l_2tau_ttbar_HTTLepID", 500, 0., +1.);
  histogram_mvaOutput_1l_2tau_ttbar_OldVar_ = book1D(dir, "mvaOutput_1l_2tau_ttbar_OldVar", "mvaOutput_1l_2tau_ttbar_OldVar", 500, -1., +1.);
  histogram_mvaOutput_1l_2tau_ttbar_OldVarHTT_ = book1D(dir, "mvaOutput_1l_2tau_ttbar_OldVarHTT", "mvaOutput_1l_2tau_ttbar_OldVarHTT", 500, -1., +1.);

  histogram_mvaOutput_1l_2tau_ttV_ = book1D(dir, "mvaOutput_1l_2tau_ttV", "mvaOutput_1l_2tau_ttV", 20, -1., +1.);
  histogram_mvaDiscr_1l_2tau_ = book1D(dir, "mvaDiscr_1l_2tau", "mvaDiscr_1l_2tau", 10, 0.5, 10.5);

  histogram_mTauTauVis_ = book1D(dir, "mTauTauVis", "mTauTauVis", 20, 0., 200.);

  histogram_EventCounter_ = book1D(dir, "EventCounter", "EventCounter", 1, -0.5, +0.5);
}

void EvtHistManager_1l_2tau::fillHistograms(int numElectrons, int numMuons, int numHadTaus, int numJets, int numBJets_loose, int numBJets_medium,
  double mvaOutput_1l_2tau_ttbar,
  double mvaOutput_1l_2tau_ttbar_HTTWithKinFit_MVAonly,
  double mvaOutput_1l_2tau_ttbar_HTTWithKinFit,
  double mvaOutput_1l_2tau_ttbar_Old,
  double mvaOutput_1l_2tau_ttbar_HTTLepID,
  double mvaOutput_1l_2tau_ttbar_OldVar,
  double mvaOutput_1l_2tau_ttbar_OldVarHTT,
  double mvaOutput_1l_2tau_ttV, double mvaDiscr_1l_2tau, double mTauTauVis, double evtWeight)
{
  double evtWeightErr = 0.;

  fillWithOverFlow(histogram_numElectrons_, numElectrons, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_numMuons_, numMuons, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_numHadTaus_, numHadTaus, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_numJets_, numJets, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_numBJets_loose_, numBJets_loose, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_numBJets_medium_, numBJets_medium, evtWeight, evtWeightErr);

  fillWithOverFlow2d(histogram_numBJets_loose_vs_numJets_, numJets, numBJets_loose, evtWeight, evtWeightErr);
  fillWithOverFlow2d(histogram_numBJets_medium_vs_numJets_, numJets, numBJets_medium, evtWeight, evtWeightErr);

  fillWithOverFlow(histogram_mvaOutput_1l_2tau_ttbar_, mvaOutput_1l_2tau_ttbar, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaOutput_1l_2tau_ttbar_Old_, mvaOutput_1l_2tau_ttbar_Old, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaOutput_1l_2tau_ttbar_HTTWithKinFit_MVAonly_, mvaOutput_1l_2tau_ttbar_HTTWithKinFit_MVAonly, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaOutput_1l_2tau_ttbar_HTTWithKinFit_, mvaOutput_1l_2tau_ttbar_HTTWithKinFit, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaOutput_1l_2tau_ttbar_HTTLepID_, mvaOutput_1l_2tau_ttbar_HTTLepID, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaOutput_1l_2tau_ttbar_OldVar_, mvaOutput_1l_2tau_ttbar_OldVar, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaOutput_1l_2tau_ttbar_OldVarHTT_, mvaOutput_1l_2tau_ttbar_OldVarHTT, evtWeight, evtWeightErr);

  fillWithOverFlow(histogram_mvaOutput_1l_2tau_ttV_, mvaOutput_1l_2tau_ttV, evtWeight, evtWeightErr);
  fillWithOverFlow(histogram_mvaDiscr_1l_2tau_, mvaDiscr_1l_2tau, evtWeight, evtWeightErr);

  fillWithOverFlow(histogram_mTauTauVis_, mTauTauVis, evtWeight, evtWeightErr);

  fillWithOverFlow(histogram_EventCounter_, 0., evtWeight, evtWeightErr);
}
