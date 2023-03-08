/* 
 * File:   AnalyzeData.cc
 * Author: rafopar
 *
 * Created on March 3, 2023, 5:46 PM
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
    if (argc > 1) {
        run = atoi(argv[1]);
        //sprintf(inputFile, "%s", argv[1]);
        sprintf(inputFile, "Data/Skim_Cross_%d_All.hipo", run);
        //sprintf(inputFile, "Data/decoded_%d.hipo", run);
        sprintf(outputFile, "outSkim.hipo");
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

    const double sigm_threshold = 5.; // Represents the threshold of the ADC in units of the sigma.
    const int crateID_test3 = 40; // the fADC is on ROC 40
    const int slot_fADC = 3; // and the slot is 3
    const int n_ts = 6;
    
    const int nCutLvls = 5;
    
    double ADC_Thresholds[nCutLvls] = {5., 5.5, 6., 6.5, 7.};

    /**
     * Definition of some geometric constants
     */

    const double strip_alpha = 10. * TMath::DegToRad(); // The strip angle in radians
    const double Y_0 = 250 + 723 * tan(strip_alpha);
    const double pitch = 1.; // mm

    TFile *file_out = new TFile(Form("AnalyzeData_%d.root", run), "Recreate");
    TH1D h_NHits1("h_NHits1", "", 36, -0.5, 35.5);
    TH2D h_ADCRel_chan("h_ADCRel_chan", "", 1711, -0.5, 1710.5, 200, 0, 20);

    TH2D h_RelADC_VU1("h_RelADC_VU1", "", 200, 0., 25, 200, 0., 25);
    TH2D h_AbsADC_VU1("h_AbsADC_VU1", "", 200, 0., 1000., 200, 0., 1000.);
    TH2D h_Cross_YXc1("h_Cross_YXc1", "", 200, -900., 900., 200, -500., 500.);
    TH2D h_Cross_YXC_CutLvl_[nCutLvls];
    
    for( int i = 0; i < nCutLvls; i++ ){
        h_Cross_YXC_CutLvl_[i] = TH2D(Form("h_Cross_YXC_CutLvl_%d", i), "", 200, -900., 900., 200, -500., 500.);
    }

    /**
     * Reading the pedestal file and and fill a map for pedestals and RMSs
     */

    std::map<int, double> m_ped_mean; // Mean value of the pedestal
    std::map<int, double> m_ped_rms; // rms of the pedestal

    ifstream inp_ped("PedFiles/CosmicPeds.dat");

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

    ofstream out_EvNumbers(Form("Data/Skim_EvNumbers_%d.dat", run));

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
                    h_ADCRel_chan.Fill(uniqueChan, (m_ped_mean[uniqueChan] - ADC) / m_ped_rms[uniqueChan]);                    
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
                double crs_x = pitch * (strip_U - strip_V) / (2 * sin(strip_alpha));
                double crs_y = tan(strip_alpha) * crs_x + Y_0 - (strip_U * pitch) / cos(strip_alpha);
                h_Cross_YXc1.Fill(crs_x, crs_y);

                h_RelADC_VU1.Fill(Max_U_ADCRel, Max_V_ADCRel);
                double Max_AbsADC_U = Max_U_ADCRel*m_ped_rms[strip_U];
                double Max_AbsADC_V = Max_V_ADCRel*m_ped_rms[strip_V];
                h_AbsADC_VU1.Fill(Max_AbsADC_U, Max_AbsADC_V);
                
                for( int iCutLvl = 0; iCutLvl < nCutLvls; iCutLvl++ ){
                    if( Max_U_ADCRel > ADC_Thresholds[iCutLvl] && Max_V_ADCRel > ADC_Thresholds[iCutLvl] ){
                        h_Cross_YXC_CutLvl_[iCutLvl].Fill(crs_x, crs_y);
                    }
                }
            }
            
            h_NHits1.Fill(n_hits);

            if (pass_tmp_threshold) {
                out_EvNumbers << evNumber << endl;
            }

        }
    } catch (const char msg) {
        cerr << msg << endl;
    }

    gDirectory->Write();
    file_out->Close();
    out_EvNumbers.close();

    return 0;
}