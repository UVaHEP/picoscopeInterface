#ifndef _PICOSCOPE_INTERFACE
#define _PICOSCOPE_INTERFACE

#include <iostream> 
#include <string> 
#include <functional>
#include <map>

using std::tuple;
using std::map; 
using std::string; 
using std::cout; 



namespace picoscope { 


  enum accessors {
    aname = 0,
    abandwidth = 1,
    acoupling = 2,
    arange = 3,
    aoffset = 4,
    aenabled = 5
  }; 

  enum chBandwidth {
    BW_FULL,
    BW_25MHZ,
    BW_20MHZ
  };

  
  enum chCoupling { 
    DC_50R,
    DC, 
    AC, 
  }; 

  enum chName { 
    A,
    B,
    C,
    D,
    EXT,
    AUX
  }; 


  
  enum chRange {
  PS_10MV,
  PS_20MV,
  PS_50MV,
  PS_100MV,
  PS_200MV,
  PS_500MV,
  PS_1V,
  PS_2V,
  PS_5V,
  PS_10V,
  PS_20V,
  PS_50V
  }; 

  enum devResolution {
    PS_8BIT,
    PS_12BIT,
    PS_14BIT,
    PS_15BIT,
    PS_16BIT
  }; 

  enum timebases {



  }; 
  
  
  typedef tuple<chName, chBandwidth, chCoupling, chRange, float, bool> Channel; 
  typedef map<chName, Channel> ChannelList; 

  
  enum triggerDirection {
    trgRising,
    trgFalling,
    trgAbove,
    trgBelow,
    trgAboveLower,
    trgBelowLower,
    trgRisingLower,
    trgFallingLower,
    trgInside,
    trgOutside,
    trgEnter,
    trgExit,
    trgEnterOrExit,
    trgPositiveRunt,
    trgNegativeRunt,
    trgNone
  };

  enum triggerThreshold {
    level,
    window
  }; 

  enum triggerState {
    trgDontCare,
    trgTrue,
    trgFalse,
    trgMax
  };

  

  
  class picoscopeBase 
  { 
    


  public:
    picoscopeBase(); 
    virtual ~picoscopeBase() {}; 

    virtual bool open(); 
    virtual bool openUnit(const string &serial);
    virtual void setDeviceResolution(devResolution r); 
    virtual void close(); 
    virtual string* unitInfo(); 
    virtual void psUpdate(); 

    //Channel Functions
    virtual void enableChannel(chName name);
    virtual void disableChannel(chName name); 
    virtual void setChCoupling(chName name, chCoupling coupling);
    virtual void enableBandwidthLimit(chName name);
    virtual void disableBandwidthLimit(chName name);
    virtual void setChAnalogOffset(chName name, float offset); 
    virtual void resetChannel(chName name);
    virtual void setChRange(chName name, chRange range);
    virtual void setBandwidthFilter(chName name, chBandwidth bw); 
    
    //Trigger Functions
    virtual void setTriggerChannel(chName name);
    virtual void setTriggerLevel(short adcCount);
    virtual void setTriggerDelay(unsigned long delay);
    virtual void setTriggerDirection(triggerDirection direction); 
    virtual void enableTrigger();
    virtual void disableTrigger();
    
    
    //Sampling related functions 
    virtual void setOversample(bool oversample); 
    virtual void setTimebase(unsigned long timebase); 
    virtual void setTimebaseNS(float interval); 
    virtual void setPreTriggerSamples(unsigned int samples); 
    virtual void setPostTriggerSamples(unsigned int samples); 
    virtual void setSamples(unsigned int samples);

    virtual float adcToMv(float raw, chRange range);
    virtual int32_t maxAdcValue(devResolution r); 
    virtual bool oversample(); 
    virtual unsigned long maxSamples(); 
    virtual unsigned long timebase(); 
    virtual float timebaseNS(); 
    virtual float calculateTimebase(unsigned int timebase, devResolution r); 

    virtual void setSegment(unsigned long segment); 
    virtual unsigned long segment();
    virtual void setCaptureCount(unsigned int caps); 
    virtual unsigned int getCaptureCount();

    
    protected:
    int8_t _serial; 
    int16_t _handle;
    int32_t _maxADCValue; 
    bool _opened; 
    bool _update; 
    devResolution _resolution;
    unsigned int _captures;

    unsigned int _preTriggerSamples;
    unsigned int _postTriggerSamples;
    unsigned int _nsamples; 
    float _timeIntervalNS; 
    unsigned long _maxSamples; 
    unsigned long _timebase; 
    ChannelList channels; 

    
  private:


  }; 




}; 


#endif


