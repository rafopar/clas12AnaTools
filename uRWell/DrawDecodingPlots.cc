/* 
 * File:   DrawDecodingPlots.cc
 * Author: rafopar
 *
 * Created on February 6, 2023, 4:47 PM
 */

#include <cstdlib>

using namespace std;

/*
 * 
 */
int DrawDecodingPlots() {

    int run = 1215;

    TFile *file_in = new TFile(Form("CheckDecoding_CSCh_%d.root", run));

    const int n_Slot = 12;

    TH2D * h_ADC_Chan_slot_[n_Slot];
    TGraph * gr_RMS_Slot[n_Slot];

    for (int igr = 0; igr < n_Slot; igr++) {
        gr_RMS_Slot[igr] = new TGraph();
    }

    for (int i = 0; i < n_Slot; i++) {

        h_ADC_Chan_slot_[i] = (TH2D*) file_in->Get(Form("h_ADC_Chan_slot_%d", i));

        int gr_ind = 0;
        for (int ch = 0; ch < h_ADC_Chan_slot_[i]->GetNbinsX(); ch++) {

            TH1D *h_tmp = (TH1D*) h_ADC_Chan_slot_[i]->ProjectionY("tmp", ch + 1, ch + 1);

            if (h_tmp->GetEntries() > 0) {
                double rms = h_tmp->GetRMS();
                gr_RMS_Slot[i]->SetPoint(gr_ind, ch + 1, rms);
                gr_ind = gr_ind + 1;
            }

        }


    }

    TCanvas *c1 = new TCanvas("c1", "", 1200, 400);
    c1->SetRightMargin(0.01);
    for (int igr = 0; igr < n_Slot; igr++) {        
        gr_RMS_Slot[igr]->SetMarkerColor(4);
        gr_RMS_Slot[igr]->SetMarkerStyle(20);
        gr_RMS_Slot[igr]->SetMaximum(30);
        gr_RMS_Slot[igr]->SetTitle(Form("Slot %d ; Ch; ADC", igr));
        gr_RMS_Slot[igr]->Draw("APl");
        c1->Print(Form("Figs/RMS_Slot_%d_Run_%d.pdf", igr, run));
        c1->Print(Form("Figs/RMS_Slot_%d_Run_%d.png", igr, run));
        c1->Print(Form("Figs/RMS_Slot_%d_Run_%d.root", igr, run));
    }


    return 0;
}