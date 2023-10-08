#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLine.h>
#include <TTree.h>
#include <fstream>
#include <TLatex.h>
#include <TGraph.h>
#include <TCanvas.h>

void Draw_calibr()
{

  TCanvas *c1 = new TCanvas("c1", "", 750, 750);

  TLatex *lat1 = new TLatex();
  lat1->SetTextSize(0.03);
  lat1->SetNDC();

  TLine *line1 = new TLine();
  line1->SetLineColor(2);

  const double Imax = 60; // Max currrent that was used
  
  const double SLM_slope = 18543.639;
  const double SLM_offset = 524.;

  ofstream out_dat("fit_pars.dat");
  
  TF1 *f_lin1 = new TF1("f_lin1", "[0] + [1]*x", 0., 100.);
  TF1 *f_Gaus = new TF1("f_Gaus", "[0]*TMath::Gaus(x, [1], [2])", 0., 2.);
  f_Gaus->SetNpx(4500);
  
  double S2b, S16b, IPM2C21A, IPM2C24A;

  const double fCup_offset = 493;
  const double fCup_slope = 906.2;
  
  TTree *tr1 = new TTree("tr1", "");
  tr1->Branch("S2b", &S2b, "S2b/D");
  tr1->Branch("S16b", &S16b, "S16b/D");
  tr1->Branch("IPM2C21A", &IPM2C21A, "IPM2C21A/D");
  tr1->Branch("IPM2C24A", &IPM2C24A, "IPM2C24A/D");
  
  tr1->ReadFile("10p2_Mar_19_2019_BB_Out.dat", "S2b:S16b:IPM2C21A:IPM2C24A");

  TGraph *gr_SLM_Fcup_bbout = new TGraph();
  TGraph *gr_SLM_Fcup_ratio_bbout = new TGraph();
  TGraph *gr_2c21_Fcup_bbout = new TGraph();
  TGraph *gr_2c21_Fcup_ratio_bbout = new TGraph();
  TGraph *gr_2c24_Fcup_bbout = new TGraph();
  TGraph *gr_2c24_Fcup_ratio_bbout = new TGraph();

  TH1D *h_2C21_fCup_ratio1 = new TH1D("h_2C21_fCup_ratio1", "", 200, 0.95, 1.15);
  TH1D *h_2C24_fCup_ratio1 = new TH1D("h_2C24_fCup_ratio1", "", 200, 0.9, 1.1);

  int n1 = tr1->GetEntries();

  cout<<"n1 = "<<n1<<endl;

  int ind_2c21 = 0;
  int ind_2c24 = 0;
  for( int i = 0; i < n1; i++ ){
    //LoadTree(i);
    tr1->GetEntry(i);

    double IFcup = (S2b - fCup_offset)/fCup_slope;
    
    gr_SLM_Fcup_bbout->SetPoint(i, IFcup, S16b);

    double ISLM = (S16b - SLM_offset)/SLM_slope;
    double SLM_FCUP_ratio = TMath::Min(ISLM/IFcup, 1000.);

    gr_SLM_Fcup_ratio_bbout->SetPoint(i, IFcup, SLM_FCUP_ratio);
    
    if( IPM2C24A >  0.9*IFcup && IPM2C24A <  1.1*IFcup ){
      gr_2c24_Fcup_bbout->SetPoint(ind_2c24, IFcup, IPM2C24A);
      h_2C24_fCup_ratio1->Fill(IPM2C24A/IFcup);
      gr_2c24_Fcup_ratio_bbout->SetPoint(ind_2c24, IFcup, IPM2C24A/IFcup);
      ind_2c24 = ind_2c24 + 1;
    }
    
    
    if( IPM2C21A >  0.9*IFcup && IPM2C21A <  1.1*IFcup ){
      gr_2c21_Fcup_bbout->SetPoint(ind_2c21, IFcup, IPM2C21A);
      h_2C21_fCup_ratio1->Fill(IPM2C21A/IFcup);
      gr_2c21_Fcup_ratio_bbout->SetPoint(ind_2c21, IFcup, IPM2C21A/IFcup);
      ind_2c21 = ind_2c21 + 1;
    }
  }


  c1->Print("Beam_current_figures.pdf[");
  gr_SLM_Fcup_bbout->SetTitle("SLM; I_{FCup} [nA]; scalerS16b");
  gr_SLM_Fcup_bbout->SetMarkerColor(4);
  gr_SLM_Fcup_bbout->SetMarkerStyle(21);
  gr_SLM_Fcup_bbout->SetMarkerSize(0.45);

  gr_SLM_Fcup_bbout->GetYaxis()->SetTitleOffset(1.4);
  gr_SLM_Fcup_bbout->Draw("AP");
  gr_SLM_Fcup_bbout->Fit(f_lin1, "MeV", "", 1, Imax);
  double SLM_offset_fit = f_lin1->GetParameter(0);
  double SLM_offset_fit_err = f_lin1->GetParError(0);
  double SLM_slope_fit = f_lin1->GetParameter(1);
  double SLM_slope_fit_err = f_lin1->GetParError(1);
  lat1->DrawLatex(0.14, 0.8, Form("Slope = %1.3f #pm %1.3f [Hz/nA]",  SLM_slope_fit, SLM_offset_fit_err));
  lat1->DrawLatex(0.14, 0.75, Form("Offset = %1.3f #pm %1.3f [Hz]",  SLM_offset_fit, SLM_slope_fit_err));
  //  lat1->DrawLatex(0.14, 0.7, Form("#chi^{2}/NDF = %1.3f ", f_lin1->GetChisquare()/f_lin1->GetNDF()));

  out_dat<<"  ============ SLM Offset and Slope ========== "<<endl;
  out_dat<<"SLM_Offset = "<<SLM_offset_fit<<"   -/+     "<<SLM_offset_fit_err<<endl;
  out_dat<<"SLM_Slope  =  "<<SLM_slope_fit<<"   -/+     "<<SLM_slope_fit_err<<endl;
  out_dat<<endl;
  out_dat<<endl;
  
  c1->Modified();
  c1->Update();
  c1->Print("Beam_current_figures.pdf");

  c1->Clear();
  gr_SLM_Fcup_ratio_bbout->SetTitle("SLM; I_{FCup} [nA]; I_{SLM}/I_{FCup}");
  gr_SLM_Fcup_ratio_bbout->SetMaximum(1.5);
  gr_SLM_Fcup_ratio_bbout->SetMinimum(0.8);
  gr_SLM_Fcup_ratio_bbout->SetMarkerColor(4);
  gr_SLM_Fcup_ratio_bbout->SetMarkerStyle(21);
  gr_SLM_Fcup_ratio_bbout->SetMarkerSize(0.45);
  gr_SLM_Fcup_ratio_bbout->Draw("AP");
  c1->Modified();
  c1->Update();
  c1->Print("Beam_current_figures.pdf");

 

  c1->Clear();
 gr_2c21_Fcup_bbout->SetTitle("2C21 ;I_{FCup} [nA]; I_{2C21} [nA]");
  gr_2c21_Fcup_bbout->SetMarkerColor(4);
  gr_2c21_Fcup_bbout->SetMarkerStyle(21);
  gr_2c21_Fcup_bbout->SetMarkerSize(0.45);
  gr_2c21_Fcup_bbout->Draw("AP");
  gr_2c21_Fcup_bbout->Fit(f_lin1, "MeV", "", 1, Imax);
  lat1->DrawLatex(0.14, 0.85, "F = a + b*I_{FCup}");
  lat1->DrawLatex(0.14, 0.8, Form("a = %1.3f #pm %1.3f nA",  f_lin1->GetParameter(0),  f_lin1->GetParError(0)));
  lat1->DrawLatex(0.14, 0.75, Form("b = %1.4f #pm %1.4f", f_lin1->GetParameter(1), f_lin1->GetParError(1) ));
  //  lat1->DrawLatex(0.14, 0.7, Form("#chi^{2}/NDF = %1.3f ", f_lin1->GetChisquare()/f_lin1->GetNDF()));
  out_dat<<"  ============ 2C21 Offset and Slope ========== "<<endl;
  out_dat<<"2c21_Offset = "<<f_lin1->GetParameter(0)<<"   -/+     "<<f_lin1->GetParError(0)<<endl;
  out_dat<<"2c21_Slope  = "<<f_lin1->GetParameter(1)<<"   -/+     "<<f_lin1->GetParError(1)<<endl;
  out_dat<<endl;
  out_dat<<endl;

  c1->Print("Beam_current_figures.pdf");

  h_2C21_fCup_ratio1->SetTitle("2C21; I_{2C21}/I_{FCup}");
  h_2C21_fCup_ratio1->Draw();
  double mean = h_2C21_fCup_ratio1->GetBinCenter(h_2C21_fCup_ratio1->GetMaximumBin());
  double rms = h_2C21_fCup_ratio1->GetRMS();
  f_Gaus->SetParameters(h_2C21_fCup_ratio1->GetMaximum(), mean, 0.5*rms);
  h_2C21_fCup_ratio1->Fit(f_Gaus, "MeV", "", mean - 1.9*rms, mean + 1.9*rms);
  double scale_2C21 = f_Gaus->GetParameter(1);
  double scale_2C21_err = f_Gaus->GetParError(1);
  double sigm_2C21 = f_Gaus->GetParameter(2);
  double sigm_2C21_err = f_Gaus->GetParError(2);
  lat1->DrawLatex(0.15, 0.8, Form("#mu = %1.4f #pm %1.4f", scale_2C21, scale_2C21_err));
  //  lat1->DrawLatex(0.15, 0.75, Form("#chi^{2}/NDF = %1.3f ", f_Gaus->GetChisquare()/f_Gaus->GetNDF()));
  c1->Print("Beam_current_figures.pdf");
  out_dat<<"  ============ 2C21 Scale (1D histogram with Gaussian fit ) ========== "<<endl;
  out_dat<<"2c21 Scale = "<<scale_2C21<<"   -/+     "<<scale_2C21_err<<endl;
  out_dat<<"2c21 Sigma = "<<sigm_2C21<<"   -/+     "<<sigm_2C21_err<<endl;
  out_dat<<endl;
  out_dat<<endl;

  
  c1->Clear();
  gr_2c21_Fcup_ratio_bbout->GetYaxis()->SetTitleOffset(1.2);
  gr_2c21_Fcup_ratio_bbout->SetTitle("2C21 ;I_{FCup} [nA]; I_{2C21}/I_{FCup}");
  gr_2c21_Fcup_ratio_bbout->SetMarkerColor(4);
  gr_2c21_Fcup_ratio_bbout->SetMarkerStyle(21);
  gr_2c21_Fcup_ratio_bbout->SetMarkerSize(0.45);
  gr_2c21_Fcup_ratio_bbout->Draw("AP");
  line1->DrawLine(0., scale_2C21, Imax, scale_2C21);
  c1->Print("Beam_current_figures.pdf");


  c1->Clear();
  gr_2c24_Fcup_bbout->SetTitle("2C24 ;I_{FCup} [nA]; I_{2C24} [nA]");
  gr_2c24_Fcup_bbout->SetMarkerColor(4);
  gr_2c24_Fcup_bbout->SetMarkerStyle(21);
  gr_2c24_Fcup_bbout->SetMarkerSize(0.45);
  gr_2c24_Fcup_bbout->Draw("AP");
  gr_2c24_Fcup_bbout->Fit(f_lin1, "MeV", "", 1, Imax);
  lat1->DrawLatex(0.14, 0.85, "F = a + b*I_{FCup}");
  lat1->DrawLatex(0.14, 0.8, Form("a = %1.3f #pm %1.3f nA",  f_lin1->GetParameter(0), f_lin1->GetParError(0)));
  lat1->DrawLatex(0.14, 0.75, Form("b = %1.4f #pm %1.4f ", f_lin1->GetParameter(1), f_lin1->GetParError(1) ));
  //  lat1->DrawLatex(0.14, 0.7, Form("#chi^{2}/NDF = %1.3f ", f_lin1->GetChisquare()/f_lin1->GetNDF()));
  out_dat<<"  ============ 2C24 Offset and Slope ========== "<<endl;
  out_dat<<"2c24_Offset = "<<f_lin1->GetParameter(0)<<"   -/+     "<<f_lin1->GetParError(0)<<endl;
  out_dat<<"2c24_Slope  = "<<f_lin1->GetParameter(1)<<"   -/+     "<<f_lin1->GetParError(1)<<endl;
  out_dat<<endl;
  out_dat<<endl;

  c1->Print("Beam_current_figures.pdf");

  h_2C24_fCup_ratio1->SetTitle("2C24; I_{2C24}/I_{FCup}");
  h_2C24_fCup_ratio1->Draw();
  mean = h_2C24_fCup_ratio1->GetBinCenter(h_2C24_fCup_ratio1->GetMaximumBin());
  rms = h_2C24_fCup_ratio1->GetRMS();
  f_Gaus->SetParameters(h_2C24_fCup_ratio1->GetMaximum(), mean, 0.5*rms);
  h_2C24_fCup_ratio1->Fit(f_Gaus, "MeV", "", mean - 1.9*rms, mean + 1.9*rms);
  double scale_2C24 = f_Gaus->GetParameter(1);
  double scale_2C24_err = f_Gaus->GetParError(1);
  double sigm_2C24 = f_Gaus->GetParameter(2);
  double sigm_2C24_err = f_Gaus->GetParError(2);

  lat1->DrawLatex(0.15, 0.8, Form("#mu = %1.4f #pm %1.4f", scale_2C24, scale_2C24_err));
  //  lat1->DrawLatex(0.15, 0.75, Form("#chi^{2}/NDF = %1.3f ", f_Gaus->GetChisquare()/f_Gaus->GetNDF()));
  out_dat<<"  ============ 2C24 Scale (1D histogram with Gaussian fit ) ========== "<<endl;
  out_dat<<"2c24 Scale = "<<scale_2C24<<"   -/+     "<<scale_2C24_err<<endl;
  out_dat<<"2c24 Sigma = "<<sigm_2C24<<"   -/+     "<<sigm_2C24_err<<endl;
  out_dat<<endl;
  out_dat<<endl;

  c1->Print("Beam_current_figures.pdf");

  c1->Clear();
  gr_2c24_Fcup_ratio_bbout->GetYaxis()->SetTitleOffset(1.2);
  gr_2c24_Fcup_ratio_bbout->SetTitle("2C24 ;I_{FCup} [nA]; I_{2C24}/I_{FCup}");
  gr_2c24_Fcup_ratio_bbout->SetMarkerColor(4);
  gr_2c24_Fcup_ratio_bbout->SetMarkerStyle(21);
  gr_2c24_Fcup_ratio_bbout->SetMarkerSize(0.45);
  gr_2c24_Fcup_ratio_bbout->Draw("AP");
  line1->DrawLine(0., scale_2C24, Imax, scale_2C24);
  c1->Print("Beam_current_figures.pdf");

  c1->Print("Beam_current_figures.pdf]");


  //  double S2b, S16b, IPM2C21A, IPM2C24A;

  TGraph *gr_2c21_Fcup_bbin = new TGraph();
  TGraph *gr_2c21_Fcup_ratio_bbin = new TGraph();
  TGraph *gr_2c24_Fcup_bbin = new TGraph();
  TGraph *gr_2c24_Fcup_ratio_bbin = new TGraph();

  TH1D *h_2C21_fCup_ratio_bbin1 = new TH1D("h_2C21_fCup_ratio_bbin1", "", 200, 8., 12.);
  TH1D *h_2C24_fCup_ratio_bbin1 = new TH1D("h_2C24_fCup_ratio_bbin1", "", 200, 8., 12.);
  
  TTree *tr2 = new TTree("tr2", "");
  tr2->Branch("S2b", &S2b, "S2b/D");
  tr2->Branch("S16b", &S16b, "S16b/D");
  tr2->Branch("IPM2C21A", &IPM2C21A, "IPM2C21A/D");
  tr2->Branch("IPM2C24A", &IPM2C24A, "IPM2C24A/D");
  
  tr2->ReadFile("10p2_Mar_19_2019_BB_In.dat", "S2b:S16b:IPM2C21A:IPM2C24A");

  int n2 = tr2->GetEntries();

  cout<<"n2 = "<<n2<<endl;


  ind_2c21 = 0;
  ind_2c24 = 0;
  for( int i = 0; i < n2; i++ ){
    //LoadTree(i);
    tr2->GetEntry(i);

    double IFcup = (S2b - fCup_offset)/fCup_slope;
    
    IPM2C24A = IPM2C24A/scale_2C24;
    
    if( IPM2C24A*0.08 < IFcup && IPM2C24A*0.12 > IFcup ){
      gr_2c24_Fcup_bbin->SetPoint(ind_2c24, IPM2C24A, IFcup);
      h_2C24_fCup_ratio_bbin1->Fill(IPM2C24A/IFcup);
      gr_2c24_Fcup_ratio_bbin->SetPoint(ind_2c24, IPM2C24A, IPM2C24A/IFcup);
      ind_2c24 = ind_2c24 + 1;
    }


    IPM2C21A = IPM2C21A/scale_2C21;
    
    if( IPM2C21A*0.08 < IFcup && IPM2C21A*0.12 > IFcup ){
      gr_2c21_Fcup_bbin->SetPoint(ind_2c21, IPM2C21A, IFcup);
      h_2C21_fCup_ratio_bbin1->Fill(IPM2C21A/IFcup);
      gr_2c21_Fcup_ratio_bbin->SetPoint(ind_2c21, IPM2C21A, IPM2C21A/IFcup);
      ind_2c21 = ind_2c21 + 1;
    }
  }

  out_dat<<"  ======================================================================== "<<endl;
  out_dat<<"  ============================= Beam Belocker in ========================= "<<endl;
  out_dat<<"  ======================================================================== "<<endl;
  out_dat<<endl;
  out_dat<<endl;
  c1->Print("BB_attenuation.pdf[");

  c1->Clear();
  gr_2c21_Fcup_bbin->SetTitle("2C21 corrected ;Corrected I_{2C21} [nA]; I_{FCup} [nA]");
  gr_2c21_Fcup_bbin->SetMarkerColor(4);
  gr_2c21_Fcup_bbin->SetMarkerStyle(21);
  gr_2c21_Fcup_bbin->SetMarkerSize(0.45);
  gr_2c21_Fcup_bbin->Draw("AP");
  gr_2c21_Fcup_bbin->Fit(f_lin1, "MeV", "", 1, Imax );
  lat1->DrawLatex(0.14, 0.85, "F = a + b*I_{FCup}");
  lat1->DrawLatex(0.14, 0.8, Form("a = %1.4f #pm %1.4f nA",  f_lin1->GetParameter(0), f_lin1->GetParError(0)));
  lat1->DrawLatex(0.14, 0.75, Form("b = %1.4f + %1.4f ", f_lin1->GetParameter(1), f_lin1->GetParError(1) ));
  //  lat1->DrawLatex(0.14, 0.7, Form("#chi^{2}/NDF = %1.3f ", f_lin1->GetChisquare()/f_lin1->GetNDF()));
  out_dat<<"  ============ FCup offset and slop wrt 2c21: Linear fit ========== "<<endl;
  out_dat<<"FCup_Offset with 2c21 = "<<f_lin1->GetParameter(0)<<"   -/+     "<<f_lin1->GetParError(0)<<endl;
  out_dat<<"FCup_Slope with 2c21  = "<<f_lin1->GetParameter(1)<<"   -/+     "<<f_lin1->GetParError(1)<<endl;
  out_dat<<endl;
  out_dat<<endl;

  c1->Print("BB_attenuation.pdf");

  h_2C21_fCup_ratio_bbin1->SetTitle(" Attenuation w/ Corrected 2C21; I_{2C21}/I_{FCup}");
  h_2C21_fCup_ratio_bbin1->Draw();
  mean = h_2C21_fCup_ratio_bbin1->GetBinCenter(h_2C21_fCup_ratio_bbin1->GetMaximumBin());
  rms = h_2C21_fCup_ratio_bbin1->GetRMS();
  f_Gaus->SetParameters(h_2C21_fCup_ratio_bbin1->GetMaximum(), mean, 0.5*rms);
  h_2C21_fCup_ratio_bbin1->Fit(f_Gaus, "MeV", "", mean - 1.9*rms, mean + 1.9*rms);
  double attenuation_2C21 = f_Gaus->GetParameter(1);
  double attenuation_2C21_err = f_Gaus->GetParError(1);
  double sigm_att_2C21 = f_Gaus->GetParameter(2);
  double sigm_att_2C21_err = f_Gaus->GetParError(2);

  lat1->DrawLatex(0.15, 0.8, Form("#mu = %1.4f #pm %1.4f", attenuation_2C21, attenuation_2C21_err));
  out_dat<<"  ============ FCup Attenuation wrt 2c21 (1D histogram with Gaussian fit ) ========== "<<endl;
  out_dat<<"FCup Attenuation wrt 2C21 corrected = "<<attenuation_2C21<<"   -/+     "<<attenuation_2C21_err<<endl;
  out_dat<<"Sigma FCup Att. wrt 2C21 corrected  = "<<sigm_att_2C21<<"   -/+     "<<sigm_att_2C21_err<<endl;
  out_dat<<endl;
  out_dat<<endl;

  c1->Print("BB_attenuation.pdf");

  c1->Clear();
  gr_2c21_Fcup_ratio_bbin->GetYaxis()->SetTitleOffset(1.2);
  gr_2c21_Fcup_ratio_bbin->SetMaximum(12.);
  gr_2c21_Fcup_ratio_bbin->SetMinimum(9.);
  gr_2c21_Fcup_ratio_bbin->SetTitle("Corrected 2C21 ;I_{2C21} [nA]; I_{2C21}/I_{FCup}");
  gr_2c21_Fcup_ratio_bbin->SetMarkerColor(4);
  gr_2c21_Fcup_ratio_bbin->SetMarkerStyle(21);
  gr_2c21_Fcup_ratio_bbin->SetMarkerSize(0.45);
  gr_2c21_Fcup_ratio_bbin->Draw("AP");
  line1->DrawLine(0., attenuation_2C21, Imax, attenuation_2C21);
  c1->Print("BB_attenuation.pdf");


  c1->Clear();
  gr_2c24_Fcup_bbin->SetTitle("2C24 corrected ;Corrected I_{2C24} [nA]; I_{FCup} [nA]");
  gr_2c24_Fcup_bbin->SetMarkerColor(4);
  gr_2c24_Fcup_bbin->SetMarkerStyle(21);
  gr_2c24_Fcup_bbin->SetMarkerSize(0.45);
  gr_2c24_Fcup_bbin->Draw("AP");
  gr_2c24_Fcup_bbin->Fit(f_lin1, "MeV", "", 1, Imax);
  lat1->DrawLatex(0.14, 0.85, "F = a + b*I_{FCup}");
  lat1->DrawLatex(0.14, 0.8, Form("a = %1.4f #pm %1.4f nA", f_lin1->GetParameter(0), f_lin1->GetParError(0)));
  lat1->DrawLatex(0.14, 0.75, Form("b = %1.4f #pm %1.4f", f_lin1->GetParameter(1), f_lin1->GetParError(1) ));
  out_dat<<"  ============ FCup offset and slop wrt 2c24: Linear fit ========== "<<endl;
  out_dat<<"FCup_Offset with 2c24 = "<<f_lin1->GetParameter(0)<<"   -/+     "<<f_lin1->GetParError(0)<<endl;
  out_dat<<"FCup_Slope with 2c24  = "<<f_lin1->GetParameter(1)<<"   -/+     "<<f_lin1->GetParError(1)<<endl;
  out_dat<<endl;
  out_dat<<endl;

  c1->Print("BB_attenuation.pdf");

  h_2C24_fCup_ratio_bbin1->SetTitle(" Attenuation w/ Corrected 2C24; I_{2C24}/I_{FCup}");
  h_2C24_fCup_ratio_bbin1->Draw();
  mean = h_2C24_fCup_ratio_bbin1->GetBinCenter(h_2C24_fCup_ratio_bbin1->GetMaximumBin());
  rms = h_2C24_fCup_ratio_bbin1->GetRMS();
  f_Gaus->SetParameters(h_2C24_fCup_ratio_bbin1->GetMaximum(), mean, 0.5*rms);
  h_2C24_fCup_ratio_bbin1->Fit(f_Gaus, "MeV", "", mean - 1.9*rms, mean + 1.9*rms);
  double attenuation_2C24 = f_Gaus->GetParameter(1);
  double attenuation_2C24_err = f_Gaus->GetParError(1);
  double sigm_att_2C24 = f_Gaus->GetParameter(2);
  double sigm_att_2C24_err = f_Gaus->GetParError(2);

  lat1->DrawLatex(0.15, 0.8, Form("#mu = %1.4f #pm %1.4f ", attenuation_2C24, attenuation_2C24_err));
  out_dat<<"  ============ FCup Attenuation wrt 2c24 (1D histogram with Gaussian fit ) ========== "<<endl;
  out_dat<<"FCup Attenuation wrt 2C24 corrected = "<<attenuation_2C24<<"   -/+     "<<attenuation_2C24_err<<endl;
  out_dat<<"Sigma FCup Att. wrt 2C24 corrected  = "<<sigm_att_2C24<<"   -/+     "<<sigm_att_2C24_err<<endl;
  out_dat<<endl;
  out_dat<<endl;

  c1->Print("BB_attenuation.pdf");

  c1->Clear();
  gr_2c24_Fcup_ratio_bbin->GetYaxis()->SetTitleOffset(1.2);
  gr_2c24_Fcup_ratio_bbin->SetMaximum(12.);
  gr_2c24_Fcup_ratio_bbin->SetMinimum(9);
  gr_2c24_Fcup_ratio_bbin->SetTitle("Corrected 2C24 ;I_{2C24} [nA]; I_{2C24}/I_{FCup}");
  gr_2c24_Fcup_ratio_bbin->SetMarkerColor(4);
  gr_2c24_Fcup_ratio_bbin->SetMarkerStyle(21);
  gr_2c24_Fcup_ratio_bbin->SetMarkerSize(0.45);
  gr_2c24_Fcup_ratio_bbin->Draw("AP");
  line1->DrawLine(0., attenuation_2C24, Imax, attenuation_2C24);
  c1->Print("BB_attenuation.pdf");
 
  
  c1->Print("BB_attenuation.pdf]");
  
}
