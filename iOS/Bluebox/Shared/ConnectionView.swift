//
//  ConnectionView.swift
//  Bluebox
//
//  Created by Hayden Pennington on 9/16/22.
//

import SwiftUI
import Combine

struct ConnectionView: View {
    let isConnected: Bool
    @Binding var showConnectedScreen: Bool
    let onConnection: () -> Void
    
    @State private var isAnimating = false
    @State private var oneCycleAnimated = false
    
    var body: some View {
        VStack {
            Spacer()
                .frame(height: 60)
            ConnectingIndicatorView(isAnimating: $isAnimating) {
                oneCycleAnimated = true
            }
            Spacer()
                .frame(height: 60)
            Text("Press connect to add effects")
            Spacer()
            BLButtonView("Connect") {
                isAnimating = true
                onConnection()
            }
            .onChange(of: Just(isConnected).combineLatest(Just(oneCycleAnimated))) { newValue in
                if oneCycleAnimated {
                    showConnectedScreen = isConnected
                }
            }
        }
    }
}

struct ConnectionView_Previews: PreviewProvider {
    static var previews: some View {
        ConnectionView(isConnected: false, showConnectedScreen: .constant(false)) {
        }
    }
}
