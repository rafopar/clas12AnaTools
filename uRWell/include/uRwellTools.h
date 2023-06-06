/*
 * File:   uRwellTools.h
 * Author: rafopar
 *
 * Created on May 7, 2023, 9:20 PM
 */

#ifndef URWELLTOOLS_H
#define URWELLTOOLS_H
#include <vector>

namespace uRwellTools {

    struct uRwellHit {
        int sector;
        int layer;
        int strip;
        int stripLocal;
        double adc;
        double adcRel;
        int ts;
        int slot;
    };

    int getSlot(int ch);
    const int nSlot = 12; // The test Prototype has only 12 slots
    extern int slot_Offset[nSlot]; // Gives the 1st strip channel (unique channel) for the given slot

    class uRwellCluster {
    public:
        uRwellCluster();

        void setEnergy(double aEnergy) {
            fEnergy = aEnergy;
        }
        
        void setHits(std::vector<uRwellHit>);
        std::vector<uRwellHit>* getHits(){
            return &fv_Hits;
        }
        void setNStrips(int aNStrips){
            fnStrips = fv_Hits.size();
        }

    private:
        int fnStrips;
        double fEnergy;
        std::vector<uRwellHit> fv_Hits;


    };

    std::vector<uRwellCluster> getGlusters(std::vector<uRwellHit>);
}

#endif /* URWELLTOOLS_H */