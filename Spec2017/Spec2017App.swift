//
//  Spec2017App.swift
//  Spec2017
//
//  Created by Junjie on 01/03/2023.
//

import SwiftUI

@main
struct Spec2017App: App {
    var body: some Scene {
        WindowGroup {
            ContentView()
                .onAppear {
                    UIApplication.shared.isIdleTimerDisabled = true
                    
                    let benchResultPath = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)[0]
                        .path() + "Results"

                    do {
                        try FileManager.default.createDirectory(atPath: benchResultPath, withIntermediateDirectories: false)
                    } catch _ as NSError {}
                }
        }
    }
}
