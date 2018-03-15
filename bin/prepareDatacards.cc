/** \executable prepareDatacards
 *
 * Prepare datacards for ttH, H->tautau analysis
 *
 * \author Christian Veelken, Tallinn
cfg_prepareDatacards 
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/InputSource.h"
#include "DataFormats/FWLite/interface/OutputFiles.h"

#include "tthAnalysis/HiggsToTauTau/interface/histogramAuxFunctions.h" // compIntegral
#include "tthAnalysis/HiggsToTauTau/interface/jetToTauFakeRateAuxFunctions.h" // getEtaBin, getPtBin 

#include <TFile.h>
#include <TH1.h>
#include <TArrayD.h>
#include <TBenchmark.h>
#include <TMath.h>
#include <TError.h> // gErrorAbortLevel, kError
#include "TPRegexp.h"
#include "TDirectory.h"
#include "TList.h"
#include "TKey.h"
#include "TObject.h"
#include "TString.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // EXIT_SUCCESS, EXIT_FAILURE
#include <assert.h>

typedef std::vector<std::string> vstring;
typedef std::vector<double> vdouble;

namespace
{/*
  std::vector<double> compBinning(TH1* histogram, double minEvents)
  {
    std::cout << "<compBinning>:" << std::endl;
    std::vector<double> histogramBinning;
    const TAxis* xAxis = histogram->GetXaxis();
    histogramBinning.push_back(xAxis->GetBinLowEdge(1));
    double sumEvents = 0.;
    int numBins = xAxis->GetNbins();
    for ( int idxBin = 1; idxBin <= numBins; ++idxBin ) {
      std::cout << "bin #" << idxBin << " (x=" << xAxis->GetBinLowEdge(idxBin) << ".." << xAxis->GetBinUpEdge(idxBin) << "):"
		<< " binContent = " << histogram->GetBinContent(idxBin) << " +/- " << histogram->GetBinError(idxBin) << std::endl;
      sumEvents += histogram->GetBinContent(idxBin);
      if ( sumEvents >= minEvents ) {
	histogramBinning.push_back(xAxis->GetBinUpEdge(idxBin));
	sumEvents = 0.;
      }
    }
    if ( TMath::Abs(histogramBinning.back() - xAxis->GetBinUpEdge(numBins)) > 1.e-3 ) {
      if ( histogramBinning.size() >= 2 ) histogramBinning.back() = xAxis->GetBinUpEdge(numBins);
      else histogramBinning.push_back(xAxis->GetBinUpEdge(numBins));
    }
    assert(histogramBinning.size() >= 2);
    std::cout << "binning = { ";
    for ( std::vector<double>::const_iterator bin = histogramBinning.begin();
	  bin != histogramBinning.end(); ++bin ) {
      if ( bin != histogramBinning.begin() ) std::cout << ", ";
      std::cout << (*bin);
    }
    std::cout << " }" << std::endl;
    return histogramBinning;
  }
 */ 
  double square(double x)
  {
    return x*x;
  }

  TH1* rebinHistogram(const std::vector<double>& histogramBinning, const TH1* histogram)
  {
    std::cout << "<rebinHistogram>: " << histogram->GetName() << std::endl;
    std::cout << "integral(histogram) = " << compIntegral(histogram, false, false) << std::endl;

    TArrayD histogramBinning_array(histogramBinning.size());
    int idx = 0;
    for ( std::vector<double>::const_iterator binEdge = histogramBinning.begin();
	  binEdge != histogramBinning.end(); ++binEdge ) {
      histogramBinning_array[idx] = (*binEdge);
      ++idx;
    }
    std::cout << " nBins = " << histogramBinning_array.GetSize() - 1 << ",";
    std::cout << " binning = { ";
    for ( int idxBin = 0; idxBin < histogramBinning_array.GetSize(); ++idxBin ) {
      if ( idxBin > 0 ) std::cout << ", ";
      std::cout << histogramBinning_array[idxBin];
    }
    std::cout << " } " << std::endl;
    std::string histogramName = histogram->GetName();
    std::string histogramTitle = histogram->GetTitle();
    int numBins_rebinned = histogramBinning_array.GetSize() - 1;
    std::cout << "numBins_rebinned = " << numBins_rebinned << std::endl;
    TH1* histogram_rebinned = new TH1D(histogramName.data(), histogramTitle.data(), numBins_rebinned, histogramBinning_array.GetArray());
    if ( !histogram_rebinned->GetSumw2N() ) histogram_rebinned->Sumw2();
    const TAxis* xAxis = histogram->GetXaxis();
    int numBins = xAxis->GetNbins();
    const TAxis* xAxis_rebinned = histogram_rebinned->GetXaxis();
    double binContentSum = 0.;
    double binError2Sum = 0.;
    int idxBin_rebinned = 1;
    for ( int idxBin = 1; idxBin <= numBins; ++idxBin ) {
      double binContent = histogram->GetBinContent(idxBin);
      binContentSum += binContent;
      double binError = histogram->GetBinError(idxBin);
      binError2Sum += (binError*binError);
      bool isNextBin_rebinned = false;
      if ( idxBin == numBins ) {
	isNextBin_rebinned = true;
      } else {
	if ( xAxis->GetBinCenter(idxBin + 1) > xAxis_rebinned->GetBinLowEdge(idxBin_rebinned + 1) ) {
	  isNextBin_rebinned = true;
	}
      }
      if ( isNextBin_rebinned ) {
	assert(idxBin_rebinned <= numBins_rebinned);
	histogram_rebinned->SetBinContent(idxBin_rebinned, binContentSum);
	histogram_rebinned->SetBinError(idxBin_rebinned, TMath::Sqrt(binError2Sum));
	binContentSum = 0.;
	binError2Sum = 0.;
	++idxBin_rebinned;
      }
      //std::cout << "binCenter = " << binCenter << ": idxBin = " << idxBin << ", idxBin_rebinned = " << idxBin_rebinned << std::endl;
    }
    std::cout << "integral(histogram_rebinned) = " << compIntegral(histogram_rebinned, false, false) << std::endl;
    return histogram_rebinned;
  } 

  TH1* copyHistogram(TDirectory* dir_input, const std::string& process, const std::string& histogramName_input, 
		     const std::string& histogramName_output, double sf, double xMin, double xMax, int rebin, const std::string& central_or_shift, 
		     bool enableException, bool setEmptySystematicFromCentral = true)
  {
    //std::cout << "<copyHistogram>:" << std::endl;
    //std::cout << " dir_input = " << dir_input->GetName() << std::endl;
    //std::cout << " process = " << process << std::endl;
    //std::cout << " histogramName_input = " << histogramName_input << std::endl;
    //std::cout << " histogramName_output = " << histogramName_output << std::endl;
    //std::cout << " central_or_shift = " << central_or_shift << std::endl;
    //std::cout << " enableException = " << enableException << std::endl;

    std::string histogramName_input_full = "";
    if ( !(central_or_shift == "" || central_or_shift == "central") ) histogramName_input_full.append(central_or_shift);
    if( histogramName_input_full != "" ) histogramName_input_full.append("_");
    histogramName_input_full.append(histogramName_input);
    TH1* histogram_input = dynamic_cast<TH1*>(dir_input->Get(histogramName_input_full.data()));
    if ( !histogram_input ) {
      if ( enableException ) 
	throw cms::Exception("copyHistogram")
	  << "Failed to find histogram = '" << histogramName_input_full << "' in directory = '" << dir_input->GetName() << "' !!\n";
      return 0;
    }   
    std::cout << " integral(" << process << ") = " << histogram_input->Integral() << std::endl;

    // special treatment for the case that systematic variation has zero events, but central value > 0
    bool isSystematicFromCentral = false;
    if ( setEmptySystematicFromCentral && !(central_or_shift == "" || central_or_shift == "central") && histogram_input->GetEntries() == 0 ) {
      TH1* histogram_central = dynamic_cast<TH1*>(dir_input->Get(histogramName_input.data()));
      if ( histogram_central->GetEntries() > 0 ) {
	histogram_input = histogram_central;
	isSystematicFromCentral = true;
      } 
    }

    TArrayD histogramBinning_input = getBinning(histogram_input);
    int numBins_input = histogramBinning_input.GetSize() - 1;
    const TAxis* xAxis_input = histogram_input->GetXaxis();

    // std::string histogramName_output_full = std::string("x").append("_").append(process);
    std::string histogramName_output_full = process; 
    if ( !(central_or_shift == "" || central_or_shift == "central") ) histogramName_output_full.append("_").append(central_or_shift);
    if ( histogramName_output != "" ) histogramName_output_full.append("_").append(histogramName_output);
    TArrayD histogramBinning_output = getBinning(histogram_input, xMin, xMax);
    int numBins_output = histogramBinning_output.GetSize() - 1;
    TH1* histogram_output = new TH1F(histogramName_output_full.data(), histogramName_output_full.data(), numBins_output, histogramBinning_output.GetArray());
    if ( !histogram_output->GetSumw2N() ) histogram_output->Sumw2();
    const TAxis* xAxis_output = histogram_output->GetXaxis();

    double binContent_output_underflow = 0.;
    double binErr2_output_underflow = 0.;
    double binContent_output_overflow = 0.;
    double binErr2_output_overflow = 0.;
    for ( int idxBin_input = 0; idxBin_input <= (numBins_input + 1); ++idxBin_input ) {
      double binContent_input, binError_input;
      if ( isSystematicFromCentral ) {
	binContent_input = 0.1*sf*histogram_input->GetBinContent(idxBin_input);
	binError_input = sf*histogram_input->GetBinError(idxBin_input);
	binError_input = TMath::Sqrt(binError_input*binError_input + binContent_input*binContent_input);
      } else {
	binContent_input = sf*histogram_input->GetBinContent(idxBin_input);
	binError_input = sf*histogram_input->GetBinError(idxBin_input);
      }
      if ( idxBin_input >= 1 && idxBin_input <= numBins_input ) {	
	double binCenter_input = xAxis_input->GetBinCenter(idxBin_input);
	if ( !((xMin == -1. || binCenter_input > xMin) && (xMax == -1. || binCenter_input < xMax)) ) continue;
	int idxBin_output = xAxis_output->FindBin(binCenter_input);
	if ( idxBin_output >= 1 && idxBin_output <= numBins_output ) {
	  histogram_output->SetBinContent(idxBin_output, binContent_input);
	  histogram_output->SetBinError(idxBin_output, binError_input);
	} else if ( idxBin_output == 0 ) { // "regular" bin of input histogram, which corresponds to underflow bin of output histogram
	  binContent_output_underflow += binContent_input;
	  binErr2_output_underflow += square(binError_input);
	} else if ( idxBin_output == (numBins_input + 1) ) { // "regular" bin of input histogram, which corresponds to overflow bin of output histogram
	  binContent_output_overflow += binContent_input;
	  binErr2_output_overflow += square(binError_input);
	} else assert(0);
      } else if ( idxBin_input == 0 ) { // underflow bin of input histogram, which corresponds to underflow bin of output histogram
	binContent_output_underflow += binContent_input;
	binErr2_output_underflow += square(binError_input);
      } else if ( idxBin_input == (numBins_input + 1) ) { // overflow bin of input histogram, which corresponds to overflow bin of output histogram
	binContent_output_overflow += binContent_input;
	binErr2_output_overflow += square(binError_input);
      } else assert(0);
    }
    // CV: set underflow and overflow bins to zero
    //histogram_output->SetBinContent(0, binContent_output_underflow);
    //histogram_output->SetBinError(0, TMath::Sqrt(binErr2_output_underflow));
    //histogram_output->SetBinContent(numBins_output + 1, binContent_output_overflow);
    //histogram_output->SetBinError(numBins_output + 1, TMath::Sqrt(binErr2_output_overflow));
    histogram_output->SetBinContent(0, 0.);
    histogram_output->SetBinError(0, 0.);
    histogram_output->SetBinContent(numBins_output + 1, 0.);
    histogram_output->SetBinError(numBins_output + 1, 0.);
        
    if ( rebin > 1 ) {
      if ( (numBins_output % rebin) != 0 )
	throw cms::Exception("copyHistogram")
	  << "Failed to rebin histogram = '" << histogramName_input_full << "':" 
	  << " numBins_input = " << numBins_input << ", numBins_output = " << numBins_output << " (xMin = " << xMin << ", xMax = " << xMax << "), rebin = " << rebin << " !!\n";
      histogram_output->Rebin(rebin);
    }    

    return histogram_output;
  }
  
  struct categoryType
  {
    categoryType(const edm::ParameterSet& cfg)
    {
      input_ = cfg.getParameter<std::string>("input");
      output_ = cfg.getParameter<std::string>("output");
    }
    ~categoryType() {}
    std::string input_;
    std::string output_;
  };

  bool compMatch(const std::string& s, TPRegexp* pattern)
  {
    bool isMatched = false;
    if ( pattern->Match(s.data()) ) {
      TObjArray* matches = pattern->MatchS(s.data());
      int numMatches = matches->GetEntries();
      for ( int idxMatch = 0; idxMatch < numMatches; ++idxMatch ) {
	TObjString* s_matched = dynamic_cast<TObjString*>(matches->At(idxMatch));
	assert(s_matched); 
	if ( std::string(s_matched->GetString().Data()).size() == s.size() ) {
	  isMatched = true;
	  break;
	}
      }
      delete matches;
    }
    return isMatched;
  }

  TArrayD getTArraDfromVector(std::vector<double>& histogramBinning)
  {
    TArrayD binning_tarray(histogramBinning.size());
    for ( int idxBin = 0; idxBin < (int)histogramBinning.size(); ++idxBin ) {
      binning_tarray[idxBin] = histogramBinning[idxBin];
    }
    return binning_tarray;
  }


}

int main(int argc, char* argv[]) 
{
//--- throw an exception in case ROOT encounters an error
  gErrorAbortLevel = kError;

//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "<prepareDatacards>:" << std::endl;

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("prepareDatacards");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ) 
    throw cms::Exception("prepareDatacards") 
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";
  
  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfg_prepareDatacards = cfg.getParameter<edm::ParameterSet>("prepareDatacards");

  vstring processesToCopy_string = cfg_prepareDatacards.getParameter<vstring>("processesToCopy");
  std::vector<TPRegexp*> processesToCopy;
  for ( vstring::const_iterator processToCopy_string = processesToCopy_string.begin();
	processToCopy_string != processesToCopy_string.end(); ++processToCopy_string ) {
    TPRegexp* processToCopy = new TPRegexp(processToCopy_string->data());
    processesToCopy.push_back(processToCopy);
  }
  
  double sf_signal = cfg_prepareDatacards.getParameter<double>("sf_signal");
  vstring signals_string = cfg_prepareDatacards.getParameter<vstring>("signals");
  std::vector<TPRegexp*> signals;
  for ( vstring::const_iterator signal_string = signals_string.begin();
	signal_string != signals_string.end(); ++signal_string ) {
    TPRegexp* signal = new TPRegexp(signal_string->data());
    signals.push_back(signal);
  }

  TPRegexp* data = new TPRegexp("data_obs");

  std::vector<categoryType> categories;
  edm::VParameterSet cfg_categories = cfg_prepareDatacards.getParameter<edm::VParameterSet>("categories");
  for ( edm::VParameterSet::const_iterator cfgCategory = cfg_categories.begin();
	cfgCategory != cfg_categories.end(); ++cfgCategory ) {
    categories.push_back(categoryType(*cfgCategory));
  }
  
  std::string histogramToFit = cfg_prepareDatacards.getParameter<std::string>("histogramToFit");
  double histogramToFit_xMin = ( cfg_prepareDatacards.exists("histogramToFit_xMin") ) ? cfg_prepareDatacards.getParameter<double>("histogramToFit_xMin") : -1.;
  double histogramToFit_xMax = ( cfg_prepareDatacards.exists("histogramToFit_xMax") ) ? cfg_prepareDatacards.getParameter<double>("histogramToFit_xMax") : -1.;
  vstring central_or_shifts = cfg_prepareDatacards.getParameter<vstring>("sysShifts");
  // CV: check if central value needs to be added
  bool containsCentralValue = false;
  for ( vstring::const_iterator central_or_shift = central_or_shifts.begin();
	central_or_shift != central_or_shifts.end(); ++central_or_shift ) {
    if ( (*central_or_shift) == "central" || (*central_or_shift) == "" ) containsCentralValue = true;
  }
  if ( !containsCentralValue ) {
    central_or_shifts.push_back(""); 
  }
  int histogramToFit_rebin = cfg_prepareDatacards.getParameter<int>("histogramToFit_rebin");
  bool apply_automatic_rebinning = cfg_prepareDatacards.getParameter<bool>("apply_automatic_rebinning");
  double minEvents_automatic_rebinning = cfg_prepareDatacards.getParameter<double>("minEvents_automatic_rebinning");
  bool apply_quantile_rebinning = cfg_prepareDatacards.getParameter<bool>("apply_quantile_rebinning");
  int nbin_quantile_rebinning = cfg_prepareDatacards.getParameter<int>("nbin_quantile_rebinning");
  fwlite::InputSource inputFiles(cfg); 
  if ( !(inputFiles.files().size() == 1) )
    throw cms::Exception("prepareDatacards") 
      << "Exactly one input file expected !!\n";
  TFile* inputFile = new TFile(inputFiles.files().front().data());

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

  for ( std::vector<categoryType>::const_iterator category = categories.begin();
	category != categories.end(); ++category ) {
    std::cout << "processing category = " << category->input_ << std::endl;
      
    TDirectory* dir = getDirectory(inputFile, category->input_, true);
    assert(dir);
      
    // copy histograms that do not require modifications
    std::cout << "copying histograms that do not require modifications" << std::endl;
    TList* list = dir->GetListOfKeys();
    TIter next(list);
    TKey* key = 0;
    TH1* histogramBackgroundSum = 0;
    std::vector<TH1*> histogramsToRebin;
    while ( (key = dynamic_cast<TKey*>(next())) ) {
      TObject* object = key->ReadObj();
      TDirectory* subdir = dynamic_cast<TDirectory*>(object);
      if ( !subdir ) continue;
      std::cout << "subdir = " << subdir->GetName() << std::endl;
      bool isToCopy = false;
      for ( std::vector<TPRegexp*>::iterator processToCopy = processesToCopy.begin();
	    processToCopy != processesToCopy.end(); ++processToCopy ) {
	bool isMatched = compMatch(subdir->GetName(), *processToCopy);
	if ( isMatched ) {
	  std::cout << " matches processToCopy = " << (*processToCopy)->GetPattern() << std::endl;
	  isToCopy = true;
	} else {
	  std::cout << " does not match processToCopy = " << (*processToCopy)->GetPattern() << std::endl;
	}
      }
      bool isSignal = false;
      for ( std::vector<TPRegexp*>::iterator signal = signals.begin();
	    signal != signals.end(); ++signal ) {
	bool isMatched = compMatch(subdir->GetName(), *signal);
	if ( isMatched ) {
	  std::cout << " matches signal = " << (*signal)->GetPattern() << std::endl;
	  isSignal = true;
	} else {
	  std::cout << " does not match signal = " << (*signal)->GetPattern() << std::endl;
	}
      }
      if ( isToCopy || isSignal ) {
	for ( vstring::const_iterator central_or_shift = central_or_shifts.begin();
	      central_or_shift != central_or_shifts.end(); ++central_or_shift ) {
	  std::cout << "histogramToFit = " << histogramToFit << ", central_or_shift = " << (*central_or_shift) << std::endl;
	  
	  TFileDirectory* subdir_output = &fs;
	  subdir_output->cd();
	  //Make subdirectory if given
	  bool makeSubDir = cfg_prepareDatacards.getParameter<bool>("makeSubDir");
	  if ( makeSubDir == true && category->output_.length() > 0 ){
	    TDirectory* subsubdir_output = createSubdirectory_recursively(fs, category->output_);
	    subsubdir_output->cd();
	  }
	  double sf = ( isSignal ) ? sf_signal : 1.;
	  TH1* histogram = copyHistogram(
	    subdir, subdir->GetName(), histogramToFit, "", 
	    sf, histogramToFit_xMin, histogramToFit_xMax, histogramToFit_rebin, *central_or_shift, (*central_or_shift) == "" || (*central_or_shift) == "central");	  
	  if ( !histogram ) continue;
	  bool isData = compMatch(subdir->GetName(), data);
	  //	  if ( !(isData || isSignal) ) {
	  if ( !(isData || isSignal) && ((*central_or_shift) == "" || (*central_or_shift) == "central") ) {
	    std::cout << "adding background = '" << subdir->GetName() << "'" << std::endl;
	    if   ( !histogramBackgroundSum ) histogramBackgroundSum = (TH1*)histogram->Clone(Form("%s_BackgroundSum", category->input_.data()));
	    else                             histogramBackgroundSum->Add(histogram);  	    
	  }
	  histogramsToRebin.push_back(histogram);
	}
      }
    }

    if ( apply_automatic_rebinning &&(!apply_quantile_rebinning)) {
      TDirectory* subsubdir_output = createSubdirectory_recursively(fs, Form("%s/rebinned", category->output_.c_str()));
      subsubdir_output->cd();
      // rebin histograms to avoid bins with zero background
      assert(histogramBackgroundSum);
      //std::vector<double> histogramBinning = compBinning(histogramBackgroundSum, minEvents_automatic_rebinning);
      //std::vector<double> histogramBinningV = {0.0, 0.2052316258054663, 0.3595713262743688, 0.4265624045768703, 1.0}; // 1B BDT
      std::vector<double> histogramBinningV = {0.0, 0.19111777466810542, 0.3723924099881871, 0.39747780534459615, 1.}; // SUM BDT
      TArrayD histogramBinning = getTArraDfromVector(histogramBinningV);
      
      //TArrayD histogramBinning = getRebinnedBinning(histogramBackgroundSum, minEvents_automatic_rebinning);

      for ( std::vector<TH1*>::iterator histogram = histogramsToRebin.begin();
	    histogram != histogramsToRebin.end(); ++histogram ) {
	//rebinHistogram(histogramBinning, *histogram);
	getRebinnedHistogram1d(*histogram, 4, histogramBinning);
      }
    }
    if ( apply_quantile_rebinning && (!apply_automatic_rebinning)) {
      TDirectory* subsubdir_output = createSubdirectory_recursively(fs, Form("%s/rebinned", category->output_.c_str()));
      subsubdir_output->cd();
      assert(histogramBackgroundSum);
      const Int_t nq = nbin_quantile_rebinning;
      Double_t xq[nq];  // position where to compute the quantiles in [0,1]
      Double_t yq[nq];  // array to contain the quantiles
      for (Int_t i=0;i<nq;i++) xq[i] = Float_t(i+1)/nq;
      histogramBackgroundSum->GetQuantiles(nq,yq,xq);
      std::cout<<"............ quantile binning ............"<<std::endl;
      for (Int_t i=0;i<nq;i++)std::cout<<yq[i]<<std::endl;
      TArrayD histogramBinning(nq+1);
      histogramBinning[0] = 0;
      for (Int_t i=0;i<nq;i++) histogramBinning[i+1] = yq[i];
      for ( std::vector<TH1*>::iterator histogram = histogramsToRebin.begin();
	    histogram != histogramsToRebin.end(); ++histogram ) {
	getRebinnedHistogram1d(*histogram, 4, histogramBinning);
      }
    }
    delete histogramBackgroundSum;
  }
  
  delete inputFile;

  clock.Show("prepareDatacards");
  
  return EXIT_SUCCESS;
}
