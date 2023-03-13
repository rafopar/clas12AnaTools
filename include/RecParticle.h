/* 
 * File:   RecParticle.h
 * Author: rafopar
 *
 * Created on December 22, 2021, 10:04 PM
 */

#ifndef RECPARTICLE_H
#define RECPARTICLE_H



// ===== Hipo headers =====
#include <reader.h>
#include <dictionary.h>

class RecParticle {
public:
    RecParticle( hipo::bank &, hipo::bank &, hipo::bank &, int, int, int, int, int );
    //RecParticle(const RecParticle& orig);
    virtual ~RecParticle();

    int pid() const {return fpid;};
    double px() const {return fpx;};
    double py() const {return fpy;};
    double pz() const {return fpz;};
    double p() const {return fp;};
    double th() const {return fth;};
    double phi() const {return fphi;};
    double vx() const {return fvx;};
    double vy() const {return fvy;};
    double vz() const {return fvz;};
    double vt() const {return fvt;};
    int charge() const {return fcharge;};
    double beta() const {return fbeta;};
    double chi2pid() const {return fchi2pid;};
    int status() const {return fstatus;};

    double SF() const {return fSF;};
    
    bool hasPCal() const {return fhasPCal;};
    double SFPCal() const {return fSFPCal;};
    int secPCal() const {return fsecPCal;};
    double energyPCal() const {return fenergyPCal;};
    double timePCal() const {return ftimePCal;};
    double xPCal() const {return fxPCal;};
    double yPCal() const {return fyPCal;};
    double zPCal() const {return fzPCal;};
    double hxPCal() const {return fhxPCal;};
    double hyPCal() const {return fhyPCal;};
    double hzPCal() const {return fhzPCal;};
    double luPCal() const {return fluPCal;};
    double lvPCal() const {return flvPCal;};
    double lwPCal() const {return flwPCal;};
    double duPCal() const {return fduPCal;};
    double dvPCal() const {return fdvPCal;};
    double dwPCal() const {return fdwPCal;};
    double m2uPCal() const {return fm2uPCal;};
    double m2vPCal() const {return fm2vPCal;};
    double m2wPCal() const {return fm2wPCal;};
    double m3uPCal() const {return fm3uPCal;};
    double m3vPCal() const {return fm3vPCal;};
    double m3wPCal() const {return fm3uPCal;};

    bool hasECin() const {return fhasECin;};
    double SFECin() const {return fSFECin;};
    int secECin() const {return fsecECin;};
    double energyECin() const {return fenergyECin;};
    double timeECin() const {return ftimeECin;};
    double xECin() const {return fxECin;};
    double yECin() const {return fyECin;};
    double zECin() const {return fzECin;};
    double hxECin() const {return fhxECin;};
    double hyECin() const {return fhyECin;};
    double hzECin() const {return fhzECin;};
    double luECin() const {return fluECin;};
    double lvECin() const {return flvECin;};
    double lwECin() const {return flwECin;};
    double duECin() const {return fduECin;};
    double dvECin() const {return fdvECin;};
    double dwECin() const {return fdwECin;};
    double m2uECin() const {return fm2uECin;};
    double m2vECin() const {return fm2vECin;};
    double m2wECin() const {return fm2wECin;};
    double m3uECin() const {return fm3uECin;};
    double m3vECin() const {return fm3vECin;};
    double m3wECin() const {return fm3wECin;};

    bool hasECout() const {return fhasECout;};
    double SFECout() const {return fSFECout;};
    int secECout() const {return fsecECout;};
    double energyECout() const {return fenergyECout;};
    double timeECout() const {return ftimeECout;};
    double xECout() const {return fxECout;};
    double yECout() const {return fyECout;};
    double zECout() const {return fzECout;};
    double hxECout() const {return fhxECout;};
    double hyECout() const {return fhyECout;};
    double hzECout() const {return fhzECout;};
    double luECout() const {return fluECout;};
    double lvECout() const {return flvECout;};
    double lwECout() const {return flwECout;};
    double duECout() const {return fduECout;};
    double dvECout() const {return fdvECout;};
    double dwECout() const {return fdwECout;};
    double m2uECout() const {return fm2uECout;};
    double m2vECout() const {return fm2vECout;};
    double m2wECout() const {return fm2wECout;};
    double m3uECout() const {return fm3uECout;};
    double m3vECout() const {return fm3vECout;};
    double m3wECout() const {return fm3uECout;};

    bool hasHTCC() const {return fhasHTCC;};
    int secHTCC() const {return fsecHTCC;};
    double npheHTCC() const {return fnpheHTCC;};
    double timeHTCC() const {return ftimeHTCC;};
    double pathHTCC() const {return fpathHTCC;};
    double chi2HTCC() const {return fchi2HTCC;};
    double xHTCC() const {return fxHTCC;};
    double yHTCC() const {return fyHTCC;};
    double zHTCC() const {return fzHTCC;};
    double dthetaHTCC() const {return fdthetaHTCC;};
    double dphiHTCC() const {return fdphiHTCC;};
    int statusHTCC() const {return fstatusHTCC;};


    void Setpid(int arg)  {fpid = arg;};
    void Setpx(double arg)  {fpx = arg;};
    void Setpy(double arg)  {fpy = arg;};
    void Setpz(double arg)  {fpz = arg;};
    void Setp(double arg)  {fp = arg;};
    void Setth(double arg)  {fth = arg;};
    void Setphi(double arg)  {fphi = arg;};
    void Setvx(double arg)  {fvx = arg;};
    void Setvy(double arg)  {fvy = arg;};
    void Setvz(double arg)  {fvz = arg;};
    void Setvt(double arg)  {fvt = arg;};
    void Setcharge(double arg)  {fcharge = arg;};
    void Setbeta(double arg)  {fbeta = arg;};
    void Setchi2pid(double arg)  {fchi2pid = arg;};
    void Setstatus(double arg)  {fstatus = arg;};

    void SetSF(double arg) {fSF = arg;};
    
    void SethasPCal(bool arg)  {fhasPCal = arg;};
    void SetSFPCal(double arg)  {fSFPCal = arg;};
    void SetsecPCal(int arg)  {fsecPCal = arg;};
    void SetenergyPCal(double arg)  {fenergyPCal = arg;};
    void SettimePCal(double arg)  {ftimePCal = arg;};
    void SetxPCal(double arg)  {fxPCal = arg;};
    void SetyPCal(double arg)  {fyPCal = arg;};
    void SetzPCal(double arg)  {fzPCal = arg;};
    void SethxPCal(double arg)  {fhxPCal = arg;};
    void SethyPCal(double arg)  {fhyPCal = arg;};
    void SethzPCal(double arg)  {fhzPCal = arg;};
    void SetluPCal(double arg)  {fluPCal = arg;};
    void SetlvPCal(double arg)  {flvPCal = arg;};
    void SetlwPCal(double arg)  {flwPCal = arg;};
    void SetduPCal(double arg)  {fduPCal = arg;};
    void SetdvPCal(double arg)  {fdvPCal = arg;};
    void SetdwPCal(double arg)  {fdwPCal = arg;};
    void Setm2uPCal(double arg)  {fm2uPCal = arg;};
    void Setm2vPCal(double arg)  {fm2vPCal = arg;};
    void Setm2wPCal(double arg)  {fm2wPCal = arg;};
    void Setm3uPCal(double arg)  {fm3uPCal = arg;};
    void Setm3vPCal(double arg)  {fm3vPCal = arg;};
    void Setm3wPCal(double arg)  {fm3wPCal = arg;};
    
    void SethasECin(bool arg)  {fhasECin = arg;};
    void SetSFECin(double arg)  {fSFECin = arg;};
    void SetsecECin(int arg)  {fsecECin = arg;};
    void SetenergyECin(double arg)  {fenergyECin = arg;};
    void SettimeECin(double arg)  {ftimeECin = arg;};
    void SetxECin(double arg)  {fxECin = arg;};
    void SetyECin(double arg)  {fyECin = arg;};
    void SetzECin(double arg)  {fzECin = arg;};
    void SethxECin(double arg)  {fhxECin = arg;};
    void SethyECin(double arg)  {fhyECin = arg;};
    void SethzECin(double arg)  {fhzECin = arg;};
    void SetluECin(double arg)  {fluECin = arg;};
    void SetlvECin(double arg)  {flvECin = arg;};
    void SetlwECin(double arg)  {flwECin = arg;};
    void SetduECin(double arg)  {fduECin = arg;};
    void SetdvECin(double arg)  {fdvECin = arg;};
    void SetdwECin(double arg)  {fdwECin = arg;};
    void Setm2uECin(double arg)  {fm2uECin = arg;};
    void Setm2vECin(double arg)  {fm2vECin = arg;};
    void Setm2wECin(double arg)  {fm2wECin = arg;};
    void Setm3uECin(double arg)  {fm3uECin = arg;};
    void Setm3vECin(double arg)  {fm3vECin = arg;};
    void Setm3wECin(double arg)  {fm3wECin = arg;};
    
    void SethasECout(bool arg)  {fhasECout = arg;};
    void SetSFECout(double arg)  {fSFECout = arg;};
    void SetsecECout(int arg)  {fsecECout = arg;};
    void SetenergyECout(double arg)  {fenergyECout = arg;};
    void SettimeECout(double arg)  {ftimeECout = arg;};
    void SetxECout(double arg)  {fxECout = arg;};
    void SetyECout(double arg)  {fyECout = arg;};
    void SetzECout(double arg)  {fzECout = arg;};
    void SethxECout(double arg)  {fhxECout = arg;};
    void SethyECout(double arg)  {fhyECout = arg;};
    void SethzECout(double arg)  {fhzECout = arg;};
    void SetluECout(double arg)  {fluECout = arg;};
    void SetlvECout(double arg)  {flvECout = arg;};
    void SetlwECout(double arg)  {flwECout = arg;};
    void SetduECout(double arg)  {fduECout = arg;};
    void SetdvECout(double arg)  {fdvECout = arg;};
    void SetdwECout(double arg)  {fdwECout = arg;};
    void Setm2uECout(double arg)  {fm2uECout = arg;};
    void Setm2vECout(double arg)  {fm2vECout = arg;};
    void Setm2wECout(double arg)  {fm2wECout = arg;};
    void Setm3uECout(double arg)  {fm3uECout = arg;};
    void Setm3vECout(double arg)  {fm3vECout = arg;};
    void Setm3wECout(double arg)  {fm3wECout = arg;};
    
    void SethasHTCC(bool arg) {fhasHTCC = arg;};
    void SetsecHTCC(int arg) { fsecHTCC = arg;};
    void SetnpheHTCC(double arg) {fnpheHTCC = arg;};
    void SettimeHTCC(double arg) {ftimeHTCC = arg;};
    void SetpathHTCC(double arg) {fpathHTCC = arg;};
    void Setchi2HTCC(double arg) {fchi2HTCC = arg;};
    void SetxHTCC(double arg) {fxHTCC = arg;};
    void SetyHTCC(double arg) {fyHTCC = arg;};
    void SetzHTCC(double arg) {fzHTCC = arg;};
    void SetdthetaHTCC(double arg) {fdthetaHTCC = arg;};
    void SetdphiHTCC(double arg) {fdphiHTCC = arg;};
    void SetstatusHTCC(int arg) {fstatusHTCC = arg;};
    

private:

    int fpid;
    double fpx;
    double fpy;
    double fpz;
    double fp;
    double fth;
    double fphi;
    double fvx;
    double fvy;
    double fvz;
    double fvt;
    int fcharge;
    double fbeta;
    double fchi2pid;
    int fstatus;

    double fSF;

    bool fhasPCal;
    double fSFPCal;
    int fsecPCal;
    double fenergyPCal;
    double ftimePCal;
    double fxPCal;
    double fyPCal;
    double fzPCal;
    double fhxPCal;
    double fhyPCal;
    double fhzPCal;
    double fluPCal;
    double flvPCal;
    double flwPCal;
    double fduPCal;
    double fdvPCal;
    double fdwPCal;
    double fm2uPCal;
    double fm2vPCal;
    double fm2wPCal;
    double fm3uPCal;
    double fm3vPCal;
    double fm3wPCal;

    bool fhasECin;
    double fSFECin;
    int fsecECin;
    double fenergyECin;
    double ftimeECin;
    double fxECin;
    double fyECin;
    double fzECin;
    double fhxECin;
    double fhyECin;
    double fhzECin;
    double fluECin;
    double flvECin;
    double flwECin;
    double fduECin;
    double fdvECin;
    double fdwECin;
    double fm2uECin;
    double fm2vECin;
    double fm2wECin;
    double fm3uECin;
    double fm3vECin;
    double fm3wECin;

    bool fhasECout;
    double fSFECout;
    int fsecECout;
    double fenergyECout;
    double ftimeECout;
    double fxECout;
    double fyECout;
    double fzECout;
    double fhxECout;
    double fhyECout;
    double fhzECout;
    double fluECout;
    double flvECout;
    double flwECout;
    double fduECout;
    double fdvECout;
    double fdwECout;
    double fm2uECout;
    double fm2vECout;
    double fm2wECout;
    double fm3uECout;
    double fm3vECout;
    double fm3wECout;

    bool fhasHTCC;
    int fsecHTCC;
    double fnpheHTCC;
    double ftimeHTCC;
    double fpathHTCC;
    double fchi2HTCC;
    double fxHTCC;
    double fyHTCC;
    double fzHTCC;
    double fdthetaHTCC;
    double fdphiHTCC;
    int fstatusHTCC;
};

#endif /* RECPARTICLE_H */