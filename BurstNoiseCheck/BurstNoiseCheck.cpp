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

    
    const int MaxNevents = 1;
    Printf ("will process %d events",MaxNevents);
    const int Nwires = 8256 , Ntime = 9594;
    std::vector<int> events_9778 = {8 , 9 , 23 , 32};
    std::vector<TH1F*> waveform_wire[MaxNevents+1];
    TH1F * htmp;
    int NeventsFound=0;
    
    
    vector<string> filenames;
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/00/99/75/PhysicsRun-2017_2_10_15_7_10-0009975-00008_20170211T081310_ext_unbiased_20170211T170653_merged.root");
//    for (int i=51;i<53;i++){
//        filenames.push_back(Form("/pnfs/uboone/scratch/users/wketchum/HVBurstFilter/hvburst_7kV/141899%d_%d/PhysicsRun*.root",))
//    }
//    {
//        "/pnfs/uboone/scratch/users/wketchum/HVBurstFilter/hvburst_7kV/14190416_7/PhysicsRun-2017_2_2_9_38_27-0009854-00084_20170202T201433_bnb_20170202T210500_merged_hv_burst_filtered_20170208T0157480.root" // 7kV burst
//        ,"/pnfs/uboone/scratch/users/wketchum/HVBurstFilter/hvburst_7kV/14189948_98/PhysicsRun-2017_2_2_9_38_27-0009854-00021_20170202T221220_bnb_unbiased_20170202T224358_merged_hv_burst_filtered_20170208T0148110.root" // 7kV burst
//        ,"/pnfs/uboone/scratch/users/wketchum/HVBurstFilter/hvburst_7kV/14190205_1/PhysicsRun-2017_2_2_9_38_27-0009854-00035_20170202T182854_ext_bnb_20170202T195753_merged_hv_burst_filtered_20170208T0210110.root" // 7kV burst
//        "/uboone/data/users/ecohen/BurstNoiseCheckEvents/cathodeHVburstEvent.root", // -65 kV burst events
//        "/uboone/data/users/ecohen/BurstNoiseCheckEvents/PhysicsRun-2017_2_3_21_48_5-0009888-00030_20170204T080336_ext_unbiased_20170204T095434_merged_hv_burst_filtered_20170207T1813510.root",  // 0 kV burst events
//        "/uboone/data/users/ecohen/BurstNoiseCheckEvents/PhysicsRun-2017_2_3_21_48_5-0009888-00033_20170204T082215_ext_numi_20170204T095414_merged_hv_burst_filtered_20170207T1754210.root", // 0 kV burst events
//        "/uboone/data/users/ecohen/BurstNoiseCheckEvents/PhysicsRun-2017_2_3_21_48_5-0009888-00097_20170204T104214_mucs_20170204T131409_merged_hv_burst_filtered_20170207T1745000.root"  // 0 kV burst events
//    };
    
    Printf("found the followning R/S/E in events gallary:");
    for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {
        NeventsFound++;
        int frun = (int)ev.eventAuxiliary().run() , fsubrun = (int)ev.eventAuxiliary().subRun()  , fevent = (int)ev.eventAuxiliary().event();
        cout << "run " << frun << " sub " << fsubrun << " event " << fevent << endl ;
    }
    cout << "In total, " << NeventsFound << " events"<< endl;
    //    cout << "returning..." << endl;
    //    return 0;

    
    TFile f_output("/uboone/data/users/ecohen/BurstNoiseCheckEvents/burst_events_7kV.root","RECREATE");
    
    
    // Feb-09
    for (int event_index=0 ; event_index < MaxNevents+1 ; event_index++){
        for (int i=0 ; i<Nwires ; i++ ){
            htmp = new TH1F(Form("h_e%d_w%d",event_index,i),"",Ntime,-0.5,Ntime-0.5);
            waveform_wire[event_index].push_back(htmp);
            del htmp;
        }
    }
    std::cout << "initialized wave-form histograms" << std::endl;
    // ----------------------------------------------------------------------------

    
    //InputTag rawdigit_tag { "digitfilter" };
    InputTag rawdigit_tag { "daq" };
    
    int event_index = 0 ;

    for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {
        if (event_index>MaxNevents) break;
        auto t_begin = high_resolution_clock::now();
        int frun = (int)ev.eventAuxiliary().run() , fsubrun = (int)ev.eventAuxiliary().subRun()  , fevent = (int)ev.eventAuxiliary().event();
        if (frun==9778 && fevent!=events_9778[0] && fevent!=events_9778[1] && fevent!=events_9778[2] && fevent!=events_9778[3]) continue;
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
            if (i_ar%200==0) std::cout << Form("processed %.0f",(100.0*float(i_ar)/Nwires)) << " % of wires in event " << fevent << " (wire " << wire << ")" << endl;
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
        cout << "\t run" << frun  << " (event " << event_index << ") done. it took me " << time_total_ms.count() << " ms to process." << endl;
        event_index ++ ;
    }
    Printf("writing tree into file...");
    f_output.Write();
    f_output.Close();
    cout << "done." << endl;
}

