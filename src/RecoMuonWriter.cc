#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonWriter.h" // RecoMuonWriter

#include "tthAnalysis/HiggsToTauTau/interface/RecoLeptonWriter.h" // RecoLeptonWriter
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuon.h" // RecoMuon
#include "tthAnalysis/HiggsToTauTau/interface/BranchAddressInitializer.h" // BranchAddressInitializer, TTree, Form()

RecoMuonWriter::RecoMuonWriter(int era)
  : RecoMuonWriter(era, "nMuon", "Muon")
{}

RecoMuonWriter::RecoMuonWriter(int era,
                               const std::string & branchName_num,
                               const std::string & branchName_obj)
  : era_(era)
  , branchName_num_(branchName_num)
  , branchName_obj_(branchName_obj)
  , leptonWriter_(new RecoLeptonWriter(branchName_num_, branchName_obj_))
  , looseIdPOG_(nullptr)
  , mediumIdPOG_(nullptr)
  , segmentCompatibility_(nullptr)
{
  setBranchNames();
}

RecoMuonWriter::~RecoMuonWriter()
{
  delete leptonWriter_;
  delete[] looseIdPOG_;
  delete[] mediumIdPOG_;
  delete[] segmentCompatibility_;
}

void RecoMuonWriter::setBranchNames()
{
  // Karl: let's write the looseIdPOG branch even though we aren't going to read it
  //       in the first place
  branchName_looseIdPOG_ = Form("%s_%s", branchName_obj_.data(), "looseIdPOG");
  branchName_mediumIdPOG_ = Form("%s_%s", branchName_obj_.data(), "mediumId");
  branchName_segmentCompatibility_ = Form("%s_%s", branchName_obj_.data(), "segmentComp");
}

void RecoMuonWriter::setBranches(TTree * tree)
{
  leptonWriter_->setBranches(tree);
  const unsigned int max_nLeptons = leptonWriter_->max_nLeptons_;
  BranchAddressInitializer bai(tree, branchName_num_, max_nLeptons);
  bai.setBranch(looseIdPOG_, branchName_looseIdPOG_);
  bai.setBranch(mediumIdPOG_, branchName_mediumIdPOG_);
  bai.setBranch(segmentCompatibility_, branchName_segmentCompatibility_);
}

void RecoMuonWriter::write(const std::vector<const RecoMuon *> & leptons)
{
  leptonWriter_->write(leptons);
  const Int_t nLeptons = leptons.size();
  for(Int_t idxLepton = 0; idxLepton < nLeptons; ++idxLepton)
  {
    const RecoMuon * lepton = leptons[idxLepton];
    assert(lepton);
    looseIdPOG_[idxLepton] = lepton->passesLooseIdPOG();
    mediumIdPOG_[idxLepton] = lepton->passesMediumIdPOG();
    segmentCompatibility_[idxLepton] = lepton->segmentCompatibility();
  }
}
