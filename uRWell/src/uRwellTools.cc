#include <uRwellTools.h>
#include <algorithm>

using namespace std;

int uRwellTools::getSlot(int ch) {

    if (ch <= 64) {
        return 0;
    } else if (ch >= 65 && ch <= 192) {
        return 1;
    } else if (ch >= 193 && ch <= 320) {
        return 2;
    } else if (ch >= 321 && ch <= 448) {
        return 4;
    } else if (ch >= 449 && ch <= 576) {
        return 9;
    } else if (ch >= 577 && ch <= 704) {
        return 11;
    } else if (ch >= 1001 && ch <= 1064) {
        return 6;
    } else if (ch >= 1065 && ch <= 1192) {
        return 7;
    } else if (ch >= 1193 && ch <= 1320) {
        return 8;
    } else if (ch >= 1321 && ch <= 1448) {
        return 10;
    } else if (ch >= 1449 && ch <= 1576) {
        return 3;
    } else if (ch >= 1577 && ch <= 11704) {
        return 5;
    } else {
        return -1; // Should not happen, non existing slot
    }

}

int uRwellTools::slot_Offset[uRwellTools::nSlot] = {0, 64, 192, 1448, 320, 1576, 1000, 1064, 1192, 448, 1320, 576};

namespace uRwellTools {

    uRwellCluster::uRwellCluster() {
        fnStrips = 0;
        fEnergy = 0;
    }

    void uRwellCluster::setStrips( std::vector<int> astrips ){
        fv_strips = astrips;
    }
    
    std::vector<uRwellCluster> getGlusters(std::vector<uRwellHit> v_Hits) {
        //
        // Let's 1st sort the vector of strips
        //
        vector<uRwellCluster> v_Clusters;
        sort(v_Hits.begin(), v_Hits.end(), [ ](const auto& lhs, const auto& rhs) {
            return lhs.strip < rhs.strip;
        });

        
        double clEnergy = 0;
        bool newCluster = true;
        int prev_Strip = -10000; // Some number that clearly is not a real strip number
        vector<int> v_strips;
        for (int i = 0; i < v_Hits.size(); i++) {

            int curStrip = v_Hits.at(i).strip;
            double curHitEnergy = v_Hits.at(i).adc;

            if (v_Hits.at(i).strip - curStrip <= 2) {
                clEnergy = clEnergy + curHitEnergy;
                v_strips.push_back(curStrip);
            } else {                
                uRwellTools::uRwellCluster curCluster;
                curCluster.setEnergy(clEnergy);
                curCluster.setStrips(v_strips);
                v_Clusters.push_back(curCluster);
                
                clEnergy = 0.;
                v_strips.clear();
                v_strips.shrink_to_fit();
                
                clEnergy = curHitEnergy;
                v_strips.push_back(curStrip);
            }
        }
        
        return v_Clusters;
    }
}