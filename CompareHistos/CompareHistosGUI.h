#ifndef COMPAREHISTOSGUI_H
#define COMPAREHISTOSGUI_H

#include <TFile.h>
#include <TGFrame.h>
#include <TGButton.h>
#include <TGListBox.h>
#include <RQ_OBJECT.h>
#include <TH1.h>
#include <TCanvas.h>
#include <set>
#include <string>

class CompareHistosGUI {
    RQ_OBJECT("CompareHistosGUI")  // enables signals/slots with ROOT dictionary

public:
    CompareHistosGUI(const TGWindow* p);
    virtual ~CompareHistosGUI();

    void LoadFile1();
    void LoadFile2();
    void OnHistoSelected(Int_t id);
    void CompareSelectedHisto(const std::string& name, bool overlay);

private:
    TGMainFrame* fMain = nullptr;
    TGListBox*   fListBox = nullptr;
    TGCheckButton* fOverlay = nullptr;
    TFile*       fFile1 = nullptr;
    TFile*       fFile2 = nullptr;
};

#endif
