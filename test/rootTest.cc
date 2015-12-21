#include "picoscopeInterface.h"
#include "ps5000a.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <iostream>


using namespace picoscope;

void Persist(TH1F* h, TH2F* p){
  for  (int i=1; i<=h->GetNbinsX(); i++)
    p->Fill(i,h->GetBinContent(i));
}


void PHD(TH1F* h, TH1F* p, int firstbin, int lastbin){
  p->Fill(h->Integral(firstbin,lastbin));
}



void rootTest(){
  ps5000a dev;
  int samples = 100;
  chRange range = PS_20MV;
  dev.open(picoscope::PS_12BIT);
  dev.setChCoupling(picoscope::A, picoscope::DC);
  dev.setChRange(picoscope::A, range);
  dev.enableChannel(picoscope::A);

  dev.enableBandwidthLimit(picoscope::A); 
  dev.setTimebase(1);
  dev.setSimpleTrigger(EXT, 18000, trgRising, 0, 0); 
  dev.setSamples(samples); 
  dev.setPreTriggerSamples(samples/2);
  dev.setPostTriggerSamples(samples/2);
  dev.setCaptureCount(100000);
  //  dev.setCaptureCount(20);
  

  dev.prepareBuffers();
  dev.captureBlock(); 
  dev.close();

  float timebase = dev.timebaseNS();
  std::cout << "Timebase: " << timebase << std::endl; 
  vector <vector<short> > data = dev.getWaveforms();

  
  //  TFile *f = new TFile("test.root", "RECREATE");
  std::cout << "writing waveforms..." << std::endl;

  TH1F *dT  = new  TH1F("dT", "Time Steps [ns]", 1,0,1);
  dT->Fill(0.0, timebase);
  TH1F *dV = new TH1F("dV", "Voltage Steps[mV]", 1,0,1);
  dV->Fill(0.0, dev.adcToMv(1, range));
  std::cout << "dV:" << dev.adcToMv(1, range) << std::endl; 
  //  dT->Write();
  //  dV->Write();

  
  TH2F *hpersist=new TH2F("hpersist","Persistance Display",samples,
			0,samples,100,0,15000);

  TH1F* hpulses1=new TH1F("hpulses1","Pulse area distribution",200,-20000,200000);
  TH1F* hpulses0=new TH1F("hpulses0","Pulse area distribution",200,-20000,200000);

  
  delete dT;
  delete dV; 
  for (auto &waveform : data) {
    TH1F *hsamp = new TH1F("hsamp","Samples", waveform.size(), 0, waveform.size());
    for (int i = 0; i < waveform.size(); i++) {
      hpersist->Fill(i, -1*waveform[i]);
      hsamp->SetBinContent(i, -1*waveform[i]);
    }
    PHD(hsamp,hpulses1,54,72);
    PHD(hsamp,hpulses0,4,22);
    delete hsamp; 
    //    hpersist->Write(); 
  }
  TFile *f = new TFile("test.root", "RECREATE");
  
  TCanvas *tc=new TCanvas("tc","Pulse heights",1800,600);
  tc->Divide(3,1);
  tc->cd(1)->SetLogy();
  hpersist->ProjectionY("_py1",54,72)->Draw();
  hpersist->ProjectionY("_py0",4,22)->Draw("same");
  tc->cd(2);
  hpersist->Draw("col");
  tc->cd(3);
  hpulses1->Draw(); 
  hpulses0->Scale( hpulses1->GetMaximum()/hpulses0->GetMaximum() );
  hpulses0->Draw("same");
  tc->Update();

  hpersist->Write();
  hpulses0->Write();
  hpulses1->Write(); 
  //f.Close(); 
  f->Close();
}



int main(int argc, char **argv) {
  TApplication theApp("App", &argc, argv);
  rootTest();

  std::cout << "Hit any ^c to exit" << std::endl;
  theApp.Run(true);
  
  
  return 0;


}




