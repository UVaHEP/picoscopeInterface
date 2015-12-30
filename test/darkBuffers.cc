#include "picoscopeInterface.h"
#include "ps5000a.h"
#include "TFile.h"
#include "TGraph.h" 
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "utils.h"
#include <TApplication.h>


using namespace picoscope;

int main(int argc, char **argv) {
  TApplication theApp("App", &argc, argv);

  ps5000a dev;
  //int samples = 100;
    int samples = 40000;
  chRange range = PS_20MV;
  dev.open(picoscope::PS_12BIT);
  dev.setChCoupling(picoscope::A, picoscope::DC);
  dev.setChRange(picoscope::A, range);
  dev.enableChannel(picoscope::A);

  dev.enableBandwidthLimit(picoscope::A); 
  dev.setTimebase(1);
  //dev.setSimpleTrigger(EXT, 18000, trgRising, 0, 0); 
  dev.setSamples(samples); 
  dev.setPreTriggerSamples(samples/2);
  dev.setPostTriggerSamples(samples/2);
  //  dev.setCaptureCount(10000);
  dev.setCaptureCount(50);
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


  // delta time distribution
  TH1F *hdTime=new TH1F("hdTime","Delta times;x [2 ns]",50,0,500);
  // pulse height distribution
  TH1F* hpeaks=new TH1F("hpeaks","Peaks",100,0,15000);
  TCanvas *tc=new TCanvas("tc","Samples",1800,600);
  TCanvas *tc1=new TCanvas("tc1","Peaks and Time distribution",0,650,1800,600);
  tc1->Divide(2,1);
  gStyle->SetOptStat(0);
  
  DarkPeaker *dPk = new DarkPeaker();
  
  for (auto &waveform : data) {
    hist = new TH1F("pulses", "pulses", waveform.size(), 0, waveform.size());
    for (int i = 0; i < waveform.size(); i++) {
      hist->SetBinContent(i, -1*waveform[i]);
    }

    dPk->SetBuffer(hist,timebase);
    dPk->AnalyzePeaks();
    
    // retrieve peak heights
    Float_t *yvals = dPk->GetBkgdCorrectedY();
    int npeaks=dPk->GetNPeaks();

    // Fill pulse height histogram
    for (int i=0;i<npeaks;i++) hpeaks->Fill(yvals[i]);
    
    // fill delta time distro
    Float_t *deltaX = dPk->GetDeltaX();
    for (int i=0;i<npeaks-1;i++) hdTime->Fill(deltaX[i]);

    // draw samples buffer with peaks and background
    tc->cd();
    hist->DrawCopy();
    dPk->GetBackground()->Draw("same");
    tc->Update();

    // draw pulse analysis plots
    tc1->cd(1);
    hpeaks->Draw();
    tc1->cd(2);
    hdTime->Fit("expo","L");
    tc1->Update();

    hist->Write(); 
    delete hist; 
  }

  theApp.Run(true);
  f.Close(); 
  


  
  return 0;


}

