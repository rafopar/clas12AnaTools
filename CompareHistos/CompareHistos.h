#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <TGFileDialog.h>

class TRootEmbeddedCanvas;
class TGListBox;
class TFile;

class TestApp {
    RQ_OBJECT("example2")
private:
    TGMainFrame *fMain;
    const TGWindow *p_wind;
    TRootEmbeddedCanvas *fEcanvas;
    TGTransientFrame *fMain_log;
    TGCheckButton *fOverlay = nullptr;
    TGFileInfo file1_info;
    TGFileInfo file2_info;
    TGTextButton *open_file1;
    TGTextButton *open_file2;
    TGListBox *fListBox = nullptr;
    TFile *fFile1 = nullptr;
    TFile *fFile2 = nullptr;

    bool file1_selected = false;
    bool file2_selected = false;

    Int_t fNameID; // id of the selected item in the ListBox
    bool fIsOverlay = true; // Whether Overlay Histograms or plot in different canvaces

public:
    TestApp(const TGWindow *p, UInt_t w, UInt_t h);

    virtual ~TestApp();
    void OpenFile();
    void CloseApp();
    void UpdateFileListBox();
    void SelectAndPlotHistos(Int_t);
    void PlotHistos();
    void ToggleOverlayAndPlotHistos( Bool_t );
};
