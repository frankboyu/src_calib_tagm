#include <iostream>
#include <fstream>
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

void tw_tagm(int run_numb, int ch_min = 1, int ch_max = 102){
  int save_plot = 1;
  char file_name[300];
  sprintf(file_name,"/work/halld2/home/boyu/src_calib_tagm/results/root_calib/Run0%d/hd_root_Run0%d_000.root",run_numb,run_numb);
  //sprintf(file_name,"/work/halld/data_monitoring/RunPeriod-2021-11/mon_ver03/rootfiles/hd_root_0%d.root",run_numb);
  TFile *file = new TFile (file_name,"R");

  char out_file[300];
  char file_tmp[300];
  sprintf(out_file,"/work/halld2/home/boyu/src_calib_tagm/results/results_calib/tw-corr-%d.txt",run_numb);
  sprintf(file_tmp,"/work/halld2/home/boyu/src_calib_tagm/results/results_calib/tw-fit-%d.pdf",run_numb);

  gStyle->SetOptFit(10);

  double cc0[103], cc1[103], cc2[103], cc3[103], ecc0[103], ecc1[103], ecc2[103], ecc3[103], cnt[103], ecnt[103];
  double x[125], y[125], ex[125], ey[125], res[125], eres[125];

  memset(cc0,0,sizeof(cc0)); memset(cc1,0,sizeof(cc1)); memset(cc2,0,sizeof(cc2)); memset(cc3,0,sizeof(cc3));
  memset(ecc0,0,sizeof(ecc0)); memset(ecc1,0,sizeof(ecc1)); memset(ecc2,0,sizeof(ecc2)); memset(ecc3,0,sizeof(ecc3));
  memset(cnt,0,sizeof(cnt)); memset(ecnt,0,sizeof(ecnt));

  double new_offsets[103], tmp_offset;
  int tmp_row, tmp_column;
  ifstream foffsets("new_offsets.txt");
  while(foffsets >> tmp_row >> tmp_column >> tmp_offset){
    if (tmp_row == 0){
      new_offsets[tmp_column] = tmp_offset;
    }
  }


  TGraph *gr1[103]={NULL};
  TH2I *hdt_ch[110];  
  
  Int_t ii = 0;
  
  for(int ch = ch_min; ch <= ch_max; ch++){
    if(ch%10==0){
      cout << " COUNTER = " << ch << endl; 
    }
    char title[30];  
    sprintf(title,"TAGM_TW/tdc-rf/h_dt_vs_pp_tdc_%d",ch);  //  h_dt_vs_pp_tdc is a histogram with no tw corrections applied

    cnt[ch] = double(ch);

    int npoint = 0;

    memset(x,0,sizeof(x));
    memset(y,0,sizeof(y));
    memset(ex,0,sizeof(ex));
    memset(ey,0,sizeof(ey));

    cc0[ch] = -100.;
    
    hdt_ch[ch] = (TH2I*)file->Get(title);

    int max_bin_scan = 1900;
    int int_win = 40;
    double tmin = 20;
    double tmax = 60;
    double adc_min = -1;
    double adc_max = -1;

    for(int slice = 0; slice < 125; slice++){

      TH1D *h1 = hdt_ch[ch]->ProjectionY("test",slice,slice);   
      int ent = h1->GetEntries();    
      if(ent < 400) continue;  // Not enough entries in the histogram to fit
      
      int sum_max = 0;    
      double x_min = -100;
      double x_max = -100;     
      int bin_min  = -100;
      
      //  Run sliding window to determine the peak to fit. Multiple peaks are possible if the bunch structure 
      //  is not resolved (ADC time is not well aligned). The RF is selected based on the ADC time. PS time can 
      //  be used instead. 

      for(int bin = 0; bin < max_bin_scan; bin++){
	int sum = 0;     
	for(int ii = 0; ii < int_win; ii++){
	  int tmp = bin + ii;   // bin size 0.05 ns
	  sum += h1->GetBinContent(tmp);	
	}     
	if(sum > sum_max){
	  sum_max = sum; 
	  bin_min = bin;
	}	
      }    
      x_min = h1->GetXaxis()->GetBinCenter(bin_min-tmin);
      x_max = h1->GetXaxis()->GetBinCenter(bin_min+tmax);
            
      TF1 *f1 = new TF1("f1","gaus",-10.,10.);

      f1->SetParameter(0,sum_max);
      f1->SetParameter(1,0);
      f1->SetParameter(2,0.25);
      
      h1->Fit("f1","Q","",x_min,x_max);
      
      if((f1->GetParameter(1)-new_offsets[ch]) < 2. && (f1->GetParameter(1)-new_offsets[ch]) > -1.){  // Restrict fitting range, the maximum allowed time shift is between [-1, 2] ns

	//if(adc_min < 0) adc_min =  hdt_ch[ch]->GetXaxis()->GetBinCenter(slice);
	adc_max = hdt_ch[ch]->GetXaxis()->GetBinCenter(slice);

	x[npoint]  = hdt_ch[ch]->GetXaxis()->GetBinCenter(slice);
	y[npoint]  = f1->GetParameter(1) - new_offsets[ch];
	ey[npoint] = f1->GetParError(1);
	res[npoint]  = f1->GetParameter(2);
	eres[npoint] = f1->GetParError(2);
	npoint++;
      }
      
    }  // Loop over slices

    if(npoint == 0) continue;

    gr1[ch]  = new TGraphErrors(npoint,x,y,ex,ey);
    char gr_title[30];
    sprintf(gr_title,"TAGM channel %d",ch);
    gr1[ch]->SetMarkerStyle(20);
    gr1[ch]->SetMarkerSize(0.6);
    gr1[ch]->SetMarkerColor(4);
    gr1[ch]->SetMaximum(6);
    gr1[ch]->SetMinimum(-3);
    gr1[ch]->SetTitle(gr_title);
    gr1[ch]->GetXaxis()->SetTitle("ADC peak amplitude (count)");
    gr1[ch]->GetYaxis()->SetTitle("T_{TAGM} - T_{RF}"); 

    TF1 *func5 = new TF1("func5","[0]+[1]*(1/(x+[3]) )**[2]",100,2000);

    func5->SetParameter(0,-1);  //-2.5
    func5->SetParameter(1,100);  //100
    func5->SetParameter(2,0.7);  //0.6
    func5->SetParameter(3,-90);  //-400

    // Fit range has to be checked and adjusted for some channels, where ADC time is not well calibrated
    // In most cases it's not needed

    for(int nn = npoint; nn >= 0; nn--){
      if(adc_min < 0){
	adc_min = x[nn];
	continue;
      }
      if((x[nn+1]-x[nn])>30 && (npoint-nn)>3) break;
      adc_min = x[nn];    
    }

    gr1[ch]->Fit("func5","EQ","",adc_min,adc_max);

    //func5->FixParameter(2,func5->GetParameter(2));
    //gr1[ch]->Fit("func5","EQ","",adc_min+60,adc_max-20);
    //gr1[ch]->Fit("func5","EQ","",adc_min,adc_max);

    cc0[ch] = func5->GetParameter(0);
    cc1[ch] = func5->GetParameter(1);  
    cc2[ch] = func5->GetParameter(2);
    cc3[ch] = func5->GetParameter(3);
    ecc0[ch]  = func5->GetParameter(0);
    ecc1[ch]  = func5->GetParameter(1);
    ecc2[ch]  = func5->GetParameter(2);
    ecc3[ch]  = func5->GetParameter(3);

  }   // Loop over TAGM counters
  

  // Print suspicious TAGM channels with the large value of C0
  for(int kk = 0; kk < 102; kk++){   
    if(fabs(cc0[kk]) > 10)      
      cout << " COUNTER = " << kk <<  "  CC0 = " <<  cc0[kk]  << endl;        
  }
  
  // Write results
  
  FILE *data_file = fopen(out_file,"w");
  for(int ch = 1; ch <= 102; ch++){
    fprintf(data_file, "%4d  %4d  %14.7f  %14.7f  %14.7f %14.7f %14.7f\n",0, ch, cc0[ch], cc1[ch], cc2[ch], cc3[ch], 0. ); 
    if((ch == 9) || (ch == 27) || (ch == 81) || (ch == 99)){
      for(int single = 1; single < 6; single++){
	fprintf(data_file, "%4d  %4d  %14.7f  %14.7f  %14.7f %14.7f %14.7f\n",single,ch, cc0[ch], cc1[ch], cc2[ch], cc3[ch], 0. );
      }      
    }   
  } 
  fclose(data_file); 
  
  if(save_plot){
    TCanvas *c10 = new TCanvas("c10","c10", 10, 200, 600, 600);
    c10->Divide(5,5);    
    int plot = 1;
    
    char tmp[300], tmp1[300];  
    strcpy(tmp,file_tmp);  
    strcpy(tmp1,file_tmp); 
    strcat(tmp,"(");   strcat(tmp1,")");
    
    for(int ch = 1; ch <= 102; ch++){    
      c10->cd(plot);     
      if(gr1[ch] != NULL){
	gr1[ch]->SetMarkerSize(0.25);
	gr1[ch]->Draw("AP");
      }     
      if((ch == 25)||(ch == 50)||(ch == 75)||(ch == 100)){
	c10->Print(tmp,"pdf");
	c10->Clear(); c10->Divide(5,5);
	plot = 0;
      }     
      plot++;
    }
    c10->Print(tmp1,"pdf");        
  }
}
