#ifndef tthAnalysis_HiggsToTauTau_hltPathWriter_h
#define tthAnalysis_HiggsToTauTau_hltPathWriter_h

#include "tthAnalysis/HiggsToTauTau/interface/hltPath.h" // hltPath

// forward declarations
class TTree;

#include <vector>
#include <string>
#include <map>

class hltPathWriter
{
 public:
  hltPathWriter(const std::vector<std::string>& hltPath_branchNames);

  void
  setBranches(TTree * tree);

  void
  write(const std::vector<hltPath*> & hltPaths);

 protected:
  struct branchEntryType
  {
    std::string branchName_;
    Int_t value_;
    bool value_isInitialized_;
  };
  std::map<std::string, branchEntryType> branches_; // key = branchName
};

#endif // tthAnalysis_HiggsToTauTau_hltPathWriter_h