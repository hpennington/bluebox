//
//  BluetoothManager.swift
//  Bluebox
//
//  Created by Hayden Pennington on 8/27/22.
//

import Foundation
import CoreBluetooth

let serviceUUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e"

class BluetoothManager: NSObject, ObservableObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    @Published var isConnected = false
    
    var centralManager: CBCentralManager!
    var discoveredPeripheral: CBPeripheral?
    
    override init() {
        super.init()
        self.centralManager = CBCentralManager(delegate: self, queue: nil)
    }

    func connect() {
        if let discoveredPeripheral = discoveredPeripheral {
            centralManager.connect(discoveredPeripheral)
        }
    }
    
    func disconnect() {
        if let discoveredPeripheral = discoveredPeripheral {
            centralManager.cancelPeripheralConnection(discoveredPeripheral)
        }
    }
    
    var peripheralName: String {
        return "Device ID"
    }
    
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        if central.state == .poweredOn {
            centralManager.scanForPeripherals(withServices: [CBUUID(string: serviceUUID)])
        }

    }
    
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        print(peripheral)
        print(advertisementData)
        print(RSSI)
        
        discoveredPeripheral = peripheral
    }
    
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("didConnect")
        isConnected = true
    }
    
    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        isConnected = false
        print("didDisconnect")
        if let error = error {
            print("didDisconnect error: \(error) \(error.localizedDescription)")
        }
    }
}
