#include "picoscopeInterface.h"
#include "ps6000.h"
#include "ps5000a.h"


using namespace picoscope;



void setupScope(picoscopeBase &dev,   chRange &range, int samples, int nbuffers) { 
  dev.setChCoupling(picoscope::A, picoscope::DC_50R);
  dev.setChRange(picoscope::A, range);//allows us to set range on Picoscope
  dev.enableChannel(picoscope::A);
  dev.setTimebase(1); //400 ns
  //  dev.setSimpleTrigger(AUX, 18000, trgFalling, 0, 0);//When triggering off anything else
  dev.setSamples(samples); 
  dev.setPreTriggerSamples(samples/2);
  dev.setPostTriggerSamples(samples/2);
  dev.setCaptureCount(nbuffers);
  dev.prepareBuffers();  
}


picoscopeBase *findScope() {
  ps5000a *d5000a = NULL; 
  ps6000 *d6000 = new ps6000(); 

  if (!d6000->open()) {
    delete d6000;
    d5000a = new ps5000a();
    if (!d5000a->open()) {
      std::cout << "No picoscope found!" << std::endl;
      delete d5000a;
      return NULL;
    }
    return d5000a;
  }

  return d6000; 
}




int main() {

  picoscopeBase *b = findScope();
  picoscopeBase &dev = *b;
  int samples = 500;  // default samples and buffer numbers
  int nbuffers = 2;
  chRange range = PS_50MV;
  setupScope(dev, range, samples, nbuffers);
  range = dev.autoRange(100); 
  dev.setCaptureCount(nbuffers);
  
  dev.prepareBuffers();
  dev.captureBlock(); 
  for (auto &waveform : dev.getWaveforms()) {
    for (int i = 0; i < waveform.size(); i++) {

      std::cout << waveform[i] << " ";
    }
    std::cout << std::endl << std::endl;
  }

  
  
  dev.close();
  delete b; 

  return 0; 

}
