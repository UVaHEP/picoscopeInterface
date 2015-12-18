#include "picoscopeInterface.h"
#include "ps5000a.h"
#include "TFile.h"
#include "TGraph.h" 

using namespace picoscope;

int main() {


  ps5000a dev;
  int samples = 100;
  chRange range = PS_20MV;
  dev.open(picoscope::PS_12BIT);
  dev.setChCoupling(picoscope::A, picoscope::DC);
  dev.setChRange(picoscope::A, range);
  //  dev.setChRange(picoscope::D, PS_5V); 
  dev.enableChannel(picoscope::A);
  //  dev.enableChannel(picoscope::D); 
  dev.enableBandwidthLimit(picoscope::A); 
  dev.setTimebase(1);
  dev.setSimpleTrigger(EXT, 18000, trgRising, 0, 0); 
  dev.setSamples(samples); 
  dev.setPreTriggerSamples(samples/2);
  dev.setPostTriggerSamples(samples/2);
  dev.setCaptureCount(10000);
  dev.prepareBuffers();
  dev.captureBlock(); 
  dev.close();

  float timebase = dev.timebaseNS();
  std::cout << "Timebase: " << timebase << std::endl; 
  vector <vector<short> > data = dev.getWaveforms();

  vector<float> graphWaveform(data[0].size());
  vector<float> graphtime(graphWaveform.size());
  float timebaseStart = timebase*samples/2*-1;

  for (int i = 0; i < data[0].size(); i++) {
    graphtime[i] = timebaseStart+i*timebase;
  }


  TFile f("test.root", "RECREATE");
  std::cout << "writing waveforms..." << std::endl; 
  for (auto &waveform : data) {

    
    for (int i = 0; i < waveform.size(); i++) {
      graphWaveform[i] = float(dev.adcToMv(waveform[i], range));
    }

    TGraph *graph = new TGraph(graphtime.size(), graphtime.data(), graphWaveform.data());
    graph->Write();
    delete graph; 

  }


  f.Close(); 
  
  /*  for (auto &waveform : data) {
    std::cout << std::endl;
    std::cout << std::endl; 
    for (int i = 0; i < waveform.size(); i++) { 
      std::cout << dev.adcToMv(waveform[i], range) << ", ";

    }
    std::cout << std::endl;
    std::cout << std::endl;
    }*/

  
  return 0;


}




