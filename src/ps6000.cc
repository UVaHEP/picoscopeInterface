#include "ps6000.h"

using std::get;

namespace picoscope {




  
  ps6000::ps6000() {
    std::cout << "ps6000!" << std::endl;
    _handle = 0;
    _serial = 0;
    _opened = false;
    _update = true;

    chNameMap.emplace(A, PS6000_CHANNEL_A);
    chNameMap.emplace(B, PS6000_CHANNEL_B);
    chNameMap.emplace(C, PS6000_CHANNEL_C);
    chNameMap.emplace(D, PS6000_CHANNEL_D); 


    chBwMap.emplace(BW_FULL, PS6000_BW_FULL);
    chBwMap.emplace(BW_25MHZ, PS6000_BW_25MHZ);
    chBwMap.emplace(BW_20MHZ, PS6000_BW_20MHZ);

    chCouplingMap.emplace(AC, PS6000_AC);
    chCouplingMap.emplace(DC, PS6000_DC_1M);
    chCouplingMap.emplace(DC_50R, PS6000_DC_50R);


    chRangeMap.emplace(PS_10MV, PS6000_10MV); 
    chRangeMap.emplace(PS_20MV, PS6000_20MV);
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
    for (auto it = channels.begin(); it != channels.end(); ++it) {
      chName n = get<aname>(*it);
      std::tie(name, limit, coupling, range, offset, enabled) = convertChannel(n);
      if (enabled) {
	std::cout <<  "enabling Channel!" << std::endl; 
      }
      status = ps6000SetChannel(_handle, name, enabled, coupling, range, offset, limit);
      if (status != PICO_OK)
	std::cout << "Failed updating channel settings" << std::endl;
      
    }
    

  }

  nativeChannel ps6000::convertChannel(chName name) {
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

  }
  
  void ps6000::setTimebaseNS(float interval) {

  }
  
  void ps6000::setPreTriggerSamples(unsigned int samples) {

  }
  
  void ps6000::setPostTriggerSamples(unsigned int samples) {

  }
  
  bool ps6000::oversample() {

  }
  
  unsigned long ps6000::maxSamples() {

  }
  
  unsigned long ps6000::timebase() {

  }
  
  float ps6000::timebaseNS() {

  }
  
  void ps6000::setDeviceResolution(resolution r) {

  }
  
  void ps6000::setSegment(unsigned long segment) {

  }
  
  unsigned long ps6000::segment() {

  }




  
}
