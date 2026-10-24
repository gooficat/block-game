#include "window/window.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

static void window_error_callback(int err, char const *desc)
{
    (void)err;
    fprintf(stderr, "GLFW ERROR:\n%s\n", desc);
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
    window_s *user;
    user = glfwGetWindowUserPointer(window);
    user->state.size[0] = width;
    user->state.size[1] = height;
}

static void window_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    window_s *user;
    (void)key, (void)mods;
    user = glfwGetWindowUserPointer(window);
    if (action == GLFW_KEY_DOWN)
    {
        user->input.keys[scancode] = true;
    }
    else if (action == GLFW_KEY_UP)
    {
        user->input.keys[scancode] = false;
    }
}

static void window_close_callback(GLFWwindow *window)
{
    window_s *user;
    user = glfwGetWindowUserPointer(window);
    user->state.state = WINDOW_CLOSING;
}

void window_init(window_s *window, int initial_width, int initial_height)
{
    glfwInit();
    glfwSetErrorCallback(window_error_callback);

    window->state.size[0] = initial_width;
    window->state.size[1] = initial_height;
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    window->internal.window = glfwCreateWindow(initial_width, initial_height, "", NULL, NULL);
    window->state.has_resized = false;
    window->state.state = WINDOW_FOCUSED;
    glfwSetWindowUserPointer(window->internal.window, window);
    glfwShowWindow(window->internal.window);
    glfwSetWindowSizeCallback(window->internal.window, window_size_callback);
    glfwSetKeyCallback(window->internal.window, window_key_callback);
    
    glfwSetWindowCloseCallback(window->internal.window, window_close_callback);

    glfwMakeContextCurrent(window->internal.window);
}

void window_update(window_s *window)
{
    glfwPollEvents();
    glfwSwapBuffers(window->internal.window);
    if (window->state.has_resized == true)
    {
        window->state.has_resized = false;
    }
}

void window_close(window_s *window)
{
    glfwDestroyWindow(window->internal.window);
}
