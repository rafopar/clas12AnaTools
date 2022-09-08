/* 
 * File:   CLAS12LUND2Root.cc
 * Author: rafopar
 *
 * Created on September 2, 2022, 3:58 PM
 */

#include <fstream>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>
#include <vector>

using namespace std;

/**
 * A very simple program that converts LUND (designed for GEMC simulations) to root
 * Variables of the format is in https://gemc.jlab.org/gemc/html/documentation/generator/lund.html?highlight=lund%20format
 */

void HelpPrint();

/*
 * 
 */
int main(int argc, char** argv) {

    if (argc < 2) {
        cout << "You didn't provide any argument" << endl;
        HelpPrint();
        exit(1);
    }


    const int nHeader = 10;
    const int npartColumns = 14;

    std::string inpFilePattern = "";
    std::string outputfileName = "";

    vector<std::string> v_fInpNames; // InputFile names

    for (int i = 1; i < argc; i++) {
        std::string curArg = argv[i];

        if (curArg.compare("-i") == 0) {
            if (argc < i + 2) {
                cout << "Input file name is missing " << endl;
                HelpPrint();
                exit(1);
            }

            std::string nextArg = argv[i + 1];

            while (nextArg.compare(0, 1, "-") != 0) {
                v_fInpNames.push_back(nextArg);
                i = i + 1;

                if (argc < i + 2) {
                    break;
                } else {
                    nextArg = argv[i + 1];
                }
            }
        } else if (curArg.compare("-o") == 0) {
            if (argc < i + 2) {
                cout << "Output file name is missing " << endl;
                HelpPrint();
                exit(1);
            }
            outputfileName = argv[i + 1];
            i = i + 1;
        } else if (curArg.compare("-h") == 0 || curArg.compare("--help") == 0) {
            HelpPrint();
            exit(0);
        } else if (curArg.compare(0, 1, "-") == 0) {
            cout << "The argument " << curArg.c_str() << " is not recognized" << endl;
            HelpPrint();
            exit(1);
        }
    }


    TFile *file_out = new TFile(outputfileName.c_str(), "Recreate");
    TTree *tr1 = new TTree("tr1", "LUND tree");

    int nPart;
    double A_Targ;
    int Z_Targ;
    double pol_targ;
    double pol_beam;
    int beamType; // 11 Electron, 22 Photon
    double Eb;
    int InterNuclID; // Interacted Nucleon ID
    int ProcessID;
    double EvWeight;

    const int nMax = 50;

    int index[nMax];
    double t_live[nMax]; // lifetime [ns]
    int type[nMax]; // 1=active
    int pid[nMax];
    int parentInd[nMax];
    int daughtInd[nMax];
    double px[nMax];
    double py[nMax];
    double pz[nMax];
    double E[nMax];
    double m[nMax];
    double vx[nMax];
    double vy[nMax];
    double vz[nMax];

    TRandom *rand = new TRandom();

    tr1->Branch("nPart", &nPart, "nPart/I");
    tr1->Branch("A_Targ", &A_Targ, "A_Targ/I");
    tr1->Branch("Z_Targ", &Z_Targ, "Z_Targ/I");
    tr1->Branch("pol_targ", &pol_targ, "pol_targ/D");
    tr1->Branch("pol_beam", &pol_beam, "pol_beam/D");
    tr1->Branch("beamType", &beamType, "beamType/I");
    tr1->Branch("Eb", &Eb, "Eb/D");
    tr1->Branch("InterNuclID", &InterNuclID, "InterNuclID/I");
    tr1->Branch("ProcessID", &ProcessID, "ProcessID/I");
    tr1->Branch("EvWeight", &EvWeight, "EvWeight/D");
    tr1->Branch("index", &index, "index[nPart]/I");
    tr1->Branch("t_live", &t_live, "t_live[nPart]/D");
    tr1->Branch("type", &type, "type[nPart]/I");
    tr1->Branch("pid", &pid, "pid[nPart]/I");
    tr1->Branch("parentInd", &parentInd, "parentInd[nPart]/I");
    tr1->Branch("daughtInd", &daughtInd, "daughtInd[nPart]/I");
    tr1->Branch("px", &px, "px[nPart]/D");
    tr1->Branch("py", &py, "py[nPart]/D");
    tr1->Branch("pz", &pz, "pz[nPart]/D");
    tr1->Branch("E", &E, "E[nPart]/D");
    tr1->Branch("m", &m, "m[nPart]/D");
    tr1->Branch("vx", &vx, "vx[nPart]/D");
    tr1->Branch("vy", &vy, "vy[nPart]/D");
    tr1->Branch("vz", &vz, "vz[nPart]/D");

    for (auto curfName : v_fInpNames) {

        ifstream cur_file(curfName.c_str());
        
        cout<<curfName<<endl;

        if (cur_file.is_open()) {

            std::string line;

            while (std::getline(cur_file, line)) {

                std::vector<std::string> result;
                std::istringstream iss(line);

                for (std::string s; iss >> s;) {
                    result.push_back(s);
                }

                if (result.size() == nHeader) {
                    
                    nPart = atoi(result.at(0).c_str());
                    A_Targ = atoi(result.at(1).c_str());
                    Z_Targ = atoi(result.at(2).c_str());
                    pol_targ = atof(result.at(3).c_str());
                    pol_beam = atof(result.at(4).c_str());
                    beamType = atoi(result.at(5).c_str());
                    Eb = atof(result.at(6).c_str());
                    InterNuclID = atoi(result.at(7).c_str());
                    ProcessID = atoi(result.at(8).c_str());
                    EvWeight = atof(result.at(9).c_str());

                    for (int ipart = 0; ipart < nPart; ipart++) {
                        result.clear();
                        result.shrink_to_fit();

                        std::getline(cur_file, line);
                        
                        std::istringstream iss(line);

                        for (std::string s; iss >> s;) {
                            result.push_back(s);
                        }

                        index[ipart] = atoi(result.at(0).c_str());
                        t_live[ipart] = atof(result.at(1).c_str());
                        type[ipart] = atoi(result.at(2).c_str());
                        pid[ipart] = atoi(result.at(3).c_str());
                        parentInd[ipart] = atoi(result.at(4).c_str());
                        daughtInd[ipart] = atoi(result.at(5).c_str());
                        px[ipart] = atof(result.at(6).c_str());
                        py[ipart] = atof(result.at(7).c_str());
                        pz[ipart] = atof(result.at(8).c_str());
                        E[ipart] = atof(result.at(9).c_str());
                        m[ipart] = atof(result.at(10).c_str());
                        vx[ipart] = atof(result.at(11).c_str());
                        vy[ipart] = atof(result.at(12).c_str());
                        vz[ipart] = atof(result.at(13).c_str());                                                
                    }
                    
                    tr1->Fill();
                }

            }

        }

        cur_file.close();
    }

    tr1->Write();
    file_out->Close();
    return 0;
}

void HelpPrint() {

    cout << "Usage: " << "CLAS12LUND2Root -i InputFileName -o OutputFileName " << endl;
    cout << endl;
    cout << " ===== Options ==== " << endl;

    cout << "-i: Input File Name: Accepts wildcards" << endl;
    cout << "-o: Output File Name" << endl;
    cout << "-h: print this command" << endl;
}