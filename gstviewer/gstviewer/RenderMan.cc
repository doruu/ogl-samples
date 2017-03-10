//
//  RenderMan.cpp
//  gltest
//
//  Created by kmlee on 2017. 3. 7..
//  Copyright © 2017년 vicsway. All rights reserved.
//

#include "RenderMan.h"
#include <cmath>
#include <gst/gst.h>

#include "glm/vec4.hpp"
#include "glm/vec2.hpp"

const char* screen_vertex_shader_texture =
"#version 330\n\
uniform vec2 p;\
in vec4 position;\
in vec2 texCoord;\
out vec2 texCoordV;\
void main (void)\
{\
texCoordV = texCoord;\
gl_Position = vec4(p, 0.0, 0.0) + position;\
}";

const char* screen_fragment_shader_texture =
"#version 330\n\
out vec4 fragColour;\
uniform sampler2D tex;\
in vec2 texCoordV;\
void main(void)\
{\
fragColour = texture(tex,texCoordV);\
}";

namespace glh {
  
  bool CheckGlError(const char* op) {
    bool ret = false;
    for (GLint error = glGetError(); error; error = glGetError()) {
      switch (error) {
        case GL_INVALID_ENUM:
          printf("after %s() glError : GL_INVALID_ENUM (0x%x)\n", op, error);
          break;
        case GL_INVALID_VALUE:
          printf("after %s() glError : GL_INVALID_VALUE (0x%x)\n", op, error);
          break;
        case GL_INVALID_OPERATION:
          printf("after %s() glError : GL_INVALID_OPERATION (0x%x)\n", op, error);
          break;
        case GL_OUT_OF_MEMORY:
          printf("after %s() glError : GL_OUT_OF_MEMORY (0x%x)\n", op, error);
          break;
      }
      ret = true;
    }
    return ret;
  };
  
  GLuint GenTexture2D(GLsizei width, GLsizei height,
                      GLenum format, GLenum type, GLenum wrap, GLenum filter,
                      const GLvoid *pixels) {
    
    GLuint tex = 0;
    glActiveTexture(GL_TEXTURE0);
    CheckGlError("10");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    CheckGlError("11");
    glGenTextures(1, &tex);
    CheckGlError("12");
    glBindTexture(GL_TEXTURE_2D, tex);
    
    CheckGlError("13");
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    return tex;
  };
  
  void DeleteTexture(int size, unsigned int* tex) {
    glDeleteTextures(size, tex);
  };
  
  GLuint CreateProgram(const char* vert_shader, const char* frag_shader, bool* bret) {
    
    GLuint program = glCreateProgram();;
    
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vert_shader, NULL);
    glCompileShader(vs);
    
    GLint status = GL_FALSE;
    
    const int buf_size = 256;
    GLchar message[buf_size] = {0,};
    
    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      glGetShaderInfoLog(vs, buf_size, 0, &message[0]);
      printf("vertex shader compile failed : %s\n", message);
    }
    
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &frag_shader, NULL);
    glCompileShader(fs);
    
    status = GL_FALSE;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      glGetShaderInfoLog(fs, buf_size, 0, &message[0]);
      printf("frag shader compile failed : %s\n", message);
    }
    
    glAttachShader(program, fs);
    glAttachShader(program, vs);
    
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
      glGetProgramInfoLog(program, buf_size, 0, &message[0]);
      printf("gl link error : %s\n", message);
    }
    return program;
  };
}

typedef struct
{
  glm::vec4 position;
  glm::vec2 texcoord;
} Vertex;

void RenderMan::LoadBuffer() {
  
#if 0
  { // make checkimage
    int i, j, c;
    for (i=0; i<checkImageHeight; i++) {
      for (j=0; j<checkImageWidth; j++) {
        c = ((   ((i&0x8)==0) ^ ((j&0x8)==0)  ))*255;
        check_image_[i][j][0] = (GLubyte)c;
        check_image_[i][j][1] = (GLubyte)c;
        check_image_[i][j][2] = (GLubyte)c;
        check_image_[i][j][3] = (GLubyte)255;
      }
    }
  }
#endif
  
  GLuint vertexArrayObject, vertexBuffer;
  
  {
    Vertex vertex_data[4] = {
      { glm::vec4(-1.0, -1.0, 0.0, 1.0 ), glm::vec2( 0.0, 1.0 ) },
      { glm::vec4(-1.0,  1.0, 0.0, 1.0 ), glm::vec2( 0.0, 0.0 ) },
      { glm::vec4( 1.0,  1.0, 0.0, 1.0 ), glm::vec2( 1.0, 0.0 ) },
      { glm::vec4( 1.0, -1.0, 0.0, 1.0 ), glm::vec2( 1.0, 1.0 ) }
    };
    
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertex_data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(position_attribute_);
    glEnableVertexAttribArray(texcoord_atrribute_);
    glVertexAttribPointer(position_attribute_, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)0);
    glVertexAttribPointer(texcoord_atrribute_ ,4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)sizeof(glm::vec4));

  }
}

void RenderMan::LoadShader() {
  
  shader_program_ = glh::CreateProgram(screen_vertex_shader_texture, screen_fragment_shader_texture, nullptr);
  
  position_uniform_ = glGetUniformLocation(shader_program_, "p");
  if (position_uniform_ < 0) {
  }
  position_attribute_ = glGetAttribLocation(shader_program_, "position");
  if (position_attribute_ < 0) {
  }
  
  colour_attribute_ = glGetAttribLocation(shader_program_, "colour");
  if (colour_attribute_ < 0) {
  }
  
  tex_uniform_ = glGetUniformLocation(shader_program_, "tex");
  if (tex_uniform_ < 0) {
    
  }
  texcoord_atrribute_ = glGetAttribLocation(shader_program_, "texCoord");
  if (texcoord_atrribute_ < 0) {
    
  }
  glUseProgram(shader_program_);
}

void RenderMan::Draw() {
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  texture_ = glh::GenTexture2D(320, 240, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, GL_LINEAR, texbuf_);
  
  glUseProgram(shader_program_);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glUniform1i(tex_uniform_, 0);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  
  glh::DeleteTexture(1, &texture_);
  
  glFlush();
}

/* gst bus signal watch callback */
static void end_stream_cb (GstBus * bus, GstMessage * msg, GMainLoop * loop) {
  switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_EOS:
      g_print ("End-of-stream\n");
      g_print
      ("For more information, try to run: GST_DEBUG=gl*:3 ./sdlshare\n");
      break;
    case GST_MESSAGE_ERROR: {
      gchar *debug = NULL;
      GError *err = NULL;
      gst_message_parse_error (msg, &err, &debug);
      g_print ("Error: %s\n", err->message);
      g_error_free (err);
      if (debug) {
        g_print ("Debug deails: %s\n", debug);
        g_free (debug);
      }
      break;
    }
    default:
      break;
  }
  g_main_loop_quit (loop);
}

static gboolean sync_bus_call(GstBus * bus, GstMessage * msg, gpointer data) {
  switch (GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_NEED_CONTEXT: {
      const gchar *context_type;
      gst_message_parse_context_type(msg, &context_type);
      g_print ("got need context %s\n", context_type);
#if 0
      if (g_strcmp0 (context_type, GST_GL_DISPLAY_CONTEXT_TYPE) == 0) {
        GstContext *display_context = gst_context_new (GST_GL_DISPLAY_CONTEXT_TYPE, TRUE);
        gst_context_set_gl_display (display_context, sdl_gl_display);
        gst_element_set_context (GST_ELEMENT (msg->src), display_context);
        return TRUE;
      } else if (g_strcmp0 (context_type, "gst.gl.app_context") == 0) {
        GstContext *app_context = gst_context_new ("gst.gl.app_context", TRUE);
        GstStructure *s = gst_context_writable_structure (app_context);
        gst_structure_set (s, "context", GST_GL_TYPE_CONTEXT, sdl_context, NULL);
        gst_element_set_context (GST_ELEMENT (msg->src), app_context);
        return TRUE;
      }
#endif
      break;
    }
    default:
      break;
  }
  return FALSE;
}

/* fakesink handoff callback */
static void on_gst_buffer (GstElement * fakesink,
                           GstBuffer * buf,
                           GstPad * pad,
                           RenderMan* render_man) {
  
  gst_buffer_ref(buf);
  
  GstMapInfo map;
  gst_buffer_map (buf, &map, GST_MAP_READ);
  //gst_util_dump_mem ((const guchar *) map.data, map.size);
  
  memcpy(render_man->texbuf_, map.data, map.size);
  gst_buffer_unmap (buf, &map);
  gst_buffer_unref(buf);
  
}

void RenderMan::AppMain() {
  GMainLoop *loop = NULL;
  GstPipeline *pipeline = NULL;
  GstBus *bus = NULL;
  GstElement *fakesink = NULL;
  GstState state;
  // init gstramer
  gst_init(NULL, NULL);
  loop = g_main_loop_new(NULL, FALSE);
  //
  pipeline = GST_PIPELINE(gst_parse_launch("videotestsrc ! "
                                           "video/x-raw, width=320, height=240, "
                                           "format=(string)RGBA, framerate=(fraction)30/1 ! "
                                           "fakesink sync=1", NULL));
  //
  bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
  gst_bus_add_signal_watch(bus);
  g_signal_connect(bus, "message::error", G_CALLBACK(end_stream_cb), loop);
  g_signal_connect(bus, "message::warning", G_CALLBACK(end_stream_cb), loop);
  g_signal_connect(bus, "message::eos", G_CALLBACK(end_stream_cb), loop);
  gst_bus_enable_sync_message_emission(bus);
  g_signal_connect(bus, "sync-message", G_CALLBACK(sync_bus_call), NULL);
  gst_object_unref(bus);
  /*
   * NULL to PAUSED state pipeline to make sure
   * the gst opengl context is created and shared with the sdl one               
   */
  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PAUSED);
  state = GST_STATE_PAUSED;
  if (gst_element_get_state(GST_ELEMENT(pipeline), &state, NULL,
                            GST_CLOCK_TIME_NONE) != GST_STATE_CHANGE_SUCCESS) {
    g_debug ("failed to pause pipeline\n");
    //return -1;
  }
  /* append a gst-gl texture to this queue when you do not need it no more */
  fakesink = gst_bin_get_by_name(GST_BIN (pipeline), "fakesink0");
  g_object_set(G_OBJECT (fakesink), "signal-handoffs", TRUE, NULL);
  g_signal_connect(fakesink, "handoff", G_CALLBACK(on_gst_buffer),
                   (gpointer)this);
  //
  g_object_set_data(G_OBJECT(fakesink), "loop", loop);
  gst_object_unref(fakesink);
  
  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
  // main loop
  g_main_loop_run(loop);
  // clear
  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
  gst_object_unref(pipeline);
}

