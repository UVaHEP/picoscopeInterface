#include "picoscopeInterface.h"


namespace picoscope { 


  picoscopeBase::picoscopeBase() {

    
    _handle = -1;
    _serial = -1; 
    _opened = false;
    _update = true; 
    _maxADCValue = 32767;
    
    channels[A] = Channel(A,
			  BW_FULL,
			  DC_50R,
			  PS_1V,
			  0.0,
			  false); 

    channels[B] = Channel(B,
			  BW_FULL,
			  DC_50R,
			  PS_1V,
			  0.0,
			  false);

    channels[C] = Channel(C,
			  BW_FULL,
			  DC_50R,
			  PS_1V,
			  0.0,
			  false); 

    channels[D] = Channel(D, 
			  BW_FULL,
			  DC_50R,
			  PS_1V,
			  0.0,
			  false); 

    
  }
  bool picoscopeBase::open() { std::cout << "Not implemented." << std::endl; };
  void picoscopeBase::close() { std::cout << "Not implemented." << std::endl; };  
  bool picoscopeBase::openUnit(const string &serial) { std::cout << "Not implemented." << std::endl; };
  void picoscopeBase::setDeviceResolution(devResolution r) { std::cout << "Not implemented." << std::endl; };  
  void picoscopeBase::psUpdate() { std::cout << "PS Update, not implemented" << std::endl; }; 
  string* picoscopeBase::unitInfo() { return NULL; };

  void picoscopeBase::enableChannel(chName name) {
    std::cout << "Turning on a channel" << std::endl; 
    std::get<aenabled>(channels[name]) = true;
    psUpdate(); 
    }
  void picoscopeBase::disableChannel(chName name) {
    std::cout << "Turning off a channel" << std::endl; 
    std::get<aenabled>(channels[name]) = false;
    psUpdate(); 
    }
  void picoscopeBase::setBandwidthFilter(chName name, chBandwidth bw) {
    std::get<abandwidth>(channels[name]) = bw;
    psUpdate();
  }
  void picoscopeBase::enableBandwidthLimit(chName name) {
    std::get<abandwidth>(channels[name]) = BW_20MHZ;
    psUpdate(); 
    }
  void picoscopeBase::disableBandwidthLimit(chName name) {
    std::get<abandwidth>(channels[name]) = BW_FULL;
    psUpdate(); 
    }
  void picoscopeBase::setChCoupling(chName name, chCoupling coupling) {
    std::get<acoupling>(channels[name]) = coupling;
    psUpdate(); 
  }
  void picoscopeBase::setChRange(chName name, chRange range) {
    std::get<arange>(channels[name]) = range;
    psUpdate(); 
    } 
  void picoscopeBase::setChAnalogOffset(chName name, float offset) {
    std::get<aoffset>(channels[name]) = offset;
    psUpdate(); 
  }
  
  void picoscopeBase::resetChannel(chName name) {
    std::cout << "Resetting Channel" << std::endl;
  }
  //Trigger Functions
  void picoscopeBase::setTriggerChannel(chName name) { std::cout << "Not implemented." << std::endl; }; 
  void picoscopeBase::setTriggerLevel(short adcCount) { std::cout << "Not implemented." << std::endl; }; 
  void picoscopeBase::setTriggerDelay(unsigned long delay) { std::cout << "Not implemented." << std::endl; }; 
  void picoscopeBase::setTriggerDirection(triggerDirection direction) { std::cout << "Not implemented." << std::endl; };  
  void picoscopeBase::enableTrigger() { std::cout << "Not implemented." << std::endl; }; 
  void picoscopeBase::disableTrigger() { std::cout << "Not implemented." << std::endl; }; 

    
  //Sampling related functions 
  void picoscopeBase::setOversample(bool oversample) { std::cout << "Not implemented." << std::endl; };  
  void picoscopeBase::setTimebase(unsigned long timebase) { std::cout << "Not implemented." << std::endl; };  
  void picoscopeBase::setTimebaseNS(float interval) { std::cout << "Not implemented." << std::endl; };  
  void picoscopeBase::setPreTriggerSamples(unsigned int samples) { std::cout << "Not implemented." << std::endl; };  
  void picoscopeBase::setPostTriggerSamples(unsigned int samples) { std::cout << "Not implemented." << std::endl; };
  void picoscopeBase::setSamples(unsigned int samples) {
std::cout << "Not implemented." << std::endl; };
  float picoscopeBase::calculateTimebase(unsigned int timebase, devResolution r) {
    std::cout << "Not Implemented." << std::endl;
  }
  bool picoscopeBase::oversample() { std::cout << "Not implemented." << std::endl; };  
  unsigned long picoscopeBase::maxSamples() { std::cout << "Not implemented." << std::endl; };  
  unsigned long picoscopeBase::timebase() { std::cout << "Not implemented." << std::endl; };  
  float picoscopeBase::timebaseNS() { std::cout << "Not implemented." << std::endl; };

  float picoscopeBase::adcToMv(float raw, chRange range) {
    return 0;
  }
  void picoscopeBase::setSegment(unsigned long segment) { std::cout << "Not implemented." << std::endl; };  
  unsigned long picoscopeBase::segment() { std::cout << "Not implemented." << std::endl; };


  void picoscopeBase::setCaptureCount(unsigned int caps) {
    _captures = caps;
  }

  unsigned int picoscopeBase::getCaptureCount() {
    return _captures;
  }
    
  int32_t picoscopeBase::maxAdcValue(devResolution r) {
    return _maxADCValue;
  }
  
}

 
