/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2022, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */

#include <emscripten.h>
#include <emscripten/html5.h>

#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#include "/home/jlblanco/code/mrpt/3rdparty/nanogui/ext/glfw/deps/linmath.h"
#include <GLFW/glfw3.h>

#include <mrpt/math/TPose3D.h>
#include <mrpt/opengl/CAxis.h>
#include <mrpt/opengl/CBox.h>
#include <mrpt/opengl/CGridPlaneXY.h>
#include <mrpt/opengl/COpenGLScene.h>
#include <mrpt/opengl/CSphere.h>
#include <mrpt/opengl/CText.h>
#include <mrpt/opengl/stock_objects.h>
#include <stdio.h>

#include <iostream>

static mrpt::opengl::COpenGLScene::Ptr theScene;

static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

float camx = 0, camy = 0, camz = 0;

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {

  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch (key) {
    case GLFW_KEY_UP:
      camy -= 0.03;
      break;
    case GLFW_KEY_DOWN:
      camy += 0.03;
      break;
    case GLFW_KEY_LEFT:
      camx -= 0.03;
      break;
    case GLFW_KEY_RIGHT:
      camx += 0.03;
      break;

    default:
      break;
    }
  }
}

std::function<void()> loop;
void main_loop() { loop(); }

// ------------------------------------------------------
//				TestDisplay3D
// ------------------------------------------------------
void TestDisplay3D() {
  using namespace mrpt;
  using namespace mrpt::opengl;

  theScene = COpenGLScene::Create();

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

  theScene->insert(mrpt::opengl::stock_objects::CornerXYZ(1.0f));

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
}

// ------------------------------------------------------
//						MAIN
// ------------------------------------------------------
int main() {
  try {

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
      exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window =
        glfwCreateWindow(800, 600, "Simple example", nullptr, nullptr);
    if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    { // Fix background alpha:
      EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx =
          emscripten_webgl_get_current_context();
    }

    glfwSwapInterval(1);

    TestDisplay3D();

    // std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

    theScene->getViewport()->getCamera().setZoomDistance(20);

    loop = [&] {
      try {
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT);

        theScene->getViewport()->getCamera().setPointingAt(camx, camy, camz);

        theScene->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
      } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
      }
    };

    emscripten_set_main_loop(main_loop, 0, true);

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);

    return 0;
  } catch (const std::exception &e) {
    std::cout << "MRPT error: " << mrpt::exception_to_str(e) << std::endl;
    return -1;
  }
}
