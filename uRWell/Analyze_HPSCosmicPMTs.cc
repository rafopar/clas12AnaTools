#include <cstdlib>

#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>

// ===== Hipo headers =====
#include <reader.h>
#include <writer.h>
#include <dictionary.h>

#include <map>
#include <vector>
#include <utility>

using namespace std;

int main(int argc, char** argv) {

  char outputFile[256];
  char inputFile[256];

  int run = 0;
  if (argc > 1) {
    run = atoi(argv[1]);
    //sprintf(inputFile, "%s", argv[1]);
    sprintf(inputFile, "Data/decoded_%d.hipo", run);
    sprintf(outputFile, "outSkim.hipo");
  } else {
    std::cout << " *** please provide a file name..." << std::endl;
    exit(0);
  }

  hipo::reader reader;
  reader.open(inputFile);

  hipo::dictionary factory;

  reader.readDictionary(factory);

  factory.show();
  hipo::event event;
  int evCounter = 0;

  hipo::bank buRWellADC(factory.getSchema("URWELL::adc"));
  hipo::bank bRAWADc(factory.getSchema("RAW::adc"));
  hipo::bank bRunConf(factory.getSchema("RUN::config"));

  const int crateID_test3 = 40; // the fADC is on ROC 40
  const int slot_fADC = 3; // and the slot is 3
  const int n_ts = 6;

  TFile *file_out = new TFile(Form("AnalyzeHPSCosmicPMTs_%d.root", run), "Recreate");

  TH2D *h_ADC_TopBot1 = new TH2D("h_ADC_TopBot1", "", 200, 0., 2000, 200, 0., 2000);

  try {

    while (reader.next() == true) {
      reader.read(event);

      evCounter = evCounter + 1;

      //if( evCounter > 2000 ){break;}
      if (evCounter % 1000 == 0) {
	cout.flush() << "Processed " << evCounter << " events \r";
      }

      event.getStructure(buRWellADC);
      event.getStructure(bRAWADc);
      event.getStructure(bRunConf);
      int n_ADC = buRWellADC.getRows();
      int n_RunConf = bRunConf.getRows();


      int n_fADC = bRAWADc.getRows();

      int ADC_SCTop = 0;
      int ADC_SCBot = 0;

      for (int ifADC = 0; ifADC < n_fADC; ifADC++) {

	int crate = bRAWADc.getInt("crate", ifADC);
	int slot = bRAWADc.getInt("slot", ifADC);

	if (!(crate == crateID_test3 && slot == slot_fADC)) {
	  cout << "Wrong crate or slot. Should not be happen" << endl;
	  cout << "              crate = " << crate << "     slot = " << slot << endl;

	  continue;
	}

	int ch = bRAWADc.getInt("channel", ifADC);
	int ADC = bRAWADc.getInt("ADC", ifADC);
	double time = bRAWADc.getFloat("time", ifADC);

	if (ch == 0 ) {
	  ADC_SCBot = ADC_SCBot + ADC;
	}else if( ch == 1 ){
	  ADC_SCTop = ADC_SCTop + ADC;
	}

	
	
      }

      h_ADC_TopBot1->Fill(ADC_SCBot, ADC_SCTop);
    }
  } catch (const char msg) {
    cerr << msg << endl;
  }

  gDirectory->Write();
  file_out->Close();

}
