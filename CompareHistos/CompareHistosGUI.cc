#include "CompareHistosGUI.h"
#include <TGClient.h>
#include <TGFileDialog.h>
#include <TSystem.h>
#include <TROOT.h>
#include <iostream>
#include <TKey.h>

// ------------------ implementation ----------------------

CompareHistosGUI::CompareHistosGUI(const TGWindow* p) {
    fMain = new TGMainFrame(p, 700, 500);
    fMain->SetCleanup(kDeepCleanup);

    TGHorizontalFrame* top = new TGHorizontalFrame(fMain);
    TGTextButton* btn1 = new TGTextButton(top, "Load File 1");
    TGTextButton* btn2 = new TGTextButton(top, "Load File 2");
    top->AddFrame(btn1, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX,4,4,4,4));
    top->AddFrame(btn2, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX,4,4,4,4));
    fMain->AddFrame(top, new TGLayoutHints(kLHintsTop | kLHintsExpandX));

    fListBox = new TGListBox(fMain);
    fMain->AddFrame(fListBox, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,4,4,4,4));

    fOverlay = new TGCheckButton(fMain, "Overlay histograms");
    fOverlay->SetOn();
    fMain->AddFrame(fOverlay, new TGLayoutHints(kLHintsBottom | kLHintsCenterX,4,4,4,4));

    // ✅ Connect using string-based slots
    btn1->Connect("Clicked()", "CompareHistosGUI", this, "LoadFile1()");
    btn2->Connect("Clicked()", "CompareHistosGUI", this, "LoadFile2()");

    fListBox->Connect("Selected(Int_t)", "CompareHistosGUI", this, "OnHistoSelected(Int_t)");

    fMain->SetWindowName("Compare ROOT Histograms");
    fMain->MapSubwindows();
    fMain->Resize();
    fMain->MapWindow();
}

CompareHistosGUI::~CompareHistosGUI() {
    if(fFile1){ fFile1->Close(); delete fFile1; fFile1=nullptr; }
    if(fFile2){ fFile2->Close(); delete fFile2; fFile2=nullptr; }
    if(fMain){ fMain->Cleanup(); delete fMain; fMain=nullptr; }
}

void CompareHistosGUI::LoadFile1() {
    TGFileInfo fi;
    static const char *filetypes[] = { "ROOT files","*.root",0,0 };
    fi.fFileTypes = filetypes;
    new TGFileDialog(gClient->GetRoot(), fMain, kFDOpen, &fi);
    if(!fi.fFilename) return;

    if(fFile1){ fFile1->Close(); delete fFile1; }
    fFile1 = TFile::Open(fi.fFilename);
    if(!fFile1 || fFile1->IsZombie()){ std::cerr<<"Failed to open file1\n"; fFile1=nullptr; return; }
    std::cout<<"Loaded file1: "<<fi.fFilename<<"\n";
}

void CompareHistosGUI::LoadFile2() {
    TGFileInfo fi;
    static const char *filetypes[] = { "ROOT files","*.root",0,0 };
    fi.fFileTypes = filetypes;
    new TGFileDialog(gClient->GetRoot(), fMain, kFDOpen, &fi);
    if(!fi.fFilename) return;

    if(fFile2){ fFile2->Close(); delete fFile2; }
    fFile2 = TFile::Open(fi.fFilename);
    if(!fFile2 || fFile2->IsZombie()){ std::cerr<<"Failed to open file2\n"; fFile2=nullptr; return; }
    std::cout<<"Loaded file2: "<<fi.fFilename<<"\n";

    if(!fFile1) return;

    fListBox->RemoveAll();
    std::set<std::string> names1, names2;
    TIter next(fFile1->GetListOfKeys()); TKey* k;
    while((k=(TKey*)next())) { if(k->GetClassName() && strstr(k->GetClassName(),"TH1")) names1.insert(k->GetName()); }
    next = TIter(fFile2->GetListOfKeys());
    while((k=(TKey*)next())) { if(k->GetClassName() && strstr(k->GetClassName(),"TH1")) names2.insert(k->GetName()); }

    for(auto& n : names1) if(names2.count(n)) fListBox->AddEntry(n.c_str(), fListBox->GetNumberOfEntries()+1);

    fListBox->Layout(); fListBox->MapSubwindows(); fListBox->MapWindow();
}

void CompareHistosGUI::OnHistoSelected(Int_t id) {
    TGTextLBEntry* entry = (TGTextLBEntry*)fListBox->GetEntry(id);
    if(!entry) return;
    std::string name = entry->GetTitle();
    CompareSelectedHisto(name, fOverlay->IsOn());
}

void CompareHistosGUI::CompareSelectedHisto(const std::string& name, bool overlay) {
    if(!fFile1 || !fFile2) return;
    TH1* h1 = dynamic_cast<TH1*>(fFile1->Get(name.c_str()));
    TH1* h2 = dynamic_cast<TH1*>(fFile2->Get(name.c_str()));
    if(!h1 || !h2) return;

    if(overlay){
        TCanvas* c = new TCanvas(("c_"+name).c_str(), name.c_str(), 900,700);
        h1->SetLineColor(kBlue);
        h2->SetLineColor(kRed);
        h1->Draw("hist"); h2->Draw("hist same");
        c->BuildLegend();
    } else {
        TCanvas* c = new TCanvas(("c_"+name).c_str(), name.c_str(), 900,700);
        c->Divide(1,2);
        c->cd(1); h1->Draw();
        c->cd(2); h2->Draw();
    }
}
