#include "FWCore/ParameterSet/interface/ParameterSet.h" // edm::ParameterSet
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h" // edm::readPSetsFrom()
#include "FWCore/Utilities/interface/Exception.h" // cms::Exception
#include "PhysicsTools/FWLite/interface/TFileService.h" // fwlite::TFileService
#include "DataFormats/FWLite/interface/InputSource.h" // fwlite::InputSource
#include "DataFormats/FWLite/interface/OutputFiles.h" // fwlite::OutputFiles
#include "DataFormats/Math/interface/LorentzVector.h" // math::PtEtaPhiMLorentzVector
#include "DataFormats/Math/interface/deltaR.h" // deltaR

#include <TBenchmark.h> // TBenchmark
#include <TString.h> // TString, Form
#include <TError.h> // gErrorAbortLevel, kError

#include "tthAnalysis/HiggsToTauTau/interface/RecoLepton.h" // RecoLepton
#include "tthAnalysis/HiggsToTauTau/interface/RecoJet.h" // RecoJet
#include "tthAnalysis/HiggsToTauTau/interface/GenLepton.h" // GenLepton
#include "tthAnalysis/HiggsToTauTau/interface/GenJet.h" // GenJet
#include "tthAnalysis/HiggsToTauTau/interface/GenHadTau.h" // GenHadTau
#include "tthAnalysis/HiggsToTauTau/interface/RecoMEt.h" // RecoMEt
#include "tthAnalysis/HiggsToTauTau/interface/EventInfo.h" // EventInfo
#include "tthAnalysis/HiggsToTauTau/interface/TMVAInterface.h" // TMVAInterface
#include "tthAnalysis/HiggsToTauTau/interface/mvaAuxFunctions.h" // check_mvaInputs, get_mvaInputVariables
#include "tthAnalysis/HiggsToTauTau/interface/mvaInputVariables.h" // auxiliary functions for computing input variables of the MVA used for signal extraction in the 3l category
#include "tthAnalysis/HiggsToTauTau/interface/LeptonFakeRateInterface.h" // LeptonFakeRateInterface
#include "tthAnalysis/HiggsToTauTau/interface/JetToTauFakeRateInterface.h" // JetToTauFakeRateInterface
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronReader.h" // RecoElectronReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonReader.h" // RecoMuonReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauReader.h" // RecoHadTauReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetReader.h" // RecoJetReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoMEtReader.h" // RecoMEtReader
#include "tthAnalysis/HiggsToTauTau/interface/MEtFilterReader.h" // MEtFilterReader
#include "tthAnalysis/HiggsToTauTau/interface/GenLeptonReader.h" // GenLeptonReader
#include "tthAnalysis/HiggsToTauTau/interface/GenParticleReader.h" // GenParticleReader
#include "tthAnalysis/HiggsToTauTau/interface/GenHadTauReader.h" // GenHadTauReader
#include "tthAnalysis/HiggsToTauTau/interface/GenPhotonReader.h" // GenPhotonReader
#include "tthAnalysis/HiggsToTauTau/interface/GenJetReader.h" // GenJetReader
#include "tthAnalysis/HiggsToTauTau/interface/LHEInfoReader.h" // LHEInfoReader
#include "tthAnalysis/HiggsToTauTau/interface/EventInfoReader.h" // EventInfoReader
#include "tthAnalysis/HiggsToTauTau/interface/convert_to_ptrs.h" // convert_to_ptrs
#include "tthAnalysis/HiggsToTauTau/interface/ParticleCollectionCleaner.h" // RecoElectronCollectionCleaner, RecoMuonCollectionCleaner, RecoHadTauCollectionCleaner, RecoJetCollectionCleaner
#include "tthAnalysis/HiggsToTauTau/interface/ParticleCollectionGenMatcher.h" // RecoElectronCollectionGenMatcher, RecoMuonCollectionGenMatcher, RecoHadTauCollectionGenMatcher, RecoJetCollectionGenMatcher
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorLoose.h" // RecoElectronCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorFakeable.h" // RecoElectronCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorTight.h" // RecoElectronCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorLoose.h" // RecoMuonCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorFakeable.h" // RecoMuonCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorTight.h" // RecoMuonCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorFakeable.h" // RecoHadTauCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelector.h" // RecoJetCollectionSelector
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelectorBtag.h" // RecoJetCollectionSelectorBtagLoose, RecoJetCollectionSelectorBtagMedium
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelectorForward.h" // RecoJetSelectorForward
#include "tthAnalysis/HiggsToTauTau/interface/RunLumiEventSelector.h" // RunLumiEventSelector
#include "tthAnalysis/HiggsToTauTau/interface/MEtFilterSelector.h" // MEtFilterSelector
#include "tthAnalysis/HiggsToTauTau/interface/ElectronHistManager.h" // ElectronHistManager
#include "tthAnalysis/HiggsToTauTau/interface/MuonHistManager.h" // MuonHistManager
#include "tthAnalysis/HiggsToTauTau/interface/JetHistManager.h" // JetHistManager
#include "tthAnalysis/HiggsToTauTau/interface/MEtHistManager.h" // MEtHistManager
#include "tthAnalysis/HiggsToTauTau/interface/MEtFilterHistManager.h" // MEtFilterHistManager
#include "tthAnalysis/HiggsToTauTau/interface/MVAInputVarHistManager.h" // MVAInputVarHistManager
#include "tthAnalysis/HiggsToTauTau/interface/EvtHistManager_4l.h" // EvtHistManager_4l
#include "tthAnalysis/HiggsToTauTau/interface/EvtYieldHistManager.h" // EvtYieldHistManager
#include "tthAnalysis/HiggsToTauTau/interface/CutFlowTableHistManager.h" // CutFlowTableHistManager
#include "tthAnalysis/HiggsToTauTau/interface/WeightHistManager.h" // WeightHistManager
#include "tthAnalysis/HiggsToTauTau/interface/GenEvtHistManager.h" // GenEvtHistManager
#include "tthAnalysis/HiggsToTauTau/interface/LHEInfoHistManager.h" // LHEInfoHistManager
#include "tthAnalysis/HiggsToTauTau/interface/leptonTypes.h" // getLeptonType, kElectron, kMuon
#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // getBTagWeight_option, getHadTau_genPdgId, isHigherPt, isMatched
#include "tthAnalysis/HiggsToTauTau/interface/leptonGenMatchingAuxFunctions.h" // getLeptonGenMatch_definitions_4lepton, getLeptonGenMatch_string, getLeptonGenMatch_int
#include "tthAnalysis/HiggsToTauTau/interface/fakeBackgroundAuxFunctions.h" // getWeight_4L
#include "tthAnalysis/HiggsToTauTau/interface/hltPath.h" // hltPath, create_hltPaths, hltPaths_isTriggered, hltPaths_delete
#include "tthAnalysis/HiggsToTauTau/interface/hltPathReader.h" // hltPathReader
#include "tthAnalysis/HiggsToTauTau/interface/Data_to_MC_CorrectionInterface_2016.h"
#include "tthAnalysis/HiggsToTauTau/interface/Data_to_MC_CorrectionInterface_2017.h"
#include "tthAnalysis/HiggsToTauTau/interface/Data_to_MC_CorrectionInterface_2018.h"
#include "tthAnalysis/HiggsToTauTau/interface/lutAuxFunctions.h" // loadTH2, get_sf_from_TH2
#include "tthAnalysis/HiggsToTauTau/interface/L1PreFiringWeightReader.h" // L1PreFiringWeightReader
#include "tthAnalysis/HiggsToTauTau/interface/cutFlowTable.h" // cutFlowTableType
#include "tthAnalysis/HiggsToTauTau/interface/NtupleFillerBDT.h" // NtupleFillerBDT
#include "tthAnalysis/HiggsToTauTau/interface/TTreeWrapper.h" // TTreeWrapper
#include "tthAnalysis/HiggsToTauTau/interface/SyncNtupleManager.h" // SyncNtupleManager
#include "tthAnalysis/HiggsToTauTau/interface/hltFilter.h" // hltFilter()
#include "tthAnalysis/HiggsToTauTau/interface/EvtWeightManager.h" // EvtWeightManager
#include "tthAnalysis/HiggsToTauTau/interface/weightAuxFunctions.h" // get_tH_weight_str()

#include <boost/algorithm/string/replace.hpp> // boost::replace_all_copy()
#include <boost/math/special_functions/sign.hpp> // boost::math::sign()

#include <iostream> // std::cerr, std::fixed
#include <iomanip> // std::setprecision(), std::setw()
#include <string> // std::string
#include <vector> // std::vector<>
#include <cstdlib> // EXIT_SUCCESS, EXIT_FAILURE
#include <fstream> // std::ofstream
#include <assert.h> // assert
#include <array> // std::array<>
#include <tuple> // std::tuple<>, std::get<>(), std::make_tuple()

typedef math::PtEtaPhiMLorentzVector LV;
typedef std::vector<std::string> vstring;

enum { kFR_disabled, kFR_4lepton };

const int hadTauSelection_antiElectron = -1; // not applied
const int hadTauSelection_antiMuon = -1; // not applied

/**
 * @brief Produce datacard and control plots for 4l categories.
 */
int main(int argc, char* argv[])
{
//--- throw an exception in case ROOT encounters an error
  gErrorAbortLevel = kError;

//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "<analyze_4l>:" << std::endl;

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("analyze_4l");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") )
    throw cms::Exception("analyze_4l")
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfg_analyze = cfg.getParameter<edm::ParameterSet>("analyze_4l");

  std::string treeName = cfg_analyze.getParameter<std::string>("treeName");

  std::string process_string = cfg_analyze.getParameter<std::string>("process");
  const bool isMC_tH = process_string == "tHq" || process_string == "tHW";
  const bool isMC_VH = process_string == "VH";
  const bool isMC_signal = process_string == "signal" || process_string == "signal_ctcvcp";
  const bool isSignal = isMC_signal || isMC_tH || isMC_VH;

  std::string histogramDir = cfg_analyze.getParameter<std::string>("histogramDir");
  bool isMCClosure_e = histogramDir.find("mcClosure_e") != std::string::npos;
  bool isMCClosure_m = histogramDir.find("mcClosure_m") != std::string::npos;

  std::string era_string = cfg_analyze.getParameter<std::string>("era");
  const int era = get_era(era_string);

  // single lepton triggers
  vstring triggerNames_1e = cfg_analyze.getParameter<vstring>("triggers_1e");
  std::vector<hltPath*> triggers_1e = create_hltPaths(triggerNames_1e, "triggers_1e");
  bool use_triggers_1e = cfg_analyze.getParameter<bool>("use_triggers_1e");
  vstring triggerNames_1mu = cfg_analyze.getParameter<vstring>("triggers_1mu");
  std::vector<hltPath*> triggers_1mu = create_hltPaths(triggerNames_1mu, "triggers_1mu");
  bool use_triggers_1mu = cfg_analyze.getParameter<bool>("use_triggers_1mu");
  // double lepton triggers
  vstring triggerNames_2e = cfg_analyze.getParameter<vstring>("triggers_2e");
  std::vector<hltPath*> triggers_2e = create_hltPaths(triggerNames_2e, "triggers_2e");
  bool use_triggers_2e = cfg_analyze.getParameter<bool>("use_triggers_2e");
  vstring triggerNames_1e1mu = cfg_analyze.getParameter<vstring>("triggers_1e1mu");
  std::vector<hltPath*> triggers_1e1mu = create_hltPaths(triggerNames_1e1mu, "triggers_1e1mu");
  bool use_triggers_1e1mu = cfg_analyze.getParameter<bool>("use_triggers_1e1mu");
  vstring triggerNames_2mu = cfg_analyze.getParameter<vstring>("triggers_2mu");
  std::vector<hltPath*> triggers_2mu = create_hltPaths(triggerNames_2mu, "triggers_2mu");
  bool use_triggers_2mu = cfg_analyze.getParameter<bool>("use_triggers_2mu");
  // triple lepton triggers
  vstring triggerNames_3e = cfg_analyze.getParameter<vstring>("triggers_3e");
  std::vector<hltPath*> triggers_3e = create_hltPaths(triggerNames_3e, "triggers_3e");
  bool use_triggers_3e = cfg_analyze.getParameter<bool>("use_triggers_3e");
  vstring triggerNames_2e1mu = cfg_analyze.getParameter<vstring>("triggers_2e1mu");
  std::vector<hltPath*> triggers_2e1mu = create_hltPaths(triggerNames_2e1mu, "triggers_2e1mu");
  bool use_triggers_2e1mu = cfg_analyze.getParameter<bool>("use_triggers_2e1mu");
  vstring triggerNames_1e2mu = cfg_analyze.getParameter<vstring>("triggers_1e2mu");
  std::vector<hltPath*> triggers_1e2mu = create_hltPaths(triggerNames_1e2mu, "triggers_1e2mu");
  bool use_triggers_1e2mu = cfg_analyze.getParameter<bool>("use_triggers_1e2mu");
  vstring triggerNames_3mu = cfg_analyze.getParameter<vstring>("triggers_3mu");
  std::vector<hltPath*> triggers_3mu = create_hltPaths(triggerNames_3mu, "triggers_3mu");
  bool use_triggers_3mu = cfg_analyze.getParameter<bool>("use_triggers_3mu");

  bool apply_offline_e_trigger_cuts_1e = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_1e");
  bool apply_offline_e_trigger_cuts_1mu = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_1mu");
  bool apply_offline_e_trigger_cuts_2e = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_2e");
  bool apply_offline_e_trigger_cuts_1e1mu = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_1e1mu");
  bool apply_offline_e_trigger_cuts_2mu = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_2mu");
  bool apply_offline_e_trigger_cuts_3e = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_3e");
  bool apply_offline_e_trigger_cuts_2e1mu = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_2e1mu");
  bool apply_offline_e_trigger_cuts_1e2mu = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_1e2mu");
  bool apply_offline_e_trigger_cuts_3mu = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_3mu");

  const std::string electronSelection_string = cfg_analyze.getParameter<std::string>("electronSelection");
  const std::string muonSelection_string     = cfg_analyze.getParameter<std::string>("muonSelection");
  std::cout << "electronSelection_string = " << electronSelection_string << "\n"
               "muonSelection_string     = " << muonSelection_string     << '\n'
  ;
  const int electronSelection = get_selection(electronSelection_string);
  const int muonSelection     = get_selection(muonSelection_string);

  bool apply_leptonGenMatching = cfg_analyze.getParameter<bool>("apply_leptonGenMatching");
  std::vector<leptonGenMatchEntry> leptonGenMatch_definitions = getLeptonGenMatch_definitions_4lepton(apply_leptonGenMatching);
  std::cout << "leptonGenMatch_definitions:" << std::endl;
  std::cout << leptonGenMatch_definitions;

  TString hadTauSelection_string = cfg_analyze.getParameter<std::string>("hadTauSelection").data();
  TObjArray* hadTauSelection_parts = hadTauSelection_string.Tokenize("|");
  assert(hadTauSelection_parts->GetEntries() >= 1);
  std::string hadTauSelection_part2 = ( hadTauSelection_parts->GetEntries() == 2 ) ? (dynamic_cast<TObjString*>(hadTauSelection_parts->At(1)))->GetString().Data() : "";
  delete hadTauSelection_parts;

  enum { kOS, kSS };
  std::string chargeSumSelection_string = cfg_analyze.getParameter<std::string>("chargeSumSelection");
  int chargeSumSelection = -1;
  if      ( chargeSumSelection_string == "OS" ) chargeSumSelection = kOS;
  else if ( chargeSumSelection_string == "SS" ) chargeSumSelection = kSS;
  else throw cms::Exception("analyze_4l")
    << "Invalid Configuration parameter 'chargeSumSelection' = " << chargeSumSelection_string << " !!\n";

  int minNumJets = cfg_analyze.getParameter<int>("minNumJets");
  std::cout << "minNumJets = " << minNumJets << std::endl;

  bool isMC = cfg_analyze.getParameter<bool>("isMC");
  bool hasLHE = cfg_analyze.getParameter<bool>("hasLHE");
  std::string central_or_shift = cfg_analyze.getParameter<std::string>("central_or_shift");
  double lumiScale = ( process_string != "data_obs" ) ? cfg_analyze.getParameter<double>("lumiScale") : 1.;
  bool apply_genWeight = cfg_analyze.getParameter<bool>("apply_genWeight");
  bool apply_l1PreFireWeight = cfg_analyze.getParameter<bool>("apply_l1PreFireWeight");
  bool apply_hlt_filter = cfg_analyze.getParameter<bool>("apply_hlt_filter");
  bool apply_met_filters = cfg_analyze.getParameter<bool>("apply_met_filters");
  edm::ParameterSet cfgMEtFilter = cfg_analyze.getParameter<edm::ParameterSet>("cfgMEtFilter");
  MEtFilterSelector metFilterSelector(cfgMEtFilter, isMC);
  const bool useNonNominal = cfg_analyze.getParameter<bool>("useNonNominal");
  const bool useNonNominal_jetmet = useNonNominal || ! isMC;

  const edm::ParameterSet syncNtuple_cfg = cfg_analyze.getParameter<edm::ParameterSet>("syncNtuple");
  const std::string syncNtuple_tree = syncNtuple_cfg.getParameter<std::string>("tree");
  const std::string syncNtuple_output = syncNtuple_cfg.getParameter<std::string>("output");
  const vstring syncNtuple_genMatch = syncNtuple_cfg.getParameter<vstring>("genMatch");
  const bool jetCleaningByIndex = cfg_analyze.getParameter<bool>("jetCleaningByIndex");
  const bool do_sync = ! syncNtuple_tree.empty() && ! syncNtuple_output.empty();

  const edm::ParameterSet additionalEvtWeight = cfg_analyze.getParameter<edm::ParameterSet>("evtWeight");
  const bool applyAdditionalEvtWeight = additionalEvtWeight.getParameter<bool>("apply");
  EvtWeightManager * eventWeightManager = nullptr;
  if(applyAdditionalEvtWeight)
  {
    eventWeightManager = new EvtWeightManager(additionalEvtWeight);
  }

  bool isDEBUG = cfg_analyze.getParameter<bool>("isDEBUG");
  if ( isDEBUG ) std::cout << "Warning: DEBUG mode enabled -> trigger selection will not be applied for data !!" << std::endl;

  checkOptionValidity(central_or_shift, isMC);
  const int jetToLeptonFakeRate_option        = getJetToLeptonFR_option(central_or_shift);
  const int hadTauPt_option                   = getHadTauPt_option            (central_or_shift);
  const int lheScale_option                   = getLHEscale_option     (central_or_shift);
  const int jetBtagSF_option                  = getBTagWeight_option   (central_or_shift);
  const PUsys puSys_option                    = getPUsys_option        (central_or_shift);
  const L1PreFiringWeightSys l1PreFire_option = getL1PreFiringWeightSys_option(central_or_shift);

  const int met_option   = useNonNominal_jetmet ? kMEt_central_nonNominal : getMET_option(central_or_shift, isMC);
  const int jetPt_option = useNonNominal_jetmet ? kJet_central_nonNominal : getJet_option(central_or_shift, isMC);

  std::cout
    << "central_or_shift = "               << central_or_shift             << "\n"
       " -> jetToLeptonFakeRate_option = " << jetToLeptonFakeRate_option   << "\n"
       " -> lheScale_option            = " << lheScale_option              << "\n"
       " -> jetBtagSF_option           = " << jetBtagSF_option             << "\n"
       " -> met_option                 = " << met_option                   << "\n"
       " -> jetPt_option               = " << jetPt_option                 << "\n"
       " -> l1PreFire_option           = " << as_integer(l1PreFire_option) << '\n'
  ;

  edm::ParameterSet cfg_dataToMCcorrectionInterface;
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("era", era_string);
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("hadTauSelection", hadTauSelection_part2);
  cfg_dataToMCcorrectionInterface.addParameter<int>("hadTauSelection_antiElectron", hadTauSelection_antiElectron);
  cfg_dataToMCcorrectionInterface.addParameter<int>("hadTauSelection_antiMuon", hadTauSelection_antiMuon);
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("central_or_shift", central_or_shift);
  Data_to_MC_CorrectionInterface_Base * dataToMCcorrectionInterface = nullptr;
  switch(era)
  {
    case kEra_2016: dataToMCcorrectionInterface = new Data_to_MC_CorrectionInterface_2016(cfg_dataToMCcorrectionInterface); break;
    case kEra_2017: dataToMCcorrectionInterface = new Data_to_MC_CorrectionInterface_2017(cfg_dataToMCcorrectionInterface); break;
    case kEra_2018: dataToMCcorrectionInterface = new Data_to_MC_CorrectionInterface_2018(cfg_dataToMCcorrectionInterface); break;
    default: throw cmsException("analyze_4l", __LINE__) << "Invalid era = " << era;
  }

  std::string applyFakeRateWeights_string = cfg_analyze.getParameter<std::string>("applyFakeRateWeights");
  int applyFakeRateWeights = -1;
  if      ( applyFakeRateWeights_string == "disabled" ) applyFakeRateWeights = kFR_disabled;
  else if ( applyFakeRateWeights_string == "4lepton"  ) applyFakeRateWeights = kFR_4lepton;
  else throw cms::Exception("analyze_4l")
    << "Invalid Configuration parameter 'applyFakeRateWeights' = " << applyFakeRateWeights_string << " !!\n";

  LeptonFakeRateInterface* leptonFakeRateInterface = 0;
  if ( applyFakeRateWeights == kFR_4lepton) {
    edm::ParameterSet cfg_leptonFakeRateWeight = cfg_analyze.getParameter<edm::ParameterSet>("leptonFakeRateWeight");
    leptonFakeRateInterface = new LeptonFakeRateInterface(cfg_leptonFakeRateWeight, jetToLeptonFakeRate_option);
  }

  bool fillGenEvtHistograms = cfg_analyze.getParameter<bool>("fillGenEvtHistograms");
  edm::ParameterSet cfg_EvtYieldHistManager = cfg_analyze.getParameter<edm::ParameterSet>("cfgEvtYieldHistManager");

  std::string branchName_electrons = cfg_analyze.getParameter<std::string>("branchName_electrons");
  std::string branchName_muons = cfg_analyze.getParameter<std::string>("branchName_muons");
  std::string branchName_hadTaus = cfg_analyze.getParameter<std::string>("branchName_hadTaus");
  std::string branchName_jets = cfg_analyze.getParameter<std::string>("branchName_jets");
  std::string branchName_met = cfg_analyze.getParameter<std::string>("branchName_met");

  std::string branchName_genLeptons = cfg_analyze.getParameter<std::string>("branchName_genLeptons");
  std::string branchName_genHadTaus = cfg_analyze.getParameter<std::string>("branchName_genHadTaus");
  std::string branchName_genPhotons = cfg_analyze.getParameter<std::string>("branchName_genPhotons");
  std::string branchName_genJets = cfg_analyze.getParameter<std::string>("branchName_genJets");

  std::string branchName_muonGenMatch     = cfg_analyze.getParameter<std::string>("branchName_muonGenMatch");
  std::string branchName_electronGenMatch = cfg_analyze.getParameter<std::string>("branchName_electronGenMatch");
  std::string branchName_jetGenMatch      = cfg_analyze.getParameter<std::string>("branchName_jetGenMatch");

  const bool redoGenMatching = cfg_analyze.getParameter<bool>("redoGenMatching");
  const bool genMatchingByIndex = cfg_analyze.getParameter<bool>("genMatchingByIndex");

  std::string selEventsFileName_input = cfg_analyze.getParameter<std::string>("selEventsFileName_input");
  std::cout << "selEventsFileName_input = " << selEventsFileName_input << std::endl;
  RunLumiEventSelector* run_lumi_eventSelector = 0;
  if ( selEventsFileName_input != "" ) {
    edm::ParameterSet cfgRunLumiEventSelector;
    cfgRunLumiEventSelector.addParameter<std::string>("inputFileName", selEventsFileName_input);
    cfgRunLumiEventSelector.addParameter<std::string>("separator", ":");
    run_lumi_eventSelector = new RunLumiEventSelector(cfgRunLumiEventSelector);
  }

  std::string selEventsFileName_output = cfg_analyze.getParameter<std::string>("selEventsFileName_output");
  std::cout << "selEventsFileName_output = " << selEventsFileName_output << std::endl;

  fwlite::InputSource inputFiles(cfg);
  int maxEvents = inputFiles.maxEvents();
  std::cout << " maxEvents = " << maxEvents << std::endl;
  unsigned reportEvery = inputFiles.reportAfter();

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

  TTreeWrapper * inputTree = new TTreeWrapper(treeName.data(), inputFiles.files(), maxEvents);

  std::cout << "Loaded " << inputTree -> getFileCount() << " file(s).\n";

//--- prepare sync Ntuple
  SyncNtupleManager * snm = nullptr;
  if(do_sync)
  {
    snm = new SyncNtupleManager(syncNtuple_output, syncNtuple_tree);
    snm->initializeBranches();
    snm->initializeHLTBranches({
      triggers_1e, triggers_1mu, triggers_2e, triggers_1e1mu, triggers_2mu,
      triggers_3e, triggers_2e1mu, triggers_1e2mu, triggers_3mu
    });
  }

//--- declare event-level variables
  EventInfo eventInfo(isMC, isSignal);
  EventInfoReader eventInfoReader(&eventInfo, puSys_option);
  inputTree -> registerReader(&eventInfoReader);

  hltPathReader hltPathReader_instance({ triggers_1e, triggers_1mu, triggers_2e, triggers_1e1mu, triggers_2mu, triggers_3e, triggers_2e1mu, triggers_1e2mu, triggers_3mu });
  inputTree -> registerReader(&hltPathReader_instance);

  if(eventWeightManager)
  {
    inputTree->registerReader(eventWeightManager);
  }

  L1PreFiringWeightReader * l1PreFiringWeightReader = nullptr;
  if(apply_l1PreFireWeight)
  {
    l1PreFiringWeightReader = new L1PreFiringWeightReader(era, l1PreFire_option);
    inputTree->registerReader(l1PreFiringWeightReader);
  }

//--- declare particle collections
  const bool readGenObjects = isMC && !redoGenMatching;
  RecoMuonReader* muonReader = new RecoMuonReader(era, branchName_muons, isMC, readGenObjects);
  inputTree -> registerReader(muonReader);
  RecoMuonCollectionGenMatcher muonGenMatcher;
  RecoMuonCollectionSelectorLoose preselMuonSelector(era, -1, isDEBUG);
  RecoMuonCollectionSelectorFakeable fakeableMuonSelector(era, -1, isDEBUG);
  RecoMuonCollectionSelectorTight tightMuonSelector(era, -1, isDEBUG);

  RecoElectronReader* electronReader = new RecoElectronReader(era, branchName_electrons, isMC, readGenObjects);
  inputTree -> registerReader(electronReader);
  RecoElectronCollectionGenMatcher electronGenMatcher;
  RecoElectronCollectionCleaner electronCleaner(0.3, isDEBUG);
  RecoElectronCollectionSelectorLoose preselElectronSelector(era, -1, isDEBUG);
  RecoElectronCollectionSelectorFakeable fakeableElectronSelector(era, -1, isDEBUG);
  RecoElectronCollectionSelectorTight tightElectronSelector(era, -1, isDEBUG);

  RecoHadTauReader* hadTauReader = new RecoHadTauReader(era, branchName_hadTaus, isMC, readGenObjects);
  hadTauReader->setHadTauPt_central_or_shift(hadTauPt_option);
  inputTree -> registerReader(hadTauReader);
  RecoHadTauCollectionGenMatcher hadTauGenMatcher;
  RecoHadTauCollectionCleaner hadTauCleaner(0.3, isDEBUG);
  RecoHadTauCollectionSelectorFakeable fakeableHadTauSelector(era, -1, isDEBUG);
  fakeableHadTauSelector.set_if_looser(hadTauSelection_part2);
  fakeableHadTauSelector.set_min_antiElectron(hadTauSelection_antiElectron);
  fakeableHadTauSelector.set_min_antiMuon(hadTauSelection_antiMuon);

  RecoJetReader* jetReader = new RecoJetReader(era, isMC, branchName_jets, readGenObjects);
  jetReader->setPtMass_central_or_shift(jetPt_option);
  jetReader->setBranchName_BtagWeight(jetBtagSF_option);
  inputTree -> registerReader(jetReader);
  RecoJetCollectionGenMatcher jetGenMatcher;
  RecoJetCollectionCleaner jetCleaner(0.4, isDEBUG);
  RecoJetCollectionCleanerByIndex jetCleanerByIndex(isDEBUG);
  RecoJetCollectionSelector jetSelector(era, -1, isDEBUG);
  RecoJetCollectionSelectorBtagLoose jetSelectorBtagLoose(era, -1, isDEBUG);
  RecoJetCollectionSelectorBtagMedium jetSelectorBtagMedium(era, -1, isDEBUG);
  const RecoJetCollectionSelectorForward jetSelectorForward(era, -1, isDEBUG);

//--- declare missing transverse energy
  RecoMEtReader* metReader = new RecoMEtReader(era, isMC, branchName_met);
  metReader->setMEt_central_or_shift(met_option);
  inputTree -> registerReader(metReader);

  MEtFilter metFilters;
  MEtFilterReader* metFilterReader = new MEtFilterReader(&metFilters, era);
  inputTree -> registerReader(metFilterReader);

  GenLeptonReader * genLeptonReader = nullptr;
  GenHadTauReader * genHadTauReader = nullptr;
  GenPhotonReader * genPhotonReader = nullptr;
  GenJetReader * genJetReader = nullptr;
  LHEInfoReader * lheInfoReader = nullptr;

  GenParticleReader * genMatchToMuonReader     = nullptr;
  GenParticleReader * genMatchToElectronReader = nullptr;
  GenParticleReader * genMatchToJetReader      = nullptr;
  if(isMC)
  {
    if(! readGenObjects)
    {
      genLeptonReader = new GenLeptonReader(branchName_genLeptons);
      inputTree -> registerReader(genLeptonReader);
      genHadTauReader = new GenHadTauReader(branchName_genHadTaus);
      inputTree -> registerReader(genHadTauReader);
      genJetReader = new GenJetReader(branchName_genJets);
      inputTree -> registerReader(genJetReader);

      if(genMatchingByIndex)
      {
        genMatchToMuonReader = new GenParticleReader(branchName_muonGenMatch);
        genMatchToMuonReader -> readGenPartFlav(false);
        inputTree -> registerReader(genMatchToMuonReader);

        genMatchToElectronReader = new GenParticleReader(branchName_electronGenMatch);
        genMatchToElectronReader -> readGenPartFlav(false);
        inputTree -> registerReader(genMatchToElectronReader);

        genMatchToJetReader = new GenParticleReader(branchName_jetGenMatch);
        genMatchToJetReader -> readGenPartFlav(false);
        inputTree -> registerReader(genMatchToJetReader);
      }
      }
      else
      {
        genPhotonReader = new GenPhotonReader(branchName_genPhotons);
        inputTree -> registerReader(genPhotonReader);
      }
    lheInfoReader = new LHEInfoReader(hasLHE);
    inputTree -> registerReader(lheInfoReader);
  }

//--- open output file containing run:lumi:event numbers of events passing final event selection criteria
  std::ostream* selEventsFile = ( selEventsFileName_output != "" ) ? new std::ofstream(selEventsFileName_output.data(), std::ios::out) : 0;

  bool selectBDT = ( cfg_analyze.exists("selectBDT") ) ? cfg_analyze.getParameter<bool>("selectBDT") : false;

//--- declare histograms
  struct selHistManagerType
  {
    ElectronHistManager* electrons_;
    MuonHistManager* muons_;
    JetHistManager* jets_;
    JetHistManager* leadJet_;
    JetHistManager* subleadJet_;
    JetHistManager* BJets_loose_;
    JetHistManager* leadBJet_loose_;
    JetHistManager* subleadBJet_loose_;
    JetHistManager* BJets_medium_;
    MEtHistManager* met_;
    MEtFilterHistManager* metFilters_;
    MVAInputVarHistManager* mvaInputVariables_4l_;
    std::map<std::string, EvtHistManager_4l*> evt_;
    std::map<std::string, std::map<std::string, EvtHistManager_4l*>> evt_in_decayModes_;
    EvtYieldHistManager* evtYield_;
    WeightHistManager* weights_;
  };
  std::map<int, selHistManagerType*> selHistManagers;

  const std::string default_cat_str = "default";
  std::vector<std::string> evt_cat_strs = { default_cat_str };
  const std::vector<edm::ParameterSet> tHweights = cfg_analyze.getParameterSetVector("tHweights");
  if((isMC_tH || isMC_signal) && ! tHweights.empty())
  {
    eventInfo.loadWeight_tH(tHweights);
    const std::vector<std::string> evt_cat_tH_strs = eventInfo.getWeight_tH_str();
    evt_cat_strs.insert(evt_cat_strs.end(), evt_cat_tH_strs.begin(), evt_cat_tH_strs.end());
  }

  for ( std::vector<leptonGenMatchEntry>::const_iterator leptonGenMatch_definition = leptonGenMatch_definitions.begin();
	leptonGenMatch_definition != leptonGenMatch_definitions.end(); ++leptonGenMatch_definition ) {

    std::string process_and_genMatch = process_string;
    if ( apply_leptonGenMatching ) process_and_genMatch += leptonGenMatch_definition->name_;

    int idxLepton = leptonGenMatch_definition->idx_;

    selHistManagerType* selHistManager = new selHistManagerType();
    selHistManager->electrons_ = new ElectronHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/electrons", histogramDir.data()), era_string, central_or_shift, "allHistograms"));
    selHistManager->electrons_->bookHistograms(fs);
    selHistManager->muons_ = new MuonHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/muons", histogramDir.data()), era_string, central_or_shift, "allHistograms"));
    selHistManager->muons_->bookHistograms(fs);
    selHistManager->jets_ = new JetHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/jets", histogramDir.data()), era_string, central_or_shift, "allHistograms"));
    selHistManager->jets_->bookHistograms(fs);
    selHistManager->leadJet_ = new JetHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/leadJet", histogramDir.data()), era_string, central_or_shift, "minimalHistograms", 0));
    selHistManager->leadJet_->bookHistograms(fs);
    selHistManager->subleadJet_ = new JetHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/subleadJet", histogramDir.data()), era_string, central_or_shift, "minimalHistograms", 1));
    selHistManager->subleadJet_->bookHistograms(fs);
    selHistManager->BJets_loose_ = new JetHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/BJets_loose", histogramDir.data()), era_string, central_or_shift, "allHistograms"));
    selHistManager->BJets_loose_->bookHistograms(fs);
    selHistManager->leadBJet_loose_ = new JetHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/leadBJet_loose", histogramDir.data()), era_string, central_or_shift, "minimalHistograms", 0));
    selHistManager->leadBJet_loose_->bookHistograms(fs);
    selHistManager->subleadBJet_loose_ = new JetHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/subleadBJet_loose", histogramDir.data()), era_string, central_or_shift, "minimalHistograms", 1));
    selHistManager->subleadBJet_loose_->bookHistograms(fs);
    selHistManager->BJets_medium_ = new JetHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/BJets_medium", histogramDir.data()), era_string, central_or_shift, "allHistograms"));
    selHistManager->BJets_medium_->bookHistograms(fs);
    selHistManager->met_ = new MEtHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/met", histogramDir.data()), era_string, central_or_shift));
    selHistManager->met_->bookHistograms(fs);
    selHistManager->metFilters_ = new MEtFilterHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/metFilters", histogramDir.data()), era_string, central_or_shift));
    selHistManager->metFilters_->bookHistograms(fs);

    for(const std::string & evt_cat_str: evt_cat_strs)
    {
      const std::string process_string_new = evt_cat_str == default_cat_str ?
        process_string + "_" :
        process_string + "_" + evt_cat_str + "_"
      ;
      const std::string process_and_genMatchName = boost::replace_all_copy(
        process_and_genMatch, process_string, process_string_new
      );

      selHistManager->evt_[evt_cat_str] = new EvtHistManager_4l(makeHistManager_cfg(
        process_and_genMatchName, Form("%s/sel/evt", histogramDir.data()), era_string, central_or_shift
      ));
      selHistManager->evt_[evt_cat_str]->bookHistograms(fs);
    }

    const vstring decayModes_evt = eventInfo.getDecayModes();
    if(isSignal)
    {
      for(const std::string & decayMode_evt: decayModes_evt)
      {
        if ( ( isMC_tH || isMC_VH ) && ( decayMode_evt == "hzg" || decayMode_evt == "hmm" ) ) continue;
        std::string decayMode_and_genMatch;
        if ( isMC_tH || isMC_VH ) {
          decayMode_and_genMatch = process_string;
          decayMode_and_genMatch += "_";
        }
        else decayMode_and_genMatch = "ttH_";
        decayMode_and_genMatch += decayMode_evt;
        if(apply_leptonGenMatching)
        {
          decayMode_and_genMatch += leptonGenMatch_definition -> name_;
        }

        for(const std::string & evt_cat_str: evt_cat_strs)
        {
          const std::string process_string_new = evt_cat_str == default_cat_str ?
            process_string:
            process_string + "_" + evt_cat_str
          ;
          const std::string decayMode_and_genMatchName = boost::replace_all_copy(
            decayMode_and_genMatch, process_string, process_string_new
          );

          selHistManager -> evt_in_decayModes_[evt_cat_str][decayMode_evt] = new EvtHistManager_4l(makeHistManager_cfg(
            decayMode_and_genMatchName, Form("%s/sel/evt", histogramDir.data()), era_string, central_or_shift
          ));
          selHistManager -> evt_in_decayModes_[evt_cat_str][decayMode_evt] -> bookHistograms(fs);
        }
      }
    }
    edm::ParameterSet cfg_EvtYieldHistManager_sel = makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/evtYield", histogramDir.data()), era_string, central_or_shift);
    cfg_EvtYieldHistManager_sel.addParameter<edm::ParameterSet>("runPeriods", cfg_EvtYieldHistManager);
    cfg_EvtYieldHistManager_sel.addParameter<bool>("isMC", isMC);
    selHistManager->evtYield_ = new EvtYieldHistManager(cfg_EvtYieldHistManager_sel);
    selHistManager->evtYield_->bookHistograms(fs);
    selHistManager->weights_ = new WeightHistManager(makeHistManager_cfg(process_and_genMatch,
      Form("%s/sel/weights", histogramDir.data()), era_string, central_or_shift));
    selHistManager->weights_->bookHistograms(fs, { "genWeight", "pileupWeight", "triggerWeight", "data_to_MC_correction", "fakeRate" });
    selHistManagers[idxLepton] = selHistManager;
  }

  GenEvtHistManager* genEvtHistManager_beforeCuts = 0;
  GenEvtHistManager* genEvtHistManager_afterCuts = 0;
  LHEInfoHistManager* lheInfoHistManager = 0;
  if ( isMC ) {
    genEvtHistManager_beforeCuts = new GenEvtHistManager(makeHistManager_cfg(process_string,
      Form("%s/unbiased/genEvt", histogramDir.data()), era_string, central_or_shift));
    genEvtHistManager_beforeCuts->bookHistograms(fs);
    genEvtHistManager_afterCuts = new GenEvtHistManager(makeHistManager_cfg(process_string,
      Form("%s/sel/genEvt", histogramDir.data()), era_string, central_or_shift));
    genEvtHistManager_afterCuts->bookHistograms(fs);
    lheInfoHistManager = new LHEInfoHistManager(makeHistManager_cfg(process_string,
      Form("%s/sel/lheInfo", histogramDir.data()), era_string, central_or_shift));
    lheInfoHistManager->bookHistograms(fs);

    if(eventWeightManager)
    {
      genEvtHistManager_beforeCuts->bookHistograms(fs, eventWeightManager);
      genEvtHistManager_afterCuts->bookHistograms(fs, eventWeightManager);
    }
  }

  NtupleFillerBDT<float, int> * bdt_filler = nullptr;
  typedef std::remove_pointer<decltype(bdt_filler)>::type::float_type float_type;
  typedef std::remove_pointer<decltype(bdt_filler)>::type::int_type   int_type;
  if ( selectBDT ) {
    bdt_filler = new std::remove_pointer<decltype(bdt_filler)>::type(
      makeHistManager_cfg(process_string, Form("%s/sel/evtntuple", histogramDir.data()), era_string, central_or_shift)
    );
    bdt_filler -> register_variable<float_type>(
      "lep1_pt", "lep1_conePt", "lep1_eta", "lep1_tth_mva", "mindr_lep1_jet", "mT_lep1",
      "lep2_pt", "lep2_conePt", "lep2_eta", "lep2_tth_mva", "mindr_lep2_jet", "mT_lep2",
      "lep3_pt", "lep3_conePt", "lep3_eta", "lep3_tth_mva", "mindr_lep3_jet", "mT_lep3",
      "lep4_pt", "lep4_conePt", "lep4_eta", "lep4_tth_mva", "mindr_lep4_jet", "mT_lep4",
      "avg_dr_jet", "ptmiss",  "htmiss", "dr_leps",
      "lumiScale", "genWeight", "evtWeight",
      "lep1_genLepPt", "lep2_genLepPt", "lep3_genLepPt", "lep4_genLepPt",
      "lep1_fake_prob", "lep2_fake_prob", "lep3_fake_prob", "lep4_fake_prob",
      "lep1_frWeight", "lep2_frWeight", "lep3_frWeight", "lep4_frWeight"
    );
    bdt_filler -> register_variable<int_type>(
      "nJet", "nBJetLoose", "nBJetMedium", "lep1_isTight", "lep2_isTight", "lep3_isTight", "lep4_isTight"
    );
    bdt_filler -> bookTree(fs);
  }

  int analyzedEntries = 0;
  int selectedEntries = 0;
  double selectedEntries_weighted = 0.;
  TH1* histogram_analyzedEntries = fs.make<TH1D>("analyzedEntries", "analyzedEntries", 1, -0.5, +0.5);
  TH1* histogram_selectedEntries = fs.make<TH1D>("selectedEntries", "selectedEntries", 1, -0.5, +0.5);
  cutFlowTableType cutFlowTable;
  const edm::ParameterSet cutFlowTableCfg = makeHistManager_cfg(
    process_string, Form("%s/sel/cutFlow", histogramDir.data()), era_string, central_or_shift
  );
  const std::vector<std::string> cuts = {
    "run:ls:event selection",
    "trigger",
    ">= 4 presel leptons",
    ">= 2 jets (1)",
    ">= 2 loose b-jets || 1 medium b-jet (1)",
    "4 sel leptons",
    "HLT filter matching",
    ">= 2 jets (2)",
    ">= 2 loose b-jets || 1 medium b-jet (2)",
    "m(ll) > 12 GeV",
    "lead lepton pT > 25 GeV && sublead lepton pT > 15 GeV && third lepton pT > 15 GeV && fourth lepton pT > 10 GeV",
    "sel lepton charge",
    "Z-boson mass veto",
    "H->ZZ*->4l veto",
    //"met LD",
    "MEt filters",
    "signal region veto",
  };
  CutFlowTableHistManager * cutFlowHistManager = new CutFlowTableHistManager(cutFlowTableCfg, cuts);
  cutFlowHistManager->bookHistograms(fs);

  while(inputTree -> hasNextEvent() && (! run_lumi_eventSelector || (run_lumi_eventSelector && ! run_lumi_eventSelector -> areWeDone())))
  {
    if(inputTree -> canReport(reportEvery))
    {
      std::cout << "processing Entry " << inputTree -> getCurrentMaxEventIdx()
                << " or " << inputTree -> getCurrentEventIdx() << " entry in #"
                << (inputTree -> getProcessedFileCount() - 1)
                << " (" << eventInfo
                << ") file (" << selectedEntries << " Entries selected)\n";
    }
    ++analyzedEntries;
    histogram_analyzedEntries->Fill(0.);
    //if ( analyzedEntries > 1 ) break;

    if (run_lumi_eventSelector && !(*run_lumi_eventSelector)(eventInfo))
    {
      continue;
    }
    cutFlowTable.update("run:ls:event selection");
    cutFlowHistManager->fillHistograms("run:ls:event selection", lumiScale);

    if ( isDEBUG ) {
      std::cout << "event #" << inputTree -> getCurrentMaxEventIdx() << ' ' << eventInfo << '\n';
    }

    if(run_lumi_eventSelector)
    {
      std::cout << "processing Entry " << inputTree -> getCurrentMaxEventIdx() << ": " << eventInfo << '\n';
      if(inputTree -> isOpen())
      {
        std::cout << "input File = " << inputTree -> getCurrentFileName() << '\n';
      }
    }

//--- build collections of generator level particles (before any cuts are applied, to check distributions in unbiased event samples)
    std::vector<GenLepton> genLeptons;
    std::vector<GenLepton> genElectrons;
    std::vector<GenLepton> genMuons;
    std::vector<GenHadTau> genHadTaus;
    std::vector<GenPhoton> genPhotons;
    std::vector<GenJet> genJets;

    std::vector<GenParticle> muonGenMatch;
    std::vector<GenParticle> electronGenMatch;
    std::vector<GenParticle> jetGenMatch;
    if(isMC && fillGenEvtHistograms)
    {
      if(genLeptonReader)
      {
        genLeptons = genLeptonReader->read();
        for(const GenLepton & genLepton: genLeptons)
        {
          const int abs_pdgId = std::abs(genLepton.pdgId());
          switch(abs_pdgId)
          {
            case 11: genElectrons.push_back(genLepton); break;
            case 13: genMuons.push_back(genLepton);     break;
            default: assert(0);
          }
        }
      }
      if(genHadTauReader) genHadTaus = genHadTauReader->read();
      if(genPhotonReader) genPhotons = genPhotonReader->read();
      if(genJetReader)    genJets = genJetReader->read();

      if(genMatchToMuonReader)     muonGenMatch = genMatchToMuonReader->read();
      if(genMatchToElectronReader) electronGenMatch = genMatchToElectronReader->read();
      if(genMatchToJetReader)      jetGenMatch = genMatchToJetReader->read();
    }

    double evtWeight_inclusive = 1.;
    double evtWeight_tH_nom = 1.;
    if(isMC)
    {
      if(apply_genWeight)         evtWeight_inclusive *= boost::math::sign(eventInfo.genWeight);
      if(eventWeightManager)      evtWeight_inclusive *= eventWeightManager->getWeight();
      if(l1PreFiringWeightReader) evtWeight_inclusive *= l1PreFiringWeightReader->getWeight();
      lheInfoReader->read();
      evtWeight_inclusive *= lheInfoReader->getWeight_scale(lheScale_option);
      evtWeight_inclusive *= eventInfo.pileupWeight;

      evtWeight_tH_nom = eventInfo.genWeight_tH();
      evtWeight_inclusive *= evtWeight_tH_nom;

      evtWeight_inclusive *= lumiScale;
      genEvtHistManager_beforeCuts->fillHistograms(genElectrons, genMuons, genHadTaus, genPhotons, genJets, evtWeight_inclusive);
      if(eventWeightManager)
      {
        genEvtHistManager_beforeCuts->fillHistograms(eventWeightManager, evtWeight_inclusive);
      }
    }

    bool isTriggered_1e = hltPaths_isTriggered(triggers_1e, isDEBUG);
    bool isTriggered_1mu = hltPaths_isTriggered(triggers_1mu, isDEBUG);
    bool isTriggered_2e = hltPaths_isTriggered(triggers_2e, isDEBUG);
    bool isTriggered_1e1mu = hltPaths_isTriggered(triggers_1e1mu, isDEBUG);
    bool isTriggered_2mu = hltPaths_isTriggered(triggers_2mu, isDEBUG);
    bool isTriggered_3e = hltPaths_isTriggered(triggers_3e, isDEBUG);
    bool isTriggered_2e1mu = hltPaths_isTriggered(triggers_2e1mu, isDEBUG);
    bool isTriggered_1e2mu = hltPaths_isTriggered(triggers_1e2mu, isDEBUG);
    bool isTriggered_3mu = hltPaths_isTriggered(triggers_3mu, isDEBUG);
        if ( isDEBUG ) {
      std::cout << "isTriggered:"
		<< " 1e = " << isTriggered_1e << ","
		<< " 1mu = " << isTriggered_1mu << ","
		<< " 2e = " << isTriggered_2e << ","
		<< " 1e1mu = " << isTriggered_1e1mu << ","
		<< " 2mu = " << isTriggered_2mu << ","
		<< " 3e = " << isTriggered_3e << ","
		<< " 2e1mu = " << isTriggered_2e1mu << ","
		<< " 1e2mu = " << isTriggered_1e2mu << ","
		<< " 3mu = " << isTriggered_3mu << std::endl;
    }

    bool selTrigger_1e = use_triggers_1e && isTriggered_1e;
    bool selTrigger_1mu = use_triggers_1mu && isTriggered_1mu;
    bool selTrigger_2e = use_triggers_2e && isTriggered_2e;
    bool selTrigger_1e1mu = use_triggers_1e1mu && isTriggered_1e1mu;
    bool selTrigger_2mu = use_triggers_2mu && isTriggered_2mu;
    bool selTrigger_3e = use_triggers_3e && isTriggered_3e;
    bool selTrigger_2e1mu = use_triggers_2e1mu && isTriggered_2e1mu;
    bool selTrigger_1e2mu = use_triggers_1e2mu && isTriggered_1e2mu;
    bool selTrigger_3mu = use_triggers_3mu && isTriggered_3mu;
    if ( !(selTrigger_1e || selTrigger_1mu   ||
	   selTrigger_2e || selTrigger_1e1mu || selTrigger_2mu   ||
	   selTrigger_3e || selTrigger_2e1mu || selTrigger_1e2mu || selTrigger_3mu) ) {
      if ( run_lumi_eventSelector ) {
    std::cout << "event " << eventInfo.str() << " FAILS trigger selection." << std::endl;
	std::cout << " (selTrigger_3mu = " << selTrigger_3mu
		  << ", selTrigger_1e2mu = " << selTrigger_1e2mu
		  << ", selTrigger_2e1mu = " << selTrigger_2e1mu
		  << ", selTrigger_3e = " << selTrigger_3e
		  << ", selTrigger_2mu = " << selTrigger_2mu
		  << ", selTrigger_1e1mu = " << selTrigger_1e1mu
		  << ", selTrigger_2e = " << selTrigger_2e
		  << ", selTrigger_1mu = " << selTrigger_1mu
		  << ", selTrigger_1e = " << selTrigger_1e << ")" << std::endl;
      }
      continue;
    }

//--- rank triggers by priority and ignore triggers of lower priority if a trigger of higher priority has fired for given event;
//    the triggers are ranked by primary dataset (PD).
//    The ranking of the PDs is as follows: DoubleMuon, MuonEG, DoubleEG, SingleMuon, SingleElectron
// CV: see https://cmssdt.cern.ch/lxr/source/HLTrigger/Configuration/python/HLT_GRun_cff.py?v=CMSSW_8_0_24 for association of triggers paths to PD
//     this logic is necessary to avoid that the same event is selected multiple times when processing different primary datasets
    if ( !isMC && !isDEBUG ) {

      //bool isTriggered_SingleElectron = isTriggered_1e;
      bool isTriggered_SingleMuon = isTriggered_1mu;
      bool isTriggered_DoubleEG = isTriggered_2e || isTriggered_3e;
      bool isTriggered_DoubleMuon = isTriggered_2mu || isTriggered_3mu;
      bool isTriggered_MuonEG = isTriggered_1e1mu || isTriggered_2e1mu || isTriggered_1e2mu;

      bool selTrigger_SingleElectron = selTrigger_1e;
      bool selTrigger_SingleMuon = selTrigger_1mu;
      bool selTrigger_DoubleEG = selTrigger_2e || selTrigger_3e;
      //bool selTrigger_DoubleMuon = selTrigger_2mu || selTrigger_3mu;
      bool selTrigger_MuonEG = selTrigger_1e1mu || selTrigger_2e1mu || selTrigger_1e2mu;
      if ( selTrigger_SingleElectron && (isTriggered_DoubleEG || isTriggered_SingleMuon || isTriggered_DoubleMuon || isTriggered_MuonEG) ) {
	if ( run_lumi_eventSelector ) {
      std::cout << "event " << eventInfo.str() << " FAILS trigger selection." << std::endl;
	  std::cout << " (selTrigger_SingleElectron = " << selTrigger_SingleElectron
		    << ", isTriggered_DoubleEG = " << isTriggered_DoubleEG
		    << ", isTriggered_SingleMuon = " << isTriggered_SingleMuon
		    << ", isTriggered_DoubleMuon = " << isTriggered_DoubleMuon
		    << ", isTriggered_MuonEG = " << isTriggered_MuonEG << ")" << std::endl;
	}
	continue;
      }
      if ( selTrigger_DoubleEG && (isTriggered_DoubleMuon || isTriggered_MuonEG) ) {
	if ( run_lumi_eventSelector ) {
      std::cout << "event " << eventInfo.str() << " FAILS trigger selection." << std::endl;
	  std::cout << " (selTrigger_DoubleEG = " << selTrigger_DoubleEG
		    << ", isTriggered_DoubleMuon = " << isTriggered_DoubleMuon
		    << ", isTriggered_MuonEG = " << isTriggered_MuonEG << ")" << std::endl;
	}
	continue;
      }
      if ( selTrigger_SingleMuon && (isTriggered_DoubleEG || isTriggered_DoubleMuon || isTriggered_MuonEG) ) {
	if ( run_lumi_eventSelector ) {
      std::cout << "event " << eventInfo.str() << " FAILS trigger selection." << std::endl;
	  std::cout << " (selTrigger_SingleMuon = " << selTrigger_SingleMuon
		    << ", isTriggered_DoubleEG = " << isTriggered_DoubleEG
		    << ", isTriggered_DoubleMuon = " << isTriggered_DoubleMuon
		    << ", isTriggered_MuonEG = " << isTriggered_MuonEG << ")" << std::endl;
	}
	continue;
      }
      if ( selTrigger_MuonEG && isTriggered_DoubleMuon ) {
	if ( run_lumi_eventSelector ) {
      std::cout << "event " << eventInfo.str() << " FAILS trigger selection." << std::endl;
	  std::cout << " (selTrigger_MuonEG = " << selTrigger_MuonEG
		    << ", isTriggered_DoubleMuon = " << isTriggered_DoubleMuon << ")" << std::endl;
	}
	continue;
      }
    }
    cutFlowTable.update("trigger");
    cutFlowHistManager->fillHistograms("trigger", lumiScale);

    if ( (selTrigger_3mu   && !apply_offline_e_trigger_cuts_3mu)   ||
	 (selTrigger_2e1mu && !apply_offline_e_trigger_cuts_2e1mu) ||
	 (selTrigger_1e2mu && !apply_offline_e_trigger_cuts_1e2mu) ||
	 (selTrigger_3e    && !apply_offline_e_trigger_cuts_3e)    ||
	 (selTrigger_2mu   && !apply_offline_e_trigger_cuts_2mu)   ||
	 (selTrigger_1e1mu && !apply_offline_e_trigger_cuts_1e1mu) ||
	 (selTrigger_2e    && !apply_offline_e_trigger_cuts_2e)    ||
	 (selTrigger_1mu   && !apply_offline_e_trigger_cuts_1mu)   ||
	 (selTrigger_1e    && !apply_offline_e_trigger_cuts_1e)    ) {
      fakeableElectronSelector.disable_offline_e_trigger_cuts();
      tightElectronSelector.disable_offline_e_trigger_cuts();
    } else {
      fakeableElectronSelector.enable_offline_e_trigger_cuts();
      tightElectronSelector.enable_offline_e_trigger_cuts();
    }

//--- build collections of electrons, muons and hadronic taus;
//    resolve overlaps in order of priority: muon, electron,
    std::vector<RecoMuon> muons = muonReader->read();
    std::vector<const RecoMuon*> muon_ptrs = convert_to_ptrs(muons);
    std::vector<const RecoMuon*> cleanedMuons = muon_ptrs; // CV: no cleaning needed for muons, as they have the highest priority in the overlap removal
    std::vector<const RecoMuon*> preselMuons = preselMuonSelector(cleanedMuons, isHigherConePt);
    std::vector<const RecoMuon*> fakeableMuons = fakeableMuonSelector(preselMuons, isHigherConePt);
    std::vector<const RecoMuon*> tightMuons = tightMuonSelector(fakeableMuons, isHigherConePt);
    if(isDEBUG || run_lumi_eventSelector)
    {
      printCollection("preselMuons",   preselMuons);
      printCollection("fakeableMuons", fakeableMuons);
      printCollection("tightMuons",    tightMuons);
    }

    std::vector<RecoElectron> electrons = electronReader->read();
    std::vector<const RecoElectron*> electron_ptrs = convert_to_ptrs(electrons);
    std::vector<const RecoElectron*> cleanedElectrons = electronCleaner(electron_ptrs, preselMuons);
    std::vector<const RecoElectron*> preselElectrons = preselElectronSelector(cleanedElectrons, isHigherConePt);
    std::vector<const RecoElectron*> fakeableElectrons = fakeableElectronSelector(preselElectrons, isHigherConePt);
    std::vector<const RecoElectron*> tightElectrons = tightElectronSelector(fakeableElectrons, isHigherConePt);
    if(isDEBUG || run_lumi_eventSelector)
    {
      printCollection("preselElectrons",   preselElectrons);
      printCollection("fakeableElectrons", fakeableElectrons);
      printCollection("tightElectrons",    tightElectrons);
    }

    std::vector<const RecoLepton*> preselLeptonsFull = mergeLeptonCollections(preselElectrons, preselMuons, isHigherConePt);
    std::vector<const RecoLepton*> fakeableLeptonsFull = mergeLeptonCollections(fakeableElectrons, fakeableMuons, isHigherConePt);
    std::vector<const RecoLepton*> tightLeptonsFull = mergeLeptonCollections(tightElectrons, tightMuons, isHigherConePt);

    std::vector<const RecoLepton*> preselLeptons = pickFirstNobjects(preselLeptonsFull, 4);
    std::vector<const RecoLepton*> fakeableLeptons = pickFirstNobjects(fakeableLeptonsFull, 4);
    std::vector<const RecoLepton*> tightLeptons = getIntersection(fakeableLeptons, tightLeptonsFull, isHigherConePt);

    std::vector<const RecoLepton*> selLeptons;
    std::vector<const RecoMuon*> selMuons;
    std::vector<const RecoElectron*> selElectrons;
    if(electronSelection == muonSelection)
    {
      // for SR, flip region and fake CR
      // doesn't matter if we supply electronSelection or muonSelection here
      selLeptons = selectObjects(muonSelection, preselLeptons, fakeableLeptons, tightLeptons);
      selMuons = getIntersection(preselMuons, selLeptons, isHigherConePt);
      selElectrons = getIntersection(preselElectrons, selLeptons, isHigherConePt);
    }
    else
    {
      // for MC closure
      // make sure that neither electron nor muon selections are loose
      assert(electronSelection != kLoose && muonSelection != kLoose);
      selMuons = selectObjects(muonSelection, preselMuons, fakeableMuons, tightMuons);
      selElectrons = selectObjects(electronSelection, preselElectrons, fakeableElectrons, tightElectrons);
      std::vector<const RecoLepton*> selLeptons_full = mergeLeptonCollections(selElectrons, selMuons, isHigherConePt);
      selLeptons = getIntersection(fakeableLeptons, selLeptons_full, isHigherConePt);
    }

    std::vector<RecoHadTau> hadTaus = hadTauReader->read();
    std::vector<const RecoHadTau*> hadTau_ptrs = convert_to_ptrs(hadTaus);
    std::vector<const RecoHadTau*> cleanedHadTaus = hadTauCleaner(hadTau_ptrs, preselMuons, preselElectrons);
    std::vector<const RecoHadTau*> fakeableHadTaus = fakeableHadTauSelector(cleanedHadTaus, isHigherPt);

    if(isDEBUG || run_lumi_eventSelector)
    {
      printCollection("selMuons", selMuons);
      printCollection("selElectrons", selElectrons);
      printCollection("selLeptons", selLeptons);
    }

//--- build collections of jets and select subset of jets passing b-tagging criteria
    std::vector<RecoJet> jets = jetReader->read();
    std::vector<const RecoJet*> jet_ptrs = convert_to_ptrs(jets);
    std::vector<const RecoJet*> cleanedJets = jetCleaningByIndex ?
      jetCleanerByIndex(jet_ptrs, fakeableLeptonsFull, fakeableHadTaus) :
      jetCleaner       (jet_ptrs, fakeableLeptonsFull, fakeableHadTaus)
    ; // X: FIXME: in principle we should clean against FO taus as well -- to implement
    std::vector<const RecoJet*> selJets = jetSelector(cleanedJets, isHigherPt);
    std::vector<const RecoJet*> selBJets_loose = jetSelectorBtagLoose(cleanedJets, isHigherPt);
    std::vector<const RecoJet*> selBJets_medium = jetSelectorBtagMedium(cleanedJets, isHigherPt);
    const std::vector<const RecoJet *> selJetsForward = jetSelectorForward(cleanedJets, isHigherPt);
    if(isDEBUG || run_lumi_eventSelector)
    {
      printCollection("uncleanedJets", jet_ptrs);
      printCollection("selJets",       selJets);
    }

//--- build collections of generator level particles (after some cuts are applied, to safe computing time)
    if(isMC && redoGenMatching && ! fillGenEvtHistograms)
    {
      if(genLeptonReader)
      {
        genLeptons = genLeptonReader->read();
        for(const GenLepton & genLepton: genLeptons)
        {
          const int abs_pdgId = std::abs(genLepton.pdgId());
          switch(abs_pdgId)
          {
            case 11: genElectrons.push_back(genLepton); break;
            case 13: genMuons.push_back(genLepton);     break;
            default: assert(0);
          }
        }
      }
      if(genHadTauReader) genHadTaus = genHadTauReader->read();
      if(genPhotonReader) genPhotons = genPhotonReader->read();
      if(genJetReader)    genJets = genJetReader->read();

      if(genMatchToMuonReader)     muonGenMatch = genMatchToMuonReader->read();
      if(genMatchToElectronReader) electronGenMatch = genMatchToElectronReader->read();
      if(genMatchToJetReader)      jetGenMatch = genMatchToJetReader->read();
    }

//--- match reconstructed to generator level particles
    if(isMC && redoGenMatching)
    {
      if(genMatchingByIndex)
      {
        muonGenMatcher.addGenLeptonMatchByIndex(preselMuons, muonGenMatch, GenParticleType::kGenMuon);
        muonGenMatcher.addGenHadTauMatch       (preselMuons, genHadTaus);
        muonGenMatcher.addGenJetMatch          (preselMuons, genJets);

        electronGenMatcher.addGenLeptonMatchByIndex(preselElectrons, electronGenMatch, GenParticleType::kGenElectron);
        electronGenMatcher.addGenPhotonMatchByIndex(preselElectrons, electronGenMatch);
        electronGenMatcher.addGenHadTauMatch       (preselElectrons, genHadTaus);
        electronGenMatcher.addGenJetMatch          (preselElectrons, genJets);

        jetGenMatcher.addGenLeptonMatch    (selJets, genLeptons);
        jetGenMatcher.addGenHadTauMatch    (selJets, genHadTaus);
        jetGenMatcher.addGenJetMatchByIndex(selJets, jetGenMatch);
      }
      else
      {
        muonGenMatcher.addGenLeptonMatch(preselMuons, genMuons);
        muonGenMatcher.addGenHadTauMatch(preselMuons, genHadTaus);
        muonGenMatcher.addGenJetMatch   (preselMuons, genJets);

        electronGenMatcher.addGenLeptonMatch(preselElectrons, genElectrons);
        electronGenMatcher.addGenPhotonMatch(preselElectrons, genPhotons);
        electronGenMatcher.addGenHadTauMatch(preselElectrons, genHadTaus);
        electronGenMatcher.addGenJetMatch   (preselElectrons, genJets);

        jetGenMatcher.addGenLeptonMatch(selJets, genLeptons);
        jetGenMatcher.addGenHadTauMatch(selJets, genHadTaus);
        jetGenMatcher.addGenJetMatch   (selJets, genJets);
      }
    }

//--- apply preselection
    // require exactly two leptons passing loose preselection criteria to avoid overlap with 3l category
    if ( !(preselLeptonsFull.size() >= 4) ) {
      if ( run_lumi_eventSelector ) {
    std::cout << "event " << eventInfo.str() << " FAILS preselLeptons selection." << std::endl;
  printCollection("preselLeptons", preselLeptonsFull);
      }
      continue;
    }
    cutFlowTable.update(">= 4 presel leptons");
    cutFlowHistManager->fillHistograms(">= 4 presel leptons", lumiScale);
    const RecoLepton* preselLepton_lead = preselLeptonsFull[0];
    const RecoLepton* preselLepton_sublead = preselLeptonsFull[1];
    const RecoLepton* preselLepton_third = preselLeptonsFull[2];
    const RecoLepton* preselLepton_fourth = preselLeptonsFull[3];
    const leptonGenMatchEntry& preselLepton_genMatch = getLeptonGenMatch(leptonGenMatch_definitions, preselLepton_lead, preselLepton_sublead, preselLepton_third, preselLepton_fourth);
    int idxPreselLepton_genMatch = preselLepton_genMatch.idx_;
    assert(idxPreselLepton_genMatch != kGen_LeptonUndefined4);

    // apply requirement on jets (incl. b-tagged jets) and hadronic taus on preselection level
    if ( !((int)selJets.size() >= minNumJets) ) {
      if ( run_lumi_eventSelector ) {
    std::cout << "event " << eventInfo.str() << " FAILS selJets selection." << std::endl;
  printCollection("selJets", selJets);
      }
      continue;
    }
    cutFlowTable.update(Form(">= %i jets (1)", minNumJets));
    cutFlowHistManager->fillHistograms(">= N jets (1)", lumiScale);
    if ( !(selBJets_loose.size() >= 2 || selBJets_medium.size() >= 1) ) {
      if ( run_lumi_eventSelector ) {
    std::cout << "event " << eventInfo.str() << " FAILS selBJets selection." << std::endl;
	printCollection("selJets", selJets);
	printCollection("selBJets_loose", selBJets_loose);
	printCollection("selBJets_medium", selBJets_medium);
      }
      continue;
    }
    cutFlowTable.update(">= 2 loose b-jets || 1 medium b-jet (1)");
    cutFlowHistManager->fillHistograms(">= 2 loose b-jets || 1 medium b-jet (1)", lumiScale);

//--- compute MHT and linear MET discriminant (met_LD)
    const RecoMEt met = metReader->read();
    const Particle::LorentzVector mht_p4 = compMHT(fakeableLeptonsFull, fakeableHadTaus, selJets);
    const double met_LD = compMEt_LD(met.p4(), mht_p4);

//--- apply final event selection
    // require exactly two leptons passing tight selection criteria of final event selection
    if ( !(selLeptons.size() >= 4) ) {
      if ( run_lumi_eventSelector ) {
    std::cout << "event " << eventInfo.str() << " FAILS selLeptons selection." << std::endl;
	printCollection("selLeptons", selLeptons);
	//printCollection("preselLeptons", preselLeptons);
      }
      continue;
    }
    cutFlowTable.update(">= 4 sel leptons", 1.);
    cutFlowHistManager->fillHistograms(">= 4 sel leptons", 1.);
    const RecoLepton* selLepton_lead = selLeptons[0];
    int selLepton_lead_type = getLeptonType(selLepton_lead->pdgId());
    const RecoLepton* selLepton_sublead = selLeptons[1];
    int selLepton_sublead_type = getLeptonType(selLepton_sublead->pdgId());
    const RecoLepton* selLepton_third = selLeptons[2];
    int selLepton_third_type = getLeptonType(selLepton_third->pdgId());
    const RecoLepton* selLepton_fourth = selLeptons[3];
    int selLepton_fourth_type = getLeptonType(selLepton_fourth->pdgId());
    const leptonGenMatchEntry& selLepton_genMatch = getLeptonGenMatch(leptonGenMatch_definitions, selLepton_lead, selLepton_sublead, selLepton_third, selLepton_fourth);
    int idxSelLepton_genMatch = selLepton_genMatch.idx_;
    assert(idxSelLepton_genMatch != kGen_LeptonUndefined4);
    if ( isDEBUG ) {
      std::cout << "selLepton_genMatch = " << getLeptonGenMatch_string(leptonGenMatch_definitions, idxSelLepton_genMatch) << std::endl;
      if ( idxSelLepton_genMatch != kGen_4l0g0j ) {
	std::cout << "--> CHECK!" << std::endl;
	printCollection("selLeptons", selLeptons);
      }
    }

//--- compute event-level weight for data/MC correction of b-tagging efficiency and mistag rate
//   (using the method "Event reweighting using scale factors calculated with a tag and probe method",
//    described on the BTV POG twiki https://twiki.cern.ch/twiki/bin/view/CMS/BTagShapeCalibration )
    double evtWeight = 1.;
    double btagWeight = 1.;
    if ( isMC ) {
      evtWeight *= evtWeight_inclusive;
      btagWeight = get_BtagWeight(selJets);
      evtWeight *= btagWeight;
      if ( isDEBUG ) {
	std::cout << "lumiScale = " << lumiScale << std::endl;
	if ( apply_genWeight ) std::cout << "genWeight = " << boost::math::sign(eventInfo.genWeight) << std::endl;
	std::cout << "pileupWeight = " << eventInfo.pileupWeight << std::endl;
	std::cout << "btagWeight = " << btagWeight << std::endl;
      }
    }

    double weight_data_to_MC_correction = 1.;
    double triggerWeight = 1.;
    double leptonSF_weight = 1.;
    if ( isMC ) {
      dataToMCcorrectionInterface->setLeptons(
        selLepton_lead_type, selLepton_lead->pt(), selLepton_lead->eta(),
	selLepton_sublead_type, selLepton_sublead->pt(), selLepton_sublead->eta(),
        selLepton_third_type, selLepton_third->pt(), selLepton_third->eta(),
        selLepton_fourth_type, selLepton_fourth->pt(), selLepton_fourth->eta());

//--- apply data/MC corrections for trigger efficiency
      double sf_triggerEff = dataToMCcorrectionInterface->getSF_leptonTriggerEff();
      if ( isDEBUG ) {
	std::cout << "sf_triggerEff = " << sf_triggerEff << std::endl;
      }
      triggerWeight *= sf_triggerEff;
      weight_data_to_MC_correction *= sf_triggerEff;

//--- apply data/MC corrections for efficiencies for lepton to pass loose identification and isolation criteria
      leptonSF_weight *= dataToMCcorrectionInterface->getSF_leptonID_and_Iso_loose();

//--- apply data/MC corrections for efficiencies of leptons passing the loose identification and isolation criteria
//    to also pass the tight identification and isolation criteria
      if ( electronSelection == kFakeable && muonSelection == kFakeable ) {
        leptonSF_weight *= dataToMCcorrectionInterface->getSF_leptonID_and_Iso_fakeable_to_loose();
      } else if ( electronSelection >= kFakeable && muonSelection >= kFakeable ) {
        // apply loose-to-tight lepton ID SFs if either of the following is true:
        // 1) both electron and muon selections are tight -> corresponds to SR
        // 2) electron selection is relaxed to fakeable and muon selection is kept as tight -> corresponds to MC closure w/ relaxed e selection
        // 3) muon selection is relaxed to fakeable and electron selection is kept as tight -> corresponds to MC closure w/ relaxed mu selection
        // we allow (2) and (3) so that the MC closure regions would more compatible w/ the SR (1) in comparison
        leptonSF_weight *= dataToMCcorrectionInterface->getSF_leptonID_and_Iso_tight_to_loose_woTightCharge();
      }
      weight_data_to_MC_correction *= leptonSF_weight;

      evtWeight *= weight_data_to_MC_correction;
    }

    double weight_fakeRate = 1.;
    if ( !selectBDT ) {
      if ( applyFakeRateWeights == kFR_4lepton ) {
	double prob_fake_lepton_lead = 1.;
	if      ( std::abs(selLepton_lead->pdgId()) == 11 ) prob_fake_lepton_lead = leptonFakeRateInterface->getWeight_e(selLepton_lead->cone_pt(), selLepton_lead->absEta());
	else if ( std::abs(selLepton_lead->pdgId()) == 13 ) prob_fake_lepton_lead = leptonFakeRateInterface->getWeight_mu(selLepton_lead->cone_pt(), selLepton_lead->absEta());
	else assert(0);
	bool passesTight_lepton_lead = isMatched(*selLepton_lead, tightElectrons) || isMatched(*selLepton_lead, tightMuons);
	double prob_fake_lepton_sublead = 1.;
	if      ( std::abs(selLepton_sublead->pdgId()) == 11 ) prob_fake_lepton_sublead = leptonFakeRateInterface->getWeight_e(selLepton_sublead->cone_pt(), selLepton_sublead->absEta());
	else if ( std::abs(selLepton_sublead->pdgId()) == 13 ) prob_fake_lepton_sublead = leptonFakeRateInterface->getWeight_mu(selLepton_sublead->cone_pt(), selLepton_sublead->absEta());
	else assert(0);
	bool passesTight_lepton_sublead = isMatched(*selLepton_sublead, tightElectrons) || isMatched(*selLepton_sublead, tightMuons);
	double prob_fake_lepton_third = 1.;
	if      ( std::abs(selLepton_third->pdgId()) == 11 ) prob_fake_lepton_third = leptonFakeRateInterface->getWeight_e(selLepton_third->cone_pt(), selLepton_third->absEta());
	else if ( std::abs(selLepton_third->pdgId()) == 13 ) prob_fake_lepton_third = leptonFakeRateInterface->getWeight_mu(selLepton_third->cone_pt(), selLepton_third->absEta());
	else assert(0);
	bool passesTight_lepton_third = isMatched(*selLepton_third, tightElectrons) || isMatched(*selLepton_third, tightMuons);
	double prob_fake_lepton_fourth = 1.;
	if      ( std::abs(selLepton_fourth->pdgId()) == 11 ) prob_fake_lepton_fourth = leptonFakeRateInterface->getWeight_e(selLepton_fourth->cone_pt(), selLepton_fourth->absEta());
	else if ( std::abs(selLepton_fourth->pdgId()) == 13 ) prob_fake_lepton_fourth = leptonFakeRateInterface->getWeight_mu(selLepton_fourth->cone_pt(), selLepton_fourth->absEta());
	else assert(0);
	bool passesTight_lepton_fourth = isMatched(*selLepton_fourth, tightElectrons) || isMatched(*selLepton_fourth, tightMuons);
        weight_fakeRate = getWeight_4L(
          prob_fake_lepton_lead, passesTight_lepton_lead,
	  prob_fake_lepton_sublead, passesTight_lepton_sublead,
	  prob_fake_lepton_third, passesTight_lepton_third,
	  prob_fake_lepton_fourth, passesTight_lepton_fourth);
	if ( isDEBUG ) {
	  std::cout << "weight_fakeRate = " << weight_fakeRate << std::endl;
	}
	evtWeight *= weight_fakeRate;
      }
    }
    if ( isDEBUG ) {
      std::cout << "evtWeight = " << evtWeight << std::endl;
    }

//--- apply HLT filter
    if(apply_hlt_filter)
    {
      const std::map<hltPathsE, bool> trigger_bits = {
        { hltPathsE::trigger_1e,    selTrigger_1e    },
        { hltPathsE::trigger_1mu,   selTrigger_1mu   },
        { hltPathsE::trigger_2e,    selTrigger_2e    },
        { hltPathsE::trigger_2mu,   selTrigger_2mu   },
        { hltPathsE::trigger_1e1mu, selTrigger_1e1mu },
        { hltPathsE::trigger_1e2mu, selTrigger_1e2mu },
        { hltPathsE::trigger_2e1mu, selTrigger_2e1mu },
        { hltPathsE::trigger_3e,    selTrigger_3e    },
        { hltPathsE::trigger_3mu,   selTrigger_3mu   },
      };
      if(! hltFilter(trigger_bits, fakeableLeptons, {}))
      {
        if(run_lumi_eventSelector || isDEBUG)
        {
          std::cout << "event " << eventInfo.str() << " FAILS HLT filter matching\n";
        }
        continue;
      }
    }
    cutFlowTable.update("HLT filter matching", evtWeight);
    cutFlowHistManager->fillHistograms("HLT filter matching", evtWeight);

    // apply requirement on jets (incl. b-tagged jets) and hadronic taus on level of final event selection
    if ( !((int)selJets.size() >= minNumJets) ) {
      if ( run_lumi_eventSelector ) {
    std::cout << "event " << eventInfo.str() << " FAILS selJets selection." << std::endl;
	printCollection("selJets", selJets);
      }
      continue;
    }
    cutFlowTable.update(Form(">= %i jets (2)", minNumJets), evtWeight);
    cutFlowHistManager->fillHistograms(">= N jets (2)", evtWeight);
    if ( !(selBJets_loose.size() >= 2 || selBJets_medium.size() >= 1)) {
      if ( run_lumi_eventSelector ) {
    std::cout << "event " << eventInfo.str() << " FAILS selBJets selection." << std::endl;
	printCollection("selJets", selJets);
	printCollection("selBJets_loose", selBJets_loose);
	printCollection("selBJets_medium", selBJets_medium);
      }
      continue;
    }
    cutFlowTable.update(">= 2 loose b-jets || 1 medium b-jet (2)", evtWeight);
    cutFlowHistManager->fillHistograms(">= 2 loose b-jets || 1 medium b-jet (2)", evtWeight);

    bool failsLowMassVeto = false;
    for ( std::vector<const RecoLepton*>::const_iterator lepton1 = preselLeptonsFull.begin();
    lepton1 != preselLeptonsFull.end(); ++lepton1 ) {
      for ( std::vector<const RecoLepton*>::const_iterator lepton2 = lepton1 + 1;
      lepton2 != preselLeptonsFull.end(); ++lepton2 ) {
	double mass = ((*lepton1)->p4() + (*lepton2)->p4()).mass();
	if ( mass < 12. ) {
	  failsLowMassVeto = true;
	}
      }
    }
    if ( failsLowMassVeto ) {
      if ( run_lumi_eventSelector ) {
    std::cout << "event " << eventInfo.str() << " FAILS low mass lepton pair veto." << std::endl;
      }
      continue;
    }
    cutFlowTable.update("m(ll) > 12 GeV", evtWeight);
    cutFlowHistManager->fillHistograms("m(ll) > 12 GeV", evtWeight);

    const double minPt_lead = 25.;
    const double minPt_sublead = 15.;
    const double minPt_third = 15.;
    const double minPt_fourth = 10.;
    // CV: according to Giovanni, the pT cuts should be applied on cone_pt
    //    (combined efficiency of single lepton, double lepton, and triple lepton triggers assumed to be high,
    //     even if one or two leptons and fakes and hence cone_pt may be significantly smaller than lepton_pt,
    //     on which pT cuts are applied on trigger level)
    if ( !(selLepton_lead->cone_pt() > minPt_lead && selLepton_sublead->cone_pt() > minPt_sublead &&
           selLepton_third->cone_pt() > minPt_third && selLepton_fourth->cone_pt() > minPt_fourth) ) {
      if ( run_lumi_eventSelector ) {
    std::cout << "event " << eventInfo.str() << " FAILS lepton pT selection." << std::endl;
	std::cout << " (leading selLepton pT = " << selLepton_lead->pt() << ", minPt_lead = " << minPt_lead
		  << ", subleading selLepton pT = " << selLepton_sublead->pt() << ", minPt_sublead = " << minPt_sublead
		  << ", third selLepton pT = " << selLepton_third->pt() << ", minPt_third = " << minPt_third
		  << ", fourth selLepton pT = " << selLepton_fourth->pt() << ", minPt_fourth = " << minPt_fourth << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update("lead lepton pT > 25 GeV && sublead lepton pT > 15 GeV && third lepton pT > 15 GeV && fourth lepton pT > 10 GeV", evtWeight);
    cutFlowHistManager->fillHistograms("lead lepton pT > 25 GeV && sublead lepton pT > 15 GeV && third lepton pT > 15 GeV && fourth lepton pT > 10 GeV", evtWeight);

    int sumLeptonCharge = selLepton_lead->charge() + selLepton_sublead->charge() + selLepton_third->charge() + selLepton_fourth->charge();
    bool isCharge_SS = std::abs(sumLeptonCharge) >  1;
    bool isCharge_OS = std::abs(sumLeptonCharge) <= 1;
    if ( chargeSumSelection == kOS && isCharge_SS ) {
      if ( run_lumi_eventSelector ) {
    std::cout << "event " << eventInfo.str() << " FAILS lepton charge selection." << std::endl;
	std::cout << " (leading selLepton charge = " << selLepton_lead->charge()
		  << ", subleading selLepton charge = " << selLepton_sublead->charge()
		  << ", third selLepton charge = " << selLepton_third->charge()
		  << ", fourth selLepton charge = " << selLepton_fourth->charge() << ", chargeSumSelection = OS)" << std::endl;
      }
      continue;
    }
    if ( chargeSumSelection == kSS && isCharge_OS ) {
      if ( run_lumi_eventSelector ) {
    std::cout << "event " << eventInfo.str() << " FAILS lepton charge selection." << std::endl;
	std::cout << " (leading selLepton charge = " << selLepton_lead->charge()
		  << ", subleading selLepton charge = " << selLepton_sublead->charge()
		  << ", third selLepton charge = " << selLepton_third->charge()
		  << ", fourth selLepton charge = " << selLepton_fourth->charge() << ", chargeSumSelection = SS)" << std::endl;
      }
      continue;
    }
    cutFlowTable.update("sel lepton charge", evtWeight);
    cutFlowHistManager->fillHistograms("sel lepton charge", evtWeight);

    const bool failsZbosonMassVeto = isfailsZbosonMassVeto(preselLeptonsFull);
    if(failsZbosonMassVeto)
    {
      if(run_lumi_eventSelector)
      {
        std::cout << "event " << eventInfo.str() << " FAILS Z-boson veto.\n";
      }
      continue;
    }
    cutFlowTable.update("Z-boson mass veto", evtWeight);
    cutFlowHistManager->fillHistograms("Z-boson mass veto", evtWeight);

    const bool failsHtoZZVeto = isfailsHtoZZVeto(preselLeptonsFull);
    if(failsHtoZZVeto)
    {
      if(run_lumi_eventSelector)
      {
        std::cout << "event " << eventInfo.str() << " FAILS H->ZZ*->4l veto.\n";
      }
      continue;
    }
    cutFlowTable.update("H->ZZ*->4l veto", evtWeight);
    cutFlowHistManager->fillHistograms("H->ZZ*->4l veto", evtWeight);

    bool isSameFlavor_OS_FO = false;
    for ( std::vector<const RecoLepton*>::const_iterator lepton1 = fakeableLeptons.begin();
    lepton1 != fakeableLeptons.end(); ++lepton1 ) {
      for ( std::vector<const RecoLepton*>::const_iterator lepton2 = lepton1 + 1;
      lepton2 != fakeableLeptons.end(); ++lepton2 ) {
	if ( (*lepton1)->pdgId() == -(*lepton2)->pdgId() ) { // pair of same flavor leptons of opposite charge
	  isSameFlavor_OS_FO = true;
	}
      }
    }

    double met_LD_cut = 0.;
    if      ( selJets.size() >= 4 ) met_LD_cut = -1.; // MET LD cut not applied
    else if ( isSameFlavor_OS_FO     ) met_LD_cut = 45.;
    else                            met_LD_cut = 30.;
    if ( met_LD_cut > 0 && met_LD < met_LD_cut ) {
      if ( run_lumi_eventSelector ) {
    	std::cout << "event " << eventInfo.str() << " FAILS MET LD selection." << std::endl;
    	std::cout << " (met_LD = " << met_LD << ", met_LD_cut = " << met_LD_cut << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update("met LD", evtWeight);
    cutFlowHistManager->fillHistograms("met LD", evtWeight);

    if ( apply_met_filters ) {
      if ( !metFilterSelector(metFilters) ) {
	if ( run_lumi_eventSelector ) {
      std::cout << "event " << eventInfo.str() << " FAILS MEt filters." << std::endl;
	}
	continue;
      }
    }
    cutFlowTable.update("MEt filters", evtWeight);
    cutFlowHistManager->fillHistograms("MEt filters", evtWeight);

    bool failsSignalRegionVeto = false;
    if ( isMCClosure_e || isMCClosure_m ) {
      bool applySignalRegionVeto = (isMCClosure_e && countFakeElectrons(selLeptons) >= 1) || (isMCClosure_m && countFakeMuons(selLeptons) >= 1);
      if ( applySignalRegionVeto && tightLeptons.size() >= 4 ) failsSignalRegionVeto = true;
    } else if ( electronSelection == kFakeable || muonSelection == kFakeable ) {
      if ( tightLeptons.size() >= 4 ) failsSignalRegionVeto = true;
    }
    if ( failsSignalRegionVeto ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event " << eventInfo.str() << " FAILS overlap w/ the SR: "
	             "# tight leptons = " << tightLeptons.size() << " >= 4\n"
        ;
	printCollection("tightLeptons", tightLeptons);
      }
      continue; // CV: avoid overlap with signal region
    }
    cutFlowTable.update("signal region veto", evtWeight);
    cutFlowHistManager->fillHistograms("signal region veto", evtWeight);

//--- fill histograms with events passing final selection
    selHistManagerType* selHistManager = selHistManagers[idxSelLepton_genMatch];
    assert(selHistManager != 0);
    selHistManager->electrons_->fillHistograms(selElectrons, evtWeight);
    selHistManager->muons_->fillHistograms(selMuons, evtWeight);
    selHistManager->jets_->fillHistograms(selJets, evtWeight);
    selHistManager->leadJet_->fillHistograms(selJets, evtWeight);
    selHistManager->subleadJet_->fillHistograms(selJets, evtWeight);
    selHistManager->BJets_loose_->fillHistograms(selBJets_loose, evtWeight);
    selHistManager->leadBJet_loose_->fillHistograms(selBJets_loose, evtWeight);
    selHistManager->subleadBJet_loose_->fillHistograms(selBJets_loose, evtWeight);
    selHistManager->BJets_medium_->fillHistograms(selBJets_medium, evtWeight);
    selHistManager->met_->fillHistograms(met, mht_p4, met_LD, evtWeight);
    selHistManager->metFilters_->fillHistograms(metFilters, evtWeight);

    std::map<std::string, double> tH_weight_map;
    for(const std::string & evt_cat_str: evt_cat_strs)
    {
      const std::string evt_cat_str_query = evt_cat_str == default_cat_str ? get_tH_SM_str() : evt_cat_str;
      tH_weight_map[evt_cat_str] = isMC_tH ?
        evtWeight / evtWeight_tH_nom * eventInfo.genWeight_tH(evt_cat_str_query):
        evtWeight
      ;
    }
    for(const auto & kv: tH_weight_map)
    {
      selHistManager->evt_[kv.first]->fillHistograms(
        selElectrons.size(), selMuons.size(),
        selJets.size(), selBJets_loose.size(), selBJets_medium.size(),
        kv.second
      );
    }
    if(isSignal)
    {
      const std::string decayModeStr = eventInfo.getDecayModeString();
      if ( ( isMC_tH || isMC_VH ) && ( decayModeStr == "hzg" || decayModeStr == "hmm" ) ) continue;
      if(! decayModeStr.empty())
      {
        for(const auto & kv: tH_weight_map)
        {
          selHistManager -> evt_in_decayModes_[kv.first][decayModeStr] -> fillHistograms(
            selElectrons.size(),
            selMuons.size(),
            selJets.size(),
            selBJets_loose.size(),
            selBJets_medium.size(),
            kv.second
          );
        }
      }
    }
    selHistManager->evtYield_->fillHistograms(eventInfo, evtWeight);
    selHistManager->weights_->fillHistograms("genWeight", eventInfo.genWeight);
    selHistManager->weights_->fillHistograms("pileupWeight", eventInfo.pileupWeight);
    selHistManager->weights_->fillHistograms("triggerWeight", triggerWeight);
    selHistManager->weights_->fillHistograms("data_to_MC_correction", weight_data_to_MC_correction);
    selHistManager->weights_->fillHistograms("fakeRate", weight_fakeRate);

    if ( isMC ) {
      genEvtHistManager_afterCuts->fillHistograms(genElectrons, genMuons, genHadTaus, genPhotons, genJets, evtWeight_inclusive);
      lheInfoHistManager->fillHistograms(*lheInfoReader, evtWeight);
      if(eventWeightManager)
      {
        genEvtHistManager_afterCuts->fillHistograms(eventWeightManager, evtWeight_inclusive);
      }
    }

    if ( selEventsFile ) {
      (*selEventsFile) << eventInfo.run << ':' << eventInfo.lumi << ':' << eventInfo.event << '\n';
    }

    const bool isGenMatched = isMC &&
      contains(syncNtuple_genMatch, selLepton_genMatch.name_)
    ;

    if ( bdt_filler ) {
      double lep1_genLepPt = ( selLepton_lead->genLepton()    ) ? selLepton_lead->genLepton()->pt()    : 0.;
      double lep2_genLepPt = ( selLepton_sublead->genLepton() ) ? selLepton_sublead->genLepton()->pt() : 0.;
      double lep3_genLepPt = ( selLepton_third->genLepton()   ) ? selLepton_third->genLepton()->pt()   : 0.;
      double lep4_genLepPt = ( selLepton_fourth->genLepton()  ) ? selLepton_fourth->genLepton()->pt()  : 0.;

      //FR weights for bdt ntuple
      double prob_fake_lepton_lead = 1.;
      if      ( std::abs(selLepton_lead->pdgId()) == 11 ) prob_fake_lepton_lead = leptonFakeRateInterface->getWeight_e(selLepton_lead->cone_pt(), selLepton_lead->absEta());
      else if ( std::abs(selLepton_lead->pdgId()) == 13 ) prob_fake_lepton_lead = leptonFakeRateInterface->getWeight_mu(selLepton_lead->cone_pt(), selLepton_lead->absEta());
      double prob_fake_lepton_sublead = 1.;
      if      ( std::abs(selLepton_sublead->pdgId()) == 11 ) prob_fake_lepton_sublead = leptonFakeRateInterface->getWeight_e(selLepton_sublead->cone_pt(), selLepton_sublead->absEta());
      else if ( std::abs(selLepton_sublead->pdgId()) == 13 ) prob_fake_lepton_sublead = leptonFakeRateInterface->getWeight_mu(selLepton_sublead->cone_pt(), selLepton_sublead->absEta());
      double prob_fake_lepton_third = 1.;
      if      ( std::abs(selLepton_third->pdgId()) == 11 ) prob_fake_lepton_third = leptonFakeRateInterface->getWeight_e(selLepton_third->cone_pt(), selLepton_third->absEta());
      else if ( std::abs(selLepton_third->pdgId()) == 13 ) prob_fake_lepton_third = leptonFakeRateInterface->getWeight_mu(selLepton_third->cone_pt(), selLepton_third->absEta());
      double prob_fake_lepton_fourth = 1.;
      if      ( std::abs(selLepton_fourth->pdgId()) == 11 ) prob_fake_lepton_fourth = leptonFakeRateInterface->getWeight_e(selLepton_fourth->cone_pt(), selLepton_fourth->absEta());
      else if ( std::abs(selLepton_fourth->pdgId()) == 13 ) prob_fake_lepton_fourth = leptonFakeRateInterface->getWeight_mu(selLepton_fourth->cone_pt(), selLepton_fourth->absEta());

      bdt_filler -> operator()({ eventInfo.run, eventInfo.lumi, eventInfo.event })
          ("lep1_pt",             selLepton_lead -> pt())
          ("lep1_conePt",         comp_lep1_conePt(*selLepton_lead))
          ("lep1_eta",            selLepton_lead -> eta())
          ("lep1_tth_mva",        selLepton_lead -> mvaRawTTH())
          ("mindr_lep1_jet",      TMath::Min(10., comp_mindr_lep1_jet(*selLepton_lead, selJets)))
          ("mT_lep1",             comp_MT_met_lep1(*selLepton_lead, met.pt(), met.phi()))
          ("lep2_pt",             selLepton_sublead -> pt())
          ("lep2_conePt",         comp_lep1_conePt(*selLepton_sublead))
          ("lep2_eta",            selLepton_sublead -> eta())
          ("lep2_tth_mva",        selLepton_sublead -> mvaRawTTH())
          ("mindr_lep2_jet",      TMath::Min(10., comp_mindr_lep1_jet(*selLepton_sublead, selJets)))
          ("mT_lep2",             comp_MT_met_lep1(*selLepton_sublead, met.pt(), met.phi()))
          ("lep3_pt",             selLepton_third -> pt())
          ("lep3_conePt",         comp_lep1_conePt(*selLepton_third))
          ("lep3_eta",            selLepton_third -> eta())
          ("lep3_tth_mva",        selLepton_third -> mvaRawTTH())
          ("mindr_lep3_jet",      TMath::Min(10., comp_mindr_lep1_jet(*selLepton_third, selJets)))
          ("mT_lep3",             comp_MT_met_lep1(*selLepton_third, met.pt(), met.phi()))
          ("lep4_pt",             selLepton_fourth -> pt())
          ("lep4_conePt",         comp_lep1_conePt(*selLepton_fourth))
          ("lep4_eta",            selLepton_fourth -> eta())
          ("lep4_tth_mva",        selLepton_fourth -> mvaRawTTH())
          ("mindr_lep4_jet",      TMath::Min(10., comp_mindr_lep1_jet(*selLepton_fourth, selJets)))
          ("mT_lep4",             comp_MT_met_lep1(*selLepton_fourth, met.pt(), met.phi()))
          ("avg_dr_jet",          comp_avg_dr_jet(selJets))
          ("ptmiss",              met.pt())
          ("htmiss",              mht_p4.pt())
          ("dr_leps",             deltaR(selLepton_lead -> p4(), selLepton_sublead -> p4()))
          ("lep1_genLepPt",       (selLepton_lead->genLepton() != 0) ? selLepton_lead->genLepton()->pt() : 0.)
          ("lep2_genLepPt",       (selLepton_sublead->genLepton() != 0) ? selLepton_sublead->genLepton() ->pt() : 0.)
          ("lep3_genLepPt",       (selLepton_third->genLepton() != 0) ? selLepton_third->genLepton() ->pt() : 0.)
          ("lep4_genLepPt",       (selLepton_fourth->genLepton() != 0) ? selLepton_fourth->genLepton() ->pt() : 0.)
          ("lep1_frWeight",       lep1_genLepPt > 0 ? 1.0 : prob_fake_lepton_lead)
          ("lep2_frWeight",       lep2_genLepPt > 0 ? 1.0 : prob_fake_lepton_sublead)
          ("lep3_frWeight",       lep3_genLepPt > 0 ? 1.0 : prob_fake_lepton_third)
          ("lep4_frWeight",       lep4_genLepPt > 0 ? 1.0 : prob_fake_lepton_fourth)
          ("lep1_fake_prob",      prob_fake_lepton_lead)
          ("lep2_fake_prob",      prob_fake_lepton_sublead)
          ("lep3_fake_prob",      prob_fake_lepton_third)
          ("lep4_fake_prob",      prob_fake_lepton_fourth)
          ("lumiScale",           lumiScale)
          ("genWeight",           eventInfo.genWeight)
          ("evtWeight",           evtWeight)
          ("mbb_loose",           selBJets_loose.size()>1 ?  (selBJets_loose[0]->p4()+selBJets_loose[1]->p4()).mass() : -1000 )
          ("mbb_medium",          selBJets_medium.size()>1 ?  (selBJets_medium[0]->p4()+selBJets_medium[1]->p4()).mass() : -1000 )
          ("nJet",                selJets.size())
          ("nBJetLoose",          selBJets_loose.size())
          ("nBJetMedium",         selBJets_medium.size())
          ("lep1_isTight",        int(selLepton_lead -> isTight()))
          ("lep2_isTight",        int(selLepton_sublead -> isTight()))
          ("lep3_isTight",        int(selLepton_third -> isTight()))
          ("lep4_isTight",        int(selLepton_fourth -> isTight()))
        .fill()
      ;
    }

    if(snm)
    {
      const double mT_lep1        = comp_MT_met_lep1(selLepton_lead->p4(), met.pt(), met.phi());
      const double mT_lep2        = comp_MT_met_lep2(selLepton_sublead->p4(), met.pt(), met.phi());
      const double mT_lep3        = comp_MT_met_lep3(selLepton_third->p4(), met.pt(), met.phi());
      const double mT_lep4        = comp_MT_met_lep4(selLepton_fourth->p4(), met.pt(), met.phi());
      const double max_dr_jet     = comp_max_dr_jet(selJets);
      const double mbb            = selBJets_medium.size() > 1 ? (selBJets_medium[0]->p4() + selBJets_medium[1]->p4()).mass() : -1.;
      const double mbb_loose      = selBJets_loose.size() > 1 ? (selBJets_loose[0]->p4() + selBJets_loose[1]->p4()).mass() : -1.;
      const double avg_dr_jet     = comp_avg_dr_jet(selJets);
      const double mindr_lep1_jet = comp_mindr_lep1_jet(*selLepton_lead, selJets);
      const double mindr_lep2_jet = comp_mindr_lep2_jet(*selLepton_sublead, selJets);
      const double mindr_lep3_jet = comp_mindr_lep3_jet(*selLepton_third, selJets);
      const double mindr_lep4_jet = comp_mindr_lep4_jet(*selLepton_fourth, selJets);
      const double min_dr_lep_jet = std::min({ mindr_lep1_jet, mindr_lep2_jet, mindr_lep3_jet, mindr_lep4_jet });
      const double dr_leps        = deltaR(selLepton_lead->p4(), selLepton_sublead->p4());
      const double max_lep_eta    = std::max({
        selLepton_lead->absEta(), selLepton_sublead->absEta(), selLepton_third->absEta(), selLepton_fourth->absEta()
      });
      const int nLightJet         = selJets.size() - selBJets_loose.size() + selJetsForward.size();

      snm->read(eventInfo);
      snm->read(selLeptons);
      snm->read(preselMuons,     fakeableMuons,     tightMuons);
      snm->read(preselElectrons, fakeableElectrons, tightElectrons);
      snm->read(selJets);

      snm->read({
        triggers_1e, triggers_1mu, triggers_2e, triggers_1e1mu, triggers_2mu,
        triggers_3e, triggers_2e1mu, triggers_1e2mu, triggers_3mu
      });
      snm->read(isGenMatched, selBJets_medium.size(), selBJets_loose.size(), nLightJet);

      snm->read(met.pt(),                               FloatVariableType::PFMET);
      snm->read(met.phi(),                              FloatVariableType::PFMETphi);
      snm->read(mht_p4.pt(),                            FloatVariableType::MHT);
      snm->read(met_LD,                                 FloatVariableType::metLD);

      snm->read(mindr_lep1_jet,                         FloatVariableType::mindr_lep1_jet);
      snm->read(mindr_lep2_jet,                         FloatVariableType::mindr_lep2_jet);
      snm->read(mindr_lep3_jet,                         FloatVariableType::mindr_lep3_jet);
      snm->read(mindr_lep4_jet,                         FloatVariableType::mindr_lep4_jet);

      // mindr_tau1_jet not filled
      // mindr_tau2_jet not filled

      snm->read(avg_dr_jet,                             FloatVariableType::avg_dr_jet);
      // avr_dr_lep_tau not filled
      snm->read(max_dr_jet,                             FloatVariableType::max_dr_jet);
      // max_dr_lep_tau not filled
      // min_dr_tau_jet not filled
      // min_dr_lep_tau not filled
      snm->read(min_dr_lep_jet,                         FloatVariableType::min_dr_lep_jet);

      snm->read(dr_leps,                                FloatVariableType::dr_leps);
      // dr_taus not filled

      // dr_lep_tau_ss not filled
      // dr_lep1_tau1 not filled
      // dr_lep1_tau2 not filled
      // dr_lep2_tau1 not filled
      // dr_lep3_tau1 not filled
      // dr_lep2_tau2 not filled

      snm->read(max_lep_eta,                            FloatVariableType::max_lep_eta);

      snm->read(mT_lep1,                                FloatVariableType::mT_met_lep1);
      snm->read(mT_lep2,                                FloatVariableType::mT_met_lep2);
      snm->read(mT_lep3,                                FloatVariableType::mT_met_lep3);
      snm->read(mT_lep4,                                FloatVariableType::mT_met_lep4);

      // mTauTauVis not filled
      // mvis_l1tau not filled
      // mvis_l2tau not filled

      snm->read(mbb,                                    FloatVariableType::mbb);
      snm->read(mbb_loose,                              FloatVariableType::mbb_loose);

      // cosThetaS_hadTau not filled
      // HTT not filled
      // HadTop_pt not filled
      // Hj_tagger not filled

      // mvaOutput_plainKin_ttV not filled
      // mvaOutput_plainKin_tt not filled
      // mvaOutput_plainKin_1B_VT not filled
      // mvaOutput_HTT_SUM_VT not filled

      // mvaOutput_plainKin_SUM_VT not filled

      // mvaOutput_2lss_ttV not filled
      // mvaOutput_2lss_tt not filled
      // mvaOutput_2lss_1tau_plainKin_tt not filled
      // mvaOutput_2lss_1tau_plainKin_ttV not filled
      // mvaOutput_2lss_1tau_plainKin_1B_M not filled
      // mvaOutput_2lss_1tau_plainKin_SUM_M not filled
      // mvaOutput_2lss_1tau_HTT_SUM_M not filled
      // mvaOutput_2lss_1tau_HTTMEM_SUM_M not filled

      // mvaOutput_3l_ttV not filled
      // mvaOutput_3l_ttbar
      // mvaOutput_plainKin_SUM_M not filled
      // mvaOutput_plainKin_1B_M not filled

      snm->read(weight_fakeRate,                        FloatVariableType::FR_weight);
      snm->read(triggerWeight,                          FloatVariableType::triggerSF_weight);
      snm->read(leptonSF_weight,                        FloatVariableType::leptonSF_weight);
      // tauSF_weight not filled
      snm->read(btagWeight,                             FloatVariableType::bTagSF_weight);
      snm->read(eventInfo.pileupWeight,                 FloatVariableType::PU_weight);
      snm->read(boost::math::sign(eventInfo.genWeight), FloatVariableType::MC_weight);

      // mvaOutput_2lss_ttV not filled
      // mvaOutput_2lss_tt not filled
      // mvaOutput_2lss_1tau_plainKin_tt not filled
      // mvaOutput_2lss_1tau_plainKin_ttV not filled
      // mvaOutput_2lss_1tau_plainKin_1B_M not filled
      // mvaOutput_2lss_1tau_plainKin_SUM_M not filled
      // mvaOutput_2lss_1tau_HTT_SUM_M not filled
      // mvaOutput_2lss_1tau_HTTMEM_SUM_M not filled

      snm->read(eventInfo.genWeight,                    FloatVariableType::genWeight);

      if(isGenMatched)
      {
        snm->fill();
      }
      else
      {
        snm->resetBranches();
      }
    }

    ++selectedEntries;
    selectedEntries_weighted += evtWeight;
    histogram_selectedEntries->Fill(0.);
  }

  if(snm)
  {
    snm->write();
  }

  std::cout << "max num. Entries = " << inputTree -> getCumulativeMaxEventCount()
            << " (limited by " << maxEvents << ") processed in "
            << inputTree -> getProcessedFileCount() << " file(s) (out of "
            << inputTree -> getFileCount() << ")\n"
            << " analyzed = " << analyzedEntries << '\n'
            << " selected = " << selectedEntries << " (weighted = " << selectedEntries_weighted << ")\n\n"
            << "cut-flow table" << std::endl;
  cutFlowTable.print(std::cout);
  std::cout << std::endl;

  std::cout << "sel. Entries by gen. matching:" << std::endl;
  for ( std::vector<leptonGenMatchEntry>::const_iterator leptonGenMatch_definition = leptonGenMatch_definitions.begin();
	leptonGenMatch_definition != leptonGenMatch_definitions.end(); ++leptonGenMatch_definition ) {

    std::string process_and_genMatch = process_string;
    if ( apply_leptonGenMatching ) process_and_genMatch += leptonGenMatch_definition->name_;

    int idxLepton = leptonGenMatch_definition->idx_;

    const TH1* histogram_EventCounter = selHistManagers[idxLepton]->evt_[default_cat_str]->getHistogram_EventCounter();
    std::cout << " " << process_and_genMatch << " = " << histogram_EventCounter->GetEntries() << " (weighted = " << histogram_EventCounter->Integral() << ")" << std::endl;
  }
  std::cout << std::endl;

  delete dataToMCcorrectionInterface;

  delete leptonFakeRateInterface;

  delete run_lumi_eventSelector;

  delete selEventsFile;

  delete muonReader;
  delete electronReader;
  delete jetReader;
  delete metReader;
  delete metFilterReader;
  delete genLeptonReader;
  delete genHadTauReader;
  delete genPhotonReader;
  delete genJetReader;
  delete lheInfoReader;

  delete genEvtHistManager_beforeCuts;
  delete genEvtHistManager_afterCuts;
  delete lheInfoHistManager;
  delete l1PreFiringWeightReader;
  delete cutFlowHistManager;
  delete eventWeightManager;

  hltPaths_delete(triggers_1e);
  hltPaths_delete(triggers_2e);
  hltPaths_delete(triggers_1mu);
  hltPaths_delete(triggers_2mu);
  hltPaths_delete(triggers_1e1mu);

  delete inputTree;
  delete snm;

  clock.Show("analyze_4l");

  return EXIT_SUCCESS;
}
