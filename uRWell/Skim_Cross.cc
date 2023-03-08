/* 
 * File:   Skim_Cross.cc
 * Author: rafopar
 *
 * Created on March 4, 2023, 11:43 PM
 */

#include <cstdlib>

#include <TH2D.h>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>

// ===== Hipo headers =====
#include <reader.h>
#include <writer.h>
#include <dictionary.h>

#include <map>
#include <vector>
#include <utility>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    char outputFile[256];
    char inputFile[256];

    int run = 0;
    int file = 0;
    if (argc > 2) {
        run = atoi(argv[1]);
        file = atoi(argv[2]);
        //sprintf(inputFile, "%s", argv[1]);
        sprintf(inputFile, "Data/decoded_%d_%d.hipo", run, file);
        sprintf(outputFile, "Data/Skim_Cross_%d_%d.hipo", run, file);
    } else {
        std::cout << " *** please provide a run number..." << std::endl;
        exit(0);
    }

    hipo::reader reader;
    reader.open(inputFile);

    hipo::dictionary factory;

    reader.readDictionary(factory);

    factory.show();
    hipo::event event;
    int evCounter = 0;

    hipo::bank buRWellADC(factory.getSchema("URWELL::adc"));
    hipo::bank bRAWADc(factory.getSchema("RAW::adc"));
    hipo::bank bRunConf(factory.getSchema("RUN::config"));

    hipo::writer writer;
    writer.addDictionary(factory);
    writer.open(outputFile);


    const double sigm_threshold = 5.; // Represents the threshold of the ADC in units of the sigma.
    const int crateID_test3 = 40; // the fADC is on ROC 40
    const int slot_fADC = 3; // and the slot is 3
    const int n_ts = 6;


    /**
     * Reading the pedestal file and and fill a map for pedestals and RMSs
     */

    std::map<int, double> m_ped_mean; // Mean value of the pedestal
    std::map<int, double> m_ped_rms; // rms of the pedestal

    //ifstream inp_ped("PedFiles/CosmicPeds.dat");
    ifstream inp_ped(Form("PedFiles/Peds_%d", run));

    if (!inp_ped.is_open()) {
        cout << "Can not find the pedestal file \"PedFiles/CosmicPeds.dat\". " << endl;
        cout << "Exiting..." << endl;
        exit(1);
    }

    cout << "Kuku" << endl;
    while (!inp_ped.eof()) {
        int ch;
        double mean, rms;
        inp_ped>>ch;
        inp_ped>>mean;
        inp_ped>>rms;

        m_ped_mean[ch] = mean;
        m_ped_rms[ch] = rms;
    }

    cout << "The pedestal map is loaded." << endl;

    try {

        while (reader.next() == true) {
            reader.read(event);

            evCounter = evCounter + 1;

            //if( evCounter > 2000 ){break;}
            if (evCounter % 1000 == 0) {
                cout.flush() << "Processed " << evCounter << " events \r";
            }

            event.getStructure(buRWellADC);
            event.getStructure(bRAWADc);
            event.getStructure(bRunConf);

            int n_uRwellADC = buRWellADC.getRows();
            int evNumber = bRunConf.getInt("event", 0);

            int n_hits = 0;

            double tmp_threshold = 10;
            bool pass_tmp_threshold = false;

            int strip_U = -1;
            int strip_V = -1;

            double Max_U_ADCRel = 0; // Maxmim relative adc among U strips
            double Max_V_ADCRel = 0; // Maxmim relative adc among V strips

            for (int i = 0; i < n_uRwellADC; i++) {
                int sector = buRWellADC.getInt("sector", i);
                int layer = buRWellADC.getInt("layer", i);
                int channel = buRWellADC.getInt("component", i);
                int ADC = buRWellADC.getInt("ADC", i);
                int uniqueChan = int(buRWellADC.getFloat("time", i));
                int ts = buRWellADC.getInt("ped", i);

                int slot = layer;

                if (sector != 6) {
                    // Those are unphysical channels, are not connected to any strip.
                    continue;
                }

                double ADC_Rel = (m_ped_mean[uniqueChan] - ADC) / m_ped_rms[uniqueChan];

                //if (ADC < m_ped_mean[uniqueChan] - sigm_threshold * m_ped_rms[uniqueChan]) {
                if (ADC_Rel > sigm_threshold) {
                    //cout << evNumber << "    " << uniqueChan << "     " << m_ped_mean[uniqueChan] << "    " << m_ped_rms[uniqueChan] << "     " << ADC << "    " << sector << "  ts = " << ts << endl;
                    n_hits = n_hits + 1;

                    if (layer == 1 /* U layer */) {
                        if (ADC_Rel > Max_U_ADCRel) {
                            Max_U_ADCRel = ADC_Rel;
                            strip_U = channel;
                        }
                    } else if (layer == 2 /* V layer */) {
                        if (ADC_Rel > Max_V_ADCRel) {
                            Max_V_ADCRel = ADC_Rel;
                            strip_V = channel;
                        }
                    }


                    if ((m_ped_mean[uniqueChan] - ADC) / m_ped_rms[uniqueChan] > tmp_threshold) {
                        pass_tmp_threshold = true;
                    }

                }
            }

            if (strip_U >= 0 && strip_V >= 0) {
                writer.addEvent(event);
            }


        }
    } catch (const char msg) {
        cerr << msg << endl;
    }

    writer.close();
    writer.showSummary();

    return 0;
}
