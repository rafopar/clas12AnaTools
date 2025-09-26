#include "clas12AnaTools.h"

#include <TGraph.h>
#include <TCanvas.h>
#include <TSpectrum.h>


namespace clas12AnaTools {

    void SliceFit(TH2D* inp_hist, double min_X, double max_X, int nBins, TF1 *f_Fit, std::string keyWord) {
        TSpectrum *sp1 = new TSpectrum();

        TGraph gr;
        gr.SetMarkerColor(2);
        gr.SetMarkerStyle(20);

        double delta_X = (max_X - min_X) / double(nBins);

        TCanvas *c_tmp = new TCanvas("c_tmp", "", 950, 950);

        c_tmp->Print(Form("Figs/debugPlots_%s.pdf[", keyWord.c_str()));

        for (int i = 0; i < nBins; i++) {

            double x1 = min_X + i*delta_X;
            double x2 = min_X + (i + 1) * delta_X;
            int bin1 = inp_hist->GetXaxis()->FindBin(x1);
            int bin2 = inp_hist->GetXaxis()->FindBin(x2);

            TH1D *h_tmp = (TH1D*) inp_hist->ProjectionY(Form("h_tmp_%d", i), bin1, bin2);
            h_tmp->Draw();
            double rms = h_tmp->GetRMS();
            sp1->Search(h_tmp, 1 * rms, "nobackground", 0.2);

            inp_hist->SetAxisRange(x1, x2);
            double avg_x = inp_hist->GetMean(1);
            inp_hist->GetXaxis()->UnZoom();

            c_tmp->Print(Form("Figs/debugPlots_%s.pdf", keyWord.c_str()));
            double *xx = sp1->GetPositionX();
            double *yy = sp1->GetPositionY();
            int n_peaks = sp1->GetNPeaks();

            /*
             *  It is possible sometime to get more than one peak.
             *  In these cases we will chose the one with Maximum Y value
             */
            double max = yy[0];
            int ind_max = 0;

            for (int i = 1; i < n_peaks; i++) {
                if (yy[i] > max) {
                    max = yy[i];
                    ind_max = i;
                }
            }

            double peak_pos = xx[ind_max];

            c_tmp->Clear();
            gr.SetPoint(i, avg_x, peak_pos);
        }

        gr.Fit(f_Fit, "MeV", "", min_X, max_X);
        inp_hist->Draw();
        gr.Draw("P Same");
        c_tmp->Print(Form("Figs/debugPlots_%s.pdf", keyWord.c_str()));

        c_tmp->Print(Form("Figs/debugPlots_%s.pdf]", keyWord.c_str()));

    }

}