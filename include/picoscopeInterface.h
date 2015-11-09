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
    D
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

  typedef tuple<chName, chBandwidth, chCoupling, chRange, float, bool> Channel; 
  typedef map<chName, Channel> ChannelList; 

  
  enum resolution {
    PS_8BIT,
    PS_12BIT,
    PS_14BIT,
    PS_15BIT,
    PS_16BIT
  }; 
  
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
    virtual void close(); 
    virtual string* unitInfo(); 
    virtual void psUpdate(); 

    //Channel Functions -- Note, have them return a PICO_STATUS value
    virtual void enableChannel(chName name);
    virtual void disableChannel(chName name); 
    virtual void setChCoupling(chName name, chCoupling coupling);
    virtual void enableBandwidthLimit(chName name);
    virtual void disableBandwidthLimit(chName name);
    virtual void setChAnalogOffset(chName name, float offset); 
    virtual void resetChannel(chName name);
    virtual void setChRange(chName name, chRange range); 
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

    virtual bool oversample(); 
    virtual unsigned long maxSamples(); 
    virtual unsigned long timebase(); 
    virtual float timebaseNS(); 
    virtual void setDeviceResolution(resolution r); 

    virtual void setSegment(unsigned long segment); 
    virtual unsigned long segment();

    protected:
    int8_t _serial; 
    int16_t _handle; 
    bool _opened; 
    bool _update; 

    float _timeIntervalNS; 
    unsigned long _maxSamples; 
    unsigned long _timebase; 
    ChannelList channels; 

    
  private:


  }; 




}; 


#endif

