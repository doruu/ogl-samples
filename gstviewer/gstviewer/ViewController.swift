//
//  ViewController.swift
//  gstviewer
//
//  Created by kmlee on 2017. 3. 10..
//  Copyright © 2017년 vicsway. All rights reserved.
//

import Cocoa

class ViewController: NSViewController {

  override func viewDidLoad() {
    super.viewDidLoad()

    let gltestview : GstView = GstView(frame: super.view.frame, pixelFormat: nil)!
    view.addSubview(gltestview)
  }

  override var representedObject: Any? {
    didSet {
    // Update the view, if already loaded.
    }
  }


}

