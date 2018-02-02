#ifndef tthAnalysis_HiggsToTauTau_EvtHistManager_1l_1tau_h
#define tthAnalysis_HiggsToTauTau_EvtHistManager_1l_1tau_h

/** \class EvtHistManager_1l_1tau
 *
 * Book and fill histograms for event-level quantities in ttH, H->tautau analysis
 * in 1l_1tau category
 *
 * \author Christian Veelken, Tallinn
 *
 */

#include "tthAnalysis/HiggsToTauTau/interface/HistManagerBase.h" // HistManagerBase

class EvtHistManager_1l_1tau
  : public HistManagerBase
{
public:
  EvtHistManager_1l_1tau(const edm::ParameterSet & cfg);
  ~EvtHistManager_1l_1tau() {}

  /// book and fill histograms
  void
  bookHistograms(TFileDirectory & dir) override;

  void
  fillHistograms(int numElectrons,
                 int numMuons,
                 int numHadTaus,
                 int numJets,
                 int numBJets_loose,
                 int numBJets_medium,
                 double mvaOutput_1l_1tau_ttbar,
                 double mTauTauVis,
                 double mTauTau,
                 double evtWeight);

  const TH1 *
  getHistogram_EventCounter() const;

private:
  TH1 * histogram_numElectrons_;
  TH1 * histogram_numMuons_;
  TH1 * histogram_numHadTaus_;
  TH1 * histogram_numJets_;
  TH1 * histogram_numBJets_loose_;
  TH1 * histogram_numBJets_medium_;

  // CV: used to check loss in signal efficiency in case events with
  // high jet and b-jet multiplicity are vetoed to avoid overlap with ttH, H->bb analysis
  // (alternative: ttH, H->bb analysis adds hadronic tau veto)
  TH2 * histogram_numBJets_loose_vs_numJets_;
  TH2 * histogram_numBJets_medium_vs_numJets_;

  TH1 * histogram_mvaOutput_1l_1tau_ttbar_;

  TH1 * histogram_mTauTauVis_;
  TH1 * histogram_mTauTau_;

  TH1 * histogram_EventCounter_;
};

#endif
