#ifndef tthAnalysis_HiggsToTauTau_MEMInterface_2lss_1tau_h
#define tthAnalysis_HiggsToTauTau_MEMInterface_2lss_1tau_h

#include "tthAnalysis/HiggsToTauTau/interface/RecoLepton.h" // RecoLepton
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTau.h" // RecoHadTau
#include "tthAnalysis/HiggsToTauTau/interface/RecoJet.h" // RecoJet
#include "tthAnalysis/HiggsToTauTau/interface/MEMOutput_2lss_1tau.h" // MEMOutput_2lss_1tau

#include "ttH_Htautau_MEM_Analysis/MEMAlgo/interface/RunConfig.h" // RunConfig

#include <TMatrixD.h> // TMatrixD
#include <TBenchmark.h> // TBenchmark

#include <vector>
#include <string>

class MEMInterface_2lss_1tau
{
 public:
  MEMInterface_2lss_1tau(const std::string& configFileName);
  ~MEMInterface_2lss_1tau();

  /**
   * @brief Calculates output of MEM integration.
   * @param pointers to leading and subleading lepton, hadronic tau, MET and MET covariance matrix, collections of b-tagged jets and non-b-tagged jets
   * @return object with         MVA output
   */

  
  MEMOutput_2lss_1tau
  operator()(const RecoLepton* selLepton_lead, const RecoLepton* selLepton_sublead, const RecoHadTau* selHadTau, 
	     double metPx, double metPy, const TMatrixD& metCov,
	     const std::vector<const RecoJet*>& selJets);

 private:
  RunConfig* config_;

  TBenchmark* clock_;
};

#endif // MEMInterface_2lss_1tau_h
