#include <cmath>

using namespace std;

void CalcDesign(){

  const double d2r = TMath::DegToRad();
    
  
  const double pitch = 1.; // mm
  const double alpha = 65.77; // deg // Angle of the trapexoid wrt big base
  const double beta = 10.; // deg, the stereo angle
  const double base_top = 1463.55; // mm // The longest base, this part even might not be used
  const double d0_NotUsed = 200.;  // Top 200 mm will not be used
  const double tot_d0 = 1200.;
  const int nSegment = 3; // The number of segements
  const int nChPerBoard = 256;

  double d0_[nSegment];   // Heigh of each segment
  d0_[0] = 300.; // mm
  d0_[1] = 500.; // mm
  d0_[2] = tot_d0 - d0_[1] - d0_[0]; // mm
  double l0_[nSegment];   // Side length of the trapezoid for each segment
  for( int i = 0; i < nSegment; i++ ){
    l0_[i] = d0_[i]/sin(alpha*d2r);
  }
  
  double bases_[nSegment + 1];


  bases_[0] = base_top - 2*d0_NotUsed/tan(alpha*d2r);
  for( int i = 1; i < nSegment + 1; i++ ){
    bases_[i] = bases_[i-1] - 2*d0_[i-1]/tan(alpha*d2r);
  }
  
  const double pitch_side = pitch/sin( (alpha + beta)*d2r );

  cout<<" ************************************************ "<<endl;
  cout<<" ************* INPUT CONDITIONS ***************** "<<endl;
  //  cout<<" ** "<<endl;
  cout<<" **   * The Pitch                     = "<<pitch<<" mm"<<endl;
  cout<<" **   * The Number of ch per board    = "<<nChPerBoard<<endl;
  cout<<" **   * The Stereo angle              = "<<beta<<" deg"<<endl;
  cout<<" **   * The Base angle of trapezoid   = "<<alpha<<" deg"<<endl;
  cout<<" **   * Tot height of the sector      = "<<tot_d0<<" mm"<<endl;
  cout<<" **   * The number of segments        = "<<nSegment<<endl;
  //  cout<<" **   * The Pitch on side is          = "<<pitch_side<<" mm"<<endl;
  for( int i = 0; i < nSegment; i++ ){
    cout<<Form(" **      * The height of segment #%d   = ", i)<<d0_[i]<<" mm "<<endl;
  }
   for( int i = 0; i < nSegment + 1; i++ ){
    cout<<Form(" **      * Length of Base #%d   = ", i)<<bases_[i]<<" mm "<<endl;
  }
 
  cout<<" ***** "<<endl;

  cout<<endl;
  cout<<"***** Expected design parameters *****"<<endl;
  cout<<endl;

  int tot_n_Strips = 0;
  int tot_n_Boards = 0;
  int tot_n_channels = 0;
  for( int i = 0; i < nSegment; i++ ){
    
    int n_MaxCh_PerSidePerView = l0_[i]/pitch_side;
    int n_MaxCh_PerView = (l0_[i] + bases_[i+1]*sin(beta*d2r)/sin( (alpha + beta)*d2r ) )/pitch_side;
    int n_MaxCh_PerSide = n_MaxCh_PerView; // Those numbers should be equal actually
    int n_MaxCh_OtherView = n_MaxCh_PerSide - n_MaxCh_PerSidePerView;
    int nBoardPerSide = n_MaxCh_PerSide/nChPerBoard + 1;
    int nChannesLeft = n_MaxCh_PerSide%nChPerBoard;
    
    nBoardPerSide = nChannesLeft == 0 ? nBoardPerSide - 1 : nBoardPerSide;
    int nUnusedChannels = nBoardPerSide*nChPerBoard - n_MaxCh_PerSide;
    tot_n_Boards = tot_n_Boards + nBoardPerSide;
    tot_n_Strips = tot_n_Strips + n_MaxCh_PerSide;
    
    cout<<" ******* Segment #"<<i<<" ******* "<<endl;    
    cout<<" *** * The Tot # of channels on left(right) side     is "<<n_MaxCh_PerSide<<endl;
    cout<<" *** * The # of U(V) channels in left(right) side    is "<<n_MaxCh_PerSidePerView<<endl;
    cout<<" *** * The # of V(U) channels in left(right) side    is "<<n_MaxCh_OtherView<<endl;
    cout<<" *** * The number of boards                          is "<<nBoardPerSide<<endl;
    cout<<" *** * The number of unused channels on the board    is "<<nUnusedChannels<<endl;
    cout<<endl<<endl;
}
  cout<<" *** * The Tot # of boards on left(right) side         is "<<tot_n_Boards<<endl;
  cout<<" *** * The Tot # of strips on left(right) side         is "<<tot_n_Strips<<endl;
  
  
  
}
