//
//  RenderMan.hpp
//  gltest
//
//  Created by kmlee on 2017. 3. 7..
//  Copyright © 2017년 vicsway. All rights reserved.
//

#ifndef RenderMan_h
#define RenderMan_h

#include <iostream>

#import <OpenGL/OpenGL.h>
#import <OpenGL/gl3.h>

class RenderMan {
 public:
  
  RenderMan() = default;
  ~RenderMan() = default;
  
  void LoadBuffer();
  void LoadShader();
  void Draw();
  void AppMain();
  
  GLuint shader_program_;
  GLint position_uniform_;
  GLint colour_attribute_;
  GLint position_attribute_;
  GLint tex_uniform_;
  GLint texcoord_atrribute_;
  
  //
  char* texbuf_;
  GLuint texture_;
};

#endif /* RenderMan_h */
