//
//  RenderManBridge.h
//  gltest
//
//  Created by kmlee on 2017. 3. 7..
//  Copyright © 2017년 vicsway. All rights reserved.
//

#ifndef RenderManBridge_h
#define RenderManBridge_h

#import <Foundation/Foundation.h>

@interface RenderManBridge : NSObject 
-(id)init;

-(void)glinit;
-(void)loadBuffer;
-(void)draw;
@end


#endif /* RenderManBridge_h */
