//
//  ConnectingIndicator.swift
//  Bluebox
//
//  Created by Hayden Pennington on 9/16/22.
//

import SwiftUI

struct ConnectingIndicatorView: View {
    let diameter: CGFloat = 128
    let strokeWidth: CGFloat = 12
    @Binding var isAnimating: Bool
    let didFinishAnimationCycle: () -> Void
    
    @State private var trimEnd = 1.0
    @State private var rotationAngle: CGFloat = 0.0
    @State private var timer: Timer?

    var body: some View {
        ZStack {
            Circle()
                .trim(from: 0.0, to: trimEnd)
                .rotation(.init(degrees: isAnimating ? rotationAngle : 0))
                .stroke(isAnimating ? Color.blue : Color.green, lineWidth: 8)
                .frame(width: diameter, height: diameter)
                .onChange(of: isAnimating) { newValue in
                    if newValue == true {
                        trimEnd = 0.8
                        self.timer = Timer.scheduledTimer(withTimeInterval: 1/60, repeats: true) { _ in
                            rotationAngle += 5
                            
                            if rotationAngle == 360 {
                                didFinishAnimationCycle()
                            }
                            
                        }
                    } else {
                        trimEnd = 1.0
                    }
                }
        }
    }
}

struct ConnectingIndicatorView_Previews: PreviewProvider {
    static var previews: some View {
        ConnectingIndicatorView(isAnimating: .constant(true)) {
            
        }
    }
}

