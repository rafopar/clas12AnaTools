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


void Draw_2C24_FCup(){

  const double SLM_Offset = -4644.;
  const double SLM_Slope = 11420.;
  const double FCup_Slope = 906.2;
  const double FCup_KPP_Offset = 170.;
  const double SLM_Slope_DT = 11420.;
  const double SLM_Offset_DT = -4644.;
  const double t_start_DataTaking = 1513234000; // Beginning of Data Taking
  const double t_End_DataTaking = 1513258012;   // End of Data Taking

  TLine *line1 = new TLine();
  TLatex *lat1 = new TLatex();
  lat1->SetTextFont(42);
  lat1->SetNDC();

  TF1 *f_lin1 = new TF1("f_lin1", "[0] + [1]*x", 0., 100.);
  TF1 *f_Gaus = new TF1("f_Gaus", "[0]*TMath::Gaus(x, [1], [2])", 0., 2.);
  f_Gaus->SetNpx(2500);

  TGraph *gr_FCup_curr = new TGraph();
  TGraph *gr_2C24_curr = new TGraph();
  TGraph *gr_2c24_Fcup_ratio = new TGraph();
  TGraph *gr_2c24_Fcup_ratio_BBOut = new TGraph();

  TH1D *h_2C24_FCup_ratio1 = new TH1D("h_2C24_FCup_ratio1", "", 200, 9., 11.);
  TH1D *h_2C24_FCup_ratio_BBOut1 = new TH1D("h_2C24_FCup_ratio_BBOut1", "", 200, 0.5, 1.5);
  
  Double_t t_S2b, count_S2b;
  Double_t t_2C24, count_2C24;

  TCanvas *c1 = new TCanvas("c1", "", 1200, 500);

  TTree *tr_S2b = new TTree("tr_S2b", "tr_S2b");
  TTree *tr_2C24 = new TTree("tr_2C24", "tr_2C24");

  tr_S2b->Branch("t_S2b", &t_S2b, "t_S2b/D");
  tr_S2b->Branch("count_S2b", &count_S2b, "count_S2b/D");


  tr_S2b->ReadFile("Scaler_S2b_Data_15_Dec_2017_BB_Out.dat", "t_S2b:count_S2b");
  
  int n_S2b = tr_S2b->GetEntries();
  Double_t *t_S2b_arr = new Double_t[n_S2b];
  Double_t *count_S2b_arr = new Double_t[n_S2b];
  
  for( int i = 0; i < n_S2b; i++ ){
    tr_S2b->GetEntry(i);
    t_S2b_arr[i] = t_S2b;
    count_S2b_arr[i] = count_S2b;
    //cout<<count_S2b_arr[i]<<endl;
    //    gr_FCup_curr_Raw->SetPoint(i, t_S2b, (count_S2b - FCup_KPP_Offset)/FCup_Slope);
  }

  
  tr_2C24->Branch("t_2C24", &t_2C24, "t_2C24/D");
  tr_2C24->Branch("cont_2C24", &count_2C24, "count_2C24/D");

  //tr_2C24->ReadFile("2C24_12_14_2017_BB_In.dat", "t_2C24:count_2C24");
  tr_2C24->ReadFile("2C24_12_14_2017_BB_Out.dat", "t_2C24:count_2C24");

  double FCup_curr;
  double FCup_curr_raw;
  double FCup_offset = 171; // The offset at the beginning

  int S2b_index = 0;
  int ind_Datataking = 0;
  int n_2C24 = tr_2C24->GetEntries();

  for( int i = 0; i < n_2C24; i++ ){
    tr_2C24->GetEntry(i);

    while( t_S2b_arr[S2b_index] <= t_2C24 ){

      if( S2b_index > 5 && S2b_index < n_S2b - 6 ){
        if( count_S2b_arr[S2b_index - 5] < 400 && count_S2b_arr[S2b_index - 4] < 400 && count_S2b_arr[S2b_index - 3] < 400
            && count_S2b_arr[S2b_index - 2] < 400 && count_S2b_arr[S2b_index - 1] < 400 && count_S2b_arr[S2b_index] > 400
            && count_S2b_arr[S2b_index + 1] > 400 && count_S2b_arr[S2b_index + 2] > 400 && count_S2b_arr[S2b_index + 3] > 400
            && count_S2b_arr[S2b_index + 4] > 400 ){
          //Offset is defined as the average of recent 5 valued of scalerS2b before beam came back
	  FCup_offset = (count_S2b_arr[S2b_index - 5] + count_S2b_arr[S2b_index - 4] + count_S2b_arr[S2b_index - 3] +
                         count_S2b_arr[S2b_index - 2] + count_S2b_arr[S2b_index - 1])/5.;
	  //FCup_offset = 100;
	  //cout<<"fcou_offset = "<<FCup_offset<<endl;
        }
      }

      //cout<<"S2b counts = "<<count_S2b_arr[S2b_index]<<"    FCup_Offset = "<<FCup_offset<<"    FCup_Slope = "<<FCup_Slope<<endl;
      FCup_curr = TMath::Max((count_S2b_arr[S2b_index] - FCup_offset)/FCup_Slope, 0.01);
      FCup_curr_raw =  (count_S2b_arr[S2b_index] - FCup_KPP_Offset)/FCup_Slope;
      S2b_index = S2b_index + 1;
    }
    
    gr_FCup_curr->SetPoint(i, t_2C24, FCup_curr);
    gr_2C24_curr->SetPoint(i, t_2C24, count_2C24);

    double ratio_2C24_FCup = count_2C24/FCup_curr;
    gr_2c24_Fcup_ratio->SetPoint(i, t_2C24, ratio_2C24_FCup);
    gr_2c24_Fcup_ratio_BBOut->SetPoint(i, t_2C24, ratio_2C24_FCup);
    h_2C24_FCup_ratio1->Fill(ratio_2C24_FCup);
    h_2C24_FCup_ratio_BBOut1->Fill(ratio_2C24_FCup);

  }

  gr_FCup_curr->SetMarkerColor(4);
  gr_FCup_curr->SetLineColor(4);
  gr_FCup_curr->SetMarkerStyle(20);
  gr_FCup_curr->SetMarkerSize(0.2);

  
  gr_2C24_curr->SetMarkerColor(2);
  gr_2C24_curr->SetLineColor(4);
  gr_2C24_curr->SetMarkerStyle(20);
  gr_2C24_curr->SetMarkerSize(0.2);

  gr_2c24_Fcup_ratio->SetMarkerColor(2);
  gr_2c24_Fcup_ratio->SetLineColor(4);
  gr_2c24_Fcup_ratio->SetMarkerStyle(20);
  gr_2c24_Fcup_ratio->SetMarkerSize(0.2);

  gr_2c24_Fcup_ratio_BBOut->SetMarkerColor(2);
  gr_2c24_Fcup_ratio_BBOut->SetLineColor(4);
  gr_2c24_Fcup_ratio_BBOut->SetMarkerStyle(20);
  gr_2c24_Fcup_ratio_BBOut->SetMarkerSize(0.2);

  
  gr_FCup_curr->Draw("AP");
  c1->Print("FCup_Curr.eps");
  c1->Print("FCup_Curr.pdf");
  c1->Print("FCup_Curr.png");
  
  c1->Clear();
  gr_2C24_curr->Draw("AP");
  c1->Print("2C24_Current.eps");
  c1->Print("2C24_Current.pdf");
  c1->Print("2C24_Current.png");

  c1->Clear();
  gr_2c24_Fcup_ratio->SetMaximum(18.);
  gr_2c24_Fcup_ratio->Draw("AP");
  c1->Print("2C24_FCup_Ratio.eps");
  c1->Print("2C24_FCup_Ratio.pdf");
  c1->Print("2C24_FCup_Ratio.png");
  c1->Print("2C24_FCup_Ratio.root");
  c1->Print("2C24_FCup_Ratio.C");

  c1->Clear();
  gr_2c24_Fcup_ratio_BBOut->SetMaximum(1.5);
  gr_2c24_Fcup_ratio_BBOut->Draw("AP");
  c1->Print("2C24_FCup_Ratio_BBOut.eps");
  c1->Print("2C24_FCup_Ratio_BBOut.pdf");
  c1->Print("2C24_FCup_Ratio_BBOut.png");
  c1->Print("2C24_FCup_Ratio_BBOut.root");
  c1->Print("2C24_FCup_Ratio_BBOut.C");

  c1->Clear();
  h_2C24_FCup_ratio1->SetTitle(";2C24/FCup");
  h_2C24_FCup_ratio1->Draw();
  f_Gaus->SetParameters(h_2C24_FCup_ratio1->GetMaximum(), h_2C24_FCup_ratio1->GetMean(), h_2C24_FCup_ratio1->GetRMS());
  h_2C24_FCup_ratio1->Fit(f_Gaus, "MeV", "", 9.5, 10.5);
  c1->Print("2C24_FCup_average_Ratio.eps");
  c1->Print("2C24_FCup_average_Ratio.pdf");
  c1->Print("2C24_FCup_average_Ratio.png");


  h_2C24_FCup_ratio_BBOut1->SetTitle(";2C24/FCup");
  h_2C24_FCup_ratio_BBOut1->Draw();
  f_Gaus->SetParameters(h_2C24_FCup_ratio_BBOut1->GetMaximum(), h_2C24_FCup_ratio_BBOut1->GetMean(), h_2C24_FCup_ratio_BBOut1->GetRMS());
  h_2C24_FCup_ratio_BBOut1->Fit(f_Gaus, "MeV", "", 0.5, 1.5);
  c1->Print("2C24_FCup_average_Ratio_BBOut.eps");
  c1->Print("2C24_FCup_average_Ratio_BBOut.pdf");
  c1->Print("2C24_FCup_average_Ratio_BBOut.png");

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
