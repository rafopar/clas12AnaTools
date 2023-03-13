/* 
 * File:   AnalyzeSec6TOFTrg.cc
 * Author: rafopar
 *
 * Created on March 11, 2023, 8:39 PM
 */

#include <cstdlib>
#include <iostream>

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TMath.h>

// ===== Hipo headers =====
#include <reader.h>
#include <writer.h>
#include <dictionary.h>
#include <vector>

// ===== Custom classes ====
#include <RecParticle.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    char inputFile[256];

    int run = 0;
    if (argc > 1) {
        run = atoi(argv[1]);
        //sprintf(inputFile, "%s", argv[1]);
        sprintf(inputFile, "/volatile/clas12/rafopar/uRwell/Data/%d/Recon_17738_All.hipo", run, run);
        //sprintf(inputFile, "/volatile/clas12/rafopar/uRwell/Data/%d/test_recon_%d.hipo", run, run);
    } else {
        std::cout << " *** please provide a file name..." << std::endl;
        exit(0);
    }

    const int det_HTCC = 15; // HTCC detector
    const int det_ECAL = 7; // ECAL detector
    const int det_FTOF = 12; // FTOF detector
    const int layer_PCal = 1; // FTOF detector
    const int layer_ECin = 4; // FTOF detector
    const int layer_ECout = 7; // FTOF detector
    const int layer_Panel2 = 3; // FTOF detector

    hipo::reader reader;
    reader.open(inputFile);

    hipo::dictionary factory;

    reader.readDictionary(factory);

    factory.show();
    hipo::event event;
    int evCounter = 0;

    hipo::bank bRecTraj(factory.getSchema("REC::Traj"));
    hipo::bank bRecPart(factory.getSchema("REC::Particle"));
    hipo::bank bRecCalo(factory.getSchema("REC::Calorimeter"));
    hipo::bank bRecCherenkov(factory.getSchema("REC::Cherenkov"));
    
    TFile *file_out = new TFile(Form("AnalyzeSec6TofTrg_%d.root", run), "Recreate");
    TH2D h_Panel2_YXc1("h_Panel2_YXc1", "", 200, -500, 500., 200, -500., 500);
    TH1D h_nTrSec6Panel2("h_nTrSec6Panel2", "", 5, -0.5, 4.5);
    
    std::map<int, TH2D> mh_Panel2_YXc1_Pid_;
    mh_Panel2_YXc1_Pid_[211] = TH2D("mh_Panel2_YXc1_Pid_211", "", 200, -500, 500., 200, -500., 500);
    mh_Panel2_YXc1_Pid_[-211] = TH2D("mh_Panel2_YXc1_Pid_Neg211", "", 200, -500, 500., 200, -500., 500);
    mh_Panel2_YXc1_Pid_[321] = TH2D("mh_Panel2_YXc1_Pid_321", "", 200, -500, 500., 200, -500., 500);
    mh_Panel2_YXc1_Pid_[2212] = TH2D("mh_Panel2_YXc1_Pid_2212", "", 200, -500, 500., 200, -500., 500);
    mh_Panel2_YXc1_Pid_[0] = TH2D("mh_Panel2_YXc1_Pid_0", "", 200, -500, 500., 200, -500., 500);

    try {

        while (reader.next() == true) {
            reader.read(event);

            evCounter = evCounter + 1;

            if (evCounter % 1000 == 0) {
                cout.flush() << "Processed " << evCounter << " events \r";
            }

            event.getStructure(bRecTraj);
            event.getStructure(bRecPart);
            event.getStructure(bRecCalo);
            event.getStructure(bRecCherenkov);
            
            std::map<int, int> m_ind_PCal;
            std::map<int, int> m_ind_ECin;
            std::map<int, int> m_ind_ECout;
            std::map<int, int> m_ind_HTCC;
            

            int n_Cherenkov = bRecCherenkov.getRows();
            int n_Calo = bRecCalo.getRows();
            int n_Part = bRecPart.getRows();
            int nTraj = bRecTraj.getRows();
            
            for( int ich = 0; ich < n_Cherenkov; ich++ ){
                int det = bRecCherenkov.getInt("detector", ich);
                int pindex = bRecCherenkov.getInt("pindex", ich);
                int index = bRecCherenkov.getInt("index", ich);
                
                if( det == det_HTCC ){
                    m_ind_HTCC[pindex] = index;
                }
            }
            
            for( int ical = 0; ical < n_Calo; ical++ ){
                int det = bRecCalo.getInt("detector", ical);
                int layer = bRecCalo.getInt("layer", ical);
                
                if( det == det_ECAL ) {
                    int pindex = bRecCalo.getInt("pindex", ical);
                    int index = bRecCalo.getInt("index", ical);
                
                    if( layer = layer_PCal ){
                        m_ind_PCal[pindex] = index;
                    }else if ( layer = layer_ECin ){
                        m_ind_ECin[pindex] = index;
                    }else if( layer = layer_ECout ){
                        m_ind_ECout[pindex] = index;
                    }
                
                }
                
            }
            
            std::map<int, RecParticle*> m_RecPart;
            
            for( int ipart = 0; ipart < n_Part; ipart++ ){
                
                int ind_PCal = m_ind_PCal.find(ipart) == m_ind_PCal.end() ? -1 : m_ind_PCal[ipart];
                int ind_ECin = m_ind_ECin.find(ipart) == m_ind_ECin.end() ? -1 : m_ind_ECin[ipart];
                int ind_ECout = m_ind_ECout.find(ipart) == m_ind_ECout.end() ? -1 : m_ind_ECout[ipart];
                int ind_HTCC = m_ind_HTCC.find(ipart) == m_ind_HTCC.end() ? -1 : m_ind_HTCC[ipart];
                
                //m_RecPart[ipart] = RecParticle(bRecPart, bRecCalo, bRecCherenkov, ipart, ind_PCal, ind_ECin, ind_ECout, ind_HTCC );
                m_RecPart[ipart] = new RecParticle(bRecPart, bRecCalo, bRecCherenkov, ipart, ind_PCal, ind_ECin, ind_ECout, ind_HTCC );
            }

            int nTrSec6 = 0;
            
            for (int itr = 0; itr < nTraj; itr++) {
                int detector = bRecTraj.getInt("detector", itr);
                int layer = bRecTraj.getInt("layer", itr);

                if ( !(detector == det_FTOF && layer == layer_Panel2)  ) {
                    continue;
                }

                int pindex = bRecTraj.getInt("pindex", itr);
                double x = double(bRecTraj.getFloat("x", itr));
                double y = double(bRecTraj.getFloat("y", itr));
                double z = double(bRecTraj.getFloat("z", itr));
                
                double phi = atan2(y, x)*TMath::RadToDeg() + 30;
                phi = phi < 0 ? phi + 360. : phi;
                int sec = phi/60;
                
                h_Panel2_YXc1.Fill(-x, y);
                
                if( sec == 5 ){
                    nTrSec6 = nTrSec6 + 1;                    
                }
                //cout<<m_RecPart[pindex]->pid()<<endl;
                int pid = m_RecPart[pindex]->pid();
                if (mh_Panel2_YXc1_Pid_.find(pid) != mh_Panel2_YXc1_Pid_.end()) {
                    mh_Panel2_YXc1_Pid_[pid].Fill(-x, y);
                }
            }
            
            h_nTrSec6Panel2.Fill(nTrSec6);
            
            
            // Clean up the content of the map
            for( std::map<int, RecParticle*>::iterator it = m_RecPart.begin(); it != m_RecPart.end(); it++ ){
                delete it->second;
            }

        }
    } catch (const char msg) {
        cerr << msg << endl;
    }

    gDirectory->Write();
    file_out->Close();

    return 0;
}
