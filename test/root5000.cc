#include "picoscopeInterface.h"
#include "ps5000a.h"
#include "TFile.h"
#include "TGraph.h" 
#include "TH1F.h"


using namespace picoscope;

int main() {


  ps5000a dev;
  //  int samples = 100;
  int samples = 40000;
  chRange range = PS_20MV;
  dev.open(picoscope::PS_12BIT);
  dev.setChCoupling(picoscope::A, picoscope::DC);
  dev.setChRange(picoscope::A, range);
  dev.enableChannel(picoscope::A);

  dev.enableBandwidthLimit(picoscope::A); 
  dev.setTimebase(1);
  //  dev.setSimpleTrigger(EXT, 18000, trgRising, 0, 0); 
  dev.setSamples(samples); 
  dev.setPreTriggerSamples(samples/2);
  dev.setPostTriggerSamples(samples/2);
  //  dev.setCaptureCount(100000);
  dev.setCaptureCount(20);
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
  TH1F *hist = NULL;

  TH1F *dT  = new  TH1F("dT", "Time Steps [ns]", 1,0,1);
  dT->Fill(0.0, timebase);
  TH1F *dV = new TH1F("dV", "Voltage Steps[mV]", 1,0,1);
  dV->Fill(0.0, dev.adcToMv(1, range));
  std::cout << "dV:" << dev.adcToMv(1, range) << std::endl; 
  dT->Write();
  dV->Write();

  delete dT;
  delete dV; 
  for (auto &waveform : data) {
    hist = new TH1F("pulses", "pulses", waveform.size(), 0, waveform.size());
    for (int i = 0; i < waveform.size(); i++) {
      hist->SetBinContent(i, -1*waveform[i]);
    }
    
    /*    for (int i = 0; i < waveform.size(); i++) {
      graphWaveform[i] = float(dev.adcToMv(waveform[i], range));
      }*/

    //    TGraph *graph = new TGraph(graphtime.size(), graphtime.data(), graphWaveform.data());
    //    graph->Write();

    //    delete graph;
    hist->Write(); 
    delete hist; 

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




