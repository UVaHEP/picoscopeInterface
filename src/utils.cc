#include "utils.h"
#include "TMath.h"
#include <iostream>

using std::cout;
using std::endl;

DarkPeaker::DarkPeaker(){
  tspectrum=new TSpectrum(5000,2);
  buf=0;
  hbkg=0;
  bkgCorrectedY=0;
  hdist=0;
}
void DarkPeaker::SetBuffer(TH1F *newbuf, double sampleTime){
  buf=newbuf;
  dT=sampleTime;
  if (hbkg) delete hbkg;
  //if (bkgCorrectedY) delete bkgCorrectedY;
  if (hdist) delete hdist;
  npeaks=0;
}
int DarkPeaker::GetNPeaks(){return npeaks;}
TH1F* DarkPeaker::GetBackground(){return hbkg;}
Float_t* DarkPeaker::GetPositionX(){ return tspectrum->GetPositionX();}
Float_t* DarkPeaker::GetPositionY(){ return tspectrum->GetPositionY();}
Float_t* DarkPeaker::GetBkgdCorrectedY(){
  if (bkgCorrectedY) return bkgCorrectedY;
  bkgCorrectedY = new Float_t[npeaks];
  Float_t *xpeaks = tspectrum->GetPositionX();
  Float_t *ypeaks = tspectrum->GetPositionY();
  for (int i=0; i<npeaks; i++)
    bkgCorrectedY[i]=ypeaks[i]-hbkg->GetBinContent((int)xpeaks[i]+1);
  return bkgCorrectedY;
}
Float_t* DarkPeaker::GetDeltaX(){
  Int_t *index=new Int_t[npeaks];
  Float_t *xpeaks = tspectrum->GetPositionX();
  TMath::Sort(npeaks, xpeaks, index, kFALSE);  // index sort by timestamp
  deltaX=new Float_t[npeaks-1];
  for (int i=0;i<npeaks-1;i++) {
    int idx1=index[i];
    int idx2=index[i+1];
    deltaX[i]=xpeaks[idx2]-xpeaks[idx1];
  }
  delete index;
  return deltaX;
}


int DarkPeaker::AnalyzePeaks(){
  if (!buf) {
    cout << "DarkPeaker::AnalyzePeaks : No buffer set!" << endl;
    return 1;
  }
  // estimate background
  hbkg=(TH1F*)buf->Clone("background");
  FindBackground();

  // estimate noise and single peak height
  FindNoise();

  // find peaks
  double threshold = (snglPeak/2) / buf->GetMaximum();
  npeaks=tspectrum->Search(buf,2,"nobackground,nomarkov,nodraw",threshold);
  //int npeaks=tspectrum->Search(buf,2,"nomarkov",threshold);
  cout << "Found " << npeaks << " peaks" << endl;
  cout << "Dark pulse rate: " << CalcDarkRate() << " MHz" << endl;
  return 0;
}

void DarkPeaker::FindBackground(){
  int nbins=buf->GetNbinsX();
  float *bksource=new float[nbins];
  for (int i = 0; i < nbins; i++) bksource[i]=buf->GetBinContent(i + 1);
  /*
  TH1 *hfft;
  hfft=buf->FFT(0,"RE");
  double sum=0, sumw=0;
  for (int i = 1; i < nbins/2; i++){
    double mag=hfft->GetBinContent(i);
    sum += hfft->GetBinCenter(i) * mag;
    sumw+= mag;
    //    cout << i << " " << hfft->GetBinContent(i) << endl;
    // cout << i << " " << sum << " " << sumw << endl;
  }
  cout << "FFT estimate of baseline range: " << sum/sumw << endl;
  cout << "FFT estimate of baseline range: " << hfft->GetMaximumBin() << endl;
  delete hfft;
  */
  tspectrum->Background(bksource,nbins,180,TSpectrum::kBackDecreasingWindow,
   			TSpectrum::kBackOrder4,kTRUE,
  			TSpectrum::kBackSmoothing3,kFALSE);
  // replace 180 w/ max fourrier component or 1/2 of it, etc
  for (int i = 0; i < nbins; i++) hbkg->SetBinContent(i + 1,bksource[i]);
  delete bksource;
}


// find noise from distribution pulse height spectrum
void DarkPeaker::FindNoise(){
  hdist=new TH1F("hdist","ADC distribution",
		       100,-0.1*buf->GetMaximum(),buf->GetMaximum());
  for (int i = 1; i <= buf->GetNbinsX(); i++)
    hdist->Fill( buf->GetBinContent(i) - hbkg->GetBinContent(i)  );

  int noiseX=hdist->GetMaximumBin();
  double noiseY=hdist->GetBinContent(noiseX);
  double r1=0,r2=0;  // range for fitting noise peak
  for (int i=1; i<hdist->GetNbinsX(); i++){
    if ( i<noiseX && hdist->GetBinContent(i) < noiseY*0.5 )
      r1=hdist->GetBinLowEdge(i);
    else if ( i>noiseX && hdist->GetBinContent(i) > noiseY*0.5 ){
      r2=hdist->GetBinLowEdge(i+2);
    }
  }
  hdist->Fit("gaus","0q","",r1,r2);
  tfNoise=hdist->GetFunction("gaus");
  tfNoise->SetRange(hdist->GetXaxis()->GetXmin(),hdist->GetXaxis()->GetXmax());
  hdist->Add(tfNoise,-1);
  snglPeak=hdist->GetMean();
  hdist->Add(tfNoise);
}
// dark count rate in MHz
double DarkPeaker::CalcDarkRate(){
  if (buf) return npeaks / (dT*1e6*buf->GetNbinsX());
  return 0;
}

