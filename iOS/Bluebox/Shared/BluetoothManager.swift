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
    var discoveredCharacteristic: CBCharacteristic?
    
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
        discoveredPeripheral?.delegate = self
    }
    
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("didConnect")
        isConnected = true
        
        peripheral.discoverServices([CBUUID(string: serviceUUID)])
    }
    
    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        isConnected = false
        print("didDisconnect")
        if let error = error {
            print("didDisconnect error: \(error) \(error.localizedDescription)")
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        if let error = error {
            print("didDiscoverServices error: \(error) \(error.localizedDescription)")
            return
        }
        
        guard let services = peripheral.services else {
            return
        }
        
        for service in services {
            let characteristics = [CBUUID(string: "6E400002-B5A3-F393-E0A9-E50E24DCCA9E")]
            peripheral.discoverCharacteristics(characteristics, for: service)
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        if let error = error {
            print("didDiscoverCharacteristicsFor error: \(error) \(error.localizedDescription)")
            return
        }
        
        guard let characteristics = service.characteristics else {
            return
        }
        
        discoveredCharacteristic = characteristics.first!
        Timer.scheduledTimer(timeInterval: 1, target: self, selector: #selector(self.writeData), userInfo: nil, repeats: true)
//        let timer = Timer(fireAt: .now, interval: 1, target: self, selector: #selector(), userInfo: nil, repeats: true)
    }
    
    @objc func writeData() {
        if var data = "!A".data(using: .utf8) {
            data.append(22)
            data.append(42)
            data.append(100)
            print("writing value")
            discoveredPeripheral!.writeValue(data, for: discoveredCharacteristic!, type: .withoutResponse)
        }
    
    }
    
//    func peripheral(_ peripheral: CBPeripheral, didUpdateNotificationStateFor characteristic: CBCharacteristic, error: Error?) {
//        if let error = error {
//            print("didUpdateNotificationStateFor error: \(error) \(error.localizedDescription)")
//            return
//        }
//
//        print("Successfully subscribed to characteristic: \(characteristic)")
//    }
//
//    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
//        if let error = error {
//            print("didUpdateValueFor error: \(error) \(error.localizedDescription)")
//            return
//        }
//
//        if let data = characteristic.value {
//            print(data)
//        }
//    }

}
