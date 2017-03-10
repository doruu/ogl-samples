//
//  GstView.swift
//  gltest
//
//  Created by kmlee on 2017. 3. 7..
//  Copyright © 2017년 vicsway. All rights reserved.
//


import Foundation
import OpenGL.GL3
import GLKit

class GstView: NSOpenGLView {
  
  var width : GLsizei
  var height : GLsizei
  let renderman_bridge : RenderManBridge
  
  var display_link_ : CVDisplayLink?
  
  override init?(frame frameRect: NSRect, pixelFormat format: NSOpenGLPixelFormat?) {
    
    renderman_bridge = RenderManBridge()
    
    self.width = GLsizei(frameRect.size.width)
    self.height = GLsizei(frameRect.size.height)
    
    //setup pixelFormatAttributes
    let attrs: [NSOpenGLPixelFormatAttribute] = [
      UInt32(NSOpenGLPFAAccelerated),            //  Use accelerated renderers
      UInt32(NSOpenGLPFAColorSize), UInt32(32),  //  Use 32-bit color
      UInt32(NSOpenGLPFAOpenGLProfile),          //  Use version's >= 3.2 core
      UInt32(NSOpenGLProfileVersion3_2Core),
      UInt32(0)                                  //  C API's expect to end with 0
    ]
    
    let glPixelFormat = NSOpenGLPixelFormat(attributes:attrs)
    
    super.init(frame:frameRect, pixelFormat:glPixelFormat)
    
    self.openGLContext?.makeCurrentContext()
    
    renderman_bridge.glinit()
    
  }
  
  override func prepareOpenGL() {
    super.prepareOpenGL()
    
    let displayLinkOutputCallback: CVDisplayLinkOutputCallback =
      {(displayLink: CVDisplayLink, inNow: UnsafePointer<CVTimeStamp>,
        inOutputTime: UnsafePointer<CVTimeStamp>,
        flagsIn: CVOptionFlags, flagsOut: UnsafeMutablePointer<CVOptionFlags>,
        displayLinkContext: UnsafeMutableRawPointer?) -> CVReturn in
        
        unsafeBitCast(displayLinkContext, to: GstView.self).drawView()
        
        return kCVReturnSuccess
    }
    
    CVDisplayLinkCreateWithActiveCGDisplays(&display_link_)
    CVDisplayLinkSetOutputCallback(display_link_!, displayLinkOutputCallback,
                                   UnsafeMutableRawPointer(Unmanaged.passUnretained(self).toOpaque()))
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(display_link_!,
                                                      (self.openGLContext?.cglContextObj)!,
                                                      (self.pixelFormat?.cglPixelFormatObj)!)
    CVDisplayLinkStart(display_link_!)
  }
  
  required init?(coder: NSCoder) {
    fatalError("init(coder:) has not been implemented")
  }
  
  fileprivate func drawView() {
    
    guard let context = self.openGLContext else {
      Swift.print("draw error")
      return
    }
    
    context.makeCurrentContext()
    CGLLockContext(context.cglContextObj!)
    
    renderman_bridge.draw()
    
    CGLFlushDrawable(context.cglContextObj!)
    CGLUnlockContext(context.cglContextObj!)
  }
  
  
  //when window is reshaped
  override func reshape() {
    glViewport(0,0,self.width,self.height)
  }
}
