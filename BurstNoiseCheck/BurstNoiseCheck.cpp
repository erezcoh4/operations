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
    
    TFile f_output("/uboone/data/users/ecohen/BurstNoiseCheckEvents/burst_events_study_output.root","RECREATE");
    
    
    // Feb-09
    const int Nwires = 8256 , Ntime = 9594;
    const int MaxNevents = 30;
    //    std::vector<int> events = {8 , 9 , 23 , 32};
    std::vector<TH1F*> waveform_wire[MaxNevents];
    TH1F * htmp;
    for (int i=0 ; i<Nwires ; i++ ){
        for (int j=0 ; j < MaxNevents ; j++){
            htmp = new TH1F(Form("h_e%d_w%d",j,i),"",Ntime,-0.5,Ntime-0.5);
            waveform_wire[j].push_back(htmp);
        }
    }
    std::cout << "initialized wave-form histograms" << std::endl;
    // ----------------------------------------------------------------------------
    
    vector<string> filenames {
        "/uboone/data/users/ecohen/BurstNoiseCheckEvents/cathodeHVburstEvent.root",
        "/uboone/data/users/ecohen/BurstNoiseCheckEvents/PhysicsRun-2017_2_3_21_48_5-0009888-00030_20170204T080336_ext_unbiased_20170204T095434_merged_hv_burst_filtered_20170207T1813510.root",
    "/uboone/data/users/ecohen/BurstNoiseCheckEvents/PhysicsRun-2017_2_3_21_48_5-0009888-00033_20170204T082215_ext_numi_20170204T095414_merged_hv_burst_filtered_20170207T1754210.root",
    "/uboone/data/users/ecohen/BurstNoiseCheckEvents/PhysicsRun-2017_2_3_21_48_5-0009888-00097_20170204T104214_mucs_20170204T131409_merged_hv_burst_filtered_20170207T1745000.root"};
    
    //InputTag rawdigit_tag { "digitfilter" };
    InputTag rawdigit_tag { "daq" };
    
    int event_index = 0 ;

    for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {
        auto t_begin = high_resolution_clock::now();
        int frun = (int)ev.eventAuxiliary().run() , fsubrun = (int)ev.eventAuxiliary().subRun()  , fevent = (int)ev.eventAuxiliary().event();
        cout << "Processing " << "Run " <<  frun << ", " << "Subun " << fsubrun << ", " << "Event " << fevent << endl;
        
        // for originial run 9778 mike gave me
        //        for (int j=0 ; j < MaxNevents  ; j++){
        //            if ((int)ev.eventAuxiliary().event() == (int)events[j]) {
        //                event_index = j;
        //            }
        //        }
        //        if (event_index==-1) continue;
        
        auto const& rawdigit_handle = ev.getValidHandle<vector<raw::RawDigit>>(rawdigit_tag);
        auto const& allrawdigits_vec(*rawdigit_handle);
        
        
        // loop over the wires
        for (size_t i_ar = 0, size_allrawdigits = rawdigit_handle->size(); i_ar != size_allrawdigits; ++i_ar) {
            int chanNum = allrawdigits_vec.at(i_ar).Channel(); // chanNum matches the wire to the channel number
            int wire = chanNum;
            if (i_ar%200==0) cout << "processed "<< (100.0*float(i_ar)/Nwires) << " % " << "of the wires in event " << fevent << "(wire " << wire << ")" << endl;
            waveform_wire[event_index][wire] -> SetName(Form("h_wf_r%d_s%d_e%d_wire%d",frun,fsubrun,fevent,(int)wire));
            waveform_wire[event_index][wire] -> SetTitle(Form("r-%d/s-%d/e-%d, wire %d wave-form; time ;ADC",frun,fsubrun,fevent,(int)wire));
            
            // loop over the time ticks
            for (size_t k = 0; k < 9594; k++) {
                double ADC = allrawdigits_vec.at(i_ar).ADC(k);
                waveform_wire[event_index][chanNum] -> SetBinContent( k , ADC );
                
            }
        }
        
        auto t_end = high_resolution_clock::now();
        duration<double,std::milli> time_total_ms(t_end-t_begin);
        cout << "\t run" << frun  << " ( even " << event_index << ") "<< "processes. it took me " << time_total_ms.count() << " ms to process." << endl;
        event_index ++ ;
        if (event_index>MaxNevents) break;
    }
    
    f_output.Write();
    f_output.Close();
}

