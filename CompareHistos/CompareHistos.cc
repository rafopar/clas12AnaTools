#include <TApplication.h>
#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <set>
#include <string>
#include <vector>
#include <TH1D.h>
#include <TMath.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iterator>
#include <TLatex.h>
#include <TKey.h>
#include <TGraph.h>
#include <TGText.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGMsgBox.h>
#include <TSpectrum.h>
#include <TGTextEdit.h>
#include <TTimeStamp.h>
#include <TGComboBox.h>
#include <TGDimension.h>
#include <TGTextEntry.h>
#include <TGFileDialog.h>
#include <TGNumberEntry.h>
#include <TRootEmbeddedCanvas.h>

#include "CompareHistos.h"

#include <iostream>
#include <TFile.h>

using namespace std;

TestApp::TestApp(const TGWindow *p, UInt_t w, UInt_t h) {
    p_wind = p;
    fMain_log = NULL;

    // Create a main frame
    fMain = new TGMainFrame(p, w, h, kHorizontalFrame);
    fMain->Connect("CloseWindow()", "TestApp", this, "CloseApp()");
    // Create canvas widget
    fEcanvas = new TRootEmbeddedCanvas("Ecanvas", fMain, 700, 900);

    TGVerticalFrame *vframe = new TGVerticalFrame(fMain, 400, 40);

    open_file1 = new TGTextButton(vframe, "&Choose 1st file");
    open_file1->Connect("Clicked()", "TestApp", this, "OpenFile()");
    vframe->AddFrame(open_file1, new TGLayoutHints(kLHintsCenterX, 55, 55, 3, 4));


    open_file2 = new TGTextButton(vframe, "&Choose 2nd file");
    open_file2->Connect("Clicked()", "TestApp", this, "OpenFile()");
    vframe->AddFrame(open_file2, new TGLayoutHints(kLHintsCenterX, 55, 55, 3, 4));

    fListBox = new TGListBox(vframe);
    fListBox->SetHeight(400);
    fListBox->Connect("Selected(Int_t)", "TestApp", this, "SelectAndPlotHistos(Int_t)");
    vframe->AddFrame(fListBox, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 3, 4));

    fOverlay = new TGCheckButton(fMain, "Overlay histograms");
    fOverlay->SetOn();
    fOverlay->Connect("Toggled(Bool_t)", "TestApp", this, "ToggleOverlayAndPlotHistos(Bool_t)");
    vframe->AddFrame(fOverlay, new TGLayoutHints(kLHintsBottom | kLHintsCenterX,4,4,4,4));



    TGTextButton *exit = new TGTextButton(vframe, "&Exit");
    exit->Connect("Clicked()", "TestApp", this, "CloseApp()");
    vframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX, 1, 1, 1, 1));


    fMain->AddFrame(vframe, new TGLayoutHints(kLHintsTop, 2, 2, 2, 2));
    fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    fMain->SetWindowName("TestApp Window");
    // Map all subwindows of main frame
    fMain->MapSubwindows();
    // Initialize the layout algorithm
    fMain->Resize(fMain->GetDefaultSize());
    // Map main frame
    fMain->MapWindow();
}

void TestApp::OpenFile() {
    const char *filetypes[] = {"Root Files", "*.root", 0, 0};

    TGFileInfo *fileInfo;
    int id;
    TGButton *sender = (TGButton *) gTQSender;
    if (sender == open_file1) {
        id = 1;
        fileInfo = &file1_info;
    } else if (sender == open_file2) {
        fileInfo = &file2_info;
        id = 2;
    } else {
        id = 0;
        cout << "Open file is called from a wrong button." << endl;
        cout << "This should not happen" << endl;
        cout << "Exiting..." << endl;
        exit(1);
    }


    fileInfo->fFileTypes = filetypes;
    fileInfo->fIniDir = StrDup("/home/rafopar/work");


    //  cout<<"Before choosing a file, the file_name is "<<file_info.fFilename<<endl;


    TGFileDialog *dialog = new TGFileDialog(gClient->GetDefaultRoot(), gClient->GetDefaultRoot(), kFDOpen, fileInfo);

    if (id == 1) {
        fFile1 = TFile::Open(fileInfo->fFilename);

        if (!fFile1 || fFile1->IsZombie()) {
            cerr << "Failed to open file1" << endl;
            fFile1 = nullptr;
            return;
        }
        file1_selected = true;
        cout << "File1 is selected" << endl;
    } else if (id == 2) {
        fFile2 = TFile::Open(fileInfo->fFilename);

        if (!fFile2 || fFile2->IsZombie()) {
            cerr << "Failed to open file2" << endl;
            fFile2 = nullptr;
            return;
        }
        file2_selected = true;
        cout << "File2 is selected" << endl;
    }

    if (file1_selected && file2_selected) {
        cout << "Both files are selected" << endl;
        UpdateFileListBox();
    }
}

void TestApp::UpdateFileListBox() {
    // Clean the List Box
    fListBox->RemoveAll();

    std::set<std::string> names1, names2;
    TIter next(fFile1->GetListOfKeys());
    TKey *k;
    while ((k = (TKey *) next())) {
        auto clsName = k->GetClassName();
        //if (k->GetClassName() && strstr(k->GetClassName(), "TH1")) names1.insert(k->GetName());
        if (clsName && (strstr(clsName, "TH1") || strstr(clsName, "TH2") ) ) names1.insert(k->GetName());
    }
    next = TIter(fFile2->GetListOfKeys());
    while ((k = (TKey *) next())) {
        auto clsName = k->GetClassName();
        //if (k->GetClassName() && strstr(k->GetClassName(), "TH1")) names2.insert(k->GetName());
        if (clsName && (strstr(clsName, "TH1") || strstr(clsName, "TH2") ) ) names2.insert(k->GetName());
    }

    for (auto &n: names1) if (names2.count(n)) fListBox->AddEntry(n.c_str(), fListBox->GetNumberOfEntries() + 1);

    fListBox->Layout();
    fListBox->MapSubwindows();
    fListBox->MapWindow();
}

void TestApp::SelectAndPlotHistos(Int_t id) {
    fNameID = id;

    PlotHistos();
}

void TestApp::ToggleOverlayAndPlotHistos(Bool_t checked) {
    fIsOverlay = checked;

    PlotHistos();
}

void TestApp::PlotHistos() {

    TGTextLBEntry *entry = (TGTextLBEntry *) fListBox->GetEntry(fNameID);
    if (!entry) return;
    std::string histName = entry->GetTitle();
    //CompareSelectedHisto(name, fOverlay->IsOn());

    if (!fFile1 || !fFile2) return;
    TH1 *h1 = dynamic_cast<TH1 *>(fFile1->Get(histName.c_str()));
    TH1 *h2 = dynamic_cast<TH1 *>(fFile2->Get(histName.c_str()));
    if (!h1 || !h2) return;

    h1->SetLineColor(kRed);
    h2->SetLineColor(kBlue);

    TCanvas *c1 = (TCanvas*) fEcanvas->GetCanvas();
    c1->Clear();
    c1->Modified();
    c1->Update();

    if (fIsOverlay) {
        c1->SetBottomMargin(0.15);
        h1->Draw();
        h2->Draw("Same");
    }else {

        c1->Divide(1, 2, 0., 0.);
        c1->cd(1);
        h1->Draw();
        c1->cd(2)->SetBottomMargin(0.15);
        h2->Draw("Same");
    }

    c1->Modified();
    c1->Update();
}

void TestApp::CloseApp() {
    cout << "Bye..." << endl;
    cout << "Հաջողություն..." << endl;
    cout << "Пока..." << endl;
    gApplication->Terminate(0);
}


TestApp::~TestApp() {
    // Clean up used widgets: frames, buttons, layouthints
    fMain->Cleanup();
    delete fMain;
}


void RunTestApp() {
    // Popup the GUI...
    cout << "Have a wonderful work․․․" << endl;
    cout << "Բարի աշխատանք․․․" << endl;
    cout << "Приятной работы․․․" << endl;
    new TestApp(gClient->GetRoot(), 400, 400);
}


int main(int argc, char **argv) {
    TApplication theApp("App", &argc, argv);
    RunTestApp();
    theApp.Run();
    return 0;
}
