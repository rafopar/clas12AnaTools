#include <cmath>

using namespace std;

void CalcDesign(){

  const double d2r = TMath::DegToRad();
  
  
  const double pitch = 1.2; // mm
  const double alpha = 62.85; // deg // Angle of the trapezoid wrt big base
  const double beta = 10.; // deg, the stereo angle
  const double base_top = 1560.; // mm // The longest base, this part even might not be used
  const double tot_Height = 1392.57; // mm // The total height of the trapezoid
  const double d0_NotUsed = 45;  // Top d0_NotUsed mm will not be used
  const double tot_d0 = tot_Height - d0_NotUsed; // Is total height that will represent active area of uRwell
  const int nSegment = 3; // The number of segements
  const int nChPerBoard = 256;
  const int nSectors = 6;
  const int nView = 2;
  const int nLayer = 2;

  double d0_[nSegment];   // Height of each segment
  d0_[2] = 500; // mm
  d0_[1] = 460 ; // mm
  d0_[0] = tot_d0 - d0_[1] - d0_[2]; // mm
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

  const int nAllStripsAllSectors = tot_n_Strips*nSectors*nView*nLayer;
  
  cout<<" *** * The Tot # of boards on left(right) side                is "<<tot_n_Boards<<endl;
  cout<<" *** * The Tot # of strips on left(right) side                is "<<tot_n_Strips<<endl;
  cout<<" *** * The Tot # of strips for 6(Sec)X2(layers)               is "<<nAllStripsAllSectors<<endl;

  TCanvas *c1 = new TCanvas("c1", "", 950, 950);

  TH2D *h_tmp = new TH2D("h_tmp", "; X [mm]; Y [mm]", 200, -800., 800., 200, -1450., 150);
  h_tmp->GetYaxis()->SetTitleOffset(1.2);
  h_tmp->SetStats(0);
  c1->SetTopMargin(0.02);
  c1->SetRightMargin(0.02);
  h_tmp->Draw("");

  TLatex *lat1 = new TLatex();
  lat1->SetTextFont(42);
  lat1->SetTextSize(0.03);
  
  TLine *line1 = new TLine();
  line1->SetLineWidth(2);
  line1->SetLineColor(2);

  line1->DrawLine(-0.5*base_top, 0., 0.5*base_top, 0.);
  lat1->DrawLatex(-0.25*base_top, -0.5*d0_NotUsed, "Unused area");
  double tmp_Y = -d0_NotUsed;
  for( int i = 0; i < nSegment + 1; i++ ){
    line1->DrawLine(-0.5*bases_[i], tmp_Y , 0.5*bases_[i], tmp_Y);
    if( i < nSegment ){
      lat1->DrawLatex(-0.25*bases_[i], tmp_Y - 0.5*d0_[i], Form("Segment %d", i));
    }

    tmp_Y = tmp_Y - d0_[i];
  }
  double Y_Bottom = tmp_Y;
  line1->DrawLine(-0.5*base_top, 0., -0.5*bases_[nSegment], Y_Bottom);
  line1->DrawLine(0.5*base_top, 0., 0.5*bases_[nSegment], Y_Bottom);
}
