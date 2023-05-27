//
//  ResultView.swift
//  Spec2017
//
//  Created by Junjie on 02/03/2023.
//

import SwiftUI

struct ResultView: View {
    @Binding var runTimes: [String:[Double]]
    @State var geoMean: Double = 1
    @State var geoMean_power: Double = 1
    let refMachine: [String:Double] =
    [
        "500.perlbench_r": 1591,
        "502.gcc_r": 1415,
        "505.mcf_r": 1615,
        "520.omnetpp_r": 1311,
        "523.xalancbmk_r": 1055,
        "525.x264_r": 1751,
        "531.deepsjeng_r": 1145,
        "541.leela_r": 1655,
        "548.exchange2_r": 2619,
        "557.xz_r": 1076,
        // FP score
        "503.bwaves_r": 10026,
        "507.cactuBSSN_r": 1264,
        "508.namd_r": 949,
        "510.parest_r": 2615,
        "511.povray_r": 2334,
        "519.lbm_r": 1026,
        "521.wrf_r": 2239,
        "526.blender_r": 1521,
        "527.cam4_r": 1748,
        "538.imagick_r": 2486,
        "544.nab_r": 1682,
        "549.fotonik3d_r": 3897,
        "554.roms_r": 1588,
        
    ]
    
    var body: some View {
        List {
            Section(header: Text("Integer")) {
                ForEach(runTimes.sorted(by: { $0.key < $1.key }), id: \.key) { bench, result in
                    HStack {
                        Text(bench)
                        Spacer()
                        VStack (alignment: .trailing) {
                            let score = refMachine[bench]! / result[0];
                            
                            Text("\(String(format: "%.2f", score))")
                            Text("\(String(format: "%.2f", result[1]))w   \(String(format: "%.2f", result[0]))s")
                                .font(.system(size: 13))
                                .foregroundColor(.gray)
                        }
                    }
                }
                HStack {
                    Text("Geometry Mean");
                    Spacer()
                    VStack (alignment: .trailing) {
                        Text("\(String(format: "%.2f", geoMean))")
                        Text("\(String(format: "%.2f", geoMean_power))w")
                            .font(.system(size: 13))
                            .foregroundColor(.gray)
                    }
                }
            }
            
            
        }.onAppear() {
            for (bench, time) in runTimes {
                geoMean = geoMean * (refMachine[bench]! / Double(time[0]));
                geoMean_power = geoMean_power * time[1];
            }
            geoMean = pow(geoMean, Double(1.0 / Double(runTimes.count)))
            geoMean_power = pow(geoMean_power, Double(1.0 / Double(runTimes.count)))
        }
    }
}

struct ResultView_Previews: PreviewProvider {
    @State private static var runTimes: [String:[Double]] =
    [
        "557.xz_r": [343, 3.2],
        "502.gcc_r": [226, 3.0],
        "505.mcf_r": [158, 2.9],
    ]

    static var previews: some View {
        ResultView(runTimes: $runTimes)
    }
}

/* 500: 359MB*/
/* 531: 700MB*/
/* 557: 1.5GB : peak 1.7G*/
