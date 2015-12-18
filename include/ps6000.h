#ifndef _PS6000_H
#define _PS6000_H

#include <string>


#include "picoscopeInterface.h"
#include <libps6000-1.4/ps6000Api.h>
#ifndef PICO_STATUS
#include <libps6000-1.4/PicoStatus.h>
#endif

using std::string; 

namespace picoscope {
  //Note: Adding a bool for chRes 6404 even though its not used to
  //keep the sizes of the tuple the same
  typedef tuple<PS6000_CHANNEL, PS6000_BANDWIDTH_LIMITER, PS6000_COUPLING, PS6000_RANGE, float, bool> nativeChannel;


  class ps6000 : public picoscopeBase {

    map<chName, PS6000_CHANNEL> chNameMap; 
    map<chBandwidth, PS6000_BANDWIDTH_LIMITER> chBwMap;
    map<chCoupling, PS6000_COUPLING> chCouplingMap;
    map<chRange, PS6000_RANGE> chRangeMap; 

    
    public: 
    ps6000();
    ~ps6000() {};

    
    bool open();
    void close();
    void psUpdate(); 
    bool statusProcessor(PICO_STATUS status);
    void setDeviceResolution(devResolution r);
    
    //Lookup Fns -- May want to replace with maps/arrays/etc
    nativeChannel convertChannel(chName name); 
    PS6000_COUPLING lookupChCoupling(chCoupling coupling); 
    PS6000_BANDWIDTH_LIMITER lookupChBandwidth(chBandwidth bw); 
    PS6000_RANGE lookupChRange(chRange range); 
    PS6000_CHANNEL lookupChName(chName name);
    
    //Channel Functions
    //    void enableChannel(chName name);
    //    void disableChannel(chName name); 
    //    void setChCoupling(chName name, chCoupling coupling);
    //    void enableBandwidthLimit(chName name);
    //    void disableBandwidthLimit(chName name);
    //    void setChAnalogOffset(chName name, float offset); 
    //    void resetChannel(chName name);
    //    void setChRange(chName name, chRange range); 

    //Trigger Functions
    void setTriggerChannel(chName name);
    void setTriggerLevel(short adcCount);
    void setTriggerDelay(unsigned long delay);
    void setTriggerDirection(triggerDirection direction); 
    void enableTrigger();
    void disableTrigger();

    //Sampling related functions 
    void setOversample(bool oversample); 
    void setTimebase(unsigned long timebase); 
    void setTimebaseNS(float interval); 
    void setPreTriggerSamples(unsigned int samples); 
    void setPostTriggerSamples(unsigned int samples); 

    bool oversample(); 
    unsigned long maxSamples(); 
    unsigned long timebase(); 
    float timebaseNS(); 

    void setSegment(unsigned long segment); 
    unsigned long segment();

    
  private:



    
  }; 


};


#endif 








