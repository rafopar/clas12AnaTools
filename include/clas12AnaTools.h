/* 
 * File:   clas12AnaTools.h
 * Author: rafopar
 *
 * Created on January 31, 2025, 5:15 PM
 */

#ifndef CLAS12ANATOOLS_H
#define CLAS12ANATOOLS_H

#include <iostream>
#include <string>

#include <TF1.h>
#include <TH2D.h>

namespace clas12AnaTools {

    void SliceFit(TH2D* inp_hist, double min_X, double max_X, int nBins, TF1 *f_Fit, std::string keyWord);

} // namespace clas12AnaTools


#endif /* CLAS12ANATOOLS_H */