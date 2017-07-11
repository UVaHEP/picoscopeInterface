#include "ps6000.h"

#include <cmath>
using std::get;

namespace picoscope {




  
  ps6000::ps6000() {
    _handle = 0;
    _serial = 0;
    _opened = false;
    _update = true;
    _model = PS_6000;

    
    chNameMap.emplace(A, PS6000_CHANNEL_A);
    chNameMap.emplace(B, PS6000_CHANNEL_B);
    chNameMap.emplace(C, PS6000_CHANNEL_C);
    chNameMap.emplace(D, PS6000_CHANNEL_D); 
    chNameMap.emplace(AUX, PS6000_TRIGGER_AUX); 

    chBwMap.emplace(BW_FULL, PS6000_BW_FULL);
    chBwMap.emplace(BW_25MHZ, PS6000_BW_25MHZ);
    chBwMap.emplace(BW_20MHZ, PS6000_BW_20MHZ);

    chCouplingMap.emplace(AC, PS6000_AC);
    chCouplingMap.emplace(DC, PS6000_DC_1M);
    chCouplingMap.emplace(DC_50R, PS6000_DC_50R);

    // ps6000 doesn't have the 10 or 20 mV Ranges
    //    chRangeMap.emplace(PS_10MV, PS6000_10MV); 
    //    chRangeMap.emplace(PS_20MV, PS6000_20MV);
    chRangeMap.emplace(PS_50MV, PS6000_50MV);
    chRangeMap.emplace(PS_100MV, PS6000_100MV);
    chRangeMap.emplace(PS_200MV, PS6000_200MV);
    chRangeMap.emplace(PS_500MV, PS6000_500MV);
    chRangeMap.emplace(PS_1V, PS6000_1V);
    chRangeMap.emplace(PS_2V, PS6000_2V);
    chRangeMap.emplace(PS_5V, PS6000_5V);
    chRangeMap.emplace(PS_10V, PS6000_10V);
    chRangeMap.emplace(PS_20V, PS6000_20V);
    chRangeMap.emplace(PS_50V, PS6000_50V);

    triggerDirectionMap.emplace(trgRising, PS6000_RISING);
    triggerDirectionMap.emplace(trgFalling, PS6000_FALLING);
    triggerDirectionMap.emplace(trgAbove, PS6000_ABOVE);
    triggerDirectionMap.emplace(trgBelow, PS6000_BELOW);
    triggerDirectionMap.emplace(trgNone, PS6000_NONE); 
    



  }

  

  bool ps6000::open() {

    if (_opened)
      return true; 
    std::cout << "opening device" << std::endl;

  PICO_STATUS status; 

  if (_serial == 0) { 
    status = ps6000OpenUnit(&_handle, NULL); 
  }
  else {
    status = ps6000OpenUnit(&_handle, &_serial); 
  }

  _opened = statusProcessor(status); 
  //  if (_opened)
  //    getPicoscopeInfo(); 

  
  return _opened; 

    
  }

  void ps6000::close() {
    PICO_STATUS status = ps6000CloseUnit(_handle); 
    statusProcessor(status); 
  }

  void ps6000::setDeviceResolution(devResolution r) {
    std::cout << "Not Supported!" << std::endl; 

  }


  bool ps6000::statusProcessor(PICO_STATUS status) { 
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


  void ps6000::psUpdate() {
    std::cout << "Updating picoscope channel settings" << std::endl;
    //for most of these we have a 1:1 mapping between enums for the
    //channels.
    PS6000_CHANNEL name;
    PS6000_RANGE range;
    PS6000_BANDWIDTH_LIMITER limit;
    PS6000_COUPLING coupling;
    float offset;
    bool enabled;
    PICO_STATUS status; 
    for (auto ch : channels) {
      chName n = get<aname>(ch);
      std::tie(name, limit, coupling, range, offset, enabled) = convertChannel(n);
      if (enabled) {
	std::cout <<  "enabling Channel!" << std::endl; 
      }
      status = ps6000SetChannel(_handle, name, enabled, coupling, range, offset, limit);
      if (status != PICO_OK)
	std::cout << "Failed updating channel settings" << std::endl;
    }


  }

  ps6000::nativeChannel ps6000::convertChannel(chName name) {
    ps6000::nativeChannel ch;
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

  void ps6000::setSimpleTrigger(chName n, short Threshold, triggerDirection direction, unsigned long delay, short autoTrigger) {
    std::cout << "Setting Trigger on Channel:" << n << std::endl;
    auto status = ps6000SetSimpleTrigger(_handle, 1, chNameMap[n], Threshold, triggerDirectionMap[direction], delay, autoTrigger);
    if (status != PICO_OK)
      std::cout << "Error:" << status << " setting simple trigger." << std::endl; 


  }
  
  void ps6000::setTriggerChannel(chName name) {

  }

  void ps6000::setTriggerLevel(short adcCount) {

  }

  void ps6000::setTriggerDelay(unsigned long delay) {

  }

  void ps6000::setTriggerDirection(triggerDirection direction) {

  }
  void ps6000::enableTrigger() {

  }
  
  void ps6000::disableTrigger() {

  }

  //Sampling related functions 
  void ps6000::setOversample(bool oversample) {

  }
  
  void ps6000::setTimebase(unsigned long timebase) {
    _timebase = timebase; 
  }
  
  void ps6000::setTimebaseNS(float interval) {

  }


  //Pull these out and put them into the base class
  void ps6000::setPreTriggerSamples(unsigned int samples) {
    _nsamples -= _preTriggerSamples;
    _preTriggerSamples = samples;
    _nsamples += _preTriggerSamples; 
  }
  
  void ps6000::setPostTriggerSamples(unsigned int samples) {
    _nsamples -= _postTriggerSamples; 
    _postTriggerSamples = samples;
    _nsamples += _postTriggerSamples; 

  }

  void ps6000::setSamples(unsigned int samples) {
    _nsamples = samples;
    _preTriggerSamples = samples/2;
    _postTriggerSamples = samples-_preTriggerSamples;

    std::cout << "Settings samples to:" << std::endl
	      << "samples:" << samples << std::endl
	      << "Pre Trigger:" << _preTriggerSamples << std::endl
	      << "Post Trigger:" << _postTriggerSamples << std::endl; 
      
  }

  
  bool ps6000::oversample() {

  }

  short ps6000::getMinADC() {

    return PS6000_MIN_VALUE;
  }
  short ps6000::getMaxADC() {

    return PS6000_MAX_VALUE;
  }

  
  unsigned long ps6000::maxSamples() {
    return _maxSamples; 
  }
  
  unsigned long ps6000::timebase() {
    return _timebase; 
  }
  
  float ps6000::timebaseNS() {
    unsigned long timebaseL, timebaseH;
    timebaseH = 156250000;
    timebaseL = 5000000000;
    if (_timebase <= 4)
      return exp2f(_timebase) / timebaseL;
    else
      return (_timebase-4) / timebaseH; 
  }

  
  void ps6000::setSegment(unsigned long segment) {

  }
  
  unsigned long ps6000::segment() {

  }



  //This should probably internally verify the sample/block values rather than checking vs the picoscope. For ease of implementation
  //I'm verifying vs the scope for now. -- TA 
  bool ps6000::checkBlockLimit(unsigned int count, unsigned int samples) {


    //Verify that we don't try to grab more samples/blocks than are available.
    unsigned int mSamples = 0; 
    auto status = ps6000MemorySegments(_handle, count, &mSamples);
    if (PICO_OK != status) {
      std::cout << "Error setting memory segments." << std::endl;
      return false;
    }
    else if (samples > mSamples) {
      std::cout << "Warning! Max samples for " << count << " segments is " << mSamples
		<<". Only " << mSamples << " will get captured." << std::endl;
      return false; 
    }
    setSamples(samples);
    _captures = count; 
      

    
    return true; 

  }
  
  void ps6000::captureBlocks(unsigned int count, unsigned int samples) {
    int32_t timeIndisposed = 0;
    void *p = this;
    if (!checkBlockLimit(count, samples))
      return;

    
    //Capture <count> blocks of <samples> size each 
    for (int i = 0; i < count; i++) {
      std::cout << "Capturing block: " << i << std::endl; 

      
      auto status = ps6000RunBlock(
				   _handle,
				   _preTriggerSamples,
				   _postTriggerSamples,
				   _timebase,
				   0,
				   &timeIndisposed,
				   i,
				   nullptr,
				   nullptr
				   );

      short ready = 0;
      ps6000IsReady(_handle, &ready); 
      while (ready == 0) {
	ps6000IsReady(_handle, &ready);
      }

      std::cout << "Post Run Block" << std::endl;
      
      if (!verifyStatus(status)) 
	return;
    }
    std::cout << "Done capturing blocks...preparing buffers" << std::endl; 
    prepareBuffers(); 

    for (int i = 0; i < _captures; i++) {
      std::cout << "Getting capture:" << i << std::endl;
      uint32_t samplesCaptured = _nsamples;
      int16_t overflow = 0;
      
      if (!verifyStatus(ps6000SetDataBuffer(_handle, PS6000_CHANNEL_A, waveforms[i].data(), _nsamples,PS6000_RATIO_MODE_NONE)))
	return;

      if(!verifyStatus(ps6000GetValues(_handle, 0, &samplesCaptured, 1, PS6000_RATIO_MODE_NONE, i, &overflow)))
	return;

    }
    ps6000Stop(_handle);     
    

  }
  

  bool ps6000::verifyStatus(PICO_STATUS status) {

    switch(status) {
    case PICO_OK: {
      return true;
    }
    default: {
      std::cout << "PICO_STATUS not ok: " << status << std::endl;
      break;
    }

    }

    return false; 

  }
  
  void ps6000::captureBlock() {

    int32_t timeIndisposed = 0;
    void *p = this; 
    auto status = ps6000RunBlock(
				  _handle,
				  _preTriggerSamples,
				  _postTriggerSamples,
				  _timebase,
				  0,
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
    ps6000IsReady(_handle, &ready); 
    while (ready == 0) {
      ps6000IsReady(_handle, &ready); 
    }

    //Set Data Buffers
    for (int i = 0; i < _captures; i++) {
      status = ps6000SetDataBufferBulk(_handle, PS6000_CHANNEL_A, waveforms[i].data(), _nsamples, i, PS6000_RATIO_MODE_NONE);
      if (status != PICO_OK)
	std::cout << "Error setting data buffer:" << status << std::endl; 
    }
    
    uint32_t samplesCaptured = _nsamples;
    std::vector<int16_t> *overflow  = new std::vector<int16_t>(_captures*2); 
    status = ps6000GetValuesBulk(_handle, &samplesCaptured,0,_captures-1, 1, PS6000_RATIO_MODE_NONE, overflow->data()); 
    
    if (status != PICO_OK) {
      std::cout << "Error getting values:" << std::hex << status << std::dec << std::endl;
    }

    ps6000Stop(_handle);


  }

  bool ps6000::prepareBuffers() {
    waveforms.resize(_captures);
    for (auto &i : waveforms) {
      i.resize(_nsamples);
      //      std::cout << "waveform size:" << i.size() << std::endl;
      
    }
    return true; 


  }

  float ps6000::mVToADC(float mv, chRange range) {
    float max = _maxADCValue;
    float r = 0;
    switch (chRangeMap[range]) {
    case PS6000_50MV:
      r = 50.0; 
      break; 
    case PS6000_100MV:
      r = 100.0; 
      break; 
    case PS6000_200MV:
      r = 200.0; 
      break; 
    case PS6000_500MV:
      r = 500.0; 
      break; 
    case PS6000_1V:
      r = 1000.0; 
      break; 
    case PS6000_2V:
      r = 2000.0; 
      break; 
    case  PS6000_5V:
      r = 5000.0; 
      break; 
    case  PS6000_10V:
      r = 10000.0; 
      break; 
    case PS6000_20V:
      r = 20000.0;
      break; 
    case  PS6000_50V:
      r = 50000.0; 
      break; 
    default:
      r = 50.0; 
      break; 
    }

    return (mv*max)/r;
  }
  
  float ps6000::adcToMv(float raw, chRange range) {
    float max = _maxADCValue; 
    float r = 0; 
    switch (chRangeMap[range]) {
  case PS6000_50MV:
    r = 50.0; 
    break; 
  case PS6000_100MV:
    r = 100.0; 
    break; 
  case PS6000_200MV:
    r = 200.0; 
    break; 
  case PS6000_500MV:
    r = 500.0; 
    break; 
  case PS6000_1V:
    r = 1000.0; 
    break; 
  case PS6000_2V:
    r = 2000.0; 
    break; 
  case  PS6000_5V:
    r = 5000.0; 
    break; 
  case  PS6000_10V:
    r = 10000.0; 
    break; 
  case PS6000_20V:
    r = 20000.0;
    break; 
  case  PS6000_50V:
    r = 50000.0; 
    break; 
  default:
    r = 50.0; 
    break; 
  }
    return (raw * r) / max; 
  }
  

  void ps6000::callBack(short handle, PICO_STATUS status, void *pParam) {
    std::cout << "Finished capturing waveforms." << std::endl;
    

  }


  void ps6000::setCaptureCount(unsigned int caps) {
    _captures = caps;
    unsigned int maxSamples = 0; 
    auto status = ps6000MemorySegments(_handle, caps, &maxSamples);
    if (status != PICO_OK) {
      std::cout << "Failed to set waveform capture count to: " <<
	caps << " setting it to 1 instead." << std::endl;
      status = ps6000MemorySegments(_handle, 1, &maxSamples);
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

    status = ps6000SetNoOfCaptures(_handle, caps);
    if (status != PICO_OK) {
      status = ps6000SetNoOfCaptures(_handle, 1);
      if (status != PICO_OK) {
	std::cout << "Error setting # of captures" << std::endl;
	return;
      }
      std::cout << "Couldn't set number of captures to:" << caps <<
	"set the count to 1 instead." << std::endl;
    }


  }


  chRange ps6000::autoRange(int nbufs) {
    std::cout << "Model:" << _model << std::endl; 
    int mvRange[]={10,20,50,100,200,500,1000,2000,5000};
    setChRange(picoscope::A, PS_50MV);
    setCaptureCount(nbufs);
    chRange autoRange=PS_50MV;
    int mvScale=0;
    for ( int psRange=PS_50MV; psRange <= PS_5V; psRange++ ){
      std::cout<<"Autoranging pass: " << mvRange[psRange] << "mV range" << std::endl;
      autoRange=(chRange)(psRange);
      mvScale=mvRange[psRange];
      setChRange(picoscope::A, autoRange);
      prepareBuffers();
      captureBlock();
      bool overThresh=false;
      for (auto &waveform : waveforms) {
	for (int i = 0; i < waveform.size(); i++) {
	  if (std::abs(waveform[i])>26000){      // ~ 80% of ADC range
	    overThresh=true;
	    break;
	  }
	}
      } // end of buffer loop
      if (!overThresh) return autoRange;  // CLEAN ME UP!
    }
    return autoRange;
  }

  
}
