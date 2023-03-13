/* 
 * File:   DrawFTofTrgPlots.cc
 * Author: rafopar
 *
 * Created on March 12, 2023, 7:39 PM
 */

#include <cstdlib>

using namespace std;

/*
 * 
 */
void DrawFTofTrgPlots() {
    
    TCanvas *c1 = new TCanvas("c1", "", 950, 950);
    
    /**
     * Following constants are from the FTOF geometry document https://www.jlab.org/Hall-B/ftof/notes/ftof_geom.pdf
     */
    const double R2 = 659.71;
    const double th2Min = 34.698*TMath::DegToRad();
    const double th2Tilt = 58.11*TMath::DegToRad();
    const int nBars = 5;
    const double width_2 = 22;
    
    /**
     * Y = ax + b
     * In this function a = -tan(60deg)
     * b = is determined from TOF geometry document, and is equal to R2*Sin(th2Min)*(tan(60)*cos(30) - sin(30)  )
     */
    
    double a = -tan(30*TMath::DegToRad() );
    double b = R2*sin(th2Min)*( -tan(30*TMath::DegToRad() )*cos(60*TMath::DegToRad()) - sin(60*TMath::DegToRad()) );
    double b2 = b - nBars*width_2*sin( (90 - th2Tilt)*TMath::DegToRad() )/cos( 30*TMath::DegToRad() );
    
    TF1 *f1 = new TF1("f1", "[0] + [1]*x", -500, 0.);
    f1->SetParameters(b, a);
    TF1 *f2 = new TF1("f2", "[0] + [1]*x", -500, 0.);
    f2->SetParameters(b2, a);
    
    int run = 17738;
    //int run = 16787;
    
    TFile *file_in = new TFile(Form("AnalyzeSec6TofTrg_%d.root", run));
    
    TH2D *h_Panel2_YXc1 = (TH2D*)file_in->Get("h_Panel2_YXc1");
    
    h_Panel2_YXc1->Draw("colz");
    f1->Draw("Same");
    f2->Draw("Same");
    
    TCanvas *c2 = new TCanvas("c2", "", 1800, 1200);
    c2->Divide(3, 2);
    const int nPIDs = 5;
    TH2D *h_Panel2_YXc1_Pid_[nPIDs];
    std::string pidNames[nPIDs] = {"211", "Neg211", "321", "2212", "0"};
    
    for( int i = 0; i < nPIDs; i++ ){
        c2->cd(i+1);
        h_Panel2_YXc1_Pid_[i] = (TH2D*)file_in->Get(Form("mh_Panel2_YXc1_Pid_%s", pidNames[i].c_str()));
        h_Panel2_YXc1_Pid_[i]->SetTitleOffset(1.4, "Y");
        h_Panel2_YXc1_Pid_[i]->Draw("colz");
        h_Panel2_YXc1_Pid_[i]->SetTitle(Form("PID = %s; -X [cm]; y [cm]", pidNames[i].c_str()));
        f1->Draw("Same");
        f2->Draw("Same");
    }
    
    
    return 0;
}

