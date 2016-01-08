#include "picoscopeInterface.h"
#include "ps5000a.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <iostream>


using namespace picoscope;


void PHD(TH1F* h, TH1F* p, int firstbin, int lastbin){
  p->Fill(h->Integral(firstbin,lastbin));
}


void setupPicoscope(ps5000a &dev, chRange range, int samples, int nbuffers) {

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
  dev.setCaptureCount(nbuffers);
  dev.prepareBuffers();  
}


void rootTest(){
  ps5000a dev;
  //  dev.setCaptureCount(20);
  const int samples = 100;  // number of ADC samples per waveform
  const int NBUFFERS=10000; // number of waveforms per capture
  const int NREPEAT=5;     // number of capture cycles
  const int NSAVE=20;
  chRange range = PS_50MV;
  setupPicoscope(dev, range, samples, NBUFFERS); 


  TH2F *hpersist=new TH2F("hpersist","Persistance Display",samples,
			0,samples,100,0,15000);

  TH1F *hsum=new TH1F("hsum","Sum of wave data",samples,0,samples);
  TH1F* hpulses1=new TH1F("hpulses1","Pulse area distribution",200,-20000,200000);
  TH1F* hpulses0=new TH1F("hpulses0","Pulse area distribution",200,-20000,200000);

  TH1F* waveForms[NSAVE];
  int wavSaved=0;
  
  for (int i = 0; i < NREPEAT; i++) {
    std::cout << "Capturing Block:" << i << std::endl;     
    dev.captureBlock();

    vector <vector<short> > &data = dev.getWaveforms();
    
    for (auto &waveform : data) {
      TH1F *hsamp = new TH1F("hsamp","Samples", waveform.size(), 0, waveform.size());
      for (int i = 0; i < waveform.size(); i++) {
	hpersist->Fill(i, -1*waveform[i]);
	hsamp->SetBinContent(i, -1*waveform[i]);
      }
      hsum->Add(hsamp);
      PHD(hsamp,hpulses1,50,65);
      PHD(hsamp,hpulses0,0,15);
      if (wavSaved<NSAVE) {
	waveForms[wavSaved]=
	  (TH1F*)hsamp->Clone(TString::Format("wave%02d",wavSaved));
	wavSaved++;
      }
      delete hsamp; 
    }
  }

  dev.close(); 
  TFile *f = new TFile("test.root", "RECREATE");
  
  TCanvas *tc=new TCanvas("tc","Pulse heights",1800,600);
  tc->Divide(3,1);
  gStyle->SetOptStat(0);
  
  // projections of persistance histogram to show counts vs threshold
  tc->cd(1)->SetLogy();
  hpersist->ProjectionY("_py1",50,65)->DrawCopy();
  hpersist->ProjectionY("_py0",0,15)->DrawCopy("same");

  // plot the persistance histogram
  tc->cd(2);
  hpersist->DrawCopy("col");
  hsum->Scale( 0.9 * hpersist->GetYaxis()->GetXmax() / hsum->GetMaximum() );
  hsum->SetLineColor(kGray);
  hsum->SetLineWidth(3);
  hsum->DrawCopy("same");
  // pulse height distributions
  tc->cd(3);
  hpulses1->DrawCopy(); 
  hpulses0->Scale( hpulses1->GetMaximum()/hpulses0->GetMaximum() );
  hpulses0->DrawCopy("same");
  tc->Update();

  hpersist->Write();
  hpulses0->Write();
  hpulses1->Write(); 

  f->Close();
}



int main(int argc, char **argv) {
  TApplication theApp("App", &argc, argv);
  rootTest();

  std::cout << "Hit any ^c to exit" << std::endl;
  theApp.Run(true);
  
  return 0;


}

