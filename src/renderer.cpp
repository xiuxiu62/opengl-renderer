#include "renderer.hpp"
#include "GLFW/glfw3.h"
#include "timer.hpp"
#include <iostream>

int Renderer::initialize(float *clear_color) {
  if (!glfwInit()) {
    puts("Failed to initialize GLFW\n");

    return -1;
  };

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 20);

  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GL_ARB_separate_shader_objects, 1);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  this->window = glfwCreateWindow(800, 600, "Example", NULL, NULL);

  if (this->window == NULL) {
    puts("Failed to create GLFW window\n");
    glfwTerminate();

    return -1;
  }

  glfwMakeContextCurrent(this->window);
  glfwSetFramebufferSizeCallback(this->window,
                                 [](GLFWwindow *window, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });

  const unsigned int gl_status = glewInit();
  if (gl_status != GLEW_OK) {
    const GLubyte *gl_error = glewGetErrorString(gl_status);
    fprintf(stderr, "Error initializing GLEW: %s\n", gl_error);
    glfwTerminate();

    return -1;
  }

  glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
  glEnable(GL_DEPTH_TEST);
  // glDepthMask(GL_FALSE);
  glDepthFunc(GL_LESS);

  return 0;
}

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

int Renderer::run() {
  std::optional<std::unique_ptr<Shader>> shader =
      Shader::load("res/shaders/triangle.vert", "res/shaders/triangle.frag");
  MeshObject *mesh_object = this->initialize_mesh();
  GLFWwindow *window = this->window;

  if (!shader.has_value()) {
    std::cout << "Shader didn't load :(" << std::endl;
    return -1;
  }

  while (!glfwWindowShouldClose(window)) {
    this->process_input();

    this->clear();
    this->display_mesh(shader.value().get(), mesh_object);

    glfwSwapBuffers(window);
    glfwPollEvents();
    core::time::global_timer.get()->update();
  }

  this->deinitialize(shader.value().get(), mesh_object);

  return 0;
}

MeshObject *Renderer::initialize_mesh() {
  MeshObject *object = new MeshObject();

  // Generate Buffers
  glGenVertexArrays(1, &object->vao);
  glGenBuffers(1, &object->vbo);
  glGenBuffers(1, &object->ebo);

  if (this->debug) {
    std::cout << "vbo: " << object->vbo << std::endl;
    std::cout << "vao: " << object->vao << std::endl;
    std::cout << "ebo: " << object->ebo << std::endl;
  }

  // Bind VAO
  glBindVertexArray(object->vao);

  // Bind VBO
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(),
               this->vertices.data(), GL_STATIC_DRAW);

  if (this->debug) {
    std::cout << "Vertices:" << std::endl;
    for (unsigned int i = 0;
         i < static_cast<unsigned int>(this->vertices.size()); i++)
      std::cout << this->vertices[i]
                << (i < this->vertices.size() - 1 ? ", " : " ");
    std::cout << std::endl;
  }

  // Bind EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(unsigned int) * this->indices.size(),
               this->indices.data(), GL_STATIC_DRAW);

  if (this->debug) {
    std::cout << "Indices:" << std::endl;
    for (unsigned int i = 0;
         i < static_cast<unsigned int>(this->indices.size()); i++)
      std::cout << this->indices[i]
                << (i < this->indices.size() - 1 ? ", " : " ");
    std::cout << std::endl;
  }

  // Setup buffer alignment
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
  glEnableVertexAttribArray(0);

  // Unbind VBO and VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);

  return object;
}

void Renderer::display_mesh(Shader *shader, MeshObject *object) {
  shader->use();

  glBindVertexArray(object->vao);
  glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::process_input() {
  if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(this->window, true);
}

void Renderer::deinitialize(Shader *shader, MeshObject *object) {
  glDeleteVertexArrays(1, &object->vao);
  glDeleteBuffers(1, &object->vbo);
  glDeleteBuffers(1, &object->ebo);
  glDeleteProgram(shader->id());

  glfwTerminate();
}
