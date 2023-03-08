/* 
 * File:   Skim_Coincidence.cc
 * Author: rafopar
 *
 * Created on January 13, 2023, 10:03 AM
 */

#include <cstdlib>

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
    char out_txtFile[256];

    int run = 0;
    if (argc > 1) {
        run = atoi(argv[1]);
        sprintf(inputFile, "Data/decoded_%d.hipo", run);
        sprintf(outputFile, "Data/SkimCoincidence_%d.hipo", run);
        sprintf(out_txtFile, "Data/Skim_EvNumbers_%d.dat", run);
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

    hipo::bank bRAWADc(factory.getSchema("RAW::adc"));
    hipo::bank bRunConf(factory.getSchema("RUN::config"));

    hipo::writer writer;
    writer.addDictionary(factory);
    writer.open(outputFile);

    ofstream out_skim(out_txtFile);

    const int ADC_Threshold = 50.;
    const int crateID_test3 = 40; // the fADC is on ROC 40
    const int slot_fADC = 3; // and the slot is 3

    try {

        while (reader.next() == true) {
            reader.read(event);

            evCounter = evCounter + 1;

            //if( evCounter > 5000 ){break;}
            if (evCounter % 10000 == 0) {
                cout.flush() << "Processed " << evCounter << " events \r";
            }

            event.getStructure(bRAWADc);
            event.getStructure(bRunConf);

            int ev_number = bRunConf.getInt("event", 0);

            int n_fADC = bRAWADc.getRows();
            /**
             * Only 4 channels on fADC are read out 0,1 and 9,10 . 0 and 1 are from Bottom scintillator, while 9 and 10 are from Top
             */
            int ADC_SCTop = 0;
            int ADC_SCBot = 0;
            int ADC_AntiCut = 0; //We need to make sure there is no signal on those channels

            for (int ifADC = 0; ifADC < n_fADC; ifADC++) {

                int crate = bRAWADc.getInt("crate", ifADC);
                int slot = bRAWADc.getInt("slot", ifADC);

                if (!(crate == crateID_test3 && slot == slot_fADC)) {
                    cout << "Wrong crate or slot. Should not be happen" << endl;
                    cout << "              crate = " << crate << "     slot = " << slot << endl;

                    continue;
                }

                int ch = bRAWADc.getInt("channel", ifADC);
                int ADC = bRAWADc.getInt("ADC", ifADC);
                double time = bRAWADc.getFloat("time", ifADC);

                if (ch == 3 || ch == 7) {
                    ADC_SCBot = ADC_SCBot + ADC;
                } else if (ch == 9 || ch == 10) {
                    ADC_SCTop = ADC_SCTop + ADC;
                } else if (ch == 0 || ch == 1 || ch == 2 || ch == 4 || ch == 5 || ch == 6 || ch == 8 || ch == 11 ||  ch == 12 || ch == 13 || ch == 14 || ch == 15) {
                    ADC_AntiCut = ADC_AntiCut + ADC;
                }
            }


            if (ADC_SCBot > ADC_Threshold && ADC_SCTop > ADC_Threshold && ADC_AntiCut < ADC_Threshold ) {
                writer.addEvent(event);
                out_skim << ev_number << endl;
            }

        }
    } catch (const char msg) {
        cerr << msg << endl;
    }

    writer.close();
    writer.showSummary();

    return 0;
}

