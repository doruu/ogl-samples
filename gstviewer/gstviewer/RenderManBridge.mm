//
//  RenderMan.m
//  gltest
//
//  Created by kmlee on 2017. 3. 7..
//  Copyright © 2017년 vicsway. All rights reserved.
//


#import "RenderManBridge.h"
#include "RenderMan.h"

@implementation RenderManBridge {
  RenderMan* render_man_;
}

-(id)init {
  self = [super init];
  if (!self) {
    return nil;
  }
  
  render_man_ = new RenderMan();
  render_man_->texbuf_ = (char*)malloc(320*240*4);
  dispatch_async(
      dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        render_man_->AppMain();
      }
  );
  return self;
}

-(void)glinit {
  render_man_->LoadShader();
  render_man_->LoadBuffer();
}

-(void)loadBuffer {
  render_man_->LoadBuffer();
}

-(void)draw {
  render_man_->Draw();
}
// dealloc ?


@end
