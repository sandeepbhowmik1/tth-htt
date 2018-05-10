#!/usr/bin/env python
import os, logging, sys, getpass
from tthAnalysis.HiggsToTauTau.configs.analyzeConfig_1l_1tau import analyzeConfig_1l_1tau
from tthAnalysis.HiggsToTauTau.jobTools import query_yes_no
from tthAnalysis.HiggsToTauTau.analysisSettings import systematics
from tthAnalysis.HiggsToTauTau.runConfig import tthAnalyzeParser, filter_samples

# E.g.: ./tthAnalyzeRun_1l_1tau.py -v 2017Dec13 -m default -e 2017

mode_choices     = [ 'default' ]
sys_choices      = [ 'central', 'full' ]
systematics.full = systematics.an_common

parser = tthAnalyzeParser()
parser.add_modes(mode_choices)
parser.add_sys(sys_choices)
parser.add_preselect()
parser.add_tau_id_wp("dR03mvaTight")
args = parser.parse_args()

# Common arguments
era                = args.era
version            = args.version
dry_run            = args.dry_run
resubmission_limit = args.resubmission_limit
resubmit           = not args.disable_resubmission
no_exec            = args.no_exec
auto_exec          = args.auto_exec
check_input_files  = args.check_input_files
debug              = args.debug
sample_filter      = args.filter

# Additional arguments
mode              = args.mode
systematics_label = args.systematics
use_preselected   = args.use_preselected
tau_id_wp         = args.tau_id_wp

# Use the arguments
max_job_resubmission = resubmission_limit if resubmit else 1
central_or_shift     = getattr(systematics, systematics_label)
max_files_per_job    = 50 if use_preselected else 1

if use_preselected:
  from tthAnalysis.HiggsToTauTau.samples.tthAnalyzeSamples_2017_preselected import samples_2017
else:
  from tthAnalysis.HiggsToTauTau.samples.tthAnalyzeSamples_2017 import samples_2017

for sample_name, sample_info in samples_2017.items():
  if sample_info["type"] == "mc":
    sample_info["triggers"] = [ "1e", "1mu", "1e1tau", "1mu1tau" ]
  if sample_name in [
        # list of data samples collected with 2e, 2mu and e-mu cross-triggers
      ]:
    sample_info["use_it"] = False

if era == "2017":
  from tthAnalysis.HiggsToTauTau.analysisSettings import lumi_2017 as lumi
  samples = samples_2017
else:
  raise ValueError("Invalid era: %s" % era)

if __name__ == '__main__':
  logging.basicConfig(
    stream = sys.stdout,
    level  = logging.INFO,
    format = '%(asctime)s - %(levelname)s: %(message)s',
  )

  logging.info(
    "Running the jobs with the following systematic uncertainties enabled: %s" % \
    ', '.join(central_or_shift)
  )

  if sample_filter:
    samples = filter_samples(samples, sample_filter)

  job_statistics_summary = {}
  run_analysis           = False
  is_last_resubmission   = False

  for idx_job_resubmission in range(max_job_resubmission):
    if is_last_resubmission:
      continue

    analysis = analyzeConfig_1l_1tau(
      configDir = os.path.join("/home",       getpass.getuser(), "ttHAnalysis", era, version),
      outputDir = os.path.join("/hdfs/local", getpass.getuser(), "ttHAnalysis", era, version),
      executable_analyze                    = "analyze_1l_1tau",
      cfgFile_analyze                       = "analyze_1l_1tau_cfg.py",
      samples                               = samples,
      hadTau_selection                      = tau_id_wp,
      applyFakeRateWeights                  = "2L",
      chargeSumSelections                   = [ "OS", "SS" ],
      central_or_shifts                     = central_or_shift,
      max_files_per_job                     = max_files_per_job,
      era                                   = era,
      use_lumi                              = True,
      lumi                                  = lumi,
      check_input_files                     = check_input_files,
      running_method                        = "sbatch",
      num_parallel_jobs                     = 100, # Karl: speed up the hadd steps
      executable_addBackgrounds             = "addBackgrounds",
      # CV: use common executable for estimating jet->lepton and jet->tau_h fake background
      executable_addBackgroundJetToTauFakes = "addBackgroundLeptonFakes",
      histograms_to_fit                     = [
        "EventCounter",
        "numJets",
        "mvaOutput_1l_1tau_ttbar",
        "mTauTauVis",
        "mTauTau",
      ],
      select_rle_output                     = True,
      verbose                               = idx_job_resubmission > 0,
      dry_run                               = dry_run,
      isDebug                               = debug,
    )

    job_statistics = analysis.create()
    for job_type, num_jobs in job_statistics.items():
      logging.info(" #jobs of type '%s' = %i" % (job_type, num_jobs))
    job_statistics_summary[idx_job_resubmission] = job_statistics

    if idx_job_resubmission == 0:
      if auto_exec:
        run_analysis = True
      elif no_exec:
        run_analysis = False
      else:
        run_analysis = query_yes_no("Start jobs ?")
    if run_analysis:
      analysis.run()
    else:
      sys.exit(0)

    if job_statistics['analyze'] == 0:
      is_last_resubmission = True

  for idx_job_resubmission in job_statistics_summary.keys():
    logging.info("Job submission #%i:" % (idx_job_resubmission + 1))
    for job_type, num_jobs in job_statistics_summary[idx_job_resubmission].items():
      logging.info(" #jobs of type '%s' = %i" % (job_type, num_jobs))
