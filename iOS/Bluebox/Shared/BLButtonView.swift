//
//  BLButton.swift
//  Bluebox
//
//  Created by Hayden Pennington on 9/16/22.
//

import SwiftUI

struct BLButtonViewProperties {
    static let width: CGFloat = 148
    static let height: CGFloat = 48
    static let textColor: Color = .white
    static let backgroundColor: Color = .blue
    static let cornerRadius: CGFloat = 4
    static let horizontalPadding: CGFloat = 30
}

struct BLButtonView: View {
    let title: String
    let action: () -> Void
    
    init(_ title: String, action: @escaping () -> Void) {
        self.title = title
        self.action = action
    }
    
    var body: some View {
        Button(action: action) {
            Text(title)
                .frame(width: BLButtonViewProperties.width, height: BLButtonViewProperties.height)
                .foregroundColor(BLButtonViewProperties.textColor)
                .background(BLButtonViewProperties.backgroundColor)
                .cornerRadius(BLButtonViewProperties.cornerRadius)
        }
    }
}

struct BLButtonView_Previews: PreviewProvider {
    static var previews: some View {
        BLButtonView("Button") {
        }
    }
}
