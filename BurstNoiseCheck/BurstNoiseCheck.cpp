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
    
    
    const int MaxNevents = 10;
    Printf ("will process %d events",MaxNevents);
    const int Nwires = 8256 , Ntime = 9594;
    //    std::vector<int> events_9778 = {8 , 9 , 23 , 32};
    //    std::vector<int> events_9975 = {88 , 89 , 90 , 91 , 92 , 93 , 94 , 95};
    //    std::vector<int> events_4172 = {20852 , 20853 , 20854 , 20855 , 20856 , 20857};
    std::vector<TH1F*> waveform_wire[MaxNevents+1];
    TH1F * htmp;
    int NeventsFound=0;
    
    
    vector<string> filenames;
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/46/PhysicsRun-2017_2_16_14_4_55-0010046-00000_20170216T211829_ext_unbiased_20170216T215439_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/40/PhysicsRun-2017_2_16_13_15_3-0010040-00000_20170216T202451_ext_unbiased_20170216T215519_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/42/PhysicsRun-2017_2_16_13_31_0-0010042-00001_20170216T203110_ext_unbiased_20170216T215559_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/44/PhysicsRun-2017_2_16_13_48_1-0010044-00008_20170216T212034_ext_unbiased_20170216T215720_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/41/PhysicsRun-2017_2_16_13_21_52-0010041-00004_20170216T204725_ext_unbiased_20170216T215705_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00095_20170216T202142_ext_unbiased_20170216T215800_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00080_20170216T210115_ext_unbiased_20170216T225626_merged.root");
    
    
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00008_20170216T205043_ext_unbiased_20170216T225939_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00024_20170216T215220_ext_unbiased_20170216T231400_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00087_20170216T214508_ext_unbiased_20170216T231557_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00000_20170216T215219_ext_unbiased_20170216T232828_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00045_20170216T220046_ext_unbiased_20170216T234720_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00065_20170216T220119_ext_unbiased_20170217T000954_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00034_20170216T210800_ext_unbiased_20170217T020333_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00035_20170216T205921_ext_unbiased_20170217T020334_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00006_20170216T211629_ext_unbiased_20170217T020638_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00038_20170216T212538_ext_unbiased_20170217T020655_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00007_20170216T220143_ext_unbiased_20170217T020835_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00040_20170216T211650_ext_unbiased_20170217T020909_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00042_20170216T221805_ext_unbiased_20170217T021220_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00012_20170216T212524_ext_unbiased_20170217T021045_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00043_20170216T214319_ext_unbiased_20170217T021447_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00044_20170216T205929_ext_unbiased_20170217T022025_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00013_20170216T214317_ext_unbiased_20170217T022435_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00031_20170216T220025_ext_unbiased_20170217T023015_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00014_20170216T213418_ext_unbiased_20170217T022642_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00026_20170216T220909_ext_unbiased_20170217T022853_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00015_20170216T210733_ext_unbiased_20170217T022830_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00003_20170216T221128_ext_unbiased_20170217T022942_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00027_20170216T215142_ext_unbiased_20170217T022906_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00028_20170216T221731_ext_unbiased_20170217T022949_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00030_20170216T213427_ext_unbiased_20170217T023034_merged.root");
    filenames.push_back("/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/01/00/39/PhysicsRun-2017_2_16_11_52_49-0010039-00029_20170216T214305_ext_unbiased_20170217T023040_merged.root");

    //                        "/pnfs/uboone/scratch/users/uboonepro/files_for_erez_feb15/PhysicsRun-2015_12_18_16_21_12-0004172-00417_20170215T212657.root");
    //                        "/pnfs/uboone/data/uboone/raw/run2_swizzle_trigger_streams/mergeext_unbiased/prod_run2_v05_08_00_03/00/00/99/75/PhysicsRun-2017_2_10_15_7_10-0009975-00008_20170211T081310_ext_unbiased_20170211T170653_merged.root");
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
        //        if (frun==9778 && fevent!=events_9778[0] && fevent!=events_9778[1] && fevent!=events_9778[2] && fevent!=events_9778[3]) continue;
        //        if (frun==9975
        //            && fevent!=events_9975[0] && fevent!=events_9975[1]
        //            && fevent!=events_9975[2] && fevent!=events_9975[3]
        //            && fevent!=events_9975[4] && fevent!=events_9975[5]
        //            && fevent!=events_9975[6] && fevent!=events_9975[7]) continue;
        //        if (frun==4172
        //            && fevent!=events_4172[0] && fevent!=events_4172[1]
        //            && fevent!=events_4172[2] && fevent!=events_4172[3]
        //            && fevent!=events_4172[4] && fevent!=events_4172[5] ) continue;
        if (    (frun==10039 && fsubrun==0 && fevent==15)
            ||  (frun==10039 && fsubrun==1 && fevent==59)
            ||  (frun==10039 && fsubrun==12 && fevent==602)
            ||  (frun==10039 && fsubrun==14 && fevent==735)
            ||  (frun==10039 && fsubrun==16 && fevent==833)
            ||  (frun==10039 && fsubrun==16 && fevent==839)
            ||  (frun==10039 && fsubrun==16 && fevent==843)
            ||  (frun==10039 && fsubrun==19 && fevent==957)
            ||  (frun==10039 && fsubrun==2 && fevent==137)
            ||  (frun==10039 && fsubrun==20 && fevent==1029)
            ){
            
            
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
    }
    Printf("writing tree into file...");
    f_output.Write();
    f_output.Close();
    cout << "done." << endl;
}

