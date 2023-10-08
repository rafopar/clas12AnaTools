void SLM_FCup_Ratio_raw_BBIN()
{
//=========Macro generated from canvas: c3/
//=========  (Wed Jan 17 09:34:48 2018) by ROOT version 6.12/04
   TCanvas *c3 = new TCanvas("c3", "",1020,180,900,900);
   c3->Range(8.72093,-0.122093,11.04651,1.098837);
   c3->SetFillColor(0);
   c3->SetBorderMode(0);
   c3->SetBorderSize(2);
   c3->SetLeftMargin(0.12);
   c3->SetRightMargin(0.02);
   c3->SetTopMargin(0.04);
   c3->SetFrameBorderMode(0);
   c3->SetFrameBorderMode(0);
   
   TH1D *h_SLM_FCup_Ratio_DT_BB_in__1 = new TH1D("h_SLM_FCup_Ratio_DT_BB_in__1","",200,9,11);
   h_SLM_FCup_Ratio_DT_BB_in__1->SetBinContent(0,67);
   h_SLM_FCup_Ratio_DT_BB_in__1->SetBinContent(201,720);
   h_SLM_FCup_Ratio_DT_BB_in__1->SetEntries(787);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *ptstats_LaTex = ptstats->AddText("h_SLM_FCup_Ratio_DT_BB_in");
   ptstats_LaTex->SetTextSize(0.0368);
   ptstats_LaTex = ptstats->AddText("Entries = 787    ");
   ptstats_LaTex = ptstats->AddText("Mean  =      0");
   ptstats_LaTex = ptstats->AddText("Std Dev   =      0");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   h_SLM_FCup_Ratio_DT_BB_in__1->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(h_SLM_FCup_Ratio_DT_BB_in__1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   h_SLM_FCup_Ratio_DT_BB_in__1->SetLineColor(ci);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetXaxis()->SetTitle(" I_{SLM}/I_{FCup}");
   h_SLM_FCup_Ratio_DT_BB_in__1->GetXaxis()->SetLabelFont(42);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetXaxis()->SetLabelSize(0.035);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetXaxis()->SetTitleSize(0.035);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetXaxis()->SetTitleFont(42);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetYaxis()->SetLabelFont(42);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetYaxis()->SetLabelSize(0.035);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetYaxis()->SetTitleSize(0.035);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetYaxis()->SetTitleOffset(0);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetYaxis()->SetTitleFont(42);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetZaxis()->SetLabelFont(42);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetZaxis()->SetLabelSize(0.035);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetZaxis()->SetTitleSize(0.035);
   h_SLM_FCup_Ratio_DT_BB_in__1->GetZaxis()->SetTitleFont(42);
   h_SLM_FCup_Ratio_DT_BB_in__1->Draw("");
   TLatex *   tex = new TLatex(0.2,0.7,"#mu = 0.00");
tex->SetNDC();
   tex->SetTextFont(42);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.2,0.65,"#sigma = 0.00");
tex->SetNDC();
   tex->SetTextFont(42);
   tex->SetLineWidth(2);
   tex->Draw();
   c3->Modified();
   c3->cd();
   c3->SetSelected(c3);
}
