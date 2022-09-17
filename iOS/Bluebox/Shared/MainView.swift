//
//  MainView.swift
//  Bluebox
//
//  Created by Hayden Pennington on 9/16/22.
//

import SwiftUI

struct MainView: View {
    @StateObject private var bluetoothManager = BluetoothManager()
    @State private var showConnectedScreen = false
    
    var body: some View {
        if bluetoothManager.isConnected && showConnectedScreen {
            ConnectedView(bluetoothManager: bluetoothManager) {
                bluetoothManager.disconnect()
                showConnectedScreen = false
            }
        } else {
            ConnectionView(isConnected: bluetoothManager.isConnected, showConnectedScreen: $showConnectedScreen) {
                bluetoothManager.connect()
            }
        }
    }
}
