#include <TProfile.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TPaveText.h>
#include <TPaveLabel.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h> 
#include <unistd.h>
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TTree.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH2D.h"
#include "TFile.h"
#include "TPolyLine.h"
#include "TImage.h"
using namespace std;

TFile *f;
TH2I *h_offsets[6];
TH2I *h_tdc[102];
TH2I *h_tw[102];
TH2I *h_tw_ind[5][4];

void plot(int run_numb) {

  char infile[300];
  //sprintf(infile,"/work/halld2/home/boyu/src_calib_tagm/results/root_verify/Run0%d/hd_root_Run0%d_000.root",run_numb,run_numb);
  sprintf(infile,"/work/halld/data_monitoring/RunPeriod-2021-11/mon_ver07/rootfiles/hd_root_0%d.root",run_numb);
  TFile *f = new TFile (infile,"R");

  char outfile[300];
  //sprintf(outfile,"/work/halld2/home/boyu/src_calib_tagm/results/results_verify/tagm-timing-plots-%d.pdf",run_numb);
  sprintf(outfile,"/work/halld2/home/boyu/src_calib_tagm/results/mon_ver07/tagm-timing-plots-%d.pdf",run_numb);
  char outfile1[300], outfile2[300];  
  strcpy(outfile1,outfile);  strcpy(outfile2, outfile); 
  strcat(outfile1,"(");   strcat(outfile2,")");  
 
  TCanvas *c1 = new TCanvas("c1","c1", 10, 200, 600, 600);
  c1->Divide(5,5);    
  int plot = 1;

  h_offsets[0] = (TH2I*)f->Get(Form("TAGM_TW/tdc_adc_all"));
  h_offsets[1] = (TH2I*)f->Get(Form("TAGM_TW/tdc_adc_all_ind"));
  h_offsets[2] = (TH2I*)f->Get(Form("TAGM_TW/t_adc_all"));
  h_offsets[3] = (TH2I*)f->Get(Form("TAGM_TW/t_adc_all_ind"));
  h_offsets[4] = (TH2I*)f->Get(Form("TAGM_TW/adc_rf_all"));
  h_offsets[5] = (TH2I*)f->Get(Form("TAGM_TW/adc_rf_all_ind"));
  for (Int_t i = 0; i < 6; ++i){
    c1->cd(plot);
    if (i<2)  h_offsets[i]->GetXaxis()->SetRangeUser(-4,4);
    if (i>=2)  h_offsets[i]->GetXaxis()->SetRangeUser(-2,2);
    h_offsets[i]->SetMarkerSize(0.25);
    h_offsets[i]->Draw("colz");
    plot++;
  }
  for (Int_t i = 0; i < 102; ++i){
    h_tdc[i] = (TH2I*)f->Get(Form("TAGM_TW/tdc-rf/h_dt_vs_pp_tdc_%i",i+1));
    h_tw[i] = (TH2I*)f->Get(Form("TAGM_TW/t-rf/h_dt_vs_pp_%i",i+1));
    h_tw[i]->SetTitle(Form("Timewalk Col %i",i+1));
    c1->cd(plot);
    h_tw[i]->Draw("colz");
    h_tw[i]->SetMarkerSize(0.25);
    plot++;
    if (plot==26){
      c1->Print(outfile1,"pdf");
      c1->Clear();
      c1->Divide(5,5);
      plot = 1;
    }
  }
  for (Int_t i = 0; i < 5; ++i){
    for (Int_t j = 0; j < 4; ++j){
      h_tw_ind[i][j] = (TH2I*)f->Get(Form("TAGM_TW/t-rf/h_dt_vs_pp_ind_%i_%i",i+1,j+1));
      h_tw_ind[i][j]->SetTitle(Form("Timewalk ind. Row %i Col %i",i+1,j+1));
      c1->cd(plot);
      h_tw_ind[i][j]->Draw("colz");
      h_tw_ind[i][j]->SetMarkerSize(0.25);
      plot++;
      if (plot==26){
        c1->Print(outfile1,"pdf");
        c1->Clear();
        c1->Divide(5,5);
        plot = 1;
      }
    }
  }
  c1->Print(outfile2,"pdf");

  char statsfile_name[300];
  //sprintf(statsfile_name,"/work/halld2/home/boyu/src_calib_tagm/results/results_verify/tagm-timing-stats-%d.txt",run_numb);
  sprintf(statsfile_name,"/work/halld2/home/boyu/src_calib_tagm/results/mon_ver07/tagm-timing-stats-%d.txt",run_numb);
  FILE *statsfile = fopen(statsfile_name,"w");
  
  double adc_rf_mean[102], adc_rf_error[102], tdc_adc_mean[102], tdc_adc_error[102], t_adc_mean[102], t_adc_error[102], tdc_rf_mean[102], tdc_rf_error[102], t_rf_mean[102], t_rf_error[102];
  memset(adc_rf_mean,0,102); memset(adc_rf_error,0,102); memset(tdc_adc_mean,0,102); memset(tdc_adc_error,0,102);
  memset(t_adc_mean,0,102); memset(t_adc_error,0,102); memset(tdc_rf_mean,0,102); memset(tdc_rf_error,0,102);
  memset(t_rf_mean,0,102); memset(t_rf_error,0,102);

  for (Int_t i = 0; i < 102; ++i){
    TF1 *f1 = new TF1("f1","gaus",-10.,10.);
    f1->SetParameter(1,0);
    f1->SetParameter(2,0.5);
    
    h_offsets[4]->ProjectionX("channel",i+1,i+1)->Fit("f1","Q","",-2,2);
    adc_rf_mean[i] = f1->GetParameter(1);
    adc_rf_error[i] = f1->GetParameter(2);

    h_offsets[0]->ProjectionX("channel",i+1,i+1)->Fit("f1","Q","",-2,2);
    tdc_adc_mean[i] = f1->GetParameter(1);
    tdc_adc_error[i] = f1->GetParameter(2);
  
    h_offsets[2]->ProjectionX("channel",i+1,i+1)->Fit("f1","Q","",-2,2);
    t_adc_mean[i] = f1->GetParameter(1);
    t_adc_error[i] = f1->GetParameter(2);

    h_tdc[i]->GetYaxis()->SetRangeUser(-2,2);
    h_tdc[i]->ProjectionY()->Fit("f1","Q","",-2,2);
    tdc_rf_mean[i] = f1->GetParameter(1);
    tdc_rf_error[i] = f1->GetParameter(2);

    h_tw[i]->GetYaxis()->SetRangeUser(-2,2);
    h_tw[i]->ProjectionY()->Fit("f1","Q","",-2,2);
    t_rf_mean[i] = f1->GetParameter(1);
    t_rf_error[i] = f1->GetParameter(2);

    fprintf(statsfile, "%4d %14.3f %14.3f %14.3f %14.3f %14.3f %14.3f %14.3f %14.3f %14.3f %14.3f \n",i+1, adc_rf_mean[i], adc_rf_error[i], tdc_adc_mean[i], tdc_adc_error[i], t_adc_mean[i], t_adc_error[i], tdc_rf_mean[i], tdc_rf_error[i], t_rf_mean[i], t_rf_error[i]); 
  }
  fclose(statsfile);


  char dtfile[300];
  //sprintf(dtfile,"/work/halld2/home/boyu/src_calib_tagm/results/results_verify/tagm-timing-dt-%d.pdf",run_numb);
  sprintf(dtfile,"/work/halld2/home/boyu/src_calib_tagm/results/mon_ver07/tagm-timing-dt-%d.pdf",run_numb);

  TCanvas *c2 = new TCanvas("c2","c2", 10, 200, 600, 600);
  c2->Divide(2,3);    

  TGraph *g_dt[5]={NULL};
  double channels[102], zeros[102];
  memset(channels,0,102); memset(zeros,0,102);
  for (Int_t i = 0; i < 102; ++i){
    channels[i] = i+1;
  }
  g_dt[0] = new TGraphErrors(102, channels, adc_rf_mean, zeros, adc_rf_error); 
  g_dt[0]->SetTitle("ADC-RF"); 
  g_dt[1] = new TGraphErrors(102, channels, t_rf_mean, zeros, t_rf_error);
  g_dt[1]->SetTitle("T-RF");  
  g_dt[2] = new TGraphErrors(102, channels, tdc_rf_mean, zeros, tdc_rf_error);
  g_dt[2]->SetTitle("TDC-RF");  
  g_dt[3] = new TGraphErrors(102, channels, tdc_adc_mean, zeros, tdc_adc_error);
  g_dt[3]->SetTitle("TDC-ADC");  
  g_dt[4] = new TGraphErrors(102, channels, t_adc_mean, zeros, t_adc_error);
  g_dt[4]->SetTitle("T-ADC");  

  for (Int_t i = 0; i < 5; ++i){
    c2->cd(i+1);
    g_dt[i]->GetYaxis()->SetRangeUser(-2,2);
    g_dt[i]->SetMarkerSize(0.25);
    g_dt[i]->GetXaxis()->SetTitle("channel");
    g_dt[i]->GetYaxis()->SetTitle("dt (ns)"); 
    g_dt[i]->Draw("AP");
    plot++;
  }
  c2->Print(dtfile,"pdf"); 
}      
