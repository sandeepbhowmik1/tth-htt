#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronReader.h" // RecoElectronReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonReader.h" // RecoMuonReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauReader.h" // RecoHadTauReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetReader.h" // RecoJetReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoMEtReader.h" // RecoMEtReader
#include "tthAnalysis/HiggsToTauTau/interface/GenLeptonReader.h" // GenLeptonReader
#include "tthAnalysis/HiggsToTauTau/interface/GenHadTauReader.h" // GenHadTauReader
#include "tthAnalysis/HiggsToTauTau/interface/GenJetReader.h" // GenJetReader
#include "tthAnalysis/HiggsToTauTau/interface/EventInfoReader.h" // EventInfoReader
#include "tthAnalysis/HiggsToTauTau/interface/convert_to_ptrs.h" // convert_to_ptrs
#include "tthAnalysis/HiggsToTauTau/interface/ParticleCollectionCleaner.h" // Reco*CollectionCleaner
#include "tthAnalysis/HiggsToTauTau/interface/ParticleCollectionGenMatcher.h" // Reco*CollectionGenMatcher
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorLoose.h" // RecoElectronCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorFakeable.h" // RecoElectronCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorTight.h" // RecoElectronCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorLoose.h" // RecoMuonCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorFakeable.h" // RecoMuonCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorTight.h" // RecoMuonCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorLoose.h" // RecoHadTauCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorFakeable.h" // RecoHadTauCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorTight.h" // RecoHadTauCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelector.h" // RecoJetCollectionSelector
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelectorBtag.h" // RecoJetCollectionSelectorBtagLoose, RecoJetCollectionSelectorBtagMedium
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronWriter.h" // RecoElectronWriter
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonWriter.h" // RecoMuonWriter
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauWriter.h" // RecoHadTauWriter
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetWriter.h" // RecoJetWriter
#include "tthAnalysis/HiggsToTauTau/interface/RecoMEtWriter.h" // RecoMEtWriter
#include "tthAnalysis/HiggsToTauTau/interface/GenParticleWriter.h" // GenParticleWriter
#include "tthAnalysis/HiggsToTauTau/interface/MEMPermutationWriter.h" // MEMPermutationWriter
#include "tthAnalysis/HiggsToTauTau/interface/EventInfoWriter.h" // EventInfoWriter
#include "tthAnalysis/HiggsToTauTau/interface/RunLumiEventSelector.h" // RunLumiEventSelector
#include "tthAnalysis/HiggsToTauTau/interface/cutFlowTable.h" // cutFlowTableType
#include "tthAnalysis/HiggsToTauTau/interface/histogramAuxFunctions.h" // createSubdirectory_recursively
#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // kEra_2017, kLoose, kFakeable, kTight
#include "tthAnalysis/HiggsToTauTau/interface/sysUncertOptions.h" // k*_central
#include "tthAnalysis/HiggsToTauTau/interface/leptonTypes.h" // getLeptonType(), kElectron, kMuon
#include "tthAnalysis/HiggsToTauTau/interface/branchEntryType.h"
#include "tthAnalysis/HiggsToTauTau/interface/branchEntryTypeAuxFunctions.h"
#include "tthAnalysis/HiggsToTauTau/interface/BranchAddressInitializer.h"

#include <FWCore/ParameterSet/interface/ParameterSet.h> // edm::ParameterSet
#include <FWCore/PythonParameterSet/interface/MakeParameterSets.h> // edm::readPSetsFrom()
#include <PhysicsTools/FWLite/interface/TFileService.h> // fwlite::TFileService
#include <DataFormats/FWLite/interface/InputSource.h> // fwlite::InputSource
#include <DataFormats/FWLite/interface/OutputFiles.h> // fwlite::OutputFiles
#include <FWCore/Utilities/interface/Parse.h> // edm::tokenize()

#include <TChain.h> // TChain
#include <TBenchmark.h> // TBenchmark

typedef std::vector<std::string> vstring;

template <typename GenParticleType>
std::vector<GenParticle>
convert_to_GenParticle(const std::vector<GenParticleType> & genOtherObjects)
{
  std::vector<GenParticle> genParticles;
  std::transform(
    genOtherObjects.cbegin(), genOtherObjects.cend(), std::back_inserter(genParticles),
    [](const GenParticleType & genOtherObject) -> const GenParticle
    {
      return static_cast<GenParticle>(genOtherObject);
    }
  );
  return genParticles;
}

/**
 * @brief Produce Ntuple containing preselected events,
 *        drop branches not needed for ttH, H->tautau analysis,
 *        and add flag indicating for which events the MEM computation is to be run
 *       (the latter is necessary in order to split MEM computation into jobs that each perform ~50 MEM integrations)
 */
int
main(int argc,
     char ** argv)
{
//--- throw an exception in case ROOT encounters an error
  gErrorAbortLevel = kError;

//--- parse command-line arguments
  if(argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " [parameters.py]\n";
    return EXIT_FAILURE;
  }

  std::cout << "<produceNtuple>:\n";

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("produceNtuple");

//--- read python configuration parameters
  if(! edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process"))
  {
    throw cmsException("produceNtuple", __LINE__)
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << "";
  }

  const edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");
  const edm::ParameterSet cfg_produceNtuple = cfg.getParameter<edm::ParameterSet>("produceNtuple");
  const std::string treeName = cfg_produceNtuple.getParameter<std::string>("treeName");

  const std::string era_string = cfg_produceNtuple.getParameter<std::string>("era");
  const int era = get_era(era_string);

  const std::string leptonSelection_string = cfg_produceNtuple.getParameter<std::string>("leptonSelection");
  const int leptonSelection = get_selection(leptonSelection_string);

  const std::string hadTauSelection_string = cfg_produceNtuple.getParameter<std::string>("hadTauSelection");
  const std::vector<std::string> hadTauSelection_parts = edm::tokenize(hadTauSelection_string, "|");
  assert(hadTauSelection_parts.size());
  const int hadTauSelection = get_selection(hadTauSelection_parts[0]);
  const std::string hadTauSelection_tauIDwp = hadTauSelection_parts[1];

  const std::string branchName_electrons  = cfg_produceNtuple.getParameter<std::string>("branchName_electrons");
  const std::string branchName_muons      = cfg_produceNtuple.getParameter<std::string>("branchName_muons");
  const std::string branchName_hadTaus    = cfg_produceNtuple.getParameter<std::string>("branchName_hadTaus");
  const std::string branchName_jets       = cfg_produceNtuple.getParameter<std::string>("branchName_jets");
  const std::string branchName_met        = cfg_produceNtuple.getParameter<std::string>("branchName_met");
  const std::string branchName_genLeptons = cfg_produceNtuple.getParameter<std::string>("branchName_genLeptons");
  const std::string branchName_genHadTaus = cfg_produceNtuple.getParameter<std::string>("branchName_genHadTaus");
  const std::string branchName_genJets    = cfg_produceNtuple.getParameter<std::string>("branchName_genJets");

  const int minNumLeptons             = cfg_produceNtuple.getParameter<int>("minNumLeptons");
  const int minNumHadTaus             = cfg_produceNtuple.getParameter<int>("minNumHadTaus");
  const int minNumLeptons_and_HadTaus = cfg_produceNtuple.getParameter<int>("minNumLeptons_and_HadTaus");
  const int minNumJets                = cfg_produceNtuple.getParameter<int>("minNumJets");
  const int minNumBJets_loose         = cfg_produceNtuple.getParameter<int>("minNumBJets_loose");
  const int minNumBJets_medium        = cfg_produceNtuple.getParameter<int>("minNumBJets_medium");
  
  const bool isMC                            = cfg_produceNtuple.getParameter<bool>("isMC");
  const bool isDEBUG                         = cfg_produceNtuple.getParameter<bool>("isDEBUG");
  const bool use_HIP_mitigation_mediumMuonId = cfg_produceNtuple.getParameter<bool>("use_HIP_mitigation_mediumMuonId");
  const bool useNonNominal                   = cfg_produceNtuple.getParameter<bool>("useNonNominal");
  const bool useNonNominal_jetmet            = useNonNominal || ! isMC;

  const std::string selEventsFileName_input = cfg_produceNtuple.getParameter<std::string>("selEventsFileName_input");
  std::cout << "selEventsFileName_input = " << selEventsFileName_input << '\n';
  RunLumiEventSelector * const run_lumi_eventSelector = selEventsFileName_input.empty() ?
    nullptr                                                                             :
    new RunLumiEventSelector(selEventsFileName_input)
  ;

  const vstring copy_histograms = cfg_produceNtuple.getParameter<vstring>("copy_histograms");

  const fwlite::InputSource inputFiles(cfg);
  const int maxEvents = inputFiles.maxEvents();
  std::cout << " maxEvents = " << maxEvents << '\n';
  const unsigned reportEvery = inputFiles.reportAfter();

  const fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

  TChain * const inputTree = new TChain(treeName.data());
  for(const std::string & inputFileName: inputFiles.files())
  {
    std::cout << "input Tree: adding file = " << inputFileName << '\n';
    inputTree->AddFile(inputFileName.data());
  }

  if(inputTree->GetListOfFiles()->GetEntries() < 1)
  {
    throw cmsException("produceNtuple", __LINE__) << "Failed to identify input Tree";
  }

  std::cout << "input Tree contains " << inputTree->GetEntries() << " entries in "
            << inputTree->GetListOfFiles()->GetEntries() << " files\n";
  
//--- declare event-level variables
  EventInfo eventInfo(false, false, false);
  EventInfoReader eventInfoReader(&eventInfo);
  eventInfoReader.setBranchAddresses(inputTree);

//--- declare particle collections
  RecoMuonReader * const muonReader = new RecoMuonReader(era, branchName_muons);
  muonReader->set_HIP_mitigation(use_HIP_mitigation_mediumMuonId);
  muonReader->setBranchAddresses(inputTree);
  const RecoMuonCollectionGenMatcher muonGenMatcher;
  const RecoMuonCollectionSelectorLoose preselMuonSelector(era, -1, isDEBUG);
  const RecoMuonCollectionSelectorFakeable fakeableMuonSelector(era, -1, isDEBUG);
  const RecoMuonCollectionSelectorTight tightMuonSelector(era, -1, isDEBUG);
  
  RecoElectronReader * const electronReader = new RecoElectronReader(era, branchName_electrons);
  electronReader->readUncorrected(useNonNominal);
  electronReader->setBranchAddresses(inputTree);
  const RecoElectronCollectionGenMatcher electronGenMatcher;
  const RecoElectronCollectionCleaner electronCleaner(0.05, isDEBUG);
  const RecoElectronCollectionSelectorLoose preselElectronSelector(era, -1, isDEBUG);
  const RecoElectronCollectionSelectorFakeable fakeableElectronSelector(era, -1, isDEBUG);
  const RecoElectronCollectionSelectorTight tightElectronSelector(era, -1, isDEBUG);

  double minPt_ele = -1.;
  double minPt_mu  = -1.;
  switch(era)
  {
    case kEra_2017: minPt_ele = 23.; minPt_mu = 18.; break;
    default:        throw cmsException("produceNtuple", __LINE__) << "Unsupported era = " << era;
  }

  RecoHadTauReader * const hadTauReader = new RecoHadTauReader(era, branchName_hadTaus);
  hadTauReader->setBranchAddresses(inputTree);
  const RecoHadTauCollectionGenMatcher hadTauGenMatcher;
  const RecoHadTauCollectionCleaner hadTauCleaner(0.3, isDEBUG);
  RecoHadTauCollectionSelectorLoose preselHadTauSelector(era, -1, isDEBUG);
  if(hadTauSelection_tauIDwp == "dR03mvaVLoose" ||
     hadTauSelection_tauIDwp == "dR03mvaVVLoose" )
  {
    preselHadTauSelector.set(hadTauSelection_tauIDwp);
  }
  preselHadTauSelector.set_min_antiElectron(-1);
  preselHadTauSelector.set_min_antiMuon(-1);
  RecoHadTauCollectionSelectorFakeable fakeableHadTauSelector(era, -1, isDEBUG);
  if(hadTauSelection_tauIDwp == "dR03mvaVLoose" ||
     hadTauSelection_tauIDwp == "dR03mvaVVLoose" )
  {
    fakeableHadTauSelector.set(hadTauSelection_tauIDwp);
  }
  fakeableHadTauSelector.set_min_antiElectron(-1);
  fakeableHadTauSelector.set_min_antiMuon(-1);
  RecoHadTauCollectionSelectorTight tightHadTauSelector(era, -1, isDEBUG);
  if(! hadTauSelection_tauIDwp.empty())
  {
    tightHadTauSelector.set(hadTauSelection_tauIDwp);
  }
  tightHadTauSelector.set_min_antiElectron(-1);
  tightHadTauSelector.set_min_antiMuon(-1);
  // CV: lower thresholds on hadronic taus by 2 GeV 
  //     with respect to thresholds applied on analysis level 
  //     to allow for tau-ES uncertainties to be estimated
  preselHadTauSelector.set_min_pt(18.); 
  fakeableHadTauSelector.set_min_pt(18.);
  tightHadTauSelector.set_min_pt(18.);
  std::cout << "hadTauSelection_tauIDwp = " << hadTauSelection_tauIDwp <<'\n';

  RecoJetReader * const jetReader = new RecoJetReader(era, isMC, branchName_jets);
  jetReader->setPtMass_central_or_shift(useNonNominal_jetmet ? kJet_central_nonNominal : kJet_central);
  jetReader->read_ptMass_systematics(isMC);
  jetReader->read_BtagWeight_systematics(isMC);
  jetReader->setBranchAddresses(inputTree);
  const RecoJetCollectionGenMatcher jetGenMatcher;
  const RecoJetCollectionCleaner jetCleaner(0.4, isDEBUG);
  const RecoJetSelector jetSelector(era);
  RecoJetCollectionSelectorBtagLoose jetSelectorBtagLoose(era, -1, isDEBUG);
  RecoJetCollectionSelectorBtagMedium jetSelectorBtagMedium(era, -1, isDEBUG);

//--- save the default settings of jetSelector
  const double min_jetSelector_pT     = jetSelector.get_min_pt();
  const double max_jetSelector_absEta = jetSelector.get_max_absEta();

//--- Disable pT cuts on the loose and medium b-tag selectors because we have to consider the effects
//    due to JECs: the jet pT may fluctuate up or down, and because of this the jet may fail pT requirement
//    depending on the choice of systematic uncertainties. The solution is to find maximum upwards
//    fluctuation in pT due to JEC and select only such cleaned jets that pass the pT cut in any systematic
//    setting. However, these jets that pass the pT cut due to JEC but otherwise wouldn't, would still be
//    cut out by the loose and medium b-tag selectors (they also employ the pT cut on top of b-tagging
//    requirements). The solution is to remove the pT cut in b-tagging selectors because the input jet
//    collection all already passes the pT cut in at least one choice of systematic uncertainties.
  jetSelectorBtagLoose.getSelector().set_min_pt(-1.);
  jetSelectorBtagMedium.getSelector().set_min_pt(-1.);

//--- declare missing transverse energy
  RecoMEtReader * const metReader = new RecoMEtReader(era, isMC, branchName_met);
  metReader->setMEt_central_or_shift(useNonNominal_jetmet ? kMEt_central_nonNominal : kMEt_central);
  metReader->read_ptPhi_systematics(isMC);
  metReader->setBranchAddresses(inputTree);

//--- declare generator level information
  GenLeptonReader * genLeptonReader = nullptr;
  GenHadTauReader * genHadTauReader = nullptr;
  GenJetReader * genJetReader = nullptr;
  if(isMC)
  {
    genLeptonReader = new GenLeptonReader(branchName_genLeptons);
    genLeptonReader->setBranchAddresses(inputTree);
    genHadTauReader = new GenHadTauReader(branchName_genHadTaus);
    genHadTauReader->setBranchAddresses(inputTree);
    genJetReader = new GenJetReader(branchName_genJets);
    genJetReader->setBranchAddresses(inputTree);
  }

  std::string outputTreeName = treeName;
  std::string outputDirName = "";
  if(outputTreeName.find_last_of("/") != std::string::npos)
  {
    std::size_t pos = outputTreeName.find_last_of("/");
    outputTreeName = std::string(outputTreeName, pos + 1);
    outputDirName = std::string(outputTreeName, 0, pos);
  }
  if(! outputDirName.empty())
  {
    TDirectory * const dir = createSubdirectory_recursively(fs, outputDirName.data());
    dir->cd();
  }
  else
  {
    fs.cd();
  }
  TTree * const outputTree = new TTree(outputTreeName.data(), outputTreeName.data());

  EventInfoWriter eventInfoWriter(false, false, false);
  eventInfoWriter.setBranches(outputTree);

  RecoMuonWriter * const muonWriter = new RecoMuonWriter(era, branchName_muons);
  muonWriter->setBranches(outputTree);
  std::cout << "writing RecoMuon objects to branch = '" << branchName_muons << "'\n";

  RecoElectronWriter * const electronWriter = new RecoElectronWriter(era, branchName_electrons);
  electronWriter->writeUncorrected(useNonNominal);
  electronWriter->setBranches(outputTree);
  std::cout << "writing RecoElectron objects to branch = '" << branchName_electrons << "'\n";

  RecoHadTauWriter * const hadTauWriter = new RecoHadTauWriter(era, branchName_hadTaus);
  hadTauWriter->setBranches(outputTree);
  std::cout << "writing RecoHadTau objects to branch = '" << branchName_hadTaus << "'\n";

  RecoJetWriter * const jetWriter = new RecoJetWriter(era, isMC, branchName_jets);
  jetWriter->setPtMass_central_or_shift(useNonNominal_jetmet ? kJet_central_nonNominal : kJet_central);
  jetWriter->write_ptMass_systematics(isMC);
  jetWriter->write_BtagWeight_systematics(isMC);
  jetWriter->setBranches(outputTree);
  std::cout << "writing RecoJet objects to branch = '" << branchName_jets << "'\n";

  RecoMEtWriter * const metWriter = new RecoMEtWriter(era, isMC, branchName_met);
  metWriter->setPtPhi_central_or_shift(useNonNominal_jetmet ? kJet_central_nonNominal : kJet_central);
  metWriter->write_ptPhi_systematics(isMC);
  metWriter->setBranches(outputTree);
  std::cout << "writing RecoMEt object to branch = '" << branchName_met << "'\n";

  GenParticleWriter * genLeptonWriter = nullptr;
  GenParticleWriter * genHadTauWriter = nullptr;
  GenParticleWriter * genJetWriter = nullptr;
  if(isMC)
  {
    genLeptonWriter = new GenParticleWriter(branchName_genLeptons);
    genLeptonWriter->setBranches(outputTree);
    std::cout << "writing GenLepton objects to branch = '" << branchName_genLeptons << "'\n";

    genHadTauWriter = new GenParticleWriter(branchName_genHadTaus);
    genHadTauWriter->setBranches(outputTree);
    std::cout << "writing GenHadTau objects to branch = '" << branchName_genHadTaus << "'\n";

    genJetWriter = new GenParticleWriter(branchName_genJets);
    genJetWriter->setBranches(outputTree);
    std::cout << "writing GenJet objects to branch = '" << branchName_genJets << "'\n";
  }

  // define the writer class for the nof MEM permutations
  MEMPermutationWriter memPermutationWriter;
  memPermutationWriter
    .setLepSelection       (leptonSelection, kTight)
    .setHadTauSelection    (hadTauSelection, kTight)
    .setHadTauWorkingPoints(hadTauSelection_tauIDwp) // up until Tight
  ;
  // add conditions for computing the nof MEM permutations in 2lss1tau and 3l1tau channels
  // the arguments are: the name of the channel, minimum number of leptons, minimum number of hadronic taus
  memPermutationWriter
    .addCondition("2lss_1tau", 2, 1)
    .addCondition("3l_1tau",   3, 1)
  ;
  memPermutationWriter.setBranchNames(outputTree, era, true);

  const std::vector<std::string> outputCommands_string = {
    "keep *",
    Form("drop %s", eventInfoWriter.getBranchName_run().data()),
    Form("drop %s", eventInfoWriter.getBranchName_lumi().data()),
    Form("drop %s", eventInfoWriter.getBranchName_event().data()),
    Form("drop n%s*", branchName_muons.data()),
    Form("drop %s_*", branchName_muons.data()),
    Form("drop n%s*", branchName_electrons.data()),
    Form("drop %s_*", branchName_electrons.data()),
    Form("drop n%s*", branchName_hadTaus.data()),
    Form("drop %s_*", branchName_hadTaus.data()),
    Form("drop n%s*", branchName_jets.data()),
    Form("drop %s_*", branchName_jets.data()),
    Form("drop %s_*", branchName_met.data()),
    Form("drop n%s", branchName_genLeptons.data()),
    Form("drop %s_*", branchName_genLeptons.data()),
    Form("drop n%s", branchName_genHadTaus.data()),
    Form("drop %s_*", branchName_genHadTaus.data()),
    Form("drop n%s", branchName_genJets.data()),
    Form("drop %s_*", branchName_genJets.data()),
    Form("drop maxPermutations_*"),
  };

  std::vector<outputCommandEntry> outputCommands = getOutputCommands(outputCommands_string);
  std::map<std::string, bool> isBranchToKeep = getBranchesToKeep(inputTree, outputCommands); // key = branchName
  std::map<std::string, branchEntryBaseType *> branchesToKeep; // key = branchName
  copyBranches_singleType(inputTree, outputTree, isBranchToKeep, branchesToKeep);
  copyBranches_vectorType(inputTree, outputTree, isBranchToKeep, branchesToKeep);

  if(isDEBUG)
  {
    std::cout << "keeping branches:\n";
    for(const auto & branchEntry: branchesToKeep)
    {
      std::cout << ' ' << branchEntry.second->outputBranchName_ << " (type ="
                   " " << branchEntry.second->outputBranchType_string_ << ")\n"
      ;
    }
  }

  const int numEntries = inputTree->GetEntries();
  int analyzedEntries = 0;
  int selectedEntries = 0;
  cutFlowTableType cutFlowTable;

  for(int idxEntry = 0; idxEntry < numEntries && (maxEvents == -1 || idxEntry < maxEvents); ++idxEntry)
  {
    inputTree->GetEntry(idxEntry);

    if(idxEntry > 0 && (idxEntry % reportEvery) == 0)
    {
      std::cout << "processing Entry " << idxEntry << ':' << eventInfo
                << " (" << selectedEntries << " Entries selected)\n"
      ;
    }
    ++analyzedEntries;
    
    cutFlowTable.update("read from file");

    if(run_lumi_eventSelector && !(*run_lumi_eventSelector)(eventInfo))
    {
      continue;
    }
    cutFlowTable.update("run:ls:event selection");

    if(run_lumi_eventSelector || isDEBUG)
    {
      std::cout << "processing Entry " << idxEntry << ':' << eventInfo << '\n';
      if(inputTree->GetFile())
      {
        std::cout << "input File = " << inputTree->GetFile()->GetName() << '\n';
      }
    }

//--- build collections of electrons, muons and hadronic taus;
//    resolve overlaps in order of priority: muon, electron,
    const std::vector<RecoMuon> muons = muonReader->read();
    const std::vector<const RecoMuon *> muon_ptrs = convert_to_ptrs(muons);
    // CV: no cleaning needed for muons, as they have the highest priority in the overlap removal
    const std::vector<const RecoMuon *> cleanedMuons  = muon_ptrs;
    const std::vector<const RecoMuon *> preselMuons   = preselMuonSelector  (cleanedMuons, isHigherPt);
    const std::vector<const RecoMuon *> fakeableMuons = fakeableMuonSelector(preselMuons,  isHigherPt);
    const std::vector<const RecoMuon *> tightMuons    = tightMuonSelector   (preselMuons,  isHigherPt);
    const std::vector<const RecoMuon *> selMuons      = selectObjects(
      leptonSelection, preselMuons, fakeableMuons, tightMuons
    );

    const std::vector<RecoElectron> electrons = electronReader->read();
    const std::vector<const RecoElectron *> electron_ptrs = convert_to_ptrs(electrons);
    const std::vector<const RecoElectron *> cleanedElectrons  = electronCleaner(electron_ptrs, fakeableMuons);
    const std::vector<const RecoElectron *> preselElectrons   = preselElectronSelector  (cleanedElectrons, isHigherPt);
    const std::vector<const RecoElectron *> fakeableElectrons = fakeableElectronSelector(preselElectrons,  isHigherPt);
    const std::vector<const RecoElectron *> tightElectrons    = tightElectronSelector   (preselElectrons,  isHigherPt);
    const std::vector<const RecoElectron *> selElectrons      = selectObjects(
      leptonSelection, preselElectrons, fakeableElectrons, tightElectrons
    );

    const std::vector<RecoHadTau> hadTaus = hadTauReader->read();
    const std::vector<const RecoHadTau *> hadTau_ptrs = convert_to_ptrs(hadTaus);
    const std::vector<const RecoHadTau *> cleanedHadTaus = hadTauCleaner(hadTau_ptrs, preselMuons, preselElectrons);
    const std::vector<const RecoHadTau *> preselHadTaus   = preselHadTauSelector  (cleanedHadTaus, isHigherPt);
    const std::vector<const RecoHadTau *> fakeableHadTaus = fakeableHadTauSelector(cleanedHadTaus, isHigherPt);
    const std::vector<const RecoHadTau *> tightHadTaus    = tightHadTauSelector   (cleanedHadTaus, isHigherPt);
    const std::vector<const RecoHadTau *> selHadTaus = selectObjects(
      hadTauSelection, preselHadTaus, fakeableHadTaus, tightHadTaus
    );

//--- build collections of jets and select subset of jets passing b-tagging criteria
    const std::vector<RecoJet> jets = jetReader->read();
    const std::vector<const RecoJet *> jet_ptrs = convert_to_ptrs(jets);
    // Karl: do not clean w.r.t the taus b/c their definition changes across the analyses
    //       we are better off if we keep a bit more jets per event
    const std::vector<const RecoJet *> cleanedJets = jetCleaner(jet_ptrs, fakeableMuons, fakeableElectrons);
    std::vector<const RecoJet *> selJets;
    for(const RecoJet * cleanedJet: cleanedJets)
    {
      // Karl: there are no JEC uncertainties for non-nominal (i.e. MET-adjusted) jet pT
      const double cleanedJet_pt_max = useNonNominal_jetmet ? cleanedJet->pt() : cleanedJet->maxPt();
      const double cleanedJet_absEta = cleanedJet->absEta();
      if(cleanedJet_pt_max >= min_jetSelector_pT && cleanedJet_absEta < max_jetSelector_absEta)
      {
        selJets.push_back(cleanedJet);
      }
    }

//--- sort the collection by their pT so that if we hit the limit of maximum number of objects
//--- in the Writer classes, we will drop the softer objects
    std::sort(selJets.begin(), selJets.end(), isHigherPt);
    const std::vector<const RecoJet *> selBJets_loose  = jetSelectorBtagLoose(selJets,  isHigherPt);
    const std::vector<const RecoJet *> selBJets_medium = jetSelectorBtagMedium(selJets, isHigherPt);

    RecoMEt met = metReader->read();

//--- construct the merged lepton collections
    const std::vector<const RecoLepton *> preselLeptons   = mergeLeptonCollections(preselElectrons,   preselMuons,   isHigherPt);
    const std::vector<const RecoLepton *> fakeableLeptons = mergeLeptonCollections(fakeableElectrons, fakeableMuons, isHigherPt);
    const std::vector<const RecoLepton *> tightLeptons    = mergeLeptonCollections(tightElectrons,    tightMuons,    isHigherPt);
    const std::vector<const RecoLepton *> selLeptons      = mergeLeptonCollections(selElectrons,      selMuons,      isHigherPt);

//--- apply preselection
    if(! (static_cast<int>(selLeptons.size()) >= minNumLeptons))
    {
      if(run_lumi_eventSelector || isDEBUG)
      {
        std::cout << "event FAILS selLeptons selection.\n"
                     " (#preselLeptons = " << preselLeptons.size() << ")\n"
        ;
        for(std::size_t idxPreselLepton = 0; idxPreselLepton < preselLeptons.size(); ++idxPreselLepton)
        {
          std::cout << "preselLepton #" << idxPreselLepton << ":\n" << (*preselLeptons[idxPreselLepton]);
        }
        std::cout << " (#selLeptons = " << selLeptons.size() << ")\n";
        for(std::size_t idxSelLepton = 0; idxSelLepton < selLeptons.size(); ++idxSelLepton)
        {
          std::cout << "selLepton #" << idxSelLepton << ":\n" << (*selLeptons[idxSelLepton]);
        }
      }
      continue;
    }
    cutFlowTable.update(Form(">= %i sel leptons", minNumLeptons));

    if(minNumLeptons > 0 && selLeptons.size() >= 1)
    {
      const RecoLepton * const selLepton_lead = selLeptons[0];
      // CV: lower threshold on leading lepton by 2 GeV 
      //     with respect to thresholds applied on analysis level
      //     to allow for e-ES and mu-ES uncertainties to be estimated
      const double minPt_lead = selLepton_lead -> is_electron() ? minPt_ele : minPt_mu;
      if(! (selLepton_lead->pt() > minPt_lead))
      {
        if(run_lumi_eventSelector || isDEBUG)
        {
          std::cout << "event FAILS lepton pT selection.\n"
                       "( leading selLepton "
                       "pT = "         << selLepton_lead->pt() << ", "
                       "minPt_lead = " << minPt_lead << ")\n"
          ;
        }
        continue;
      }
      cutFlowTable.update(Form("lead lepton pT > %.1f (electron) / %.1f (muon) GeV", minPt_ele, minPt_mu));
    }

    if(! (static_cast<int>(selHadTaus.size()) >= minNumHadTaus))
    {
      if(run_lumi_eventSelector || isDEBUG)
      {
        std::cout << "event FAILS selHadTaus selection\n"
                     " (#preselHadTaus = " << preselHadTaus.size() << ")\n"
        ;
        for(std::size_t idxPreselHadTau = 0; idxPreselHadTau < preselHadTaus.size(); ++idxPreselHadTau)
        {
          std::cout << "preselHadTau #" << idxPreselHadTau << ":\n" << (*preselHadTaus[idxPreselHadTau]);
        }
        std::cout << " (#selHadTaus = " << selHadTaus.size() << ")\n";
        for(std::size_t idxSelHadTau = 0; idxSelHadTau < selHadTaus.size(); ++idxSelHadTau )
        {
          std::cout << "selHadTau #" << idxSelHadTau << ":\n" << (*selHadTaus[idxSelHadTau]);
        }
      }
      continue;
    }
    cutFlowTable.update(Form(">= %i sel hadTaus", minNumHadTaus));

    if (! (static_cast<int>(selLeptons.size() + selHadTaus.size()) >= minNumLeptons_and_HadTaus))
    {
      continue;
    }
    cutFlowTable.update(Form(">= %i sel leptons+hadTaus", minNumLeptons_and_HadTaus));

    // apply requirement on jets 
    if(! (static_cast<int>(selJets.size()) >= minNumJets))
    {
      if(run_lumi_eventSelector || isDEBUG)
      {
        std::cout << "event FAILS selJets selection\n"
                     " (#selJets = " << selJets.size() << ")\n"
        ;
        for(std::size_t idxSelJet = 0; idxSelJet < selJets.size(); ++idxSelJet)
        {
          std::cout << "selJet #" << idxSelJet << ":\n" << (*selJets[idxSelJet]);
        }
      }
      continue;
    }
    cutFlowTable.update(Form(">= %i jets", minNumJets));

    // apply requirement on b-jets 
    if(! (static_cast<int>(selBJets_loose.size())  >= minNumBJets_loose  ||
          static_cast<int>(selBJets_medium.size()) >= minNumBJets_medium ))
    {
      if(run_lumi_eventSelector || isDEBUG)
      {
        std::cout << "event FAILS selBJets selection\n"
                     " (#selJets = " << selJets.size() << ")\n"
        ;
        for(std::size_t idxSelJet = 0; idxSelJet < selJets.size(); ++idxSelJet)
        {
          std::cout << "selJet #" << idxSelJet << ":\n" << (*selJets[idxSelJet]);
        }
        std::cout << " (#selBJets_loose = " << selBJets_loose.size() << ")\n";
        for(std::size_t idxSelBJet_loose = 0; idxSelBJet_loose < selBJets_loose.size(); ++idxSelBJet_loose)
        {
          std::cout << "selBJet_loose #" << idxSelBJet_loose << ":\n" << (*selBJets_loose[idxSelBJet_loose]);
        }
        std::cout << " (#selBJets_medium = " << selBJets_medium.size() << ")\n";
        for(std::size_t idxSelBJet_medium = 0; idxSelBJet_medium < selBJets_medium.size(); ++idxSelBJet_medium)
        {
          std::cout << "selBJet_medium #" << idxSelBJet_medium << ":\n" << (*selBJets_medium[idxSelBJet_medium]);
        }
      }
      continue;
    }
    cutFlowTable.update(Form(">= %i loose b-jets || %i medium b-jet", minNumBJets_loose, minNumBJets_medium));

    std::vector<GenLepton> genLeptons;
    std::vector<GenLepton> genElectrons;
    std::vector<GenLepton> genMuons;
    std::vector<GenHadTau> genHadTaus;
    std::vector<GenJet> genJets;
    if(isMC)
    {
//--- build collections of generator level particles
      genLeptons = genLeptonReader->read();
      for(const GenLepton genLepton: genLeptons)
      {
        const int genLeptonType = getLeptonType(genLepton.pdgId());
        switch(genLeptonType)
        {
          case kElectron: genElectrons.push_back(genLepton); break;
          case kMuon:     genMuons.push_back(genLepton);     break;
          default: assert(0);
        }
      }
      genHadTaus = genHadTauReader->read();
      genJets = genJetReader->read();

//--- match reconstructed to generator level particles
      muonGenMatcher.addGenLeptonMatch(preselMuons, genLeptons, 0.2);
      muonGenMatcher.addGenHadTauMatch(preselMuons, genHadTaus, 0.2);
      muonGenMatcher.addGenJetMatch   (preselMuons, genJets,    0.2);

      electronGenMatcher.addGenLeptonMatch(preselElectrons, genLeptons, 0.2);
      electronGenMatcher.addGenHadTauMatch(preselElectrons, genHadTaus, 0.2);
      electronGenMatcher.addGenJetMatch   (preselElectrons, genJets,    0.2);

      hadTauGenMatcher.addGenLeptonMatch(selHadTaus, genLeptons, 0.2);
      hadTauGenMatcher.addGenHadTauMatch(selHadTaus, genHadTaus, 0.2);
      hadTauGenMatcher.addGenJetMatch   (selHadTaus, genJets,    0.2);

      jetGenMatcher.addGenLeptonMatch(selJets, genLeptons, 0.2);
      jetGenMatcher.addGenHadTauMatch(selJets, genHadTaus, 0.2);
      jetGenMatcher.addGenJetMatch   (selJets, genJets,    0.2);
    }

    memPermutationWriter.write(
      {{preselLeptons, fakeableLeptons, tightLeptons}}, {{selBJets_loose, selBJets_medium}}, cleanedHadTaus
    );

    eventInfoWriter.write(eventInfo);
    muonWriter->write(preselMuons);
    electronWriter->write(preselElectrons);
    hadTauWriter->write(fakeableHadTaus);
    jetWriter->write(selJets);
    metWriter->write(met);

    if(isMC)
    {
      genLeptonWriter->write(convert_to_GenParticle(genLeptons));
      genHadTauWriter->write(convert_to_GenParticle(genHadTaus));
      genJetWriter   ->write(convert_to_GenParticle(genJets));
    }

    for(const auto & branchEntry: branchesToKeep)
    {
      branchEntry.second->copyBranch();
    }

    outputTree->Fill();
    ++selectedEntries;
  }

  std::cout << "num. Entries = "  << numEntries      << "\n"
               " analyzed = "     << analyzedEntries << "\n"
               " selected = "     << selectedEntries << "\n"
               "cut-flow table\n" << cutFlowTable    << '\n'
  ;

  if(isDEBUG)
  {
    std::cout << "output Tree:\n";
    outputTree->Print();
  }

  delete run_lumi_eventSelector;

  delete muonReader;
  delete electronReader;
  delete hadTauReader;
  delete jetReader;
  delete metReader;

  delete muonWriter;
  delete electronWriter;
  delete hadTauWriter;
  delete jetWriter;
  delete metWriter;

  delete genLeptonWriter;
  delete genHadTauWriter;
  delete genJetWriter;

//--- copy histograms keeping track of number of processed events from input files to output file
  std::cout << "copying histograms:\n";
  delete inputTree;
  std::map<std::string, TH1 *> histograms;
  for(const std::string & inputFileName: inputFiles.files())
  {
    TFile * const inputFile = new TFile(inputFileName.data());
    if(! inputFile)
    {
      throw cmsException("produceNtuple", __LINE__)
        << "Failed to open input File = '" << inputFileName << '\''
      ;
    }

    for(const std::string & histogramName: copy_histograms)
    {
      if(inputFiles.files().size() > 1)
      {
        std::cout << ' ' << histogramName << " from input File = '" << inputFileName << "'\n";
      }
      else
      {
        std::cout << ' ' << histogramName << '\n';
      }
      TH1 * const histogram_input = dynamic_cast<TH1 *>(inputFile->Get(histogramName.data()));
      if(! histogram_input)
      {
        continue;
      }

      TH1 * histogram_output = histograms[histogramName];
      if(histogram_output)
      {
        histogram_output->Add(histogram_input);
      }
      else
      {
        if(dynamic_cast<TH1F *>(histogram_input))
        {
          histogram_output = fs.make<TH1F>(*(dynamic_cast<TH1F *>(histogram_input)));
        }
        else if(dynamic_cast<TH1D*>(histogram_input))
        {
          histogram_output = fs.make<TH1D>(*(dynamic_cast<TH1D *>(histogram_input)));
        }
        assert(histogram_output);
        histograms[histogramName] = histogram_output;
      }
    }
    delete inputFile;
  }

  clock.Show("produceNtuple");
  return EXIT_SUCCESS;
}
