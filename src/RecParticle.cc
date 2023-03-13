/* 
 * File:   RecParticle.cc
 * Author: rafopar
 * 
 * Created on December 22, 2021, 10:04 PM
 */

#include "RecParticle.h"

#include <TMath.h>

RecParticle::RecParticle(hipo::bank &bRecPart, hipo::bank &bRecCalo, hipo::bank &bRecCherenkov, int pind, int ind_PCal, int ind_ECin, int ind_ECout, int ind_HTCC) {

    fpid = bRecPart.getInt("pid", pind);
    fpx = double( bRecPart.getFloat("px", pind));
    fpy = double( bRecPart.getFloat("py", pind));
    fpz = double( bRecPart.getFloat("pz", pind));
    fp = sqrt(fpx * fpx + fpy * fpy + fpz * fpz);
    fth = acos(fpz / fp) * TMath::RadToDeg();
    fphi = atan2(fpy, fpx) * TMath::RadToDeg() + 30.;
    fphi = fphi < 0 ? fphi + 360. : fphi;
    fvx = double( bRecPart.getFloat("vx", pind));
    fvy = double( bRecPart.getFloat("vy", pind));
    fvz = double( bRecPart.getFloat("vz", pind));
    fvt = double( bRecPart.getFloat("vt", pind));
    fcharge = bRecPart.getInt("charge", pind);
    fbeta = double( bRecPart.getFloat("beta", pind));
    fchi2pid = double( bRecPart.getFloat("chi2pid", pind));
    fstatus = bRecPart.getInt("status", pind);

    fenergyPCal = 0;
    fenergyECin = 0;
    fenergyECout = 0;

    if (ind_PCal >= 0) {
        fhasPCal = true;

        fsecPCal = bRecCalo.getInt("sector", ind_PCal);
        fenergyPCal = double(bRecCalo.getFloat("energy", ind_PCal));

        fSFPCal = fenergyPCal / fp;
        ftimePCal = double(bRecCalo.getFloat("time", ind_PCal));
        fxPCal = double(bRecCalo.getFloat("x", ind_PCal));
        fyPCal = double(bRecCalo.getFloat("y", ind_PCal));
        fzPCal = double(bRecCalo.getFloat("z", ind_PCal));
        fhxPCal = double(bRecCalo.getFloat("hx", ind_PCal));
        fhyPCal = double(bRecCalo.getFloat("hy", ind_PCal));
        fhzPCal = double(bRecCalo.getFloat("hz", ind_PCal));
        fluPCal = double(bRecCalo.getFloat("lu", ind_PCal));
        flvPCal = double(bRecCalo.getFloat("lv", ind_PCal));
        flwPCal = double(bRecCalo.getFloat("lw", ind_PCal));
        fduPCal = double(bRecCalo.getFloat("du", ind_PCal));
        fdvPCal = double(bRecCalo.getFloat("dv", ind_PCal));
        fdwPCal = double(bRecCalo.getFloat("dw", ind_PCal));
        fm2uPCal = double(bRecCalo.getFloat("m2u", ind_PCal));
        fm2vPCal = double(bRecCalo.getFloat("m2v", ind_PCal));
        fm2wPCal = double(bRecCalo.getFloat("m2w", ind_PCal));
        fm3uPCal = double(bRecCalo.getFloat("m3u", ind_PCal));
        fm3vPCal = double(bRecCalo.getFloat("m3v", ind_PCal));
        fm3wPCal = double(bRecCalo.getFloat("m3w", ind_PCal));
    }

    if (ind_ECin >= 0) {

        fhasECin = true;

        fsecECin = bRecCalo.getInt("sector", ind_ECin);
        fenergyECin = double(bRecCalo.getFloat("energy", ind_ECin));

        fSFECin = fenergyECin / fp;
        ftimeECin = double(bRecCalo.getFloat("time", ind_ECin));
        fxECin = double(bRecCalo.getFloat("x", ind_ECin));
        fyECin = double(bRecCalo.getFloat("y", ind_ECin));
        fzECin = double(bRecCalo.getFloat("z", ind_ECin));
        fhxECin = double(bRecCalo.getFloat("hx", ind_ECin));
        fhyECin = double(bRecCalo.getFloat("hy", ind_ECin));
        fhzECin = double(bRecCalo.getFloat("hz", ind_ECin));
        fluECin = double(bRecCalo.getFloat("lu", ind_ECin));
        flvECin = double(bRecCalo.getFloat("lv", ind_ECin));
        flwECin = double(bRecCalo.getFloat("lw", ind_ECin));
        fduECin = double(bRecCalo.getFloat("du", ind_ECin));
        fdvECin = double(bRecCalo.getFloat("dv", ind_ECin));
        fdwECin = double(bRecCalo.getFloat("dw", ind_ECin));
        fm2uECin = double(bRecCalo.getFloat("m2u", ind_ECin));
        fm2vECin = double(bRecCalo.getFloat("m2v", ind_ECin));
        fm2wECin = double(bRecCalo.getFloat("m2w", ind_ECin));
        fm3uECin = double(bRecCalo.getFloat("m3u", ind_ECin));
        fm3vECin = double(bRecCalo.getFloat("m3v", ind_ECin));
        fm3wECin = double(bRecCalo.getFloat("m3w", ind_ECin));
    }

    if (ind_ECout >= 0) {

        fhasECout = true;

        fsecECout = bRecCalo.getInt("sector", ind_ECout);
        fenergyECout = double(bRecCalo.getFloat("energy", ind_ECout));

        fSFECout = fenergyECout / fp;
        ftimeECout = double(bRecCalo.getFloat("time", ind_ECout));
        fxECout = double(bRecCalo.getFloat("x", ind_ECout));
        fyECout = double(bRecCalo.getFloat("y", ind_ECout));
        fzECout = double(bRecCalo.getFloat("z", ind_ECout));
        fhxECout = double(bRecCalo.getFloat("hx", ind_ECout));
        fhyECout = double(bRecCalo.getFloat("hy", ind_ECout));
        fhzECout = double(bRecCalo.getFloat("hz", ind_ECout));
        fluECout = double(bRecCalo.getFloat("lu", ind_ECout));
        flvECout = double(bRecCalo.getFloat("lv", ind_ECout));
        flwECout = double(bRecCalo.getFloat("lw", ind_ECout));
        fduECout = double(bRecCalo.getFloat("du", ind_ECout));
        fdvECout = double(bRecCalo.getFloat("dv", ind_ECout));
        fdwECout = double(bRecCalo.getFloat("dw", ind_ECout));
        fm2uECout = double(bRecCalo.getFloat("m2u", ind_ECout));
        fm2vECout = double(bRecCalo.getFloat("m2v", ind_ECout));
        fm2wECout = double(bRecCalo.getFloat("m2w", ind_ECout));
        fm3uECout = double(bRecCalo.getFloat("m3u", ind_ECout));
        fm3vECout = double(bRecCalo.getFloat("m3v", ind_ECout));
        fm3wECout = double(bRecCalo.getFloat("m3w", ind_ECout));
    }

    if (ind_HTCC >= 0) {
        fhasHTCC = true;
        fsecHTCC = bRecCherenkov.getInt("sector", ind_HTCC);
        fnpheHTCC = double( bRecCherenkov.getFloat("nphe", ind_HTCC) );
        ftimeHTCC = double( bRecCherenkov.getFloat("time", ind_HTCC) );
        fpathHTCC = double( bRecCherenkov.getFloat("path", ind_HTCC) );
        fchi2HTCC = double( bRecCherenkov.getFloat("chi2", ind_HTCC) );
        fxHTCC = double( bRecCherenkov.getFloat("x", ind_HTCC) );
        fyHTCC = double( bRecCherenkov.getFloat("y", ind_HTCC) );
        fzHTCC = double( bRecCherenkov.getFloat("z", ind_HTCC) );
        fdthetaHTCC = double( bRecCherenkov.getFloat("dtheta", ind_HTCC) );
        fdphiHTCC = double( bRecCherenkov.getFloat("dphi", ind_HTCC) );
        fstatusHTCC = bRecCherenkov.getInt("status", ind_HTCC) ;

    }
    
    fSF = (fenergyPCal + fenergyECin + fenergyECout)/fp;

}

//RecParticle::RecParticle(const RecParticle& orig) {
//}

RecParticle::~RecParticle() {
}