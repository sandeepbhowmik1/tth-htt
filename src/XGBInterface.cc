#include "tthAnalysis/HiggsToTauTau/interface/XGBInterface.h"
#include "tthAnalysis/HiggsToTauTau/interface/LocalFileInPath.h" // LocalFileInPath

#include <FWCore/Utilities/interface/Exception.h> // cms::Exception

#include <boost/filesystem.hpp> // boost::filesystem::

#include <fstream> // std::ifstream
#include <streambuf> // std::istreambuf_iterator<>()
#include <iostream>

XGBInterface::XGBInterface(const std::string & mvaFileName,
                           const std::vector<std::string> & mvaInputVariables)
  : mode_(Mode::k_old)
  , mvaFileName_(LocalFileInPath(mvaFileName).fullPath())
  , pkldata_odd_(nullptr)
  , moduleMainString_odd_(nullptr)
  , moduleMain_odd_(nullptr)
  , pkldata_even_(nullptr)
  , moduleMainString_even_(nullptr)
  , moduleMain_even_(nullptr)
  , mvaInputVariables_(mvaInputVariables)
{
  if(mode_ != Mode::k_old){
    assert(mode_ == Mode::k_old);
    std::cout<< "Using wrong Mode for this constructor" << std::endl;
  }

  // AC: limit number of threads running in python to one
  setenv("OMP_NUM_THREADS", "1", 0);

  // read the python file that we're about to execute
  const std::string applicationLoadPath = (
    boost::filesystem::path(std::getenv("CMSSW_BASE")) /
    boost::filesystem::path("src/tthAnalysis/HiggsToTauTau/python/xgb/applicationLoad.py")
  ).string();
  std::ifstream applicationLoadFile(applicationLoadPath);
  std::string applicationLoadStr;

  applicationLoadFile.seekg(0, std::ios::end);
  applicationLoadStr.reserve(applicationLoadFile.tellg());
  applicationLoadFile.seekg(0, std::ios::beg);
  applicationLoadStr.assign(std::istreambuf_iterator<char>(applicationLoadFile), std::istreambuf_iterator<char>());

  // https://ubuntuforums.org/archive/index.php/t-324544.html
  // https://stackoverflow.com/questions/4060221/how-to-reliably-open-a-file-in-the-same-directory-as-a-python-script
  // https://gist.github.com/rjzak/5681680
  Py_SetProgramName(const_cast<char *>("applicationLoad"));
  moduleMainString_ = PyString_FromString("__main__");
  moduleMain_ = PyImport_Import(moduleMainString_);
  PyRun_SimpleString(applicationLoadStr.c_str());
  PyObject* func = PyObject_GetAttrString(moduleMain_, "load");
  PyObject* args = PyTuple_Pack(1, PyString_FromString(mvaFileName_.data()));
  pkldata_ = PyObject_CallObject(func, args);

  Py_XDECREF(func);
  Py_XDECREF(args);
}

XGBInterface::XGBInterface(const std::string & mvaFileName_odd,
			   const std::string & mvaFileName_even,
			   const std::vector<std::string> & mvaInputVariables)
  : mode_(Mode::k_odd_even)
  , mvaFileName_odd_(LocalFileInPath(mvaFileName_odd).fullPath())
  , mvaFileName_even_(LocalFileInPath(mvaFileName_even).fullPath())
  , pkldata_(nullptr)
  , moduleMainString_(nullptr)
  , moduleMain_(nullptr)
  , mvaInputVariables_(mvaInputVariables)
{
  if(mode_ != Mode::k_odd_even){
    assert(mode_ == Mode::k_odd_even);
    std::cout<< "Using wrong Mode for this constructor" << std::endl;
  }

  // AC: limit number of threads running in python to one
  setenv("OMP_NUM_THREADS", "1", 0);

  // read the python file that we're about to execute
  const std::string applicationLoadPath = (
    boost::filesystem::path(std::getenv("CMSSW_BASE")) /
    boost::filesystem::path("src/tthAnalysis/HiggsToTauTau/python/xgb/applicationLoad.py")
  ).string();
  std::ifstream applicationLoadFile(applicationLoadPath);
  std::string applicationLoadStr;

  applicationLoadFile.seekg(0, std::ios::end);
  applicationLoadStr.reserve(applicationLoadFile.tellg());
  applicationLoadFile.seekg(0, std::ios::beg);
  applicationLoadStr.assign(std::istreambuf_iterator<char>(applicationLoadFile), std::istreambuf_iterator<char>());

  // https://ubuntuforums.org/archive/index.php/t-324544.html
  // https://stackoverflow.com/questions/4060221/how-to-reliably-open-a-file-in-the-same-directory-as-a-python-script
  // https://gist.github.com/rjzak/5681680
  Py_SetProgramName(const_cast<char *>("applicationLoad"));
  // moduleMainString_ = PyString_FromString("__main__");
  // moduleMain_ = PyImport_Import(moduleMainString_);

  moduleMainString_odd_ = PyString_FromString("__main__");
  moduleMain_odd_ = PyImport_Import(moduleMainString_odd_);

  moduleMainString_even_ = PyString_FromString("__main__");
  moduleMain_even_ = PyImport_Import(moduleMainString_even_);

  PyRun_SimpleString(applicationLoadStr.c_str());

  //PyObject* func = PyObject_GetAttrString(moduleMain_, "load");
  //PyObject* args = PyTuple_Pack(1, PyString_FromString(mvaFileName_.data()));
  //pkldata_ = PyObject_CallObject(func, args);

  PyObject* func_odd = PyObject_GetAttrString(moduleMain_odd_, "load");
  PyObject* args_odd = PyTuple_Pack(1, PyString_FromString(mvaFileName_odd_.data()));
  pkldata_odd_ = PyObject_CallObject(func_odd, args_odd);

  PyObject* func_even = PyObject_GetAttrString(moduleMain_even_, "load");
  PyObject* args_even = PyTuple_Pack(1, PyString_FromString(mvaFileName_even_.data()));
  pkldata_even_ = PyObject_CallObject(func_even, args_even);

  //Py_XDECREF(func);
  //Py_XDECREF(args);

  Py_XDECREF(func_odd);
  Py_XDECREF(args_odd);

  Py_XDECREF(func_even);
  Py_XDECREF(args_even);
}

XGBInterface::~XGBInterface()
{
  Py_XDECREF(pkldata_);
  Py_XDECREF(moduleMainString_);
  Py_XDECREF(moduleMain_);

  Py_XDECREF(pkldata_odd_);
  Py_XDECREF(moduleMainString_odd_);
  Py_XDECREF(moduleMain_odd_);

  Py_XDECREF(pkldata_even_);
  Py_XDECREF(moduleMainString_even_);
  Py_XDECREF(moduleMain_even_);
}

namespace
{
  // https://gist.github.com/rjzak/5681680
  PyObject *
  vectorToTuple_Float(const std::vector<float> & data)
  {
    PyObject * tuple = PyTuple_New(data.size());
    if(! tuple)
    {
      throw std::logic_error("Unable to allocate memory for Python tuple");
    }

    for(unsigned int i = 0; i < data.size(); ++i)
    {
      PyObject * num = PyFloat_FromDouble(static_cast<double>(data[i]));
      if(! num)
      {
        Py_XDECREF(tuple);
        throw cms::Exception("vectorToTuple_Float") << "Unable to allocate memory for Python tuple!";
      }
      PyTuple_SET_ITEM(tuple, i, num);
    }
    return tuple;
  }

  PyObject *
  vectorToTuple_String(const std::vector<std::string> & data)
  {
    PyObject * tuple = PyTuple_New(data.size());
    if(! tuple)
    {
      throw std::logic_error("Unable to allocate memory for Python tuple");
    }

    for(unsigned int i = 0; i < data.size(); ++i)
    {
      PyObject * num = PyString_FromString(data[i].c_str());
      if(! num)
      {
        Py_XDECREF(tuple);
        throw cms::Exception("vectorToTuple_String") << "Unable to allocate memory for Python tuple!";
      }
      PyTuple_SET_ITEM(tuple, i, num);
    }
    return tuple;
  }
}

double
XGBInterface::operator()(const std::map<std::string, double>& mvaInputs) const
{
  std::vector<float> vectorValuesVec;
  std::vector<std::string> vectorNamesVec;

  for(const std::string & mvaInputVariable: mvaInputVariables_)
  {
    if(mvaInputs.count(mvaInputVariable))
    {
      vectorValuesVec.push_back(mvaInputs.at(mvaInputVariable));
      vectorNamesVec.push_back(mvaInputVariable);
    }
    else
    {
      throw cms::Exception("XGBInterface::operator()")
        << "Missing value for MVA input variable = '" << mvaInputVariable << "' !!\n";
    }
  }

  PyObject* func = PyObject_GetAttrString(moduleMain_, "evaluate");
  PyObject* vectorValues = vectorToTuple_Float(vectorValuesVec);
  PyObject* vecNames = vectorToTuple_String(vectorNamesVec);
  PyObject* args = PyTuple_Pack(3, vectorValues, vecNames, pkldata_);
  PyObject* result = PyObject_CallObject(func, args);
  const double mvaOutput = PyFloat_AsDouble(result);

  Py_XDECREF(func);
  Py_XDECREF(vectorValues);
  Py_XDECREF(vecNames);
  Py_XDECREF(args);
  Py_XDECREF(result);

  return mvaOutput;
}

double
XGBInterface::operator()(const std::map<std::string, double>& mvaInputs, const int event_number) const
{
  std::vector<float> vectorValuesVec;
  std::vector<std::string> vectorNamesVec;

  for(const std::string & mvaInputVariable: mvaInputVariables_)
  {
    if(mvaInputs.count(mvaInputVariable))
    {
      vectorValuesVec.push_back(mvaInputs.at(mvaInputVariable));
      vectorNamesVec.push_back(mvaInputVariable);
    }
    else
    {
      throw cms::Exception("XGBInterface::operator()")
        << "Missing value for MVA input variable = '" << mvaInputVariable << "' !!\n";
    }
  }

  PyObject* vectorValues = vectorToTuple_Float(vectorValuesVec);
  PyObject* vecNames = vectorToTuple_String(vectorNamesVec);
  PyObject* func = 0;
  PyObject* args = 0;
  
  if(event_number % 2){ // Odd event number                                                                                                                                                               
    func = PyObject_GetAttrString(moduleMain_odd_, "evaluate");
    args = PyTuple_Pack(3, vectorValues, vecNames, pkldata_odd_);
  }else{ // Even event number                                                                                                                                                                             
    func = PyObject_GetAttrString(moduleMain_even_, "evaluate");
    args = PyTuple_Pack(3, vectorValues, vecNames, pkldata_even_);
  }

  PyObject* result = PyObject_CallObject(func, args);
  const double mvaOutput = PyFloat_AsDouble(result);

  Py_XDECREF(vectorValues);
  Py_XDECREF(vecNames);
  Py_XDECREF(func);
  Py_XDECREF(args);
  Py_XDECREF(result);
  
  std::cout<< "XGB: mvaOutput " << mvaOutput << std::endl; 
  return mvaOutput;
}


