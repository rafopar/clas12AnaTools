/* 
 * File:   CheckTrgBits.cc
 * Author: rafopar
 *
 * Created on March 9, 2023, 7:25 PM
 */

#include <cstdlib>
#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>

// ===== Hipo headers =====
#include <reader.h>
#include <writer.h>
#include <dictionary.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    char outputFile[256];
    char inputFile[256];

    int run = 0;
    if (argc > 1) {
        run = atoi(argv[1]);
        //sprintf(inputFile, "%s", argv[1]);
        sprintf(inputFile, "/volatile/clas12/rafopar/uRwell/Data/decoded_%d_All.hipo", run);
        //sprintf(outputFile, "outSkim.hipo");
    } else {
        std::cout << " *** please provide a file name..." << std::endl;
        exit(0);
    }

    hipo::reader reader;
    reader.open(inputFile);

    hipo::dictionary factory;

    reader.readDictionary(factory);

    factory.show();
    hipo::event event;
    int evCounter = 0;

    hipo::bank bRunConf(factory.getSchema("RUN::config"));
    hipo::bank bRunTrg(factory.getSchema("RUN::trigger"));

    TFile *file_out = new TFile(Form("CheckTrgBits_%d.root", run), "Recreate");

    TH1D h_trgWord1("h_trgWord1", "", 33, -0.5, 32.5);
    TH1D h_trgWord2("h_trgWord2", "", 33, -0.5, 32.5);
    TH1D h_trgWord3("h_trgWord3", "", 33, -0.5, 32.5);

    try {

        while (reader.next() == true) {
            reader.read(event);

            evCounter = evCounter + 1;

//            if (evCounter > 2000) {
//                break;
//            }
            if (evCounter % 1000 == 0) {
                cout.flush() << "Processed " << evCounter << " events \r";
            }

            event.getStructure(bRunConf);
            event.getStructure(bRunTrg);

            int nTrg = bRunTrg.getRows();

            if (nTrg != 3) {
                cout << "nTrg = " << nTrg << "     It should be " << nTrg << endl;
                continue;
            }

            int trgWord1 = bRunTrg.getInt("trigger", 0);
            int trgWord2 = bRunTrg.getInt("trigger", 1);
            int trgWord3 = bRunTrg.getInt("trigger", 2);

//            cout<<trgWord1<<"    "<<trgWord2<<"    "<<trgWord3<<endl;
            
            for (int ibit = 0; ibit < 32; ibit++) {
                if (trgWord1 & (1<<ibit) ) {
                    h_trgWord1.Fill(ibit);
                }
                if (trgWord2 & (1<<ibit) ) {
                    h_trgWord2.Fill(ibit);
                }
                if (trgWord3 & (1<<ibit) ) {
                    h_trgWord3.Fill(ibit);
                }
            }
        }
    } catch (const char msg) {
        cerr << msg << endl;
    }

    gDirectory->Write();
    file_out->Close();

    return 0;
}