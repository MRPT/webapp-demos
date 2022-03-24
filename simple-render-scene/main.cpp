/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2022, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengles2.h>

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES3/gl3.h>
#include <assert.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <mrpt/math/TPose3D.h>
#include <mrpt/opengl/CAxis.h>
#include <mrpt/opengl/CBox.h>
#include <mrpt/opengl/CGridPlaneXY.h>
#include <mrpt/opengl/COpenGLScene.h>
#include <mrpt/opengl/CSphere.h>
#include <mrpt/opengl/CText.h>
#include <stdio.h>

#include <iostream>

#define GL_CALL(x)                                                             \
  {                                                                            \
    x;                                                                         \
    GLenum error = glGetError();                                               \
    if (error != GL_NO_ERROR) {                                                \
      printf("GL ERROR: %d,  %s\n", (int)error, #x);                           \
      assert(false);                                                           \
    }                                                                          \
  }

static bool quitting = false;
static SDL_Window *window = NULL;
static SDL_GLContext gl_context;
static mrpt::opengl::COpenGLScene::Ptr theScene;

void render() {
  SDL_GL_MakeCurrent(window, gl_context);

  theScene->render();

  SDL_GL_SwapWindow(window);
}

void update() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quitting = true;
    }
  }

  render();
};

// ------------------------------------------------------
//				TestDisplay3D
// ------------------------------------------------------
void TestDisplay3D() {
  using namespace mrpt;
  using namespace mrpt::opengl;

  theScene = COpenGLScene::Create();

  // Add a clone viewport, using [0,1] factor X,Y,Width,Height coordinates:
  {
    COpenGLViewport::Ptr vi = theScene->createViewport("myClone");
    vi->setViewportPosition(0.7, 0.05, 0.28, 0.28);
    vi->setCloneView("main");
    vi->setTransparent(true);
    vi->getCamera().setAzimuthDegrees(45);
    vi->getCamera().setElevationDegrees(45);
    vi->getCamera().setZoomDistance(10);
  }

  // Another clone viewport, using absolute coordinates
  {
    COpenGLViewport::Ptr vi = theScene->createViewport("myClone2");
    vi->setViewportPosition(
        /*x px*/ -250, /*y px*/ -250, /*width px*/ 250,
        /*height px*/ 200); // x,y negative means pixels from the
    // top/right, instead of from the bottom/left.
    vi->setCloneView("main");
    vi->setTransparent(false);
    vi->getCamera().setAzimuthDegrees(-95);
    vi->getCamera().setElevationDegrees(30);
    vi->getCamera().setZoomDistance(8);
  }

  // And another transparent viewport just to show 3D text:
  if (false) {
    mrpt::opengl::CText::Ptr txt1 = mrpt::opengl::CText::Create();
    COpenGLViewport::Ptr vi = theScene->createViewport("flat_viewport");
    vi->setViewportPosition(0, 0, 0.3, 0.3);
    vi->setTransparent(true);
    vi->setBorderSize(0);
    vi->getCamera().setAzimuthDegrees(0);
    vi->getCamera().setElevationDegrees(90);
    vi->getCamera().setZoomDistance(5);
    vi->getCamera().setOrthogonal(true);

    vi->insert(txt1);
  }

  // Modify the scene:
  // ------------------------------------------------------
  {
    opengl::CGridPlaneXY::Ptr obj =
        opengl::CGridPlaneXY::Create(-20, 20, -20, 20, 0, 1);
    obj->setColor(0.8f, 0.8f, 0.8f);
    theScene->insert(obj);
  }

  {
    opengl::CAxis::Ptr obj = opengl::CAxis::Create();
    obj->setFrequency(5);
    obj->enableTickMarks();
    obj->setAxisLimits(-10, -10, -10, 10, 10, 10);
    theScene->insert(obj);
  }

  {
    opengl::CBox::Ptr obj = opengl::CBox::Create();
    obj->setWireframe(false);
    obj->setColor(1, 0, 0);
    obj->setLineWidth(3.0);
    obj->setPose(mrpt::math::TPose3D(10, 0, 0, 0.2, 0.3, 0.1));
    theScene->insert(obj);
  }

  {
    opengl::CSphere::Ptr obj = opengl::CSphere::Create();
    obj->setColor(0, 0, 1);
    obj->setRadius(0.3f);
    obj->setLocation(0, 0, 1);
    obj->setName("ball_1");
    theScene->insert(obj);
  }
  {
    opengl::CSphere::Ptr obj = opengl::CSphere::Create();
    obj->setColor(1, 0, 0);
    obj->setRadius(0.3f);
    obj->setLocation(-1, -1, 1);
    obj->setName("ball_2");
    theScene->insert(obj);
  }

  {
    opengl::CSphere::Ptr obj = opengl::CSphere::Create();
    obj->setColor(0, 1, 0);
    obj->setRadius(0.5);
    obj->setLocation(0, 0, 0);
    obj->setName("USER_MOUSE_PICK");
    theScene->insert(obj);
  }

  emscripten_set_canvas_element_size("#canvas", 800, 600);
  EmscriptenWebGLContextAttributes attrs;
  emscripten_webgl_init_context_attributes(&attrs);

  attrs.enableExtensionsByDefault = 1;
  attrs.majorVersion = 3;
  attrs.minorVersion = 0;

  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context =
      emscripten_webgl_create_context("#canvas", &attrs);
  if (!context) {
    printf("Skipped: WebGL 2 is not supported.\n");
    return;
  }

  std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

  emscripten_webgl_make_context_current(context);
  theScene->render();
}

// ------------------------------------------------------
//						MAIN
// ------------------------------------------------------
int main() {
  try {
    TestDisplay3D();

    return 0;
  } catch (const std::exception &e) {
    std::cout << "MRPT error: " << mrpt::exception_to_str(e) << std::endl;
    return -1;
  }
}
