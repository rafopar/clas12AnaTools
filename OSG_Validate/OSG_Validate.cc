/* 
 * File:   OSG_Validate.cc
 * Author: rafopar
 *
 * Created on August 30, 2023, 2:49 PM
 */

#include <cstdlib>

using namespace std;

/*
 * 
 */
void OSG_Validate(int job1, std::string comment1, int job2, std::string comment2, int job3, std::string comment3) {
    
    cout<<"job1 is "<<job1<<"   Description is "<<comment1<<endl;
    cout<<"job2 is "<<job2<<"   Description is "<<comment2<<endl;
    cout<<"job3 is "<<job3<<"   Description is "<<comment3<<endl;

    TLatex *lat1 = new TLatex();
    lat1->SetNDC();
    lat1->SetTextFont(42);

    TFile *file_CurPortal = new TFile(Form("Hists_OSGValidate_Job_%d.root", job1), "Read");
    TFile *file_NewPortal = new TFile(Form("Hists_OSGValidate_Job_%d.root", job2), "Read");
    TFile *file_NewPortalOldSW = new TFile(Form("Hists_OSGValidate_Job_%d.root", job3), "Read");

    TH2D *h_MC_Memep_12_1_CurPortal = (TH2D*) file_CurPortal->Get("h_MC_Memep_12_1");
    h_MC_Memep_12_1_CurPortal->SetTitle("; M(e^{-}_{1}e^{+}) [GeV]; M(e^{-}_{2}e^{+}) [GeV]");
    TH1D *h_MC_emep_pair2_CurPortal = (TH1D*) h_MC_Memep_12_1_CurPortal->ProjectionY("h_MC_emep_pair2_CurPortal", 1, h_MC_Memep_12_1_CurPortal->GetNbinsX());
    h_MC_emep_pair2_CurPortal->SetLineColor(4);
    h_MC_emep_pair2_CurPortal->Draw("hist");
    TH1D *h_MC_emep_pair1_CurPortal = (TH1D*) h_MC_Memep_12_1_CurPortal->ProjectionX("h_MC_emep_pair1_CurPortal", 1, h_MC_Memep_12_1_CurPortal->GetNbinsY());
    h_MC_emep_pair1_CurPortal->SetLineColor(4);
    h_MC_emep_pair1_CurPortal->Draw("hist");

    TH2D *h_MC_Memep_12_1_NewPortal = (TH2D*) file_NewPortal->Get("h_MC_Memep_12_1");
    h_MC_Memep_12_1_NewPortal->SetTitle("; M(e^{-}_{1}e^{+}) [GeV]; M(e^{-}_{2}e^{+}) [GeV]");
    TH1D *h_MC_emep_pair2_NewPortal = (TH1D*) h_MC_Memep_12_1_NewPortal->ProjectionY("h_MC_emep_pair2_NewPortal", 1, h_MC_Memep_12_1_NewPortal->GetNbinsX());
    h_MC_emep_pair2_NewPortal->SetLineColor(2);
    h_MC_emep_pair2_NewPortal->Draw("hist");
    TH1D *h_MC_emep_pair1_NewPortal = (TH1D*) h_MC_Memep_12_1_NewPortal->ProjectionX("h_MC_emep_pair1_NewPortal", 1, h_MC_Memep_12_1_NewPortal->GetNbinsY());
    h_MC_emep_pair1_NewPortal->SetLineColor(2);
    h_MC_emep_pair1_NewPortal->Draw("hist");

    TH2D *h_MC_Memep_12_1_NewPortalOldSW = (TH2D*) file_NewPortalOldSW->Get("h_MC_Memep_12_1");
    h_MC_Memep_12_1_NewPortalOldSW->SetTitle("; M(e^{-}_{1}e^{+}) [GeV]; M(e^{-}_{2}e^{+}) [GeV]");
    TH1D *h_MC_emep_pair2_NewPortalOldSW = (TH1D*) h_MC_Memep_12_1_NewPortalOldSW->ProjectionY("h_MC_emep_pair2_NewPortalOldSW", 1, h_MC_Memep_12_1_NewPortalOldSW->GetNbinsX());
    h_MC_emep_pair2_NewPortalOldSW->SetLineColor(6);
    h_MC_emep_pair2_NewPortalOldSW->Draw("hist");
    TH1D *h_MC_emep_pair1_NewPortalOldSW = (TH1D*) h_MC_Memep_12_1_NewPortalOldSW->ProjectionX("h_MC_emep_pair1_NewPortalOldSW", 1, h_MC_Memep_12_1_NewPortalOldSW->GetNbinsY());
    h_MC_emep_pair1_NewPortalOldSW->SetLineColor(6);
    h_MC_emep_pair1_NewPortalOldSW->Draw("hist");


    TCanvas *c1 = new TCanvas("c1", "", 1200, 900);

    TRatioPlot *rp_MC_Memep1 = new TRatioPlot(h_MC_emep_pair1_NewPortal, h_MC_emep_pair1_CurPortal);
    rp_MC_Memep1->Draw();

    TRatioPlot *rp_MC_Memep2 = new TRatioPlot(h_MC_emep_pair2_NewPortal, h_MC_emep_pair2_CurPortal);
    rp_MC_Memep2->Draw();
    rp_MC_Memep2->GetLowerRefGraph()->SetMaximum(1.2);
    rp_MC_Memep2->GetLowerRefGraph()->SetMinimum(0.8);
    lat1->SetTextColor(h_MC_emep_pair2_NewPortal->GetLineColor());
    lat1->DrawLatex(0.55, 0.7, comment2.c_str());
    lat1->SetTextColor(h_MC_emep_pair2_CurPortal->GetLineColor());
    lat1->DrawLatex(0.55, 0.65, comment1.c_str());
    c1->Print("OSG_ValidateFigs/rp_MC_Memep2.pdf");
    c1->Print("OSG_ValidateFigs/rp_MC_Memep2.png");
    c1->Print("OSG_ValidateFigs/rp_MC_Memep2.root");

    TRatioPlot *rp_MC_Memep2_OldSW = new TRatioPlot(h_MC_emep_pair2_NewPortalOldSW, h_MC_emep_pair2_CurPortal);
    rp_MC_Memep2_OldSW->Draw();
    rp_MC_Memep2_OldSW->GetLowerRefGraph()->SetMaximum(1.2);
    rp_MC_Memep2_OldSW->GetLowerRefGraph()->SetMinimum(0.8);
    rp_MC_Memep2_OldSW->GetUpperPad()->cd();
    lat1->SetTextColor(h_MC_emep_pair2_CurPortal->GetLineColor());
    lat1->DrawLatex(0.55, 0.7, comment1.c_str());
    lat1->SetTextColor(h_MC_emep_pair2_NewPortalOldSW->GetLineColor());
    lat1->DrawLatex(0.55, 0.65, comment3.c_str());
    c1->Print("OSG_ValidateFigs/rp_MC_Memep2_OldSW.pdf");
    c1->Print("OSG_ValidateFigs/rp_MC_Memep2_OldSW.png");
    c1->Print("OSG_ValidateFigs/rp_MC_Memep2_OldSW.root");


    TH1D *h_Mmis1_CurPortal = (TH1D*) file_CurPortal->Get("h_Mmis1");
    h_Mmis1_CurPortal->SetTitle("; M_{X} [GeV]");
    h_Mmis1_CurPortal->SetLineColor(4);

    TH1D *h_Mmis1_NewPortal = (TH1D*) file_NewPortal->Get("h_Mmis1");
    h_Mmis1_NewPortal->SetTitle("; M_{X} [GeV]");
    h_Mmis1_NewPortal->SetLineColor(2);

    TH1D *h_Mmis1_NewPortal_OldSW = (TH1D*) file_NewPortalOldSW->Get("h_Mmis1");
    h_Mmis1_NewPortal_OldSW->SetTitle("; M_{X} [GeV]");
    h_Mmis1_NewPortal_OldSW->SetLineColor(6);
    
    c1->Clear();
    TRatioPlot *rp_Mmis1 = new TRatioPlot(h_Mmis1_NewPortal, h_Mmis1_CurPortal);
    rp_Mmis1->Draw();
    //rp_Mmis1->GetLowerRefGraph()->SetMaximum(3.8);
    rp_Mmis1->GetLowerRefGraph()->SetMinimum(0.5);
    rp_Mmis1->GetLowerPad()->SetLogy();
    lat1->SetTextColor(h_Mmis1_CurPortal->GetLineColor());
    lat1->DrawLatex(0.35, 0.7, comment1.c_str() );
    lat1->SetTextColor(h_Mmis1_NewPortal->GetLineColor());
    lat1->DrawLatex(0.35, 0.65, comment2.c_str());
    c1->Print("OSG_ValidateFigs/rp_Mmis1.pdf");
    c1->Print("OSG_ValidateFigs/rp_Mmis1.png");
    c1->Print("OSG_ValidateFigs/rp_Mmis1.root");

    c1->Clear();

    h_Mmis1_NewPortal_OldSW->SetMaximum(1.1 * h_Mmis1_NewPortal_OldSW->GetMaximum());
    TRatioPlot *rp_Mmis1_OldSW = new TRatioPlot(h_Mmis1_NewPortal_OldSW, h_Mmis1_CurPortal);
    rp_Mmis1_OldSW->Draw();
    rp_Mmis1_OldSW->GetLowerRefGraph()->SetMaximum(3.8);
    rp_Mmis1_OldSW->GetLowerRefGraph()->SetMinimum(0.5);
    //rp_Mmis1_OldSW->GetLowerPad()->SetLogy();
    lat1->SetTextColor(h_Mmis1_CurPortal->GetLineColor());
    lat1->DrawLatex(0.35, 0.7, comment1.c_str());
    lat1->SetTextColor(h_Mmis1_NewPortal_OldSW->GetLineColor());
    lat1->DrawLatex(0.35, 0.65, comment3.c_str());

    c1->Print("OSG_ValidateFigs/rp_Mmis1_OldSW.pdf");
    c1->Print("OSG_ValidateFigs/rp_Mmis1_OldSW.png");
    c1->Print("OSG_ValidateFigs/rp_Mmis1_OldSW.root");

    c1->Clear();
    lat1->SetTextColor(1);
    TF1 *f_Gaus = new TF1("f_Gaus", "[0]*TMath::Gaus(x, [1], [2])", 0.5, 1.5);
    f_Gaus->SetNpx(4500);
    f_Gaus->SetParameters(h_Mmis1_CurPortal->GetMaximum(), 0.9383, 0.02);
    h_Mmis1_CurPortal->Draw();
    h_Mmis1_CurPortal->SetAxisRange(0.8, 1.1);
    h_Mmis1_CurPortal->Fit(f_Gaus, "MeV", "", 0.85, 0.99);
    lat1->DrawLatex(0.15, 0.8, Form("#mu = %1.3f GeV", f_Gaus->GetParameter(1)));
    lat1->DrawLatex(0.15, 0.75, Form("#sigma = %1.2f MeV", f_Gaus->GetParameter(2)*1000));
    c1->Print("OSG_ValidateFigs/MMFit_CurPortal.pdf");
    c1->Print("OSG_ValidateFigs/MMFit_CurPortal.png");
    c1->Print("OSG_ValidateFigs/MMFit_CurPortal.root");

    h_Mmis1_NewPortal->Draw();
    h_Mmis1_NewPortal->SetAxisRange(0.8, 1.1);
    h_Mmis1_NewPortal->Fit(f_Gaus, "MeV", "", 0.85, 0.99);
    lat1->DrawLatex(0.15, 0.8, Form("#mu = %1.3f GeV", f_Gaus->GetParameter(1)));
    lat1->DrawLatex(0.15, 0.75, Form("#sigma = %1.2f MeV", f_Gaus->GetParameter(2)*1000));
    c1->Print("OSG_ValidateFigs/MMFit_NewPortal.pdf");
    c1->Print("OSG_ValidateFigs/MMFit_NewPortal.png");
    c1->Print("OSG_ValidateFigs/MMFit_NewPortal.root");

    h_Mmis1_NewPortal_OldSW->Draw();
    h_Mmis1_NewPortal_OldSW->SetAxisRange(0.8, 1.1);
    h_Mmis1_NewPortal_OldSW->Fit(f_Gaus, "MeV", "", 0.85, 0.99);
    lat1->DrawLatex(0.15, 0.8, Form("#mu = %1.3f GeV", f_Gaus->GetParameter(1)));
    lat1->DrawLatex(0.15, 0.75, Form("#sigma = %1.2f MeV", f_Gaus->GetParameter(2)*1000));
    c1->Print("OSG_ValidateFigs/MMFit_NewPortal_OldSW.pdf");
    c1->Print("OSG_ValidateFigs/MMFit_NewPortal_OldSW.png");
    c1->Print("OSG_ValidateFigs/MMFit_NewPortal_OldSW.root");

    return 0;
}