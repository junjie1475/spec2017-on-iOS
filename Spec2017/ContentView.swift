//
//  ContentView.swift
//  Spec2017
//
//  Created by Junjie on 01/03/2023.
//

import SwiftUI


struct Ocean: Identifiable, Hashable {
    let name: String
    let id = UUID()
}
enum BenchmarkSelection : String, CaseIterable {
    case Int = "Integer"
    case Fp = "Floating Point"
}


// global variables use by threads
var results: [Double] = [0, 0]
var bench_: String = ""
var running_: Int = 0
var testECore_: Bool = false

func runBench(_ bench: String, _ testECore: Bool) -> [Double] {
    bench_ = bench
    testECore_ = testECore
    
    var thread: pthread_t? = nil
    var qosAttribute = pthread_attr_t()
    pthread_attr_init(&qosAttribute)
    var size: Int = 50000 * 1024;
    pthread_attr_setstacksize(&qosAttribute, size)
    running_ = 1
    pthread_create(&thread, &qosAttribute, { arg in
        if(testECore_) {
            var param = sched_param()
            param.sched_priority = 6
            pthread_setschedparam(pthread_self(), SCHED_OTHER, &param)
        }
        
        // 1. setup the running directory
        let benchRunPath = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)[0]
            .path() + bench_
        do {
            let bundleInputPath = Bundle.main.bundlePath + "/Input/" + bench_
            try FileManager.default.copyItem(atPath: bundleInputPath, toPath: benchRunPath)
        } catch _ as NSError {
            // do nothing if directory already exists
        }
        
        // 2. set current directory to run directory
        FileManager.default.changeCurrentDirectoryPath(benchRunPath)
        
        // 3. run the benchmark
        specEntry(bench_, &results)
        running_ = 0
        
        return UnsafeMutableRawPointer.allocate(byteCount: 1, alignment: 1)
    }, nil)
    
    // since pthread_join() will effects the E-Core run, scan memory every 5s to check the state.
    while(true) {
        sleep(5);
        if(running_ == 0) {
            break;
        }
    }
    return results
}

struct ContentView: View {
    @State private var selection = Set<String>()
    @State var segmentationSelection : BenchmarkSelection = .Int
    @State private var isEditMode: EditMode = .active
    @State private var isRunning = false
    @State private var currentBench = ""
    @State private var currentIndex = 0
    @State private var runnedBench: [String] = []
    @State private var testECore = false
    let itemsInt = [
        "500.perlbench_r",
        "502.gcc_r",
        "505.mcf_r",
        "520.omnetpp_r",
        "523.xalancbmk_r",
        "525.x264_r",
        "531.deepsjeng_r",
        "541.leela_r",
        "548.exchange2_r",
        "557.xz_r",
    ]
    
    let itemsFp = [
        "503.bwaves_r",
        "507.cactuBSSN_r",
        "508.namd_r",
        "510.parest_r",
        "519.lbm_r",
        "527.cam4_r",
        "526.blender_r",
        "538.imagick_r",
        "544.nab_r",
        "549.fotonik3d_r",
        "554.roms_r",
    ]
    
    @State private var isPresentingConfirm: Bool = false
    
    @State private var runTimes: [String:[Double]] = [:]
    @State private var path: [[String:[Double]]] = []
    var body: some View {
        ZStack {
            NavigationStack(path: $path) {
                VStack(spacing: 0) {
                    Picker("", selection: $segmentationSelection) {
                        ForEach(BenchmarkSelection.allCases, id: \.self) { option in
                            Text(option.rawValue).disabled(true)
                        }
                    }
                    .pickerStyle(.segmented)
                    
                    List(selection: $selection) {
                        ForEach(segmentationSelection == BenchmarkSelection.Int ? itemsInt : itemsFp, id: \.self) { name in
                            Text(name)
                        }
                        Section(footer: VStack {
                            Text("\n\nCompiler            : Apple Clang14/Flang17\nCompiler Flags : -Ofast -arch arm64\nAuthor                 : junjie1475;jht5132").font(.system(size: 16)).bold()
                        }) {}
                    }
                         .environment(\.editMode, self.$isEditMode)
                         .listStyle(.grouped)
                    
                    HStack {
                        Button(action: {
                            for string in segmentationSelection == BenchmarkSelection.Int ? itemsInt : itemsFp {
                                selection.insert(string)
                            }
                        }, label: {Text("Select All")})
                        Spacer()
                        Button(action: {
                            testECore = !testECore
                        }, label: {Text(testECore ? "Test P Core" : "Test E Core")})
                        Spacer()
                        Button(action: {
                            // check if already runned
                            for bench in selection {
                                if runnedBench.contains(bench) {
                                    isPresentingConfirm = true
                                    return
                                }
                            }
                            // run the selected benchmarks
                            currentIndex = 0
                            currentBench = ""
                            isRunning = true
                            runTimes.removeAll()
                            
                            Task.detached {
                                for bench in selection.sorted(by: {$0 < $1}) {
                                    currentBench = bench
                                    currentIndex += 1
                                    runnedBench.append(bench)
                                    runTimes[bench] = runBench(bench, testECore)
                                }
                                isRunning = false
                                path.append(runTimes)
                            }

                        }, label: {Text("Run")})
                        .disabled(selection.isEmpty)
                        .confirmationDialog("Are you sure?",
                                            isPresented: $isPresentingConfirm) {
                            Button("Quit the App?", role: .destructive) {
                                exit(0)
                            }
                        } message: {
                            Text("Due to some ios limitation, you need to restart the App to run spec")
                        }
                        .navigationDestination(for: [String:[Double]].self) { runTimes in
                            ResultView(runTimes: $runTimes).navigationTitle("Result")
                        }
                    }.padding()
                }
                .navigationTitle("SPECrate2017")
            }
            if isRunning == true {
                VisualEffectView(effect: UIBlurEffect(style: .dark)).ignoresSafeArea(.all)
                VStack {
                    Text("Running \(currentBench)[\(currentIndex)/\(selection.count)]")
                        .font(.title)
                        .foregroundColor(.white)
                }
            }
        }
    }
}

struct VisualEffectView: UIViewRepresentable {
    var effect: UIVisualEffect?
    func makeUIView(context: UIViewRepresentableContext<Self>) -> UIVisualEffectView { UIVisualEffectView() }
    func updateUIView(_ uiView: UIVisualEffectView, context: UIViewRepresentableContext<Self>) { uiView.effect = effect }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
