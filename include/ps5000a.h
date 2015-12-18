#ifndef _PS5000A_H
#define _PS5000A_H

#include <string>
#include <vector>

#include "picoscopeInterface.h"

#include <libps5000a-1.1/ps5000aApi.h>
#ifndef PICO_STATUS
#include <libps5000a-1.1/PicoStatus.h>
#endif

using std::string;
using std::vector;

namespace picoscope {

  typedef tuple<PS5000A_CHANNEL, PS5000A_BANDWIDTH_LIMITER, PS5000A_COUPLING, PS5000A_RANGE, float, bool> nativeChannel;


  class ps5000a : public picoscopeBase {


    
    map<chName, PS5000A_CHANNEL> chNameMap; 
    map<chBandwidth, PS5000A_BANDWIDTH_LIMITER> chBwMap;
    map<chCoupling, PS5000A_COUPLING> chCouplingMap;
    map<chRange, PS5000A_RANGE> chRangeMap; 
    map<devResolution, PS5000A_DEVICE_RESOLUTION> devResolutionMap;
    map<triggerDirection, PS5000A_THRESHOLD_DIRECTION> triggerDirectionMap; 
    public: 
    ps5000a();
    ~ps5000a() {};

    bool open(devResolution r = PS_8BIT);
    void close();
    void psUpdate(); 
    bool statusProcessor(PICO_STATUS status);
    void setDeviceResolution(devResolution r); 

    nativeChannel convertChannel(chName name); 
        
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
    void setSamples(unsigned int samples);
    float calculateTimebase(unsigned int timebase, devResolution r);
    float calculateTimebase(unsigned int timebase, PS5000A_DEVICE_RESOLUTION resolution); 
    bool oversample(); 
    unsigned long maxSamples(); 
    unsigned long timebase(); 
    float timebaseNS(); 
    void setCaptureCount(unsigned int caps); 
    void setSimpleTrigger(chName n, short Threshold, triggerDirection direction, unsigned long delay, short autoTrigger); 

    
    void setSegment(unsigned long segment); 
    unsigned long segment();
    bool prepareBuffers(); 
    vector< vector<short> > &getWaveforms();
    void captureBlock();
    float adcToMv(float raw, chRange range);
    int32_t maxADCValue(devResolution r);
    static void callBack(short handle, PICO_STATUS status, void *pParam);

  private:
    vector< vector< short> > waveforms; 
    bool *ready; 

    
  }; 


};


#endif 
