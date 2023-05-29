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
                .onAppear { UIApplication.shared.isIdleTimerDisabled = true }
        }
    }
}
