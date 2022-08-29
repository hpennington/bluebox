//
//  ContentView.swift
//  Shared
//
//  Created by Hayden Pennington on 8/11/22.
//

import SwiftUI
import Combine

struct ContentView: View {
    @StateObject private var bluetoothManager = BluetoothManager()
    
    @State private var prevOutput = (false, false)
    
    var changeEffectPublisher: AnyPublisher<(Bool, Bool), Never> {
        return bluetoothManager.$isFuzzOn.combineLatest(bluetoothManager.$isTremoloOn).eraseToAnyPublisher()
    }
    
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
                    Toggle("Fuzz", isOn: $bluetoothManager.isFuzzOn)
                    Toggle("Tremolo", isOn: $bluetoothManager.isTremoloOn)
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
            .onReceive(changeEffectPublisher) { output in
                if output != prevOutput {
                    print(output)
                    if output.0 == true && output.1 == true {
                        bluetoothManager.writeData(command: .all)
                    } else if output.0 == true && output.1 == false {
                        bluetoothManager.writeData(command: .fuzz)
                    } else if output.0 == false && output.1 == true {
                        bluetoothManager.writeData(command: .tremolo)
                    } else {
                        bluetoothManager.writeData(command: .noEffect)
                    }
                    
                    prevOutput = output
                }
                
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
