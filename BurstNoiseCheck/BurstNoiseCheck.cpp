//some standard C++ includes
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>

//some ROOT includes
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TFile.h"
#include "TMath.h"
#include "TTree.h"
#include "TBranch.h"

//"art" includes (canvas, and gallery)
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"

//"larsoft" object includes
#include "lardataobj/RecoBase/OpFlash.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/AnalysisBase/T0.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h"

//our own includes
//#include "hist_utilities.h"

using namespace art;
using namespace std;
using namespace std::chrono;

int main() {

  TFile f_output("output.root","RECREATE");

  TH1F *maxvalHist_event8 = new TH1F("maxvalHist_event8","",8256,-0.5,8255.5);
  TH1F *maxvalHist_event9 = new TH1F("maxvalHist_event9","",8256,-0.5,8255.5);
  TH1F *maxvalHist_event23 = new TH1F("maxvalHist_event23","",8256,-0.5,8255.5);
  TH1F *maxvalHist_event32 = new TH1F("maxvalHist_event32","",8256,-0.5,8255.5);

  TH1F *avgwfHist_Uplane_event8 = new TH1F("avgwfHist_Uplane_event8","",9594,-0.5,9593.5);
  TH1F *avgwfHist_Uplane_event9 = new TH1F("avgwfHist_Uplane_event9","",9594,-0.5,9593.5);
  TH1F *avgwfHist_Uplane_event23 = new TH1F("avgwfHist_Uplane_event23","",9594,-0.5,9593.5);
  TH1F *avgwfHist_Uplane_event32 = new TH1F("avgwfHist_Uplane_event32","",9594,-0.5,9593.5);

  TH1F *avgwfHist_Vplane_event8 = new TH1F("avgwfHist_Vplane_event8","",9594,-0.5,9593.5);
  TH1F *avgwfHist_Vplane_event9 = new TH1F("avgwfHist_Vplane_event9","",9594,-0.5,9593.5);
  TH1F *avgwfHist_Vplane_event23 = new TH1F("avgwfHist_Vplane_event23","",9594,-0.5,9593.5);
  TH1F *avgwfHist_Vplane_event32 = new TH1F("avgwfHist_Vplane_event32","",9594,-0.5,9593.5);

  TH1F *avgwfHist_Yplane_event8 = new TH1F("avgwfHist_Yplane_event8","",9594,-0.5,9593.5);
  TH1F *avgwfHist_Yplane_event9 = new TH1F("avgwfHist_Yplane_event9","",9594,-0.5,9593.5);
  TH1F *avgwfHist_Yplane_event23 = new TH1F("avgwfHist_Yplane_event23","",9594,-0.5,9593.5);
  TH1F *avgwfHist_Yplane_event32 = new TH1F("avgwfHist_Yplane_event32","",9594,-0.5,9593.5);

  vector<string> filenames { "cathodeHVburstEvent.root" };

  //InputTag rawdigit_tag { "digitfilter" };
  InputTag rawdigit_tag { "daq" };

  int totalEventNum = 0;
  for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {
    auto t_begin = high_resolution_clock::now();
    totalEventNum++;

    cout << "Processing "
	 << "Run " << ev.eventAuxiliary().run() << ", "
	 << "Subun " << ev.eventAuxiliary().subRun() << ", "
	 << "Event " << ev.eventAuxiliary().event() << endl;

    if (((ev.eventAuxiliary().event() != 8) && (ev.eventAuxiliary().event() != 9) && (ev.eventAuxiliary().event() != 23) && (ev.eventAuxiliary().event() != 32)) || (ev.eventAuxiliary().subRun() != 0)) continue;

    auto const& rawdigit_handle = ev.getValidHandle<vector<raw::RawDigit>>(rawdigit_tag);
    auto const& allrawdigits_vec(*rawdigit_handle);
    

    int maxNumCounter = 0;
    for (size_t i_ar = 0, size_allrawdigits = rawdigit_handle->size(); i_ar != size_allrawdigits; ++i_ar) {
      int chanNum = allrawdigits_vec.at(i_ar).Channel();

      double maxVal = -9999999.0;
      //double maxValTime = -9999999.0;
      for (size_t k = 0; k < 9594; k++) {
        double val = allrawdigits_vec.at(i_ar).ADC(k);

        if (fabs(val) > maxVal) {
          maxVal = fabs(val);
          //maxValTime = k;
        }

        if (ev.eventAuxiliary().event() == 8) {
          if ((chanNum > 1000) && (chanNum < 1700)) {
            avgwfHist_Uplane_event8->Fill(k, val/700.0);
	  }
          else if ((chanNum > 3200) && (chanNum < 3900)) {
            avgwfHist_Vplane_event8->Fill(k, val/700.0);
	  }
          else if ((chanNum > 6000) && (chanNum < 6700)) {
            avgwfHist_Yplane_event8->Fill(k, val/700.0);
	  }
	}
        else if (ev.eventAuxiliary().event() == 9) {
          if ((chanNum > 1000) && (chanNum < 1700)) {
            avgwfHist_Uplane_event9->Fill(k, val/700.0);
	  }
          else if ((chanNum > 3200) && (chanNum < 3900)) {
            avgwfHist_Vplane_event9->Fill(k, val/700.0);
	  }
          else if ((chanNum > 6000) && (chanNum < 6700)) {
            avgwfHist_Yplane_event9->Fill(k, val/700.0);
	  }
	}
        else if (ev.eventAuxiliary().event() == 23) {
          if ((chanNum > 1000) && (chanNum < 1700)) {
            avgwfHist_Uplane_event23->Fill(k, val/700.0);
	  }
          else if ((chanNum > 3200) && (chanNum < 3900)) {
            avgwfHist_Vplane_event23->Fill(k, val/700.0);
	  }
          else if ((chanNum > 6000) && (chanNum < 6700)) {
            avgwfHist_Yplane_event23->Fill(k, val/700.0);
	  }
	}
        else if (ev.eventAuxiliary().event() == 32) {
          if ((chanNum > 1000) && (chanNum < 1700)) {
            avgwfHist_Uplane_event32->Fill(k, val/700.0);
	  }
          else if ((chanNum > 3200) && (chanNum < 3900)) {
            avgwfHist_Vplane_event32->Fill(k, val/700.0);
	  }
          else if ((chanNum > 6000) && (chanNum < 6700)) {
            avgwfHist_Yplane_event32->Fill(k, val/700.0);
	  }
	}
      }

      if (maxVal > 4000) {
        maxNumCounter++;
      }

      if (ev.eventAuxiliary().event() == 8) {
        maxvalHist_event8->SetBinContent(chanNum, maxVal);
      }
      else if (ev.eventAuxiliary().event() == 9) {
        maxvalHist_event9->SetBinContent(chanNum, maxVal);
      }
      else if (ev.eventAuxiliary().event() == 23) {
        maxvalHist_event23->SetBinContent(chanNum, maxVal);
      }
      else if (ev.eventAuxiliary().event() == 32) {
        maxvalHist_event32->SetBinContent(chanNum, maxVal);
      }
    }

    if (maxNumCounter > 100) {
      cout << "CATHODE HV BURST CANDIDATE EVENT" << endl;
    }

    auto t_end = high_resolution_clock::now();
    duration<double,std::milli> time_total_ms(t_end-t_begin);
    cout << "\tEvent took " << time_total_ms.count() << " ms to process." << endl;
  }

  f_output.Write();
  f_output.Close();
}
