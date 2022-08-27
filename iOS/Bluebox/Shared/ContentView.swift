//
//  ContentView.swift
//  Shared
//
//  Created by Hayden Pennington on 8/11/22.
//

import SwiftUI

struct ContentView: View {
    @State private var isFuzzOn = false
    @State private var isTremoloOn = false
    @StateObject private var bluetoothManager = BluetoothManager()
    
    var body: some View {
        GeometryReader { geometry in
            HStack {
                Spacer()
                VStack(alignment: .center) {
                    Spacer()
                    Text(bluetoothManager.peripheralName)
                    Circle()
                        .foregroundColor(bluetoothManager.isConnected ? .blue : .red)
                        .frame(width: 40, height: 40)
                    Spacer()
                    Toggle("Fuzz", isOn: $isFuzzOn)
                    Toggle("Tremolo", isOn: $isTremoloOn)
                    Spacer()
                        .frame(height: 40)
                    Button(!bluetoothManager.isConnected ? "Connect" : "Disconnect") {
                        if bluetoothManager.isConnected {
                            bluetoothManager.disconnect()
                        } else {
                            bluetoothManager.connect()
                        }
                    }
                    .frame(width: 200, height: 40)
                    .background(.indigo)
                    .foregroundColor(.white)
                    .cornerRadius(4)
                    Spacer()
                        .frame(height: 40)
                }
                .frame(maxWidth: geometry.size.width * 0.8)
                .frame(height: geometry.size.height)
                Spacer()
            }
            
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
