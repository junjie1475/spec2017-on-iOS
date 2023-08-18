//
//  ResultView.swift
//  Spec2017
//
//  Created by Junjie on 02/03/2023.
//

import SwiftUI

struct ResultView: View {
    @Binding var runTimes: [String:[Double]]
    @Binding var frequency: Bool
    @Binding var testECore: Bool
    @Binding var benchResultPath: String
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
    func fmtp(_ x: Int, _ results: [Double]) -> String {
        return String(format: "%.2f", results[x]) + "w"
    }
    func fmtf(_ x: Int, _ results: [Double]) -> String {
        return String(format: "%.2f", results[x]) + "Mhz"
    }
    
    var body: some View {
        List {
            Section(header: Text("Integer")) {
                ForEach(runTimes.sorted(by: { $0.key < $1.key }), id: \.key) { bench, result in
                    if(frequency) {
                        DisclosureGroup(
                            content: {
                                VStack {
                                    HStack {
                                        Text("Average Power")
                                        Spacer()
                                        Text(fmtp(1, result))
                                    }
                                    HStack {
                                        Text("Min Power")
                                        Spacer()
                                        Text(fmtp(2, result))
                                    }
                                    HStack {
                                        Text("Max Power")
                                        Spacer()
                                        Text(fmtp(3, result))
                                    }
                                    HStack {
                                        Text("Average Frequency")
                                        Spacer()
                                        Text(fmtf(4, result))
                                    }
                                    HStack {
                                        Text("Min Frequency")
                                        Spacer()
                                        Text(fmtf(5, result))
                                    }
                                    HStack {
                                        Text("Max Frequency")
                                        Spacer()
                                        Text(fmtf(6, result))
                                    }
                                }
                            },
                            
                            label: {
                                HStack {
                                    Text(bench)
                                    Spacer()
                                    VStack (alignment: .trailing) {
                                        let score = refMachine[bench]! / result[0];
                                        Text("\(String(format: "%.2f", score))")
                                        Text("\(String(format: "%.2f", result[0]))s")
                                            .font(.system(size: 13))
                                            .foregroundColor(.gray)
                                    }
                                }
                            }
                        )
                    } else {
                        HStack {
                            Text(bench)
                            Spacer()
                            VStack (alignment: .trailing) {
                                let score = refMachine[bench]! / result[0];
                                Text("\(String(format: "%.2f", score))")
                                Text("\(fmtp(1, result))   \(String(format: "%.2f", result[0]))s")
                                    .font(.system(size: 13))
                                    .foregroundColor(.gray)
                            }
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
            
            Section(footer: VStack {
                Text("\n\nCompiler            : Apple Clang14/Flang17\nCompiler Flags : -Ofast -arch arm64\nAuthor                 : junjie1475;jht5132").font(.system(size: 16)).bold()
            }) {}
            
        }.onAppear() {
            for (bench, time) in runTimes {
                geoMean = geoMean * (refMachine[bench]! / Double(time[0]));
                geoMean_power = geoMean_power * time[1];
            }
            geoMean = pow(geoMean, Double(1.0 / Double(runTimes.count)))
            geoMean_power = pow(geoMean_power, Double(1.0 / Double(runTimes.count)))


            var buf = "Bench,Time(s),Score,Power(w),Frequency(Mhz),Core\n"
            for (bench, result) in Array(runTimes).sorted(by: {$0.0 < $1.0}) {
                let score = refMachine[bench]! / result[0];
                buf += bench + "," + String(format: "%.2f", result[0]) + "," + String(format: "%.2f", score) + "," + String(format: "%.2f", result[1]) + "," + String(format: "%.2f", result[4]) + "," + (testECore ? "E" : "P") + "\n"
            }

            buf += "GeoMean," + "\"\"," + String(format: "%.2f", geoMean) + "," + String(format: "%.2f", geoMean_power) + ",\"\",\"\""

            print(buf)
            do {
                try buf.write(toFile: benchResultPath + "/result.csv", atomically: true, encoding: .utf8)
            } catch
            {}
        }
    }
}

//struct ResultView_Previews: PreviewProvider {
//    @State private static var runTimes: [String:[Double]] =
//    [
//        "557.xz_r": [343, 3.2],
//        "502.gcc_r": [226, 3.0],
//        "505.mcf_r": [158, 2.9],
//    ]
//    @State private static var frequency: Bool = true;
//    static var previews: some View {
//        ResultView(runTimes: $runTimes, frequency: $frequency)
//    }
//}

/* 500: 359MB*/
/* 531: 700MB*/
/* 557: 1.5GB : peak 1.7G*/
