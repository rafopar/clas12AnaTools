/* 
 * File:   AnaDDVCS.cc
 * Author: rafopar
 *
 * Created on August 17, 2023, 8:40 PM
 */

#include <cstdlib>
#include <iostream>

#include <TF1.h>
#include <TKey.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TMath.h>
#include <TROOT.h>
#include <TChain.h>
#include <TRandom.h>
#include <TVector3.h>
#include <TLorentzVector.h>

#include <RecParticle.h>

// ===== Hipo headers =====
#include <reader.h>
#include <writer.h>
#include <dictionary.h>
#include <vector>

using namespace std;

bool emAcc(TLorentzVector&);
bool mumAcc(TLorentzVector&);
bool mupAcc(TLorentzVector&);
bool protAcc(TLorentzVector&);

TF1 *f_mumAccThP;

/*
 * 
 */
int main(int argc, char** argv) {

    char inputFile[256];

    int job_id = -1;

    if (argc < 2) {
        cerr << "You should provide the job_ID. Exiting" << endl;
        exit(1);
    }

    job_id = atoi(argv[1]);

    sprintf(inputFile, "/volatile/clas12/rafopar/OSG_Validation/Merged_Outputs/dst_%d.hipo", job_id);  // <---- Change to /volatile/clas12/ungaro/OSG_Validation/Merged_Outputs/dst_%d.hipo

    f_mumAccThP = new TF1("f_mumAccThP", "[0] + [1]/(x-[2])", 0., 25);
    f_mumAccThP->SetParameters(5.07868, 18.1913, -0.120759);

    const double Hist_Pmax = 11.;
    const double Eb = 10.2;
    const int nMax_samePID = 20;
    const double Mp = 0.9383;
    const double chi2PID_Prot_Max = 10.;
    const double chi2PID_Prot_Min = -10.;
    const int HTCC_TYPE = 15;
    const int DET_FTOF = 12;
    const int layer1b = 2; // panel 1a = 1, panely2 = 3
    const double HTCC_MomThr = 4.8;
    const double PCalEmin = 0.07;
    const double PCalVmin = 0.07;
    const double PCalWmin = 0.07;
    const double vzMax = 1.;
    const double vzMin = -8.;
    const double v_dtCut = 2.5;
    const double Mmis_Max = 1.2;
    const double Mmis_Min = 0.8;

    const double light_Speed = 29.9792458;


    TLorentzVector L_beam;
    L_beam.SetPxPyPzE(0., 0., Eb, Eb);
    TLorentzVector L_targ;
    L_targ.SetPxPyPzE(0., 0., 0., Mp);

    TObjArray Hlist(0);

    TFile *file_out = new TFile(Form("Hists_OSGValidate_Job_%d.root", job_id), "Recreate");
    TH1D h_nphe_em1("h_nphe_em1", "", 200, 0., 35.);
    TH1D h_nphe_ep1("h_nphe_ep1", "", 200, 0., 35.);

    TH1D h_vz_em1("h_vz_em1", "", 200, -25., 35.);
    TH1D h_vz_ep1("h_vz_ep1", "", 200, -25., 35.);
    TH1D h_vz_prot1("h_vz_prot1", "", 200, -25., 35.);
    TH1D h_chi2PID_em1("h_chi2PID_em1", "", 400, -200, 200.);
    TH1D h_chi2PID_ep1("h_chi2PID_ep1", "", 400, -200, 200.);
    TH1D h_chi2PID_prot1("h_chi2PID_prot1", "", 400, -200, 200.);

    TH2D h_th_P_em1("h_th_P_em1", "", 200, 0., 1.2 * Hist_Pmax, 200, 0., 55.);
    TH2D h_th_P_ep1("h_th_P_ep1", "", 200, 0., 1.2 * Hist_Pmax, 200, 0., 55.);
    TH2D h_th_P_prot1("h_th_P_prot1", "", 200, 0., 1.2 * Hist_Pmax, 200, 0., 55.);

    TH2D h_beta_P_prot1("h_beta_P_prot1", "", 200, 0., 1.2 * Hist_Pmax, 200, 0., 1.5);
    TH1D h_E_PCal_em1("h_E_PCal_em1", "", 200, 0., 1.);
    TH1D h_E_PCal_ep1("h_E_PCal_ep1", "", 200, 0., 1.);

    TH1D h_lUPCal_em1("h_lUPCal_em1", "", 200, 0., 450.);
    TH1D h_lVPCal_em1("h_lVPCal_em1", "", 200, 0., 450.);
    TH1D h_lWPCal_em1("h_lWPCal_em1", "", 200, 0., 450.);
    TH1D h_lUPCal_ep1("h_lUPCal_ep1", "", 200, 0., 450.);
    TH1D h_lVPCal_ep1("h_lVPCal_ep1", "", 200, 0., 450.);
    TH1D h_lWPCal_ep1("h_lWPCal_ep1", "", 200, 0., 450.);
    TH2D h_SF_PCal_ECin_ep1("h_SF_PCal_ECin_ep1", "", 200, 0., 0.3, 200, 0., 0.3);
    TH2D h_SF_PCal_ECin_ep2("h_SF_PCal_ECin_ep2", "", 200, 0., 0.3, 200, 0., 0.3);
    TH2D h_n_emep1("h_n_emep1", "", 11, -0.5, 10.5, 11, -0.5, 10.5);
    TH1D h_n_prot1("h_n_prot1", "", 11, -0.5, 10.5);
    TH1D h_n_charged1("h_n_charged1", "", 11, -0.5, 10.5);


    TH1D h_Mmis1("h_Mmis1", "", 200, 0., 4.);
    Hlist.Add(&h_Mmis1);
    TH1D h_Mmis2("h_Mmis2", "", 200, 0., 4.);
    Hlist.Add(&h_Mmis2);
    TH1D h_Mmis3("h_Mmis3", "", 200, 0., 4.);
    Hlist.Add(&h_Mmis3);
    TH1D h_Mmis4("h_Mmis4", "", 200, 0., 4.);
    Hlist.Add(&h_Mmis4);
    TH2D h_Minv12_1("h_Minv12_1", "", 200, 0., 2.5, 200, 0., 2.5);
    Hlist.Add(&h_Minv12_1);
    TH2D h_Minv12_2("h_Minv12_2", "", 200, 0., 2.5, 200, 0., 2.5);
    Hlist.Add(&h_Minv12_2);
    TH1D h_vt_Diff_em1("h_vt_Diff_em1", "", 200, -0.5, 0.5);
    Hlist.Add(&h_vt_Diff_em1);
    TH2D h_vt_Diff_emep1("h_vt_Diff_emep1", "", 200, -2., 2., 200, -2., 2.);
    Hlist.Add(&h_vt_Diff_emep1);
    TH2D h_vz_Diff_emep1("h_vz_Diff_emep1", "", 200, -10., 10., 200, -10., 10.);
    Hlist.Add(&h_vz_Diff_emep1);
    TH1D h_vt_Diff_em2("h_vt_Diff_em2", "", 200, -18.5, 18.5);
    Hlist.Add(&h_vt_Diff_em2);
    TH1D h_vt_Diff_em1_ep("h_vt_Diff_em1_ep", "", 200, -18.5, 18.5);
    Hlist.Add(&h_vt_Diff_em1_ep);
    TH1D h_vt_Diff_em2_ep("h_vt_Diff_em2_ep", "", 200, -18.5, 18.5);
    Hlist.Add(&h_vt_Diff_em2_ep);

    TH2D h_MC_Memep_12_1("h_MC_Memep_12_1", "", 200, 0., 3., 200, 0., 3.);
    Hlist.Add(&h_MC_Memep_12_1);

    hipo::reader reader;
    reader.open(inputFile);

    hipo::dictionary factory;

    reader.readDictionary(factory);

    factory.show();

    hipo::event event;

    int evCounter = 0;

    hipo::bank bRunConf(factory.getSchema("RUN::config"));
    hipo::bank bRecPart(factory.getSchema("REC::Particle"));
    hipo::bank bRecCalo(factory.getSchema("REC::Calorimeter"));
    hipo::bank bRecCC(factory.getSchema("REC::Cherenkov"));
    hipo::bank bRecSC(factory.getSchema("REC::Scintillator"));
    hipo::bank bRecEV(factory.getSchema("REC::Event"));
    hipo::bank bMCPart(factory.getSchema("MC::Particle"));

    int ind_em[nMax_samePID];
    int ind_ep[nMax_samePID];
    int ind_prot[nMax_samePID];
    int sec_em[nMax_samePID];
    int sec_ep[nMax_samePID];

    try {

        while (reader.next() == true) {
            reader.read(event);

            evCounter = evCounter + 1;

            //            if (evCounter > 15500) {
            //                break;
            //            }
            if (evCounter % 10000 == 0) {
                cout.flush() << "Processed " << evCounter << " events \r";
            }

            event.getStructure(bRecPart);
            event.getStructure(bRecCalo);
            event.getStructure(bRecCC);
            event.getStructure(bRecSC);
            event.getStructure(bRecEV);
            event.getStructure(bRunConf);

            map<int, int> ind_HTCC;
            map<int, int> ind_PCal;
            map<int, int> ind_ECin;
            map<int, int> ind_ECout;
            map<int, int> ind_FTOF;

            int nPart = bRecPart.getRows();
            for (int i_part = 0; i_part < nPart; i_part++) {

                // ==== Before assigning, index, all indexes are initialized to -1, this way we can check, whether
                // ==== that particular detector is related to the particle "i_part"
                ind_HTCC[i_part] = -1;
                ind_PCal[i_part] = -1;
                ind_ECin[i_part] = -1;
                ind_ECout[i_part] = -1;
                ind_FTOF[i_part] = -1;

                int nCherenkov = bRecCC.getRows();
                // =============================== HTCC ===============================
                for (int i_cc = 0; i_cc < nCherenkov; i_cc++) {

                    // Want only HTCC for now
                    if (bRecCC.getInt("detector", i_cc) == HTCC_TYPE) {

                        if (bRecCC.getInt("pindex", i_cc) == i_part) {
                            ind_HTCC[i_part] = i_cc;
                        }
                    }
                }

                int nTOF = bRecSC.getRows();

                // ===================== FTOF ========================
                for (int itof = 0; itof < nTOF; itof++) {

                    if (bRecSC.getInt("detector", itof) == DET_FTOF && bRecSC.getInt("layer", itof) == layer1b) {

                        if (bRecSC.getInt("pindex", itof) == i_part) {
                            ind_FTOF[i_part] = itof;
                        }
                    }


                }

                int nCal = bRecCalo.getRows();

                // =============================== PCal, ECin, ECout ===============================
                for (int i_cal = 0; i_cal < nCal; i_cal++) {

                    if (bRecCalo.getInt("pindex", i_cal) == i_part) {

                        int layer = bRecCalo.getInt("layer", i_cal);

                        if (layer == 1) {
                            ind_PCal[i_part] = i_cal;
                        } else if (layer == 4) {
                            ind_ECin[i_part] = i_cal;
                        } else if (layer == 7) {
                            ind_ECout[i_part] = i_cal;
                        }
                    }

                }

            }

            int n_em = 0;
            int n_ep = 0;
            int n_prot = 0;
            int n_charged = 0;


            for (int ipart = 0; ipart < nPart; ipart++) {
                RecParticle recp(bRecPart, bRecCalo, bRecCC, ipart, ind_PCal[ipart], ind_ECin[ipart], ind_ECout[ipart], ind_HTCC[ipart]);

                if (recp.pid() == 11 && TMath::Abs(recp.status()) >= 2000 && TMath::Abs(recp.status()) < 4000) {

                    h_vz_em1.Fill(recp.vz());
                    h_chi2PID_em1.Fill(recp.chi2pid());
                    h_nphe_em1.Fill(recp.npheHTCC());

                    h_th_P_em1.Fill(recp.p(), recp.th());
                    h_E_PCal_em1.Fill(recp.energyPCal());

                    h_lUPCal_em1.Fill(recp.luPCal());
                    h_lVPCal_em1.Fill(recp.lvPCal());
                    h_lWPCal_em1.Fill(recp.lwPCal());

                    bool isPCalEmin = recp.energyPCal() > PCalEmin;
                    bool isPCalVmin = recp.lvPCal() > PCalVmin;
                    bool isPCalWmin = recp.lvPCal() > PCalWmin;
                    bool isVz = recp.vz() > vzMin && recp.vz() < vzMax;

                    if (isPCalEmin && isPCalVmin && isPCalWmin && isVz) {
                        ind_em[n_em] = ipart;
                        sec_em[n_em] = recp.phi() / 60;
                        n_em = n_em + 1;
                    }

                } else if (recp.pid() == -11 && TMath::Abs(recp.status()) >= 2000 && TMath::Abs(recp.status()) < 4000) {
                    h_vz_ep1.Fill(recp.vz());
                    h_chi2PID_ep1.Fill(recp.chi2pid());
                    h_th_P_ep1.Fill(recp.p(), recp.th());
                    h_E_PCal_ep1.Fill(recp.energyPCal());

                    h_nphe_ep1.Fill(recp.npheHTCC());

                    h_lUPCal_ep1.Fill(recp.luPCal());
                    h_lVPCal_ep1.Fill(recp.lvPCal());
                    h_lWPCal_ep1.Fill(recp.lwPCal());

                    h_SF_PCal_ECin_ep1.Fill(recp.SFPCal(), recp.SFECin());

                    if (recp.p() > HTCC_MomThr) {
                        h_SF_PCal_ECin_ep2.Fill(recp.SFPCal(), recp.SFECin());
                    }

                    bool isPCalEmin = recp.energyPCal() > PCalEmin;
                    bool isPCalVmin = recp.lvPCal() > PCalVmin;
                    bool isPCalWmin = recp.lvPCal() > PCalWmin;

                    if (((recp.energyECin() < 0.001 && recp.SFPCal() > 0.15) || (recp.energyECin() >= 0.001 && (recp.SFPCal() + recp.SFECin()) > 0.195)) && isPCalEmin && isPCalVmin && isPCalWmin) {
                        ind_ep[n_ep] = ipart;
                        sec_ep[n_ep] = recp.phi() / 60;
                        n_ep = n_ep + 1;
                    }

                } else if (recp.pid() == 2212) {

                    h_vz_prot1.Fill(recp.vz());
                    h_chi2PID_prot1.Fill(recp.chi2pid());
                    h_th_P_prot1.Fill(recp.p(), recp.th());
                    h_beta_P_prot1.Fill(recp.p(), recp.beta());

                    if (recp.chi2pid() > chi2PID_Prot_Min && recp.chi2pid() < chi2PID_Prot_Max) {
                        ind_prot[n_prot] = ipart;
                        n_prot = n_prot + 1;
                    }
                }

            }

            if (n_em == 2 && n_ep == 1) {
                TLorentzVector L_em1, L_em2, L_ep;

                RecParticle part_em1(bRecPart, bRecCalo, bRecCC, ind_em[0], ind_PCal[ind_em[0]], ind_ECin[ind_em[0]], ind_ECout[ind_em[0]], ind_HTCC[ind_em[0]]);

                RecParticle part_em2(bRecPart, bRecCalo, bRecCC, ind_em[1], ind_PCal[ind_em[1]], ind_ECin[ind_em[1]], ind_ECout[ind_em[1]], ind_HTCC[ind_em[1]]);
                RecParticle part_ep(bRecPart, bRecCalo, bRecCC, ind_ep[0], ind_PCal[ind_ep[0]], ind_ECin[ind_ep[0]], ind_ECout[ind_ep[0]], ind_HTCC[ind_ep[0]]);

                double v_t_em2 = bRecSC.getFloat("time", ind_FTOF[ ind_em[1] ]) - bRecSC.getFloat("path", ind_FTOF[ ind_em[1] ]) / light_Speed;
                double v_t_ep = bRecSC.getFloat("time", ind_FTOF[ ind_ep[0] ]) - bRecSC.getFloat("path", ind_FTOF[ ind_ep[0] ]) / light_Speed;


                L_em1.SetPxPyPzE(part_em1.px(), part_em1.py(), part_em1.pz(), part_em1.p());
                L_em2.SetPxPyPzE(part_em2.px(), part_em2.py(), part_em2.pz(), part_em2.p());
                L_ep.SetPxPyPzE(part_ep.px(), part_ep.py(), part_ep.pz(), part_ep.p());

                TLorentzVector L_mis = L_beam + L_targ - (L_em1 + L_em2 + L_ep);
                TLorentzVector L_emep1 = L_em1 + L_ep;
                TLorentzVector L_emep2 = L_em2 + L_ep;

                double m_emep1 = L_emep1.M();
                double m_emep2 = L_emep2.M();

                h_Mmis1.Fill(L_mis.M());
                h_Minv12_1.Fill(m_emep1, m_emep2);

                if (L_mis.M() > Mmis_Min && L_mis.M() < Mmis_Max) {
                    h_Minv12_2.Fill(m_emep1, m_emep2);
                }

                h_vt_Diff_em1.Fill(part_em1.vt() - part_em2.vt());
                h_vt_Diff_em2.Fill(part_em1.vt() - v_t_em2);
                h_vt_Diff_emep1.Fill(part_em1.vt() - part_em2.vt(), part_em1.vt() - part_ep.vt());
                h_vz_Diff_emep1.Fill(part_em1.vz() - part_em2.vz(), part_em1.vz() - part_ep.vz());

                if (TMath::Abs(part_em1.vt() - v_t_em2) < v_dtCut) {

                    h_vt_Diff_em1_ep.Fill(part_em1.vt() - v_t_ep);
                    h_vt_Diff_em2_ep.Fill(v_t_em2 - v_t_ep);

                    if (m_emep1 > 0.25 && m_emep2 > 0.25) { // This is to skip low mass events which seems are not produced at the target
                        //if (m_emep1 > 1. && m_emep2 > 1.) { // This is to skip low mass events which seems are not produced at the target
                        h_Mmis3.Fill(L_mis.M());

                        if (n_prot == 1) {
                            h_Mmis4.Fill(L_mis.M());
                        }
                    }

                    if (n_prot == 1) {
                        h_Mmis2.Fill(L_mis.M());
                    }
                }

            }

            event.getStructure(bMCPart);

            double px_ep, py_ep, pz_ep, pt_ep, p_ep;
            double px_em[2], py_em[2], pz_em[2], pt_em[2], p_em[2];

            int pid_em1 = bMCPart.getInt("pid", 1);
            int pid_em2 = bMCPart.getInt("pid", 3);
            int pid_ep = bMCPart.getInt("pid", 2);

            if (pid_em1 != 11 || pid_em2 != 11 || pid_ep != -11) {
                cout << "This should not happen. Change the code structure!!!!" << endl;
            }

            px_ep = bMCPart.getFloat("px", 2);
            py_ep = bMCPart.getFloat("py", 2);
            pz_ep = bMCPart.getFloat("pz", 2);
            pt_ep = sqrt(px_ep * px_ep + py_ep * py_ep);
            p_ep = sqrt(px_ep * px_ep + py_ep * py_ep + pz_ep * pz_ep);

            px_em[0] = bMCPart.getFloat("px", 1);
            py_em[0] = bMCPart.getFloat("py", 1);
            pz_em[0] = bMCPart.getFloat("pz", 1);
            pt_em[0] = sqrt(px_em[0] * px_em[0] + py_em[0] * py_em[0]);
            p_em[0] = sqrt(px_em[0] * px_em[0] + py_em[0] * py_em[0] + pz_em[0] * pz_em[0]);

            px_em[1] = bMCPart.getFloat("px", 3);
            py_em[1] = bMCPart.getFloat("py", 3);
            pz_em[1] = bMCPart.getFloat("pz", 3);
            pt_em[1] = sqrt(px_em[1] * px_em[1] + py_em[1] * py_em[1]);
            p_em[1] = sqrt(px_em[1] * px_em[1] + py_em[1] * py_em[1] + pz_em[1] * pz_em[1]);

            TLorentzVector L_MC_em1, L_MC_em2, L_MC_ep;
            L_MC_ep.SetPxPyPzE(px_ep, py_ep, pz_ep, p_ep);
            L_MC_em1.SetPxPyPzE(px_em[0], py_em[0], pz_em[0], p_em[0]);
            L_MC_em2.SetPxPyPzE(px_em[1], py_em[1], pz_em[1], p_em[1]);

            bool epAcc = mupAcc(L_MC_ep);
            bool em1_emAcc = emAcc(L_MC_em1);
            bool em1_mum = mumAcc(L_MC_em1);
            bool em2_emAcc = emAcc(L_MC_em2);
            bool em2_mum = mumAcc(L_MC_em2);

            if (epAcc && ((em1_emAcc && em2_mum) || (em2_emAcc && em1_mum))) {

                TLorentzVector L_emep1 = L_MC_em1 + L_MC_ep;
                TLorentzVector L_emep2 = L_MC_em2 + L_MC_ep;

                h_MC_Memep_12_1.Fill(L_emep1.M(), L_emep2.M());
            }



        }
    } catch (const char *msg) {
        cerr << msg << endl;
    }


    cout << "evCounter = " << evCounter << endl;

    for (TObject *cur_Obj : Hlist) {

        TH1 *curHist = (TH1*) cur_Obj;
        curHist->Scale(1000. / double(evCounter)); // The factor 1000 is for no reason, just will make number so be above 1.
    }



    gDirectory->Write();
    return 0;
}

bool emAcc(TLorentzVector& L) {

    const double th_max = 29.5; // deg
    const double th_min = 7.5; // deg
    const double P_max = 11.; // GeV
    const double P_min = 1.; // GeV

    return L.Theta() * TMath::RadToDeg() > th_min && L.Theta() * TMath::RadToDeg() < th_max && L.P() > P_min && L.P() < P_max;
}

bool mumAcc(TLorentzVector& L) {

    const double th_max = 29.5; // deg
    const double th_min = 7.5; // deg
    const double P_max = 11.; // GeV
    const double P_min = 1.5; // GeV

    return L.Theta() * TMath::RadToDeg() > th_min && L.Theta() * TMath::RadToDeg() < th_max && L.P() > P_min && L.P() < P_max &&
            L.Theta() * TMath::RadToDeg() > f_mumAccThP->Eval(L.P());
}

bool mupAcc(TLorentzVector& L) {

    const double th_max = 29.5; // deg
    const double th_min = 7.5; // deg
    const double P_max = 11.; // GeV
    const double P_min = 1.5; // GeV

    return L.Theta() * TMath::RadToDeg() > th_min && L.Theta() * TMath::RadToDeg() < th_max && L.P() > P_min && L.P() < P_max;
}

bool protAcc(TLorentzVector& L) {

    const double th_max = 120; // deg
    const double th_min = 40.; // deg
    const double P_max = 11.; // GeV
    const double P_min = 0.3; // GeV

    return L.Theta() * TMath::RadToDeg() > th_min && L.Theta() * TMath::RadToDeg() < th_max && L.P() > P_min && L.P() < P_max;
}