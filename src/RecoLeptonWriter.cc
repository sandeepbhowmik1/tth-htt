#include "tthAnalysis/HiggsToTauTau/interface/RecoLeptonWriter.h" // RecoLeptonWriter

#include "tthAnalysis/HiggsToTauTau/interface/BranchAddressInitializer.h" // BranchAddressInitializer, TTree, Form()

RecoLeptonWriter::RecoLeptonWriter(const std::string & branchName_num,
                                   const std::string & branchName_obj)
  : max_nLeptons_(32)
  , branchName_num_(branchName_num)
  , branchName_obj_(branchName_obj)
  , genLeptonWriter_(nullptr)
  , genHadTauWriter_(nullptr)
  , genJetWriter_(nullptr)
  , pt_(nullptr)
  , eta_(nullptr)
  , phi_(nullptr)
  , mass_(nullptr)
  , pdgId_(nullptr)
  , dxy_(nullptr)
  , dz_(nullptr)
  , relIso_all_(nullptr)
  , hadRelIso03_chg_(nullptr)
  , absIso_chg_(nullptr)
  , absIso_neu_(nullptr)
  , sip3d_(nullptr)
  , mvaRawTTH_(nullptr)
  , jetPtRatio_(nullptr)
  , jetBtagCSV_(nullptr)
  , tightCharge_(nullptr)
  , charge_(nullptr)
{
  genLeptonWriter_ = new GenParticleWriter(Form("%s_genLepton", branchName_num_.data()), Form("%s_genLepton", branchName_obj_.data()));
  genHadTauWriter_ = new GenParticleWriter(Form("%s_genTau",    branchName_num_.data()), Form("%s_genTau",    branchName_obj_.data()));
  genJetWriter_    = new GenParticleWriter(Form("%s_genJet",    branchName_num_.data()), Form("%s_genJet",    branchName_obj_.data()));
  setBranchNames();
}

RecoLeptonWriter::~RecoLeptonWriter()
{
  delete genLeptonWriter_;
  delete genHadTauWriter_;
  delete genJetWriter_;
  delete pt_;
  delete eta_;
  delete phi_;
  delete mass_;
  delete pdgId_;
  delete dxy_;
  delete dz_;
  delete relIso_all_;
  delete hadRelIso03_chg_;
  delete absIso_chg_;
  delete absIso_neu_;
  delete sip3d_;
  delete mvaRawTTH_;
  delete jetPtRatio_;
  delete jetBtagCSV_;
  delete tightCharge_;
  delete charge_;
}

void RecoLeptonWriter::setBranchNames()
{
  branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
  branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
  branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
  branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
  branchName_pdgId_ = Form("%s_%s", branchName_obj_.data(), "pdgId");
  branchName_dxy_ = Form("%s_%s", branchName_obj_.data(), "dxy");
  branchName_dz_ = Form("%s_%s", branchName_obj_.data(), "dz");
  branchName_relIso_all_ = Form("%s_%s", branchName_obj_.data(), "miniPFRelIso_all");
  branchName_hadRelIso03_chg_ = Form("%s_%s", branchName_obj_.data(), "pfRelIso03_chg");
  branchName_absIso_chg_ = Form("%s_%s", branchName_obj_.data(), "miniPFAbsIso_chg");
  branchName_absIso_neu_ = Form("%s_%s", branchName_obj_.data(), "miniPFAbsIso_neu");
  branchName_sip3d_ = Form("%s_%s", branchName_obj_.data(), "sip3d");
  branchName_mvaRawTTH_ = Form("%s_%s", branchName_obj_.data(), "mvaTTH");
  branchName_jetPtRatio_ = Form("%s_%s", branchName_obj_.data(), "jetPtRatio");
  branchName_jetBtagCSV_ = Form("%s_%s", branchName_obj_.data(), "jetBtag_csvv2");
  branchName_tightCharge_ = Form("%s_%s", branchName_obj_.data(), "tightCharge");
  branchName_charge_ = Form("%s_%s", branchName_obj_.data(), "charge");
}

void RecoLeptonWriter::setBranches(TTree * tree)
{
  genLeptonWriter_->setBranches(tree);
  genHadTauWriter_->setBranches(tree);
  genJetWriter_->setBranches(tree);
  BranchAddressInitializer bai(tree, branchName_num_, max_nLeptons_);
  bai.setBranch(nLeptons_, branchName_num_);
  bai.setBranch(pt_, branchName_pt_);
  bai.setBranch(eta_, branchName_eta_);
  bai.setBranch(phi_, branchName_phi_);
  bai.setBranch(mass_, branchName_mass_);
  bai.setBranch(pdgId_, branchName_pdgId_);
  bai.setBranch(dxy_, branchName_dxy_);
  bai.setBranch(dz_, branchName_dz_);
  bai.setBranch(relIso_all_, branchName_relIso_all_);
  bai.setBranch(hadRelIso03_chg_, branchName_hadRelIso03_chg_);
  bai.setBranch(absIso_chg_, branchName_absIso_chg_);
  bai.setBranch(absIso_neu_, branchName_absIso_neu_);
  bai.setBranch(sip3d_, branchName_sip3d_);
  bai.setBranch(mvaRawTTH_, branchName_mvaRawTTH_);
  bai.setBranch(jetPtRatio_, branchName_jetPtRatio_);
  bai.setBranch(jetBtagCSV_, branchName_jetBtagCSV_);
  bai.setBranch(tightCharge_, branchName_tightCharge_);
  bai.setBranch(charge_, branchName_charge_);
}
