using namespace RooFit;

TH1* GetHistogram(TFile *f, int counter) {
  stringstream ss; ss << counter;
  TH1 *h;
  TH2I *h2 = (TH2I*)f->Get("TAGH_timewalk/Offsets/TAGH_tdcadcTimeDiffVsSlotID");
  h = h2->ProjectionY(TString(h2->GetName())+"_"+TString(ss.str()),counter,counter);
  TString title = "TAGH counter "+TString(ss.str());
  h->SetTitle(title);
  h->GetXaxis()->SetTitle(h2->GetYaxis()->GetTitle());
  return h;
}

double GetMode(TH1 *h) {
  if (h->GetEntries() == 0.0) return 999.0;
  int max_bin = h->GetMaximumBin();
  double max = h->GetBinContent(max_bin);
  if (max < 7.0) return 999.0;
  return h->GetBinCenter(max_bin);
}

void WriteFitResults(ofstream &fout, TH1 *h, int counter, TCanvas *canvas, double center=-999) {
  h->GetXaxis()->SetRangeUser(-4,4);
  h->Draw();
  canvas->Update();
  //double center;
  if(center==-999){
    cout << "What is the center?     ";
    cin >> center;
  }
  
  TF1 *gaus1 = new TF1("gaus_fit1", "gaus", center-1, center+1);
  //TF1 *gaus2 = new TF1("gaus_fit2", "gaus", center+.25, center+.75);
  h->Fit(gaus1, "LR");
  //h->Fit(gaus2, "LR");
  canvas->Print("TDCADC_fit.pdf", "pdf");
  TString sep = ",";
  fout << counter << sep << 0 << sep << gaus1->GetParameter(1) << sep << gaus1->GetParError(1) << sep << gaus1->GetParameter(2) << endl;

  delete gaus1;
  //delete gaus2;





  /*TString sep = ",";
  double max = h->GetBinContent(h->GetMaximumBin());
  double mode = GetMode(h);
  if (h->GetEntries() < 100.0 || mode == 999.0) {
    fout << counter << sep << max << sep << 0.0 << sep << 0.0 << sep << 0.0 << endl;
    return;
  }
  double xlow = h->GetBinCenter(h->FindFirstBinAbove(0.5*max)) - 1.0;
  double xhigh = h->GetBinCenter(h->FindLastBinAbove(0.5*max)) + 1.0;
  RooRealVar x("TDC time difference","TDC time difference [ns]",xlow,xhigh);
  RooDataHist data("data","data",RooArgList(x),h);
  // model: gaussian
  RooRealVar mean("mean","mean",mode,xlow,xhigh);
  RooRealVar sigma("sigma","sigma",0.2,0.01,0.6);//0.01,0.5
  RooGaussian gauss("gauss","gauss",x,mean,sigma);
  gauss.fitTo(data,PrintLevel(-1));
  // Make plot
  TCanvas *canvas = new TCanvas("c","c",800,500);
  RooPlot *plot = x.frame();
  plot->SetTitle(h->GetTitle());
  plot->SetXTitle(h->GetXaxis()->GetTitle());
  plot->SetYTitle("TAGH hits");
  plot->SetTitleSize(0.0474,"XY");
  data.plotOn(plot);
  gauss.plotOn(plot,LineColor(kRed));
  plot->Draw();
  gauss.paramOn(plot,Layout(0.58,0.9,0.9),Format("NEU",AutoPrecision(1)));
  plot->Draw();
  stringstream ss; ss << counter;
  system(TString("mkdir -p fits_TDCADC").Data());
  canvas->Print("fits_TDCADC/counter_"+TString(ss.str())+".gif");
  canvas->Print("TDCADC_fit.pdf", "pdf");
  delete canvas; delete plot;
  fout << counter << sep << max << sep << mean.getVal() << sep << mean.getError() << sep << sigma.getVal() << endl;*/
}

//The main difference here is the n_manual parameter
//In my experience the first 16 counters may need manual intervention
//This design assumes that the counters that need manually adjusted at the beginning
//This could be better generalized by taking a list of counters, but at the moment the work seems unnecessary
int tdcadc_manual(TString rootFile, int N_manual=274) {
  TFile *f = new TFile(rootFile,"read");
  system("mkdir -p fits-csv");
  ofstream fout; fout.open("fits-csv/results_TDCADC.txt");
  const int N = 274;
  TCanvas *canvas = new TCanvas("c","c",800,500);
  canvas->Print("TDCADC_fit.pdf(","pdf");
  for (int i = 1; i <= N; i++) { // counters
    TH1 *h = GetHistogram(f,i);
    if(i<=N_manual){
      WriteFitResults(fout,h,i, canvas);
    }else{
      WriteFitResults(fout,h,i, canvas,0);
    }
  }
  canvas->Print("TDCADC_fit.pdf)","pdf");
  fout.close();
  delete canvas;
  return 0;
}
