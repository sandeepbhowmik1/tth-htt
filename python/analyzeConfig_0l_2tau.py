import logging

from tthAnalysis.HiggsToTauTau.analyzeConfig_new import *
from tthAnalysis.HiggsToTauTau.jobTools import create_if_not_exists
from tthAnalysis.HiggsToTauTau.analysisTools import initDict, getKey, create_cfg, createFile, generateInputFileList

def get_hadTau_selection_and_frWeight(hadTau_selection, hadTau_frWeight):
  hadTau_selection_and_frWeight = hadTau_selection
  if hadTau_selection.startswith("Fakeable"):
    if hadTau_frWeight == "enabled":
      hadTau_selection_and_frWeight += "_wFakeRateWeights"
    elif hadTau_frWeight == "disabled":
      hadTau_selection_and_frWeight += "_woFakeRateWeights"
  hadTau_selection_and_frWeight = hadTau_selection_and_frWeight.replace("|", "_")    
  return hadTau_selection_and_frWeight

def getHistogramDir(hadTau_selection, hadTau_frWeight, hadTau_charge_selection):
  hadTau_selection_part1 = hadTau_selection
  for separator in [ "|" ]:
    if hadTau_selection_part1.find(separator) != -1:
      hadTau_selection_part1 = hadTau_selection_part1[:hadTau_selection_part1.find(separator)]
  histogramDir = "0l_2tau_%s_%s" % (hadTau_charge_selection, hadTau_selection_part1)
  if hadTau_selection_part1.find("Fakeable") != -1:
    if hadTau_frWeight == "enabled":
      histogramDir += "_wFakeRateWeights"
    elif hadTau_frWeight == "disabled":
      histogramDir += "_woFakeRateWeights"
  return histogramDir
  
class analyzeConfig_0l_2tau(analyzeConfig):
  """Configuration metadata needed to run analysis in a single go.
  
  Sets up a folder structure by defining full path names; no directory creation is delegated here.
  
  Args specific to analyzeConfig_0l_2tau:
    hadTau_selection: either `Tight`, `Loose` or `Fakeable`
    hadTau_charge_selection: either `OS` or `SS` (opposite-sign or same-sign)

  See $CMSSW_BASE/src/tthAnalysis/HiggsToTauTau/python/analyzeConfig.py
  for documentation of further Args.
  
  """
  def __init__(self, configDir, outputDir, executable_analyze, cfgFile_analyze, samples, hadTau_selection, hadTau_charge_selections, applyFakeRateWeights, central_or_shifts,
               max_files_per_job, era, use_lumi, lumi, debug, running_method, num_parallel_jobs, 
               executable_addBackgrounds, executable_addBackgroundJetToTauFakes, histograms_to_fit, select_rle_output = False, executable_prep_dcard="prepareDatacard"):
    analyzeConfig.__init__(self, configDir, outputDir, executable_analyze, "0l_2tau", central_or_shifts,
      max_files_per_job, era, use_lumi, lumi, debug, running_method, num_parallel_jobs, 
      histograms_to_fit)

    self.samples = samples
    
    ##self.hadTau_selections = [ "Tight", "Fakeable", "Fakeable_mcClosure" ]
    self.hadTau_selections = [ "Tight", "Fakeable" ]
    self.hadTau_frWeights = [ "enabled", "disabled" ]
    self.hadTau_selection_part2 = hadTau_selection
    self.hadTau_charge_selections = hadTau_charge_selections
    self.applyFakeRateWeights = applyFakeRateWeights
        
    self.hadTau_genMatches = [ "2t0e0m0j", "1t1e0m0j", "1t0e1m0j", "1t0e0m1j", "0t2e0m0j", "0t1e1m0j", "0t1e0m1j", "0t0e2m0j", "0t0e1m1j", "0t0e0m2j" ]
    
    self.apply_hadTauGenMatching = None
    self.hadTau_genMatches_nonfakes = []
    self.hadTau_genMatches_fakes = []
    if applyFakeRateWeights == "2tau":
      self.apply_leptonGenMatching = False
      self.apply_hadTauGenMatching = True
      for hadTau_genMatch in self.hadTau_genMatches:
        if hadTau_genMatch.endswith("0j"):
          self.hadTau_genMatches_nonfakes.append(hadTau_genMatch)
        else:
          self.hadTau_genMatches_fakes.append(hadTau_genMatch)
    else:
      raise ValueError("Invalid Configuration parameter 'applyFakeRateWeights' = %s !!" % applyFakeRateWeights)

    if era == '2016':
      self.triggers_2tau = [
        "HLT_BIT_HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v",
        "HLT_BIT_HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg_v"
      ]

    self.executable_addBackgrounds = executable_addBackgrounds
    self.executable_addFakes = executable_addBackgroundJetToTauFakes
    
    self.nonfake_backgrounds = [ "TT", "TTW", "TTZ", "EWK", "Rares" ]
    self.prep_dcard_processesToCopy = [ "data_obs" ] + self.nonfake_backgrounds + [ "fakes_data", "fakes_mc" ]
    ##self.make_plots_backgrounds = self.nonfake_backgrounds + [ "fakes_data" ]
    self.make_plots_backgrounds = [ "TTW", "TTZ", "EWK", "Rares", "fakes_data" ]
    
    self.cfgFile_analyze = os.path.join(self.workingDir, cfgFile_analyze)    
    self.histogramDir_prep_dcard = "0l_2tau_OS_Tight"
    self.histogramDir_prep_dcard_SS = "0l_2tau_SS_Tight"
    self.cfgFile_make_plots = os.path.join(self.workingDir, "makePlots_0l_2tau_cfg.py")
    self.cfgFile_make_plots_mcClosure = os.path.join(self.workingDir, "makePlots_mcClosure_0l_2tau_cfg.py")

    self.select_rle_output = select_rle_output

  def createCfg_analyze(self, jobOptions):
    """Create python configuration file for the analyze_0l_2tau executable (analysis code)

    Args:
      inputFiles: list of input files (Ntuples)
      outputFile: output file of the job -- a ROOT file containing histogram
      process: either `TT`, `TTW`, `TTZ`, `EWK`, `Rares`, `data_obs`, `ttH_hww`, `ttH_hzz` or `ttH_htt`
      is_mc: flag indicating whether job runs on MC (True) or data (False)
      lumi_scale: event weight (= xsection * luminosity / number of events)
      central_or_shift: either 'central' or one of the systematic uncertainties defined in $CMSSW_BASE/src/tthAnalysis/HiggsToTauTau/bin/analyze_0l_2tau.cc
    """  
    lines = []
    ##lines.append("process.fwliteInput.fileNames = cms.vstring(%s)" % [ os.path.basename(inputFile) for inputFile in jobOptions['ntupleFiles'] ])
    lines.append("process.fwliteInput.fileNames = cms.vstring(%s)" % jobOptions['ntupleFiles'])
    lines.append("process.fwliteOutput.fileName = cms.string('%s')" % os.path.basename(jobOptions['histogramFile']))
    lines.append("process.analyze_0l_2tau.process = cms.string('%s')" % jobOptions['sample_category'])
    hadTau_frWeight = None
    if jobOptions['applyFakeRateWeights'] == "disabled":
      hadTau_frWeight = "disabled"
    else:
      hadTau_frWeight = "enabled"
    histogramDir = getHistogramDir(jobOptions['hadTau_selection'], hadTau_frWeight, jobOptions['hadTau_charge_selection'])
    lines.append("process.analyze_0l_2tau.histogramDir = cms.string('%s')" % histogramDir)
    lines.append("process.analyze_0l_2tau.era = cms.string('%s')" % self.era)
    lines.append("process.analyze_0l_2tau.triggers_2tau = cms.vstring(%s)" % self.triggers_2tau)
    lines.append("process.analyze_0l_2tau.use_triggers_2tau = cms.bool(%s)" % ("2tau" in jobOptions['triggers']))
    lines.append("process.analyze_0l_2tau.hadTauSelection = cms.string('%s')" % jobOptions['hadTau_selection'])
    lines.append("process.analyze_0l_2tau.hadTauChargeSelection = cms.string('%s')" % jobOptions['hadTau_charge_selection'])
    lines.append("process.analyze_0l_2tau.apply_hadTauGenMatching = cms.bool(%s)" % (jobOptions['apply_hadTauGenMatching'] and jobOptions['is_mc']))
    lines.append("process.analyze_0l_2tau.applyFakeRateWeights = cms.string('%s')" % jobOptions['applyFakeRateWeights'])
    if jobOptions['hadTau_selection'].find("Fakeable") != -1 and jobOptions['applyFakeRateWeights'] in [ "2tau" ]:
      fitFunctionName = None
      if self.era == "2015":
        lines.append("process.analyze_0l_2tau.hadTauFakeRateWeight.inputFileName = cms.string('tthAnalysis/HiggsToTauTau/data/FR_tau_2015.root')")
        # CV: use data/MC corrections determined for dR03mvaLoose discriminator,
        #     as the event statistics in 2015 data is too low to determine data/MC corrections for tighter working-points
        fitFunctionName = "jetToTauFakeRate/dR03mvaLoose/$etaBin/fitFunction_data_div_mc_hadTaus_pt"        
      elif self.era == "2016":
        lines.append("process.analyze_0l_2tau.hadTauFakeRateWeight.inputFileName = cms.string('tthAnalysis/HiggsToTauTau/data/FR_tau_2016.root')")
        # CV: use data/MC corrections determined for dR03mvaMedium discriminator for 2016 data
        fitFunctionName = "jetToTauFakeRate/dR03mvaMedium/$etaBin/fitFunction_data_div_mc_hadTaus_pt"   
      else:
        raise ValueError("Invalid parameter 'era' = %s !!" % era)
      lines.append("process.analyze_0l_2tau.hadTauFakeRateWeight.lead.fitFunctionName = cms.string('%s')" % fitFunctionName)
      lines.append("process.analyze_0l_2tau.hadTauFakeRateWeight.sublead.fitFunctionName = cms.string('%s')" % fitFunctionName)
    if jobOptions['hadTau_selection'].find("mcClosure") != -1:
      lines.append("process.analyze_0l_2tau.hadTauFakeRateWeight.applyFitFunction_lead = cms.bool(False)")
      lines.append("process.analyze_0l_2tau.hadTauFakeRateWeight.applyFitFunction_sublead = cms.bool(False)")
    lines.append("process.analyze_0l_2tau.use_HIP_mitigation_bTag = cms.bool(%s)" % jobOptions['use_HIP_mitigation_bTag'])
    lines.append("process.analyze_0l_2tau.use_HIP_mitigation_mediumMuonId = cms.bool(%s)" % jobOptions['use_HIP_mitigation_mediumMuonId'])  
    lines.append("process.analyze_0l_2tau.isMC = cms.bool(%s)" % jobOptions['is_mc'])
    lines.append("process.analyze_0l_2tau.central_or_shift = cms.string('%s')" % jobOptions['central_or_shift'])
    lines.append("process.analyze_0l_2tau.lumiScale = cms.double(%f)" % jobOptions['lumi_scale'])
    lines.append("process.analyze_0l_2tau.apply_genWeight = cms.bool(%s)" % jobOptions['apply_genWeight'])
    lines.append("process.analyze_0l_2tau.apply_trigger_bits = cms.bool(%s)" % jobOptions['apply_trigger_bits'])
    lines.append("process.analyze_0l_2tau.selEventsFileName_output = cms.string('%s')" % jobOptions['rleOutputFile'])
    create_cfg(self.cfgFile_analyze, jobOptions['cfgFile_modified'], lines)

  def createCfg_makePlots_mcClosure(self, jobOptions):
    """Fills the template of python configuration file for making control plots

    Args:
      histogramFile: name of the input ROOT file 
    """
    lines = []
    lines.append("process.fwliteInput.fileNames = cms.vstring('%s')" % jobOptions['inputFile'])
    lines.append("process.makePlots_mcClosure.outputFileName = cms.string('%s')" % jobOptions['outputFile'])
    lines.append("process.makePlots_mcClosure.processesBackground = cms.vstring(%s)" % self.make_plots_backgrounds)
    lines.append("process.makePlots_mcClosure.processSignal = cms.string('%s')" % self.make_plots_signal)
    lines.append("process.makePlots_mcClosure.categories = cms.VPSet(")
    lines.append("  cms.PSet(")
    lines.append("    signal = cms.string('%s')," % self.histogramDir_prep_dcard)
    lines.append("    sideband = cms.string('%s')," % self.histogramDir_prep_dcard.replace("Tight", "Fakeable_mcClosure_wFakeRateWeights"))
    lines.append("    label = cms.string('%s')" % self.channel)
    lines.append("  )")
    lines.append(")")
    create_cfg(self.cfgFile_make_plots_mcClosure, jobOptions['cfgFile_modified'], lines)

  def addToMakefile_backgrounds_from_data(self, lines_makefile):
    self.addToMakefile_addBackgrounds(lines_makefile, "sbatch_addBackgrounds", self.sbatchFile_addBackgrounds, self.jobOptions_addBackgrounds)
    self.addToMakefile_addBackgrounds(lines_makefile, "sbatch_addBackgrounds_sum", self.sbatchFile_addBackgrounds_sum, self.jobOptions_addBackgrounds_sum)
    self.addToMakefile_hadd_stage1_5(lines_makefile)
    self.addToMakefile_addFakes(lines_makefile)
    
  def create(self):
    """Creates all necessary config files and runs the complete analysis workfow -- either locally or on the batch system
    """

    for sample_name, sample_info in self.samples.items():
      if not sample_info["use_it"] or sample_info["sample_category"] in [ "additional_signal_overlap", "background_data_estimate" ]:
        continue
      process_name = sample_info["process_name_specific"]
      for hadTau_selection in self.hadTau_selections:
        for hadTau_frWeight in self.hadTau_frWeights:
          if hadTau_frWeight == "enabled" and not hadTau_selection.startswith("Fakeable"):
            continue
          hadTau_selection_and_frWeight = get_hadTau_selection_and_frWeight(hadTau_selection, hadTau_frWeight)
          for hadTau_charge_selection in self.hadTau_charge_selections:
            key_dir = getKey(process_name, hadTau_selection_and_frWeight, hadTau_charge_selection)  
            for dir_type in [ DKEY_CFGS, DKEY_HIST, DKEY_LOGS, DKEY_RLES ]:
              initDict(self.dirs, [ key_dir, dir_type ])
              if dir_type in [ DKEY_CFGS, DKEY_LOGS ]:
                self.dirs[key_dir][dir_type] = os.path.join(self.configDir, dir_type, self.channel,
                  "_".join([ hadTau_selection_and_frWeight, hadTau_charge_selection ]), process_name)
              else:
                self.dirs[key_dir][dir_type] = os.path.join(self.outputDir, dir_type, self.channel,
                  "_".join([ hadTau_selection_and_frWeight, hadTau_charge_selection ]), process_name)
    for dir_type in [ DKEY_CFGS, DKEY_SCRIPTS, DKEY_HIST, DKEY_LOGS, DKEY_DCRD, DKEY_PLOT, DKEY_HADD_RT ]:
      initDict(self.dirs, [ dir_type ])
      if dir_type in [ DKEY_CFGS, DKEY_SCRIPTS, DKEY_LOGS, DKEY_DCRD, DKEY_PLOT, DKEY_HADD_RT ]:
        self.dirs[dir_type] = os.path.join(self.configDir, dir_type, self.channel)   
      else:
        self.dirs[dir_type] = os.path.join(self.outputDir, dir_type, self.channel)          
    ##print "self.dirs = ", self.dirs

    for key in self.dirs.keys():
      if type(self.dirs[key]) == dict:
        for dir_type in self.dirs[key].keys():
          create_if_not_exists(self.dirs[key][dir_type])
      else:
        create_if_not_exists(self.dirs[key])

    inputFileLists = {}
    for sample_name, sample_info in self.samples.items():
      if not sample_info["use_it"] or sample_info["sample_category"] in [ "additional_signal_overlap", "background_data_estimate" ]:
        continue
      logging.info("Checking input files for sample %s" % sample_info["process_name_specific"])
      inputFileLists[sample_name] = generateInputFileList(sample_name, sample_info, self.max_files_per_job, self.debug)

    for hadTau_selection in self.hadTau_selections:
      for hadTau_frWeight in self.hadTau_frWeights:
        if hadTau_frWeight == "enabled" and not hadTau_selection.startswith("Fakeable"):
          continue
        if hadTau_frWeight == "disabled" and not hadTau_selection == "Tight":
          continue
        hadTau_selection_and_frWeight = get_hadTau_selection_and_frWeight(hadTau_selection, hadTau_frWeight)

        for hadTau_charge_selection in self.hadTau_charge_selections:

          for sample_name, sample_info in self.samples.items():
            if not sample_info["use_it"] or sample_info["sample_category"] in [ "additional_signal_overlap", "background_data_estimate" ]:
              continue
            process_name = sample_info["process_name_specific"]
            logging.info("Creating configuration files to run '%s' for sample %s" % (self.executable_analyze, process_name))  
            
            sample_category = sample_info["sample_category"]            
            is_mc = (sample_info["type"] == "mc")
            is_signal = (sample_category == "signal")
            
            for central_or_shift in self.central_or_shifts:

              inputFileList = inputFileLists[sample_name]
              for jobId in inputFileList.keys():
                if central_or_shift != "central":
                  isFR_shape_shift = False
                  for FR_shape_shift in [
                    "CMS_ttHl_FRjt_norm",
                    "CMS_ttHl_FRjt_shape" ]:
                    if central_or_shift.find(FR_shape_shift) != -1:
                      isFR_shape_shift = True
                  if not ((hadTau_selection == "Fakeable" and hadTau_charge_selection == "OS" and isFR_shape_shift) or
                          (hadTau_selection == "Tight"    and hadTau_charge_selection == "OS")):
                    continue
                  if not is_mc and not isFR_shape_shift:
                    continue
                if central_or_shift.startswith("CMS_ttHl_thu_shape_ttH") and sample_category != "signal":
                  continue
                if central_or_shift.startswith("CMS_ttHl_thu_shape_ttW") and sample_category != "TTW":
                  continue
                if central_or_shift.startswith("CMS_ttHl_thu_shape_ttZ") and sample_category != "TTZ":
                  continue
                ##print "processing sample %s: jobId = %i, central_or_shift = '%s'" % (process_name, jobId, central_or_shift)

                # build config files for executing analysis code
                key_dir = getKey(process_name, hadTau_selection_and_frWeight, hadTau_charge_selection)
                key_analyze_job = getKey(process_name, hadTau_selection_and_frWeight, hadTau_charge_selection, central_or_shift, jobId)
                ntupleFiles = inputFileList[jobId]
                if len(ntupleFiles) == 0:
                  print "Warning: ntupleFiles['%s'] = %s --> skipping job !!" % (key_file, ntupleFiles)
                  continue

                self.jobOptions_analyze[key_analyze_job] = {
                  'ntupleFiles' : ntupleFiles,
                  'cfgFile_modified' : os.path.join(self.dirs[key_dir][DKEY_CFGS], "analyze_%s_%s_%s_%s_%s_%i_cfg.py" % \
                    (self.channel, process_name, hadTau_selection_and_frWeight, hadTau_charge_selection, central_or_shift, jobId)),
                  'histogramFile' : os.path.join(self.dirs[key_dir][DKEY_HIST], "%s_%s_%s_%s_%i.root" % \
                    (process_name, hadTau_selection_and_frWeight, hadTau_charge_selection, central_or_shift, jobId)),
                  'logFile' : os.path.join(self.dirs[key_dir][DKEY_LOGS], "analyze_%s_%s_%s_%s_%s_%i.log" % \
                    (self.channel, process_name, hadTau_selection_and_frWeight, hadTau_charge_selection, central_or_shift, jobId)),
                  'rleOutputFile' : os.path.join(self.dirs[key_dir][DKEY_RLES], "rle_%s_%s_%s_%s_%s_%i.txt" % \
                    (self.channel, process_name, hadTau_selection_and_frWeight, hadTau_charge_selection, central_or_shift, jobId)) if self.select_rle_output else "",
                  'sample_category' : sample_category,
                  'process_name_specific' : sample_info["process_name_specific"],
                  'triggers' : sample_info["triggers"],
                  'hadTau_selection' : "|".join([ hadTau_selection, self.hadTau_selection_part2 ]),
                  'apply_hadTauGenMatching' : self.apply_hadTauGenMatching,
                  'hadTau_charge_selection' : hadTau_charge_selection,
                  'applyFakeRateWeights' : "disabled" if hadTau_selection.find("Tight") != -1 else self.applyFakeRateWeights,
                  ##'use_HIP_mitigation_bTag' : sample_info["use_HIP_mitigation_bTag"],
                  ##'use_HIP_mitigation_mediumMuonId' : sample_info["use_HIP_mitigation_mediumMuonId"],
                  'use_HIP_mitigation_bTag' : True,
                  'use_HIP_mitigation_mediumMuonId' : True,
                  'is_mc' : is_mc,
                  'central_or_shift' : central_or_shift,
                  'lumi_scale' : 1. if not (self.use_lumi and is_mc) else sample_info["xsection"] * self.lumi / sample_info["nof_events"],
                  'apply_genWeight' : sample_info["genWeight"] if (is_mc and "genWeight" in sample_info) else False,
                  'apply_trigger_bits' : (is_mc and (self.era == "2015" or (self.era == "2016" and sample_info["reHLT"]))) or not is_mc
                }
                self.createCfg_analyze(self.jobOptions_analyze[key_analyze_job])

                # initialize input and output file names for hadd_stage1
                key_hadd_stage1 = getKey(process_name, hadTau_selection_and_frWeight, hadTau_charge_selection)
                if not key_hadd_stage1 in self.inputFiles_hadd_stage1:
                  self.inputFiles_hadd_stage1[key_hadd_stage1] = []
                self.inputFiles_hadd_stage1[key_hadd_stage1].append(self.jobOptions_analyze[key_analyze_job]['histogramFile'])
                self.outputFile_hadd_stage1[key_hadd_stage1] = os.path.join(self.dirs[DKEY_HIST], "histograms_harvested_stage1_%s_%s_%s_%s.root" % \
                    (self.channel, process_name, hadTau_selection_and_frWeight, hadTau_charge_selection))

            if is_mc:
              logging.info("Creating configuration files to run 'addBackgrounds' for sample %s" % process_name)

              sample_categories = [ sample_category ]
              if is_signal:
                sample_categories = [ "signal", "ttH", "ttH_htt", "ttH_hww", "ttH_hzz" ]
              for sample_category in sample_categories:
                # sum non-fake contributions for each MC sample separately
                # input processes: TT2t0e0m0j, TT1t1e0m0j, TT1t0e1m0j", TT0t2e0m0j, TT0t1e1m0j, TT0t0e2m0j; TTW2t0e0m0j,...
                # output processes: TT; ...
                key_hadd_stage1 = getKey(process_name, hadTau_selection_and_frWeight, hadTau_charge_selection)
                key_addBackgrounds_job = getKey(process_name, sample_category, hadTau_selection_and_frWeight, hadTau_charge_selection)
                processes_input = None
                if sample_category in [ "signal" ]:
                  hadTau_genMatches = []
                  hadTau_genMatches.extend(self.hadTau_genMatches_nonfakes)
                  hadTau_genMatches.extend(self.hadTau_genMatches_fakes)
                  processes_input = [ "%s%s" % (sample_category, genMatch) for genMatch in hadTau_genMatches ]
                elif sample_category in [ "ttH" ]:
                  processes_input = []
                  processes_input.extend([ "%s%s" % ("ttH_htt", genMatch) for genMatch in self.hadTau_genMatches_nonfakes ])
                  processes_input.extend([ "%s%s" % ("ttH_hww", genMatch) for genMatch in self.hadTau_genMatches_nonfakes ])
                  processes_input.extend([ "%s%s" % ("ttH_hzz", genMatch) for genMatch in self.hadTau_genMatches_nonfakes ])                
                else:
                  processes_input = [ "%s%s" % (sample_category, genMatch) for genMatch in self.hadTau_genMatches_nonfakes ]
                if processes_input:  
                  self.jobOptions_addBackgrounds[key_addBackgrounds_job] = {
                    'inputFile' : self.outputFile_hadd_stage1[key_hadd_stage1],
                    'cfgFile_modified' : os.path.join(self.dirs[DKEY_CFGS], "addBackgrounds_%s_%s_%s_%s_%s_cfg.py" % \
                      (self.channel, process_name, sample_category, hadTau_selection_and_frWeight, hadTau_charge_selection)),
                    'outputFile' : os.path.join(self.dirs[DKEY_HIST], "addBackgrounds_%s_%s_%s_%s_%s.root" % \
                      (self.channel, process_name, sample_category, hadTau_selection_and_frWeight, hadTau_charge_selection)),
                    'logFile' : os.path.join(self.dirs[DKEY_LOGS], "addBackgrounds_%s_%s_%s_%s_%s.log" % \
                      (self.channel, process_name, sample_category, hadTau_selection_and_frWeight, hadTau_charge_selection)),
                    'categories' : [ getHistogramDir(hadTau_selection, hadTau_frWeight, hadTau_charge_selection) ],
                    'processes_input' : processes_input,
                    'process_output' : sample_category
                  }
                  self.createCfg_addBackgrounds(self.jobOptions_addBackgrounds[key_addBackgrounds_job])
                      
                  # initialize input and output file names for hadd_stage1_5
                  key_hadd_stage1_5 = getKey(hadTau_selection_and_frWeight, hadTau_charge_selection)
                  if not key_hadd_stage1_5 in self.inputFiles_hadd_stage1_5:
                    self.inputFiles_hadd_stage1_5[key_hadd_stage1_5] = []
                  self.inputFiles_hadd_stage1_5[key_hadd_stage1_5].append(self.jobOptions_addBackgrounds[key_addBackgrounds_job]['outputFile'])
                  self.outputFile_hadd_stage1_5[key_hadd_stage1_5] = os.path.join(self.dirs[DKEY_HIST], "histograms_harvested_stage1_5_%s_%s_%s.root" % \
                    (self.channel, hadTau_selection_and_frWeight, hadTau_charge_selection))

                # sum fake contributions for each MC sample separately
                # input processes: TT1t0e0m1j, TT0t1e0m1j, TT0t0e1m1j, TT0t0e0m2j; TTW1t0e0m1j,...
                # output processes: TT_fake; ...
                key_hadd_stage1 = getKey(process_name, hadTau_selection_and_frWeight, hadTau_charge_selection)
                key_addBackgrounds_job = getKey(process_name, "%s_fake" % sample_category, hadTau_selection_and_frWeight, hadTau_charge_selection)
                processes_input = None
                if sample_category in [ "signal" ]:
                  processes_input = [ "%s%s" % (sample_category, genMatch) for genMatch in self.hadTau_genMatches_fakes ]
                elif sample_category in [ "ttH" ]:
                  processes_input = []
                  processes_input.extend([ "%s%s" % ("ttH_htt", genMatch) for genMatch in self.hadTau_genMatches_fakes ])
                  processes_input.extend([ "%s%s" % ("ttH_hww", genMatch) for genMatch in self.hadTau_genMatches_fakes ])
                  processes_input.extend([ "%s%s" % ("ttH_hzz", genMatch) for genMatch in self.hadTau_genMatches_fakes ])                
                else:
                  processes_input = [ "%s%s" % (sample_category, genMatch) for genMatch in self.hadTau_genMatches_fakes ]
                if processes_input:
                  self.jobOptions_addBackgrounds[key_addBackgrounds_job] = {
                    'inputFile' : self.outputFile_hadd_stage1[key_hadd_stage1],
                    'cfgFile_modified' : os.path.join(self.dirs[DKEY_CFGS], "addBackgrounds_%s_fakes_%s_%s_%s_%s_cfg.py" % \
                      (self.channel, process_name, sample_category, hadTau_selection_and_frWeight, hadTau_charge_selection)),
                    'outputFile' : os.path.join(self.dirs[DKEY_HIST], "addBackgrounds_%s_fakes_%s_%s_%s_%s.root" % \
                      (self.channel, process_name, sample_category, hadTau_selection_and_frWeight, hadTau_charge_selection)),
                    'logFile' : os.path.join(self.dirs[DKEY_LOGS], "addBackgrounds_%s_fakes_%s_%s_%s_%s.log" % \
                      (self.channel, process_name, sample_category, hadTau_selection_and_frWeight, hadTau_charge_selection)),
                    'categories' : [ getHistogramDir(hadTau_selection, hadTau_frWeight, hadTau_charge_selection) ],
                    'processes_input' : processes_input,
                    'process_output' : "%s_fake" % sample_category
                  }
                  self.createCfg_addBackgrounds(self.jobOptions_addBackgrounds[key_addBackgrounds_job])
                      
                  # initialize input and output file names for hadd_stage1_5
                  key_hadd_stage1_5 = getKey(hadTau_selection_and_frWeight, hadTau_charge_selection)
                  if not key_hadd_stage1_5 in self.inputFiles_hadd_stage1_5:
                    self.inputFiles_hadd_stage1_5[key_hadd_stage1_5] = []
                  self.inputFiles_hadd_stage1_5[key_hadd_stage1_5].append(self.jobOptions_addBackgrounds[key_addBackgrounds_job]['outputFile'])
                  self.outputFile_hadd_stage1_5[key_hadd_stage1_5] = os.path.join(self.dirs[DKEY_HIST], "histograms_harvested_stage1_5_%s_%s_%s.root" % \
                    (self.channel, hadTau_selection_and_frWeight, hadTau_charge_selection))

            # add output files of hadd_stage1 for data to list of input files for hadd_stage1_5
            if not is_mc:
              key_hadd_stage1 = getKey(process_name, hadTau_selection_and_frWeight, hadTau_charge_selection)
              key_hadd_stage1_5 = getKey(hadTau_selection_and_frWeight, hadTau_charge_selection)
              if not key_hadd_stage1_5 in self.inputFiles_hadd_stage1_5:
                self.inputFiles_hadd_stage1_5[key_hadd_stage1_5] = []
              self.inputFiles_hadd_stage1_5[key_hadd_stage1_5].append(self.outputFile_hadd_stage1[key_hadd_stage1])

          # sum fake contributions for the total of all MC sample
          # input processes: TT1t0e0m1j, TT0t1e0m1j, TT0t0e1m1j, TT0t0e0m2j; TTW1t0e0m1j,...
          # output process: fakes_mc
          key_addBackgrounds_job = getKey(hadTau_selection_and_frWeight, hadTau_charge_selection)
          sample_categories = []
          sample_categories.extend(self.nonfake_backgrounds)
          sample_categories.extend([ "signal" ])
          processes_input = []
          for sample_category in sample_categories:
            processes_input.append("%s_fake" % sample_category)
          self.jobOptions_addBackgrounds_sum[key_addBackgrounds_job] = {
            'inputFile' : self.outputFile_hadd_stage1_5[key_hadd_stage1_5],
            'cfgFile_modified' : os.path.join(self.dirs[DKEY_CFGS], "addBackgrounds_%s_fakes_mc_%s_%s_cfg.py" % \
              (self.channel, hadTau_selection_and_frWeight, hadTau_charge_selection)),
            'outputFile' : os.path.join(self.dirs[DKEY_HIST], "addBackgrounds_%s_fakes_mc_%s_%s.root" % \
              (self.channel, hadTau_selection_and_frWeight, hadTau_charge_selection)),
            'logFile' : os.path.join(self.dirs[DKEY_LOGS], "addBackgrounds_%s_fakes_mc_%s_%s.log" % \
              (self.channel, hadTau_selection_and_frWeight, hadTau_charge_selection)),
            'categories' : [ getHistogramDir(hadTau_selection, hadTau_frWeight, hadTau_charge_selection) ],
            'processes_input' : processes_input,
            'process_output' : "fakes_mc"
          }
          self.createCfg_addBackgrounds(self.jobOptions_addBackgrounds_sum[key_addBackgrounds_job])

          # initialize input and output file names for hadd_stage2
          key_hadd_stage2 = getKey(hadTau_selection_and_frWeight, hadTau_charge_selection)
          if not key_hadd_stage2 in self.inputFiles_hadd_stage2:
            self.inputFiles_hadd_stage2[key_hadd_stage2] = []
          if hadTau_selection == "Tight":
            self.inputFiles_hadd_stage2[key_hadd_stage2].append(self.jobOptions_addBackgrounds_sum[key_addBackgrounds_job]['outputFile'])
          key_hadd_stage1_5 = getKey(hadTau_selection_and_frWeight, hadTau_charge_selection)
          self.inputFiles_hadd_stage2[key_hadd_stage2].append(self.outputFile_hadd_stage1_5[key_hadd_stage1_5])
          self.outputFile_hadd_stage2[key_hadd_stage2] = os.path.join(self.dirs[DKEY_HIST], "histograms_harvested_stage2_%s_%s_%s.root" % \
            (self.channel, hadTau_selection_and_frWeight, hadTau_charge_selection))

    logging.info("Creating configuration files to run 'addBackgroundFakes'")
    for hadTau_charge_selection in self.hadTau_charge_selections:
      key_addFakes_job = getKey("fakes_data", hadTau_charge_selection)
      key_hadd_stage1_5 = getKey(get_hadTau_selection_and_frWeight("Fakeable", "enabled"), hadTau_charge_selection)
      category_sideband = None
      if self.applyFakeRateWeights == "2tau":
        category_sideband = "0l_2tau_%s_Fakeable_wFakeRateWeights" % hadTau_charge_selection
      else:
        raise ValueError("Invalid Configuration parameter 'applyFakeRateWeights' = %s !!" % applyFakeRateWeights)
      self.jobOptions_addFakes[key_addFakes_job] = {
        'inputFile' : self.outputFile_hadd_stage1_5[key_hadd_stage1_5],
        'cfgFile_modified' : os.path.join(self.dirs[DKEY_CFGS], "addBackgroundLeptonFakes_%s_%s_cfg.py" % \
          (self.channel, hadTau_charge_selection)),
        'outputFile' : os.path.join(self.dirs[DKEY_HIST], "addBackgroundLeptonFakes_%s_%s.root" % \
          (self.channel, hadTau_charge_selection)),
        'logFile' : os.path.join(self.dirs[DKEY_LOGS], "addBackgroundLeptonFakes_%s_%s.log" % \
          (self.channel, hadTau_charge_selection)),
        'category_signal' : "0l_2tau_%s_Tight" % hadTau_charge_selection,
        'category_sideband' : category_sideband
      }
      self.createCfg_addFakes(self.jobOptions_addFakes[key_addFakes_job])
      key_hadd_stage2 = getKey(get_hadTau_selection_and_frWeight("Tight", "disabled"), hadTau_charge_selection)
      self.inputFiles_hadd_stage2[key_hadd_stage2].append(self.jobOptions_addFakes[key_addFakes_job]['outputFile'])

    logging.info("Creating configuration files to run 'prepareDatacards'")
    for histogramToFit in self.histograms_to_fit:
      key_prep_dcard_job = getKey(histogramToFit)
      key_hadd_stage2 = getKey(get_hadTau_selection_and_frWeight("Tight", "disabled"), "OS")                      
      self.jobOptions_prep_dcard[key_prep_dcard_job] = {
        'inputFile' : self.outputFile_hadd_stage2[key_hadd_stage2],
        'cfgFile_modified' : os.path.join(self.dirs[DKEY_CFGS], "prepareDatacards_%s_%s_cfg.py" % (self.channel, histogramToFit)),
        'datacardFile' : os.path.join(self.dirs[DKEY_DCRD], "prepareDatacards_%s_%s.root" % (self.channel, histogramToFit)),
        'histogramDir' : self.histogramDir_prep_dcard,
        'histogramToFit' : histogramToFit,
        'label' : None
      }                            
      self.createCfg_prep_dcard(self.jobOptions_prep_dcard[key_prep_dcard_job])
      if "SS" in self.hadTau_charge_selections:
        key_prep_dcard_job = getKey(histogramToFit, "SS")
        key_hadd_stage2 = getKey(get_hadTau_selection_and_frWeight("Tight", "disabled"), "SS")                      
        self.jobOptions_prep_dcard[key_prep_dcard_job] = {
          'inputFile' : self.outputFile_hadd_stage2[key_hadd_stage2],
          'cfgFile_modified' : os.path.join(self.dirs[DKEY_CFGS], "prepareDatacards_%s_SS_%s_cfg.py" % (self.channel, histogramToFit)),
          'datacardFile' : os.path.join(self.dirs[DKEY_DCRD], "prepareDatacards_%s_SS_%s.root" % (self.channel, histogramToFit)),
          'histogramDir' : self.histogramDir_prep_dcard_SS,
          'histogramToFit' : histogramToFit,
          'label' : 'SS'
        }                            
        self.createCfg_prep_dcard(self.jobOptions_prep_dcard[key_prep_dcard_job])

    logging.info("Creating configuration files to run 'makePlots'")
    key_makePlots_job = getKey("OS")
    key_hadd_stage2 = getKey(get_hadTau_selection_and_frWeight("Tight", "disabled"), "OS")                            
    self.jobOptions_make_plots[key_makePlots_job] = {
      'executable' : self.executable_make_plots,
      'inputFile' : self.outputFile_hadd_stage2[key_hadd_stage2],
      'cfgFile_modified' : os.path.join(self.dirs[DKEY_CFGS], "makePlots_%s_cfg.py" % self.channel),
      'outputFile' : os.path.join(self.dirs[DKEY_PLOT], "makePlots_%s.png" % self.channel),
      'histogramDir' : self.histogramDir_prep_dcard,
      'label' : None,
      'make_plots_backgrounds' : self.make_plots_backgrounds
    }
    self.createCfg_makePlots(self.jobOptions_make_plots[key_makePlots_job])
    if "SS" in self.hadTau_charge_selections:
      key_makePlots_job = getKey("SS")
      key_hadd_stage2 = getKey(get_hadTau_selection_and_frWeight("Tight", "disabled"), "SS")                        
      self.jobOptions_make_plots[key_makePlots_job] = {
        'executable' : self.executable_make_plots,
        'inputFile' : self.outputFile_hadd_stage2[key_hadd_stage2],
        'cfgFile_modified' : os.path.join(self.dirs[DKEY_CFGS], "makePlots_%s_SS_cfg.py" % self.channel),
        'outputFile' : os.path.join(self.dirs[DKEY_PLOT], "makePlots_%s_SS.png" % self.channel),                       
        'histogramDir' : self.histogramDir_prep_dcard_SS,
        'label' : "SS",
        'make_plots_backgrounds' : self.make_plots_backgrounds
      }
      self.createCfg_makePlots(self.jobOptions_make_plots[key_makePlots_job])
    if "Fakeable_mcClosure" in self.hadTau_selections:
      key_makePlots_job = getKey("OS")
      key_hadd_stage2 = getKey(get_hadTau_selection_and_frWeight("Tight", "disabled"), "OS")
      self.jobOptions_make_plots[key_makePlots_job] = {
        'executable' : self.executable_make_plots_mcClosure,
        'inputFile' : self.outputFile_hadd_stage2[key_hadd_stage2],
        'cfgFile_modified' : os.path.join(self.dirs[DKEY_CFGS], "makePlots_mcClosure_%s_cfg.py" % self.channel),
        'outputFile' : os.path.join(self.dirs[DKEY_PLOT], "makePlots_mcClosure_%s.png" % self.channel)
      }
      self.createCfg_makePlots_mcClosure(self.jobOptions_make_plots[key_makePlots_job])
                
    if self.is_sbatch:
      logging.info("Creating script for submitting '%s' jobs to batch system" % self.executable_analyze)
      self.sbatchFile_analyze = os.path.join(self.dirs[DKEY_SCRIPTS], "sbatch_analyze_%s.py" % self.channel)
      self.createScript_sbatch_analyze(self.executable_analyze, self.sbatchFile_analyze, self.jobOptions_analyze)
      logging.info("Creating script for submitting '%s' jobs to batch system" % self.executable_addBackgrounds)
      self.sbatchFile_addBackgrounds = os.path.join(self.dirs[DKEY_SCRIPTS], "sbatch_addBackgrounds_%s.py" % self.channel)
      self.createScript_sbatch(self.executable_addBackgrounds, self.sbatchFile_addBackgrounds, self.jobOptions_addBackgrounds)
      self.sbatchFile_addBackgrounds_sum = os.path.join(self.dirs[DKEY_SCRIPTS], "sbatch_addBackgrounds_sum_%s.py" % self.channel)
      self.createScript_sbatch(self.executable_addBackgrounds, self.sbatchFile_addBackgrounds_sum, self.jobOptions_addBackgrounds_sum)
      logging.info("Creating script for submitting '%s' jobs to batch system" % self.executable_addFakes)
      self.sbatchFile_addFakes = os.path.join(self.dirs[DKEY_SCRIPTS], "sbatch_addFakes_%s.py" % self.channel)
      self.createScript_sbatch(self.executable_addFakes, self.sbatchFile_addFakes, self.jobOptions_addFakes)
            
    logging.info("Creating Makefile")
    lines_makefile = []
    self.addToMakefile_analyze(lines_makefile)
    self.addToMakefile_hadd_stage1(lines_makefile)
    self.addToMakefile_backgrounds_from_data(lines_makefile)
    self.addToMakefile_hadd_stage2(lines_makefile)
    self.addToMakefile_prep_dcard(lines_makefile)
    self.addToMakefile_make_plots(lines_makefile)
    self.createMakefile(lines_makefile)
  
    logging.info("Done")
