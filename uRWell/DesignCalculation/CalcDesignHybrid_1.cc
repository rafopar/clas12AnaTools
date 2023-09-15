#include <cmath>

using namespace std;

void CalcDesignHybrid_1() {

    const double d2r = TMath::DegToRad();


    const double alpha = 62.85; // deg // Angle of the trapezoid wrt big base
    const double beta = 10.; // deg, the stereo angle
    const double base_top = 1560.; // mm // The longest base, this part even might not be used
    const double tot_Height = 1392.57; // mm // The total height of the trapezoid
    const double d0_NotUsed = 0.1; // Top d0_NotUsed mm will not be used
    const double tot_d0 = tot_Height - d0_NotUsed; // Is total height that will represent active area of uRwell
    const int nSegment = 3; // The number of segements
    const int nChPerBoard = 256;
    const int nSectors = 6;
    const int nView = 2;
    const int nLayer = 2;
    const int n_Ch_InASIC = 64;
    const int n_ASIC_InBoard = nChPerBoard / n_Ch_InASIC;

    double d0_[nSegment]; // Height of each segment
    d0_[2] = 312.47; // mm
    d0_[1] = 540.; // mm
    d0_[0] = tot_d0 - d0_[1] - d0_[2]; // mm
    double l0_[nSegment]; // Side length of the trapezoid for each segment
    for (int i = 0; i < nSegment; i++) {
        l0_[i] = d0_[i] / sin(alpha * d2r);
    }


    // In this model, different segments can have different strip orientations.
    // * 0 : Strips are parallel to sides of trapezoid, or in other words the stereo angle = the base angle of the trapezoid
    // * 1 : Strips are oriented with +/- 10 degree stereo angle.    
    int StripOrientation[nSegment];
    StripOrientation[0] = 0; //  In Seg 0 strips are parallel to sides
    StripOrientation[1] = 0; //  In Seg 1 strips have +/- 10 degree stereo angles
    StripOrientation[2] = 1; //  In Seg 2 strips have +/- 10 degree stereo angles

    std::map<int, std::string> m_StripOrientName;
    m_StripOrientName[0] = "Parallel to sides";
    m_StripOrientName[1] = "+/- 10 degree";
    //m_StripOrientName[1] = Form("+/- %1.1f degree", beta);


    // In this model different segments can have different pitch sizes
    const double pitch[nSegment] = {1., 0.9, 0.75};

    double bases_[nSegment + 1];

    bases_[0] = base_top - 2 * d0_NotUsed / tan(alpha * d2r);
    for (int i = 1; i < nSegment + 1; i++) {
        bases_[i] = bases_[i - 1] - 2 * d0_[i - 1] / tan(alpha * d2r);
    }


    cout << " ************************************************ " << endl;
    cout << " ************* INPUT CONDITIONS ***************** " << endl;
    cout << " **   * The Number of ch per board    = " << nChPerBoard << endl;
    cout << " **   * The Stereo angle              = " << beta << " deg" << endl;
    cout << " **   * The Base angle of trapezoid   = " << alpha << " deg" << endl;
    cout << " **   * Tot height of the sector      = " << tot_d0 << " mm" << endl;
    cout << " **   * The number of segments        = " << nSegment << endl;
    for (int i = 0; i < nSegment; i++) {
        cout << Form(" **      * The height of segment #%d   = ", i) << d0_[i] << " mm " << endl;
    }
    for (int i = 0; i < nSegment + 1; i++) {
        cout << Form(" **      * Length of Base #%d   = ", i) << bases_[i] << " mm " << endl;
    }
    cout << " ***** " << endl;

    cout << endl;
    cout << "***** Expected design parameters *****" << endl;
    cout << endl;

    int tot_n_Strips = 0; // Total number of strips for the detectors
    int tot_n_Boards = 0; // The number of boards for the given sector = tot_n_Boards_Base + 2*tot_n_Boards_Side
    int tot_n_Boards_Base = 0; // The number of boards that will be mounted on the base of the trapezoid. Only Segments 0 channels can be readout with this boards
    int tot_n_Boards_Side = 0; // Number of boards that will be mounted on the side of the trapezoid
    int tot_n_channels = 0;
    for (int i = 0; i < nSegment; i++) {

        cout << " ******* Segment #" << i << " ******* " << endl;
        cout << " *** * Wire orientations                              : " << m_StripOrientName[StripOrientation[i]].c_str() << endl;
        cout << " *** * Pitch [mm]                                     : " << pitch[i] << endl;

        int n_MaxCh_PerView;
        if (StripOrientation[i] == 0) {
            double pitch_base = pitch[i] / sin(alpha * d2r);
            n_MaxCh_PerView = bases_[i] / pitch_base;

            int n_ASIC_inView;
            int n_BoardView;
            int nUnusedChannelsView;

            cout << " *** * Number of U(V) channels                        : " << n_MaxCh_PerView << endl;
            if (i == 0) { // Top segment, i.e. segment 0
                n_ASIC_inView = n_MaxCh_PerView / n_Ch_InASIC + 1; // Number of ASICs for U(V) strip readout.
                int n_unusedChInASIC = n_MaxCh_PerView % n_Ch_InASIC; // Number of unused 
                n_ASIC_inView = n_unusedChInASIC == 0 ? n_ASIC_inView - 1 : n_ASIC_inView;
                n_BoardView = n_ASIC_inView / n_ASIC_InBoard + 1; // The number of boards needed to readout U(V) strips
                int nUnused_ASICsView = n_ASIC_inView % n_ASIC_InBoard; // The number of Unused ASICs for the given view
                n_BoardView = nUnused_ASICsView == 0 ? n_BoardView - 1 : n_BoardView;
                nUnusedChannelsView = n_BoardView * nChPerBoard - n_MaxCh_PerView;
                tot_n_Boards_Base = tot_n_Boards_Base + 2*n_BoardView;
                cout << " *** * Number of U(V) boards on the base              : " << n_BoardView << endl;
                cout << " *** * Number of U+V boards on the base               : " << 2*n_BoardView << endl;
                cout << " *** * Number of unused channels on the U(V) board    : " << nUnusedChannelsView << endl;
            } else{
                n_BoardView = n_MaxCh_PerView / nChPerBoard + 1;
                int nChannesLeft = n_MaxCh_PerView % n_BoardView;
                n_BoardView = nChannesLeft == 0 ? n_BoardView - 1 : n_BoardView;
                tot_n_Boards_Side = tot_n_Boards_Side + n_BoardView;
                cout << " *** * Number of boards on the left(right) side       : " << n_BoardView << endl;
                cout << " *** * Number of unused channels on the U(V) board    : " << nChannesLeft << endl;
            }

            cout << endl << endl;

            tot_n_Strips = tot_n_Strips + n_MaxCh_PerView;
        } else if (StripOrientation[i] == 1) {
            double pitch_side = pitch[i] / sin((alpha + beta) * d2r);
            int n_MaxCh_PerSidePerView = l0_[i] / pitch_side;
            n_MaxCh_PerView = (l0_[i] + bases_[i + 1] * sin(beta * d2r) / sin((alpha + beta) * d2r)) / pitch_side;
            int n_MaxCh_PerSide = n_MaxCh_PerView; // Those numbers should be equal actually
            int n_MaxCh_OtherView = n_MaxCh_PerSide - n_MaxCh_PerSidePerView;

            int n_View_ASICs = n_MaxCh_PerSidePerView / n_Ch_InASIC; // Number of ASICS to be used to RO U(V) channels from the left(right) side.
            int n_ViewCh_RO_ThisSide = n_View_ASICs*n_Ch_InASIC; // Number of U(V) channels to be RO from the left(right) side
            int n_ViewCh_BothSide = n_MaxCh_PerSidePerView - n_ViewCh_RO_ThisSide; // Number of U(V) channels that touches both sides, but will be read on right(left) side.
            int n_ViewCh_RO_OtherSide = n_ViewCh_BothSide + n_MaxCh_OtherView; // Number of U(V) channels RO in right(left) side

            int n_OtherView_ASICs = n_ViewCh_RO_OtherSide / n_Ch_InASIC + 1; // The number of ASICs to RO V(U) channels from the left(right) side.
            int n_ASICS = n_View_ASICs + n_OtherView_ASICs;
            int nBoards = n_ASICS / n_ASIC_InBoard + 1;
            int nUnusedASIC = n_ASICS % n_ASIC_InBoard;
            nBoards = nUnusedASIC == 0 ? nBoards - 1 : nBoards;

            int nUnusedChannels = nBoards * nChPerBoard - n_ViewCh_RO_ThisSide - n_MaxCh_OtherView - n_ViewCh_BothSide; // The nu
            n_OtherView_ASICs = nUnusedChannels == 0 ? n_OtherView_ASICs - 1 : n_OtherView_ASICs;

            tot_n_Boards_Side = tot_n_Boards_Side + nBoards;
            tot_n_Strips = tot_n_Strips + n_MaxCh_PerView;

            cout << " *** * The Tot # of channels on left(right) side      : " << n_MaxCh_PerSide << endl;
            cout << " *** * The # of U(V) channels in left(right) side     : " << n_MaxCh_PerSidePerView << endl;
            cout << " *** * The # of V(U) channels in left(right) side     : " << n_MaxCh_OtherView << endl;
            cout << " *** * The # of U(V) channels RO in left(right) side  : " << n_ViewCh_RO_ThisSide << setw(10) << n_View_ASICs << " ASICs" << endl;
            cout << " *** * The # of V(U) channels RO in left(right) side  : " << n_ViewCh_RO_OtherSide << setw(10) << n_OtherView_ASICs << " ASICs" << endl;
            cout << " *** * The number of boards in left(right) side       : " << nBoards << endl;
            cout << " *** * The number of unused channels on the board     : " << nUnusedChannels << endl;
            cout << endl << endl;

        } else {
            cout << "Unknown strip orientation " << StripOrientation[i] << ".The code should be fixed..." << endl;
            cout << "exiting " << endl;
            exit(1);
        }


    }

    const int nAllStripsAllSectors = tot_n_Strips * nSectors * nView*nLayer;
    tot_n_Boards = tot_n_Boards_Base + 2 * tot_n_Boards_Side;

    cout << " ****     =============================================      **** " << endl;
    cout << " *** * Number of boards for one detector              : " << tot_n_Boards << endl;
    cout << " *** * Number of boards on the base                   : " << tot_n_Boards_Base << endl;
    cout << " *** * Number of boards on the left(right) side       : " << tot_n_Boards_Side << endl;
    cout << " *** * Tot # of U(V) strips                           : " << tot_n_Strips << endl;
    cout << " *** * The Tot # of strips for 6(Sec)X2(layers)       : " << nAllStripsAllSectors << endl;

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

    line1->DrawLine(-0.5 * base_top, 0., 0.5 * base_top, 0.);
    lat1->DrawLatex(-0.25 * base_top, -0.5 * d0_NotUsed, "Unused area");
    double tmp_Y = -d0_NotUsed;
    for (int i = 0; i < nSegment + 1; i++) {
        line1->DrawLine(-0.5 * bases_[i], tmp_Y, 0.5 * bases_[i], tmp_Y);
        if (i < nSegment) {
            lat1->DrawLatex(-0.25 * bases_[i], tmp_Y - 0.5 * d0_[i], Form("Segment %d", i));
        }

        tmp_Y = tmp_Y - d0_[i];
    }
    double Y_Bottom = tmp_Y;
    line1->DrawLine(-0.5 * base_top, 0., -0.5 * bases_[nSegment], Y_Bottom);
    line1->DrawLine(0.5 * base_top, 0., 0.5 * bases_[nSegment], Y_Bottom);
}
