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
        "557.xz_r": 1076
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
                    Text("\(String(format: "%.2f", geoMean))")
                }
            }
            
            
        }.onAppear() {
            for (bench, time) in runTimes {
                geoMean = geoMean * (refMachine[bench]! / Double(time[0]));
            }
            geoMean = pow(geoMean, Double(1.0 / Double(runTimes.count)))
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
