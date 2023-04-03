/* 
 * File:   DrawRateVsHV.cc
 * Author: rafopar
 *
 * Created on March 7, 2023, 5:39 PM
 */

#include <cstdlib>

using namespace std;

/*
 * 
 */
void DrawRateVsHV() {

    TCanvas *c1 = new TCanvas("c1", "", 1200, 1200);
    c1->SetTopMargin(0.02);
    c1->SetRightMargin(0.02);
    c1->SetLeftMargin(0.14);
    
    const int n_HV_Set = 4;
    const int nCutLvls = 5;
    double ADC_Thresholds[nCutLvls] = {5., 5.5, 6., 6.5, 7.};

    int runs[n_HV_Set] = {1287, 1290, 1291, 1515};
    int HVs[n_HV_Set] = {560, 570, 575, 576};
    int N_analyzedEvents[n_HV_Set] = {696397, 999817, 910525, 760872};


    TGraph * gr_NormalizedCounts_[nCutLvls];

    for (int i = 0; i < nCutLvls; i++) {
        gr_NormalizedCounts_[i] = new TGraph();
        gr_NormalizedCounts_[i]->SetMarkerColor(7-i);
        gr_NormalizedCounts_[i]->SetLineColor(7-i);
        gr_NormalizedCounts_[i]->SetLineWidth(2);
        gr_NormalizedCounts_[i]->SetMarkerStyle(i+20);
        gr_NormalizedCounts_[i]->SetMarkerSize(2);
    }

    for (int iHV = 0; iHV < n_HV_Set; iHV++){
        
        TFile *f_cur_file = new TFile(Form("AnalyzeData_%d.root", runs[iHV]));
        
        for( int iCut = 0; iCut < nCutLvls; iCut++ ){
            TH2D *h_curHist = (TH2D*)f_cur_file->Get(Form("h_Cross_YXC_CutLvl_%d", iCut));
            double norm_counts = h_curHist->Integral()/double(N_analyzedEvents[iHV]);
            gr_NormalizedCounts_[iCut]->SetPoint(iHV, HVs[iHV], norm_counts);
        }
        
    }
    
    TMultiGraph *mtgr1 = new TMultiGraph();
    
    for( int i = 0; i < nCutLvls; i++ ){
        mtgr1->Add(gr_NormalizedCounts_[i]);
    }
    
    TLegend *leg1 = new TLegend(0.16, 0.75, 0.45, 0.97);
    leg1->SetBorderSize(0);
    for( int iCut = 0; iCut < nCutLvls; iCut++ ){
    	leg1->AddEntry(gr_NormalizedCounts_[iCut], Form("%1.1f #sigma cut", ADC_Thresholds[iCut]));
    }

    mtgr1->SetTitle("; HV Sec B MESH [V]; Normalized # of Events w/ a UxV cross ");
    mtgr1->Draw("APL");
    mtgr1->SetMinimum(0);
    leg1->Draw();
    c1->Print("Figs/Rate_VS_HV.pdf");
    c1->Print("Figs/Rate_VS_HV.png");
    c1->Print("Figs/Rate_VS_HV.root");

return 0;
}
