#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TLine.h>
#include <TLatex.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TMultiGraph.h>

void StyleGraph(TGraph*, int, int, double, bool); // Marker style, Marker Color, Marker size, TimeFormat

void Draw_FCup_SLM(){

  const double SLM_Offset = 0;
  const double SLM_Slope = 1;
  const double FCup_Slope = 906.2;
  const double FCup_KPP_Offset = 100.;
  const double SLM_Slope_DT = 1;
  const double SLM_Offset_DT = 0.;
  const double t_start_DataTaking = 1516186810; // Beginning of Data Taking
  const double t_End_DataTaking = 1516189517;   // End of Data Taking

  TLine *line1 = new TLine();
  TLatex *lat1 = new TLatex();
  lat1->SetTextFont(42);
  lat1->SetNDC();
  
  TF1 *f_lin1 = new TF1("f_lin1", "[0] + [1]*x", 0., 100.);
  TF1 *f_Gaus = new TF1("f_Gaus", "[0]*TMath::Gaus(x, [1], [2])", 0., 2.);
  f_Gaus->SetNpx(2500);
    
  TGraph *gr_FCup_curr_Raw = new TGraph(); // FCup current assuming Offset = 100 , as was During the KPP
  TGraph *gr_FCup_curr = new TGraph();
  TGraph *gr_SLM_curr = new TGraph();
  TGraph *gr_SLM_FCup_Ratio = new TGraph();
  TGraph *gr_SLM_calibr_global = new TGraph();
  TGraph *gr_SLM_calibr_DataTaking = new TGraph();
  TGraph *gr_SLM_FCup_Ratio_DT = new TGraph();      // this represents SLM/FCup ration, when SLM slope and offset were determined usin only data taken at data taking
  TGraph *gr_SLM_FCup_Ratio_DT_RAW = new TGraph();  // this represents SLM/FCup ration, when SLM slope and offset were determined usin only data taken at data taking, FCup current
                                                    // is not corrected for the offset, but is calculaed using the KPP fixed offset

  TH1D *h_SLM_FCup_Ratio_DT = new TH1D("h_SLM_FCup_Ratio_DT", "", 200, 0.5, 1.5);
  TH1D *h_SLM_FCup_Ratio_DT_BB_in = new TH1D("h_SLM_FCup_Ratio_DT_BB_in", "", 200, 9., 11.);
  TH1D *h_SLM_FCup_Ratio_DT_RAW = new TH1D("h_SLM_FCup_Ratio_DT_RAW", "", 200, 0.5, 1.5);
  
  Double_t t_S2b, count_S2b;
  Double_t t_S16b, count_S16b;

  TCanvas *c1 = new TCanvas("c1", "", 1200, 500);
  
  TTree *tr_S2b = new TTree("tr_S2b", "tr_S2b");
  TTree *tr_S16b = new TTree("tr_S16b", "tr_S16b");

  tr_S2b->Branch("t_S2b", &t_S2b, "t_S2b/D");
  tr_S2b->Branch("count_S2b", &count_S2b, "count_S2b/D");
  
  //tr_S2b->ReadFile("Scaler_S2b_Data_15_Dec_2017_BB_Out.dat", "t_S2b:count_S2b");
  tr_S2b->ReadFile("S2b_Jan_17_18_1.dat", "t_S2b:count_S2b");

  int n_S2b = tr_S2b->GetEntries();
  Double_t *t_S2b_arr = new Double_t[n_S2b];
  Double_t *count_S2b_arr = new Double_t[n_S2b];

  for( int i = 0; i < n_S2b; i++ ){
    tr_S2b->GetEntry(i);
    t_S2b_arr[i] = t_S2b;
    count_S2b_arr[i] = count_S2b;
    gr_FCup_curr_Raw->SetPoint(i, t_S2b, (count_S2b - FCup_KPP_Offset)/FCup_Slope);
  }
  
  tr_S16b->Branch("t_S16b", &t_S16b, "t_S16b/D");
  tr_S16b->Branch("count_S16b", &count_S16b, "count_S16b/D");
  
  tr_S16b->ReadFile("S16b_Jan_17_18_1.dat", "t_S16b:count_S16b");

  tr_S16b->Draw("count_S16b:t_S16b>>hh_counts_time(2000, 1513234600, 1513258999, 600, 8000., 20000.)", "", "colz", 10000);

  double FCup_curr;
  double FCup_curr_raw;
  double FCup_offset = 271; // The offset at the beginning

  int S2b_index = 0;
  int ind_Datataking = 0;
  int n_S16b = tr_S16b->GetEntries();

  for( int i = 0; i < n_S16b; i++ ){
    tr_S16b->GetEntry(i);

    double SLM_current = (count_S16b - SLM_Offset)/SLM_Slope;
    double SLM_current_DT = (count_S16b - SLM_Offset_DT)/SLM_Slope_DT;
    //cout<<"SLM_current = "<<SLM_current<<endl;

    while( t_S2b_arr[S2b_index] <= t_S16b ){

      if( S2b_index > 5 && S2b_index < n_S2b - 6 ){
	if( count_S2b_arr[S2b_index - 5] < 800 && count_S2b_arr[S2b_index - 4] < 800 && count_S2b_arr[S2b_index - 3] < 800
	    && count_S2b_arr[S2b_index - 2] < 800 && count_S2b_arr[S2b_index - 1] < 800 && count_S2b_arr[S2b_index] > 800
	    && count_S2b_arr[S2b_index + 1] > 800 && count_S2b_arr[S2b_index + 2] > 800 && count_S2b_arr[S2b_index + 3] > 800
	    && count_S2b_arr[S2b_index + 4] > 800 ){
	  //Offset is defined as the average of recent 5 valued of scalerS2b before beam came back
	   FCup_offset = (count_S2b_arr[S2b_index - 5] + count_S2b_arr[S2b_index - 4] + count_S2b_arr[S2b_index - 3] +
	  		 count_S2b_arr[S2b_index - 2] + count_S2b_arr[S2b_index - 1])/5.;
	   //FCup_offset = 100;
	  
	}
      }
      
      FCup_curr = (count_S2b_arr[S2b_index] - FCup_offset)/FCup_Slope;
      FCup_curr_raw =  (count_S2b_arr[S2b_index] - FCup_KPP_Offset)/FCup_Slope;
      S2b_index = S2b_index + 1;
    }
    
    gr_SLM_curr->SetPoint(i, t_S16b, SLM_current);
    gr_FCup_curr->SetPoint(i, t_S16b, FCup_curr);
    double ratio = (FCup_curr > 1.2)? SLM_current/FCup_curr: 0.;
    double ratio_DT = (FCup_curr > 1.2)? SLM_current_DT/FCup_curr: 0.;
    double ratio_RAW_DT = (FCup_curr_raw > 1.2)? SLM_current_DT/FCup_curr_raw: 0.;
    gr_SLM_FCup_Ratio->SetPoint(i, t_S16b, ratio);
    gr_SLM_calibr_global->SetPoint(i, FCup_curr, count_S16b);
    gr_SLM_FCup_Ratio_DT->SetPoint(i, t_S16b, ratio_DT);
    gr_SLM_FCup_Ratio_DT_RAW->SetPoint(i, t_S16b, ratio_RAW_DT);
    
    
    if( t_S16b > t_start_DataTaking && t_S16b < t_End_DataTaking){
      gr_SLM_calibr_DataTaking->SetPoint(ind_Datataking, FCup_curr, count_S16b);
      h_SLM_FCup_Ratio_DT->Fill(ratio_DT);
      h_SLM_FCup_Ratio_DT_RAW->Fill(ratio_RAW_DT);
      h_SLM_FCup_Ratio_DT_BB_in->Fill(ratio_DT);
      ind_Datataking = ind_Datataking + 1;
    }
  }

  gr_SLM_curr->SetMarkerColor(2);
  gr_SLM_curr->SetLineColor(2);
  gr_SLM_curr->SetMarkerStyle(21);
  gr_SLM_curr->SetMarkerSize(0.2);
  
  gr_FCup_curr->SetMarkerColor(4);
  gr_FCup_curr->SetLineColor(4);
  gr_FCup_curr->SetMarkerStyle(20);
  gr_FCup_curr->SetMarkerSize(0.2);


  gr_FCup_curr_Raw->SetMarkerColor(3);
  gr_FCup_curr_Raw->SetLineColor(3);
  gr_FCup_curr_Raw->SetMarkerStyle(20);
  gr_FCup_curr_Raw->SetMarkerSize(0.2);

  
  TMultiGraph *mtgr1 = new TMultiGraph();
  mtgr1->Add(gr_SLM_curr, "l");
  mtgr1->Add(gr_FCup_curr, "l");
  mtgr1->Add(gr_FCup_curr_Raw, "l");

  c1->Clear();
  mtgr1->Draw("AP");
  mtgr1->GetXaxis()->SetTimeDisplay(1);
  mtgr1->GetXaxis()->SetTimeOffset(0, "local");
  mtgr1->GetXaxis()->SetTimeFormat("%m/%d/%y %H:%M");

  TCanvas *c2 = new TCanvas("c2", "", 1800, 750);
  c2->SetTopMargin(0.02);
  c2->SetRightMargin(0.05);
  c2->SetLeftMargin(0.05);
  c2->Clear();
  gr_SLM_FCup_Ratio->SetMarkerStyle(22);
  gr_SLM_FCup_Ratio->SetMarkerColor(2);
  gr_SLM_FCup_Ratio->SetMarkerSize(0.2);
  gr_SLM_FCup_Ratio->GetXaxis()->SetTimeDisplay(1);
  gr_SLM_FCup_Ratio->GetXaxis()->SetTimeOffset(0, "local");
  gr_SLM_FCup_Ratio->GetXaxis()->SetTimeFormat("%m/%d/%y %H:%M");
  gr_SLM_FCup_Ratio->GetXaxis()->SetNdivisions(-504);
  gr_SLM_FCup_Ratio->Draw("Apl");

  StyleGraph(gr_SLM_FCup_Ratio_DT, 21, 2, 0.2, true);
  gr_SLM_FCup_Ratio_DT->Draw("AP");

  StyleGraph(gr_SLM_FCup_Ratio_DT_RAW, 20, 4, 0.2, true);
  TMultiGraph *mtgr2 = new TMultiGraph();
  mtgr2->Add(gr_SLM_FCup_Ratio_DT);
  c2->Clear("D");
  c2->Modified();
  c2->Update();
  c2->SetGridy();
  mtgr2->Draw("AP");
  mtgr2->SetTitle("; Date; I_{SLM}/I_{FCup}");
  mtgr2->SetMinimum(0.5);
  mtgr2->GetXaxis()->SetTimeDisplay(1);
  mtgr2->GetXaxis()->SetTimeOffset(0, "local");
  mtgr2->GetXaxis()->SetTimeFormat("%m/%d/%y,%H:%M");
  mtgr2->GetXaxis()->SetNdivisions(-508);
  mtgr2->GetYaxis()->SetTitleOffset(0.6);
  line1->SetLineColor(1);
  line1->DrawLine(t_start_DataTaking, 0.5, t_start_DataTaking, 1.2);
  line1->DrawLine(t_End_DataTaking, 0.5, t_End_DataTaking, 1.2);
  line1->SetLineStyle(9);
  line1->SetLineWidth(2);
  line1->SetLineColor(65);
  line1->DrawLine(t_start_DataTaking - 18000, 1., t_End_DataTaking + 8000., 1.);
  c2->Print("SLM_FCup_Ratio_time_dep.eps");
  c2->Print("SLM_FCup_Ratio_time_dep.pdf");
  c2->Print("SLM_FCup_Ratio_time_dep.png");
  c2->Print("SLM_FCup_Ratio_time_dep.root");

  c2->Clear();
  mtgr2->Add(gr_SLM_FCup_Ratio_DT_RAW);
  mtgr2->Draw("AP");
  // line1->SetLineColor(1);
  // line1->SetLineStyle(1);
  // line1->DrawLine(t_start_DataTaking, 0.5, t_start_DataTaking, 1.2);
  // line1->DrawLine(t_End_DataTaking, 0.5, t_End_DataTaking, 1.2);
  // line1->SetLineStyle(9);
  // line1->SetLineWidth(2);
  // line1->SetLineColor(65);
  // line1->DrawLine(t_start_DataTaking - 18000, 1., t_End_DataTaking + 8000., 1.);

  c2->Print("SLM_FCup_Ratio_compare.eps");
  c2->Print("SLM_FCup_Ratio_compare.pdf");
  c2->Print("SLM_FCup_Ratio_compare.png");
  c2->Print("SLM_FCup_Ratio_compare.root");

  
  TCanvas *c3 = new TCanvas("c3", "", 900, 900);

  gr_SLM_calibr_global->SetTitle("; I_{FCup} [nA]; SLM Rate [Hz]");
  gr_SLM_calibr_global->GetXaxis()->SetLimits(0.,30);
  gr_SLM_calibr_global->GetYaxis()->SetTitleOffset(1.4);
  gr_SLM_calibr_global->SetMarkerStyle(22);
  gr_SLM_calibr_global->SetMarkerColor(2);
  gr_SLM_calibr_global->SetMarkerSize(0.4);
  gr_SLM_calibr_global->Draw("AP");
  c3->Print("SLM_calibr_global.eps");
  c3->Print("SLM_calibr_global.pdf");
  c3->Print("SLM_calibr_global.png");
  

  c3->Clear();
  c3->SetTopMargin(0.04);
  c3->SetRightMargin(0.02);
  c3->SetLeftMargin(0.12);
  f_lin1->SetLineColor(4);
  f_lin1->SetLineWidth(1);
  gr_SLM_calibr_DataTaking->SetTitle("; I_{FCup} [nA]; SLM Rate [Hz]");
  gr_SLM_calibr_DataTaking->GetYaxis()->SetTitleOffset(1.8);
  gr_SLM_calibr_DataTaking->SetMarkerStyle(22);
  gr_SLM_calibr_DataTaking->SetMarkerColor(2);
  gr_SLM_calibr_DataTaking->SetMarkerSize(0.2);
  //gr_SLM_calibr_DataTaking->SetMaximum(500000.);
  gr_SLM_calibr_DataTaking->SetMinimum(0.);
  gr_SLM_calibr_DataTaking->GetXaxis()->SetLimits(0., 30.);
  gr_SLM_calibr_DataTaking->Draw("AP");
  gr_SLM_calibr_DataTaking->Fit(f_lin1, "MeV", "", 2., 45.);
  lat1->DrawLatex(0.15, 0.8, Form("Offset = %1.3f", f_lin1->GetParameter(0)));
  lat1->DrawLatex(0.15, 0.7, Form("Slope = %1.3f", f_lin1->GetParameter(1)));
  c3->Print("SLM_calibr_DataTaking.eps");
  c3->Print("SLM_calibr_DataTaking.pdf");
  c3->Print("SLM_calibr_DataTaking.eps");
  c3->Print("SLM_calibr_DataTaking.png");
  c3->Print("SLM_calibr_DataTaking.root");

  c3->Clear();
  h_SLM_FCup_Ratio_DT_RAW->Draw();
  h_SLM_FCup_Ratio_DT_RAW->SetTitle("; I_{SLM}/I_{FCup}");
  f_Gaus->SetParameters(h_SLM_FCup_Ratio_DT_RAW->GetMaximum(), h_SLM_FCup_Ratio_DT_RAW->GetMean(), h_SLM_FCup_Ratio_DT_RAW->GetRMS());
  h_SLM_FCup_Ratio_DT_RAW->Fit(f_Gaus, "Mev", "", 0.6, 1.2);
  c3->Print("SLM_FCup_Ratio_raw.eps");
  c3->Print("SLM_FCup_Ratio_raw.pdf");
  c3->Print("SLM_FCup_Ratio_raw.png");
  c3->Print("SLM_FCup_Ratio_raw.root");

  h_SLM_FCup_Ratio_DT_BB_in->SetTitle("; I_{SLM}/I_{FCup}");
  h_SLM_FCup_Ratio_DT_BB_in->Draw();
  f_Gaus->SetParameters(h_SLM_FCup_Ratio_DT_BB_in->GetMaximum(), h_SLM_FCup_Ratio_DT_BB_in->GetMean(), h_SLM_FCup_Ratio_DT_BB_in->GetRMS());
  h_SLM_FCup_Ratio_DT_BB_in->Fit(f_Gaus, "Mev", "", 9.5, 10.5);
  lat1->DrawLatex(0.2, 0.7, Form("#mu = %1.2f", f_Gaus->GetParameter(1)));
  lat1->DrawLatex(0.2, 0.65, Form("#sigma = %1.2f", f_Gaus->GetParameter(2)));
  c3->Print("SLM_FCup_Ratio_raw_BBIN.eps");
  c3->Print("SLM_FCup_Ratio_raw_BBIN.pdf");
  c3->Print("SLM_FCup_Ratio_raw_BBIN.png");
  c3->Print("SLM_FCup_Ratio_raw_BBIN.C");
  c3->Print("SLM_FCup_Ratio_raw_BBIN.root");
  
  h_SLM_FCup_Ratio_DT->Draw();
  h_SLM_FCup_Ratio_DT->SetTitle("; I_{SLM}/I_{FCup}");
  f_Gaus->SetParameters(h_SLM_FCup_Ratio_DT->GetMaximum(), h_SLM_FCup_Ratio_DT->GetMean(), h_SLM_FCup_Ratio_DT->GetRMS());
  h_SLM_FCup_Ratio_DT->Fit(f_Gaus, "Mev", "", 0.6, 1.2);
  c3->Print("SLM_FCup_Ratio.eps");
  c3->Print("SLM_FCup_Ratio.pdf");
  c3->Print("SLM_FCup_Ratio.png");
  c3->Print("SLM_FCup_Ratio.root");
}


void StyleGraph(TGraph *gr, int st, int col, double size, bool timeFormat){
  gr->SetMarkerStyle(st);
  gr->SetMarkerColor(col);
  gr->SetMarkerSize(size);

  if( timeFormat ){
    gr->GetXaxis()->SetTimeDisplay(1);
    gr->GetXaxis()->SetTimeOffset(0, "local");
    gr->GetXaxis()->SetTimeFormat("%m/%d/%y,%H:%M");
    gr->GetXaxis()->SetNdivisions(-508);

  }
}
