//
//  ConnectedView.swift
//  Bluebox
//
//  Created by Hayden Pennington on 9/16/22.
//

import SwiftUI
import Combine

struct ConnectedView: View {
    @StateObject var bluetoothManager: BluetoothManager
    let disconnect: () -> Void
    
    @State private var prevOutput = (false, false)
    
    var changeEffectPublisher: AnyPublisher<(Bool, Bool), Never> {
        return bluetoothManager.$isFuzzOn.combineLatest(bluetoothManager.$isTremoloOn).eraseToAnyPublisher()
    }
    
    var body: some View {
        VStack {
            Spacer()
            List {
                Toggle("Fuzz", isOn: $bluetoothManager.isFuzzOn)
                Toggle("Tremolo", isOn: $bluetoothManager.isTremoloOn)
            }
            Spacer()
            BLButtonView("Disconnect") {
                disconnect()
            }
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

struct ConnectedView_Previews: PreviewProvider {
    static var previews: some View {
        ConnectedView(bluetoothManager: BluetoothManager()) {
            
        }
    }
}

