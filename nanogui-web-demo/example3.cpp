/*
    src/example3.cpp -- C++ version of an example application that shows
    how to use nanogui in an application with an already created and managed
    glfw context.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <emscripten.h>
#include <emscripten/html5.h>

#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <nanogui/nanogui.h>

using namespace nanogui;

enum test_enum { Item1 = 0, Item2, Item3 };

bool bvar = true;
int ivar = 12345678;
double dvar = 3.1415926;
float fvar = (float)dvar;
std::string strval = "A string";
test_enum enumval = Item2;
Color colval(0.5f, 0.5f, 0.7f, 1.f);

Screen *screen = nullptr;

static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

std::function<void()> loop;
void main_loop() { loop(); }

int main(int /* argc */, char ** /* argv */) {
  try {

    glfwSetErrorCallback(error_callback);

    glfwInit();

    glfwSetTime(0);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create a GLFWwindow object
    GLFWwindow *window =
        glfwCreateWindow(800, 800, "example3", nullptr, nullptr);
    if (window == nullptr) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
    }
    glfwMakeContextCurrent(window);

    glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Create a nanogui screen and pass the glfw pointer to initialize
    screen = new Screen();
    screen->initialize(window, true);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapInterval(0);
    glfwSwapBuffers(window);

    // Create nanogui gui
    bool enabled = true;
    FormHelper *gui = new FormHelper(screen);
    ref<Window> nanoguiWindow =
        gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");
    gui->addGroup("Basic types");
    gui->addVariable("bool", bvar)->setTooltip("Test tooltip.");
    gui->addVariable("string", strval);

    gui->addGroup("Validating fields");
    gui->addVariable("int", ivar)->setSpinnable(true);
    gui->addVariable("float", fvar)->setTooltip("Test.");
    gui->addVariable("double", dvar)->setSpinnable(true);

    gui->addGroup("Complex types");
    gui->addVariable("Enumeration", enumval, enabled)
        ->setItems({"Item 1", "Item 2", "Item 3"});
    gui->addVariable("Color", colval)->setFinalCallback([](const Color &c) {
      std::cout << "ColorPicker Final Callback: [" << c.r() << ", " << c.g()
                << ", " << c.b() << ", " << c.w() << "]" << std::endl;
    });

    gui->addGroup("Other widgets");
    gui->addButton("A button",
                   []() { std::cout << "Button pressed." << std::endl; })
        ->setTooltip("Testing a much longer tooltip, that will wrap around to "
                     "new lines multiple times.");

    screen->setVisible(true);
    screen->performLayout();
    nanoguiWindow->center();

    glfwSetCursorPosCallback(window, [](GLFWwindow *, double x, double y) {
      screen->cursorPosCallbackEvent(x, y);
    });

    glfwSetMouseButtonCallback(
        window, [](GLFWwindow *, int button, int action, int modifiers) {
          screen->mouseButtonCallbackEvent(button, action, modifiers);
        });

    glfwSetKeyCallback(
        window, [](GLFWwindow *, int key, int scancode, int action, int mods) {
          screen->keyCallbackEvent(key, scancode, action, mods);
        });

    glfwSetCharCallback(window, [](GLFWwindow *, unsigned int codepoint) {
      screen->charCallbackEvent(codepoint);
    });

    glfwSetDropCallback(window,
                        [](GLFWwindow *, int count, const char **filenames) {
                          screen->dropCallbackEvent(count, filenames);
                        });

    glfwSetScrollCallback(window, [](GLFWwindow *, double x, double y) {
      screen->scrollCallbackEvent(x, y);
    });

    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow *, int width, int height) {
                                     screen->resizeCallbackEvent(width, height);
                                   });

    // loop
    loop = [&] {
      try {
        // Check if any events have been activated (key pressed, mouse moved
        // etc.) and call corresponding response functions
        glfwPollEvents();

        glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw nanogui
        screen->drawContents();
        screen->drawWidgets();

        glfwSwapBuffers(window);
      } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
      }
    };

    emscripten_set_main_loop(main_loop, 0, true);

    glfwDestroyWindow(window);

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    return 0;
  } catch (const std::exception &e) {
    fprintf(stderr, "Exception: %s\n", e.what());
    return 1;
  }
}
