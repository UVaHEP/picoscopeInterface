#include "ps5000a.h"

#include <cmath> 
using std::get;

namespace picoscope {




  
  ps5000a::ps5000a() {
    std::cout << "ps5000a!" << std::endl;
    _handle = 0;
    _serial = 0;
    _opened = false;
    _update = true;
    _preTriggerSamples = 100;
    _postTriggerSamples = 100;
    _nsamples = 200; 
    _timebase = 2; 
    _resolution = PS_8BIT; 
    chNameMap.emplace(A, PS5000A_CHANNEL_A);
    chNameMap.emplace(B, PS5000A_CHANNEL_B);
    chNameMap.emplace(C, PS5000A_CHANNEL_C);
    chNameMap.emplace(D, PS5000A_CHANNEL_D);
    chNameMap.emplace(EXT, PS5000A_EXTERNAL); 


    chBwMap.emplace(BW_FULL, PS5000A_BW_FULL);
    chBwMap.emplace(BW_20MHZ, PS5000A_BW_20MHZ);

    chCouplingMap.emplace(AC, PS5000A_AC);
    chCouplingMap.emplace(DC, PS5000A_DC);

    chRangeMap.emplace(PS_10MV, PS5000A_10MV); 
    chRangeMap.emplace(PS_20MV, PS5000A_20MV);
    chRangeMap.emplace(PS_50MV, PS5000A_50MV);
    chRangeMap.emplace(PS_100MV, PS5000A_100MV);
    chRangeMap.emplace(PS_200MV, PS5000A_200MV);
    chRangeMap.emplace(PS_500MV, PS5000A_500MV);
    chRangeMap.emplace(PS_1V, PS5000A_1V);
    chRangeMap.emplace(PS_2V, PS5000A_2V);
    chRangeMap.emplace(PS_5V, PS5000A_5V);
    chRangeMap.emplace(PS_10V, PS5000A_10V);
    chRangeMap.emplace(PS_20V, PS5000A_20V);
    chRangeMap.emplace(PS_50V, PS5000A_50V);

    triggerDirectionMap.emplace(trgRising, PS5000A_RISING);
    triggerDirectionMap.emplace(trgFalling, PS5000A_FALLING);
    triggerDirectionMap.emplace(trgAbove, PS5000A_ABOVE);
    triggerDirectionMap.emplace(trgBelow, PS5000A_BELOW);
    triggerDirectionMap.emplace(trgNone, PS5000A_NONE); 
    
    
    devResolutionMap.emplace(PS_8BIT, PS5000A_DR_8BIT);
    devResolutionMap.emplace(PS_12BIT, PS5000A_DR_12BIT);
    devResolutionMap.emplace(PS_14BIT, PS5000A_DR_14BIT);
    devResolutionMap.emplace(PS_15BIT, PS5000A_DR_15BIT);
    devResolutionMap.emplace(PS_16BIT, PS5000A_DR_16BIT);
		

  }

  void ps5000a::setDeviceResolution(devResolution r) {


  }

  
  bool ps5000a::open(devResolution r) {

    if (_opened)
      return true;

    PS5000A_DEVICE_RESOLUTION resolution = devResolutionMap[r];
    _resolution = r;
    _maxADCValue = maxADCValue(r); 
    std::cout << "opening device" << std::endl;

  PICO_STATUS status; 

 
  if (_serial == 0) { 
    status = ps5000aOpenUnit(&_handle, NULL, resolution); 
  }
  else {
    status = ps5000aOpenUnit(&_handle, &_serial, resolution); 
  }

  _opened = statusProcessor(status); 
  //  if (_opened)
  //    getPicoscopeInfo(); 

  
  return _opened; 

    
  }

  void ps5000a::close() {
    PICO_STATUS status = ps5000aCloseUnit(_handle); 
    statusProcessor(status); 
  }


  bool ps5000a::statusProcessor(PICO_STATUS status) { 
  switch (status) { 
  case PICO_OK: 
    return true; 
    break; 
  case PICO_NOT_FOUND:
    std::cout << "Couldn't find a pico scope" << std::endl; 
    return false; 
    break; 
  default:
    std::cerr << "Bad PICO_STATUS Value: " << status << std::endl; 
    return false; 
  }

  }


  void ps5000a::psUpdate() {
    std::cout << "Updating picoscope channel settings" << std::endl;
    //for most of these we have a 1:1 mapping between enums for the
    //channels.
    PS5000A_CHANNEL name;
    PS5000A_RANGE range;
    PS5000A_COUPLING coupling;
    PS5000A_BANDWIDTH_LIMITER limit; 
    float offset;
    bool enabled; 
    PICO_STATUS status; 
    for (auto it = channels.begin(); it != channels.end(); ++it) {
      chName n = get<aname>(*it);
      std::tie(name, limit, coupling, range, offset, enabled) = convertChannel(n);
      if (enabled) {
	std::cout <<  "enabling Channel!" << std::endl; 
      }
      status = ps5000aSetBandwidthFilter(_handle, name, limit);
      if (status != PICO_OK)
	std::cout << "Failed to set Bandwidth limit" << std::endl;
      
	
      status = ps5000aSetChannel(_handle, name, enabled, coupling, range, offset);
      if (status != PICO_OK)
	std::cout << "Failed updating channel settings" << std::endl;
      
    }
    

  }

  nativeChannel ps5000a::convertChannel(chName name) {
    nativeChannel ch;
    Channel c = channels[name];
    get<aname>(ch) = chNameMap[name];
    get<abandwidth>(ch) = chBwMap[get<abandwidth>(c)];
    get<acoupling>(ch) = chCouplingMap[get<acoupling>(c)];
    get<arange>(ch) = chRangeMap[get<arange>(c)];
    get<aoffset>(ch) = get<aoffset>(c);
    get<aenabled>(ch) = get<aenabled>(c); 
    

    return ch; 
  }
  

  //Trigger Functions
  void ps5000a::setTriggerChannel(chName name) {

  }

  void ps5000a::setTriggerLevel(short adcCount) {

  }

  void ps5000a::setTriggerDelay(unsigned long delay) {

  }

  void ps5000a::setTriggerDirection(triggerDirection direction) {

  }
  void ps5000a::enableTrigger() {

  }
  
  void ps5000a::disableTrigger() {

  }

  void ps5000a::setSimpleTrigger(chName n, short Threshold, triggerDirection direction, unsigned long delay, short autoTrigger) {
    std::cout << "Setting trigger" << std::endl; 
    auto status = ps5000aSetSimpleTrigger(_handle, 1, chNameMap[n], Threshold, triggerDirectionMap[direction], delay, autoTrigger);
    if (status != PICO_OK) {
      std::cout << "Error:" << status << " setting simple trigger." << std::endl;
    }


  }
  
  //Sampling related functions 
  void ps5000a::setOversample(bool oversample) {

  }
  
  void ps5000a::setTimebase(unsigned long timebase) {
    _timebase = timebase; 
  }
  
  void ps5000a::setTimebaseNS(float interval) {

  }
  //Note: Add checks to # of samples
  //Verify against # of captures, will want to rewrite it
  //to have some checking function that both samples and
  //captures use
  void ps5000a::setPreTriggerSamples(unsigned int samples) {
    _nsamples -= _preTriggerSamples;
    _preTriggerSamples = samples;
    _nsamples += _preTriggerSamples; 
  }
  
  void ps5000a::setPostTriggerSamples(unsigned int samples) {
    _nsamples -= _postTriggerSamples; 
    _postTriggerSamples = samples;
    _nsamples += _postTriggerSamples; 
  }

  void ps5000a::setSamples(unsigned int samples) {
    _nsamples = samples;
    _preTriggerSamples = samples/2;
    _postTriggerSamples = samples-_preTriggerSamples; 
  }
  
  bool ps5000a::oversample() {
    //    return _oversample; 
  }
  
  unsigned long ps5000a::maxSamples() {
    return _maxSamples; 
  }
  
  unsigned long ps5000a::timebase() {
    return _timebase; 
  }
  
  float ps5000a::timebaseNS() {
    return calculateTimebase(_timebase, _resolution); 
  }

  
  void ps5000a::setSegment(unsigned long segment) {

  }
  
  unsigned long ps5000a::segment() {

  }


  float ps5000a::adcToMv(float raw, chRange range) {
    float max = _maxADCValue; 
    float r = 0; 
    switch (chRangeMap[range]) {
  case PS5000A_10MV:
    r = 10.0; 
    break; 
  case PS5000A_20MV:
    r = 20.0; 
    break; 
  case PS5000A_50MV:
    r = 50.0; 
    break; 
  case PS5000A_100MV:
    r = 100.0; 
    break; 
  case PS5000A_200MV:
    r = 200.0; 
    break; 
  case PS5000A_500MV:
    r = 500.0; 
    break; 
  case PS5000A_1V:
    r = 1000.0; 
    break; 
  case PS5000A_2V:
    r = 2000.0; 
    break; 
  case  PS5000A_5V:
    r = 5000.0; 
    break; 
  case  PS5000A_10V:
    r = 10000.0; 
    break; 
  case PS5000A_20V:
    r = 20000.0;
    break; 
  case  PS5000A_50V:
    r = 50000.0; 
    break; 
  default:
    r = 50.0; 
    break; 
  }
    return (raw * r) / max; 
  }
  

  //convenience forwarder
  float ps5000a::calculateTimebase(unsigned int timebase, devResolution r)
  {
    return calculateTimebase(timebase, devResolutionMap[r]); 
  }
  
  //Calculate the float-point value of the timebase the picoscope
  //will apply given a particular integer timebase value
  float ps5000a::calculateTimebase(unsigned int timebase, PS5000A_DEVICE_RESOLUTION resolution) {
    unsigned int timebaseL, timebaseH; 
    if (resolution == PS5000A_DR_8BIT) { 
      timebaseL = 1e9; 
      timebaseH = 125e6; 
      if (timebase <= 2) 
	return exp2f(timebase) / timebaseL; 
      else 
	return (timebase - 2) / timebaseH; 
    }
    else if (resolution == PS5000A_DR_12BIT) { 
      std::cout << "12 bit" << " timebase:" << timebase << std::endl; 
      timebaseL = 500e6; 
      timebaseH = 62500000; 
      if (timebase <= 3) 
	return exp2f(timebase-1)/timebaseL; 
      else 
	return (timebase-3) / timebaseH; 
    }
    else if ((resolution == PS5000A_DR_14BIT) || (resolution == PS5000A_DR_15BIT))
      return exp2f(timebase - 2) / 125000000; 
    else if (resolution == PS5000A_DR_16BIT) 
      return exp2f(timebase - 3) / 625000000; 

  }


  void ps5000a::setCaptureCount(unsigned int caps) {
    _captures = caps;
    int32_t maxSamples = 0; 
    auto status = ps5000aMemorySegments(_handle, caps, &maxSamples);
    if (status != PICO_OK) {
      std::cout << "Failed to set waveform capture count to: " <<
	caps << " setting it to 1 instead." << std::endl;
      status = ps5000aMemorySegments(_handle, 1, &maxSamples);
      if (status != PICO_OK) {
	std::cout << "Picoscope problem in setting waveform count." << std::endl;
	return; 
      }
    }
    if (_nsamples > maxSamples) {
      std::cout << "Sample size too large reducing it to: " <<
	maxSamples << "." << std::endl;
      setSamples(maxSamples);
    }

    status = ps5000aSetNoOfCaptures(_handle, caps);
    if (status != PICO_OK) {
      status = ps5000aSetNoOfCaptures(_handle, 1);
      if (status != PICO_OK) {
	std::cout << "Error setting # of captures" << std::endl;
	return;
      }
      std::cout << "Couldn't set number of captures to:" << caps <<
	"set the count to 1 instead." << std::endl;
    }

    
  }
  //I should probably replace this with a set of functions handling
  //this in the set sample, timebase, trigger, and capture count
  //functions. Since it depends on all of those. 
  bool ps5000a::prepareBuffers() {

    waveforms.resize(_captures);
    for (auto &i : waveforms) {
      i.resize(_nsamples);
      //      std::cout << "waveform size:" << i.size() << std::endl;
      
    }
    return true; 
  }

  void ps5000a::callBack(short handle, PICO_STATUS status, void *pParam) {
    std::cout << "Finished capturing waveforms." << std::endl;
    

  }

  void ps5000a::captureBlock() {

    int32_t timeIndisposed = 0;
    void *p = this; 
    /*auto status = ps5000aRunBlock(
				  _handle,
				  _preTriggerSamples,
				  _postTriggerSamples,
				  _timebase,
				  &timeIndisposed,
				  0,
				  &ps5000a::callBack,
				  p); */

    auto status = ps5000aRunBlock(
				  _handle,
				  _preTriggerSamples,
				  _postTriggerSamples,
				  _timebase,
				  &timeIndisposed,
				  0,
				  nullptr,
				  nullptr); 
    if (status != PICO_OK) {
      std::cout << "Error running block: " << status << std::endl;
    }

    //Read data, this will get moved elsewhere eventually, but
    //taking the quick approach for now
    short ready = 0;
    ps5000aIsReady(_handle, &ready); 
    while (ready == 0) {
      ps5000aIsReady(_handle, &ready); 
    }

    //Set Data Buffers
    for (int i = 0; i < _captures; i++) {
      status = ps5000aSetDataBuffer(_handle, PS5000A_CHANNEL_A, waveforms[i].data(), _nsamples, i, PS5000A_RATIO_MODE_NONE);
      if (status != PICO_OK)
	std::cout << "Error setting data buffer:" << status << std::endl; 
    }
    
    uint32_t samplesCaptured = _nsamples;
    std::vector<int16_t> *overflow  = new std::vector<int16_t>(_captures*2); 
    status = ps5000aGetValuesBulk(_handle, &samplesCaptured,0,_captures-1, 1, PS5000A_RATIO_MODE_NONE, overflow->data()); 
    
    if (status != PICO_OK) {
      std::cout << "Error getting values:" << std::hex << status << std::dec << std::endl;
    }

    ps5000aStop(_handle);

  }

  vector< vector<short> > & ps5000a::getWaveforms() {

    return waveforms; 

  }


  int32_t ps5000a::maxADCValue(devResolution r) {
    int32_t max = 0; 
    switch(r) {
    case PS_8BIT:
      max = 32512;
      break;
    case PS_12BIT:
      max = 32767;
      break;
    case PS_14BIT:
      max = 32767;
      break;
    case PS_15BIT:
      max = 32767;
      break;
    case PS_16BIT:
      max = 32767;
      break;
    default:
      max = 32512;
    }
    return max; 

  }
  
}







