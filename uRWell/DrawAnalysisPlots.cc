/* 
 * File:   DrawAnalysisPlots.cc
 * Author: rafopar
 *
 * Created on March 5, 2023, 7:05 PM
 */

#include <cstdlib>

using namespace std;

void DrawActiveArea();

/*
 * 
 */
void DrawAnalysisPlots() {

    int run = 1291;
    const int nCutLvls = 5;
    double ADC_Thresholds[nCutLvls] = {5., 5.5, 6., 6.5, 7.};
    TLatex *lat1 = new TLatex();
    lat1->SetNDC();
    TCanvas *c1 = new TCanvas("c1", "", 2700, 1000);

    TFile *file_in = new TFile(Form("AnalyzeData_%d.root", run));
    
    TH2D *h_Cross_YXC_CutLvl_[nCutLvls];
    
    c1->Divide(3, 2);
    for( int i = 0; i < nCutLvls; i++ ){
        c1->cd(i+1);
        h_Cross_YXC_CutLvl_[i] = (TH2D*)file_in->Get(Form("h_Cross_YXC_CutLvl_%d", i));
        h_Cross_YXC_CutLvl_[i]->SetTitle("; Cross X [mm]; Cross Y [mm]");
        h_Cross_YXC_CutLvl_[i]->Draw("colz");
        lat1->DrawLatex(0.35, 0.92, Form("Signal threshold: %1.2f #sigma", ADC_Thresholds[i] ));
        DrawActiveArea();
    }
    c1->Print(Form("Figs/Cross_coord_%d.pdf", run));
    c1->Print(Form("Figs/Cross_coord_%d.png", run));
    c1->Print(Form("Figs/Cross_coord_%d.root", run));
        
    TLine *line1 = new TLine();
    line1->SetLineColor(2);
    line1->SetLineWidth(2);
    TCanvas *c2 = new TCanvas("c2", "", 1200, 1200);
    TH2D *h_AbsADC_VU1 = (TH2D*)file_in->Get("h_AbsADC_VU1");
    h_AbsADC_VU1->SetTitle("; ADC U ;ADC V");
    h_AbsADC_VU1->SetTitleOffset(1.5, "Y");
    h_AbsADC_VU1->SetStats(0);
    h_AbsADC_VU1->Draw("colz");
    line1->DrawLine(0., 0., 1000., 1000.);
    c2->Print(Form("Figs/AbsADC_VU1_%d.pdf", run));
    c2->Print(Form("Figs/AbsADC_VU1_%d.png", run));
    c2->Print(Form("Figs/AbsADC_VU1_%d.root", run));
    
}

void DrawActiveArea() {

    TLine *line1 = new TLine();
    line1->SetLineColor(2);
    line1->SetLineWidth(3);
    line1->DrawLine(-723, 250., 723., 250.);
    line1->DrawLine(-506.14, -250., 506.14, -250.);
    line1->DrawLine(-506.14, -250., -723, 250.);
    line1->DrawLine(506.14, -250., 723, 250.);
}
