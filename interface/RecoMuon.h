#ifndef tthAnalysis_HiggsToTauTau_RecoMuon_h
#define tthAnalysis_HiggsToTauTau_RecoMuon_h

#include "tthAnalysis/HiggsToTauTau/interface/RecoLepton.h" // RecoLepton

class RecoMuon
  : public RecoLepton
{
public:
  RecoMuon() = default;
  RecoMuon(const RecoLepton & lepton,
           Bool_t passesLooseIdPOG,
           Bool_t passesMediumIdPOG,
           Float_t segmentCompatibility);

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */
  Bool_t
  passesLooseIdPOG() const;

  Bool_t
  passesMediumIdPOG() const;

  Float_t
  segmentCompatibility() const;

  /**
   * @brief Checks whether a given lepton is an electron by its PDG id
   * @return True if it is an electron; false otherwise
   */
  bool
  is_electron() const override;

  /**
   * @brief Checks whether a given lepton is a muon by its PDG id
   * @return True if it is a muon; false otherwise
   */
  bool
  is_muon() const override;

//--- observables specific to muons
  Bool_t passesLooseIdPOG_;      ///< flag indicating if muon passes (true) or fails (false) loose PFMuon id
  Bool_t passesMediumIdPOG_;     ///< flag indicating if muon passes (true) or fails (false) medium PFMuon id
  Float_t segmentCompatibility_; ///< muon segment compatibility
};

std::ostream &
operator<<(std::ostream & stream,
           const RecoMuon & muon);

#endif // tthAnalysis_HiggsToTauTau_RecoMuon_h
