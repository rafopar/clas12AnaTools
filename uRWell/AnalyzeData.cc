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
    int fnum = -1;
    if (argc > 2) {
        run = atoi(argv[1]);
        fnum = atoi(argv[2]);
        //sprintf(inputFile, "%s", argv[1]);
        sprintf(inputFile, "Data/Skim_Cross_%d_%d.hipo", run, fnum);
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
    const int n_ts = 12;

    const int nCutLvls = 5;

    double ADC_Thresholds[nCutLvls] = {5., 5.5, 6., 6.5, 7.};

    /**
     * Definition of some geometric constants
     */

    const double strip_alpha = 10. * TMath::DegToRad(); // The strip angle in radians
    const double Y_0 = 250 + 723 * tan(strip_alpha);
    const double pitch = 1.; // mm

    TFile *file_out = new TFile(Form("AnalyzeData_%d_%d.root", run, fnum), "Recreate");
    TH1D h_NHits1("h_NHits1", "", 36, -0.5, 35.5);
    TH2D h_ADCRel_chan("h_ADCRel_chan", "", 1711, -0.5, 1710.5, 200, 0, 20);

    TH2D h_RelADC_VU1("h_RelADC_VU1", "", 200, 0., 25, 200, 0., 25);
    TH2D h_AbsADC_VU1("h_AbsADC_VU1", "", 200, 0., 1000., 200, 0., 1000.);
    TH2D h_Cross_YXc1("h_Cross_YXc1", "", 200, -900., 900., 200, -500., 500.);
    TH2D h_Cross_YXC_CutLvl_[nCutLvls];
    TH2D h_N_UV_Hits_CutLvl_[nCutLvls];
    TH2D h_ts_VU_CutLvl_[nCutLvls];
    TH2D h_ADCRel_ts_U_CutLvl_[nCutLvls];
    TH2D h_ADCRel_ts_V_CutLvl_[nCutLvls];
    TH2D h_ADC_ts_U_CutLvl_[nCutLvls];
    TH2D h_ADC_ts_V_CutLvl_[nCutLvls];

    for (int i = 0; i < nCutLvls; i++) {
        h_Cross_YXC_CutLvl_[i] = TH2D(Form("h_Cross_YXC_CutLvl_%d", i), "", 200, -900., 900., 200, -500., 500.);
        h_N_UV_Hits_CutLvl_[i] = TH2D(Form("h_N_UV_Hits_CutLvl_%d", i), "", 26, -0.5, 25.5, 26, -0.5, 25.5);
        h_ts_VU_CutLvl_[i] = TH2D(Form("h_ts_VU_CutLvl_%d", i), "", n_ts+1, -0.5, n_ts + 0.5, n_ts + 1, -0.5, n_ts + 0.5);
        h_ADCRel_ts_V_CutLvl_[i] = TH2D(Form("h_ADCRel_ts_V_CutLvl_%d", i), "", n_ts + 1, -0.5, n_ts  + 0.5, 25, 0., 35);
        h_ADCRel_ts_U_CutLvl_[i] = TH2D(Form("h_ADCRel_ts_U_CutLvl_%d", i), "", n_ts + 1, -0.5, n_ts + 0.5, 25, 0., 35);
        h_ADC_ts_V_CutLvl_[i] = TH2D(Form("h_ADC_ts_V_CutLvl_%d", i), "", n_ts + 1, -0.5, n_ts + 0.5, 25, 0., 1500);
        h_ADC_ts_U_CutLvl_[i] = TH2D(Form("h_ADC_ts_U_CutLvl_%d", i), "", n_ts + 1, -0.5, n_ts + 0.5, 25, 0., 1500);
    }

    /**
     * Reading the pedestal file and and fill a map for pedestals and RMSs
     */

    std::map<int, double> m_ped_mean; // Mean value of the pedestal
    std::map<int, double> m_ped_rms; // rms of the pedestal

    //ifstream inp_ped("PedFiles/CosmicPeds.dat");
    //    ifstream inp_ped("PedFiles/HallPeds.dat");
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

    ofstream out_EvNumbers(Form("Data/Skim_EvNumbers_%d_%d.dat", run, fnum));

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
            int nUhits[nCutLvls] = {0};
            int nVhits[nCutLvls] = {0};

            double tmp_threshold = 6.5;
            bool pass_tmp_threshold = false;

            int strip_U = -1;
            int strip_V = -1;
            int ts_U = -1;
            int ts_V = -1;

            double Max_U_ADCRel = 0; // Maxmim relative adc among U strips
            double Max_V_ADCRel = 0; // Maxmim relative adc among V strips

            map<int, vector<double> > mv_UADCRel;
            map<int, vector<double> > mv_UADC;
            map<int, vector<double> > mv_UTS;
            map<int, vector<double> > mv_VADCRel;
            map<int, vector<double> > mv_VADC;
            map<int, vector<double> > mv_VTS;

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

                //cout<<"ADC = "<<ADC<<"    ped_mean = "<<m_ped_mean[uniqueChan]<<"    ped_rms = "<<m_ped_rms[uniqueChan]<<"   ADC_REL = "<<ADC_Rel<<endl;

                //if (ADC < m_ped_mean[uniqueChan] - sigm_threshold * m_ped_rms[uniqueChan]) {
                if (ADC_Rel > sigm_threshold) {
                    //cout << evNumber << "    " << uniqueChan << "     " << m_ped_mean[uniqueChan] << "    " << m_ped_rms[uniqueChan] << "     " << ADC << "    " << sector << "  ts = " << ts << endl;
                    h_ADCRel_chan.Fill(uniqueChan, (m_ped_mean[uniqueChan] - ADC) / m_ped_rms[uniqueChan]);
                    n_hits = n_hits + 1;

                    for (int iCut = 0; iCut < nCutLvls; iCut++) {

                        if (layer == 1) {
                            if (ADC_Rel > ADC_Thresholds[iCut]) {
                                nUhits[iCut] = nUhits[iCut] + 1;
                            }
                        } else if (layer == 2) {
                            if (ADC_Rel > ADC_Thresholds[iCut]) {
                                nVhits[iCut] = nVhits[iCut] + 1;
                            }
                        }
                    }

                    if (layer == 1 /* U layer */) {
                        mv_UADCRel[channel].push_back(ADC_Rel);
                        mv_UADC[channel].push_back(m_ped_mean[uniqueChan] - ADC);
                        mv_UTS[channel].push_back(ts);
                        if (ADC_Rel > Max_U_ADCRel) {
                            Max_U_ADCRel = ADC_Rel;
                            strip_U = channel;
                            ts_U = ts;
                        }
                    } else if (layer == 2 /* V layer */) {
                        mv_VADCRel[channel].push_back(ADC_Rel);
                        mv_VADC[channel].push_back(m_ped_mean[uniqueChan] - ADC);
                        mv_VTS[channel].push_back(ts);
                        if (ADC_Rel > Max_V_ADCRel) {
                            Max_V_ADCRel = ADC_Rel;
                            strip_V = channel;
                            ts_V = ts;
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
                double Max_AbsADC_U = Max_U_ADCRel * m_ped_rms[strip_U];
                double Max_AbsADC_V = Max_V_ADCRel * m_ped_rms[strip_V];
                h_AbsADC_VU1.Fill(Max_AbsADC_U, Max_AbsADC_V);

                for (int iCutLvl = 0; iCutLvl < nCutLvls; iCutLvl++) {
                    if (Max_U_ADCRel > ADC_Thresholds[iCutLvl] && Max_V_ADCRel > ADC_Thresholds[iCutLvl]) {
                        h_Cross_YXC_CutLvl_[iCutLvl].Fill(crs_x, crs_y);
                        h_ts_VU_CutLvl_[iCutLvl].Fill(ts_U, ts_V);
                        for( int iU = 0; iU < mv_UADC[strip_U].size(); iU++ ){
                            h_ADCRel_ts_U_CutLvl_[iCutLvl].Fill(mv_UTS[strip_U].at(iU), mv_UADCRel[strip_U].at(iU) );
                            h_ADC_ts_U_CutLvl_[iCutLvl].Fill(mv_UTS[strip_U].at(iU), mv_UADC[strip_U].at(iU) );
                        }
                        for( int iV = 0; iV < mv_VADC[strip_V].size(); iV++ ){
                            //cout<<mv_VADC[strip_V].at(iV)<<endl;
                            h_ADCRel_ts_V_CutLvl_[iCutLvl].Fill(mv_VTS[strip_V].at(iV), mv_VADCRel[strip_V].at(iV) );
                            h_ADC_ts_V_CutLvl_[iCutLvl].Fill(mv_VTS[strip_V].at(iV), mv_VADC[strip_V].at(iV) );
                        }
                    }
                }
            }


            for (int iCutLvl = 0; iCutLvl < nCutLvls; iCutLvl++) {
                h_N_UV_Hits_CutLvl_[iCutLvl].Fill(nUhits[iCutLvl], nVhits[iCutLvl]);
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
