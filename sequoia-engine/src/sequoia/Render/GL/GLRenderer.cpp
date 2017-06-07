//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/DrawCommandList.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLProgramManager.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include "sequoia/Render/GL/GLStateCacheManager.h"
#include "sequoia/Render/RenderSystem.h"
#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>
#include <glbinding/glbinding-version.h>
#include <sstream>

// TODO: Kill me
#include <iostream>

namespace sequoia {

namespace render {

static const char* getGLType(GLenum type) {
  switch(type) {
  case GL_DEBUG_TYPE_ERROR:
    return "ERROR";
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    return "DEPRECATED_BEHAVIOR";
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    return "UNDEFINED_BEHAVIOR";
  case GL_DEBUG_TYPE_PORTABILITY:
    return "PORTABILITY";
  case GL_DEBUG_TYPE_PERFORMANCE:
    return "PERFORMANCE";
  case GL_DEBUG_TYPE_OTHER:
    return "OTHER";
  default:
    return "UNKNOWN";
  }
}

static void GLDebugLogging(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                           const GLchar* message, const void* userParam) {
  switch(severity) {
  case GL_DEBUG_SEVERITY_LOW:
    LOG(INFO) << "GL_" << getGLType(type) << ": " << message;
    return;
  case GL_DEBUG_SEVERITY_MEDIUM:
    LOG(WARNING) << "GL_" << getGLType(type) << ": " << message;
    return;
  case GL_DEBUG_SEVERITY_HIGH:
    LOG(ERROR) << "GL_" << getGLType(type) << ": " << message;
    return;
  default:
    LOG(DEBUG) << "GL_" << getGLType(type) << ": " << message;
    return;
  }
}

static std::string functionCallToString(const glbinding::FunctionCall& call) {
  std::stringstream ss;
  ss << call.function->name()
     << core::RangeToString(", ", "(", ")")(
            call.parameters, [](glbinding::AbstractValue* value) { return value->asString(); });
  if(call.returnValue)
    ss << " -> " << call.returnValue->asString();
  return ss.str();
}

GLRenderer::GLRenderer(GLRenderWindow* target) : window_(target) {
  LOG(INFO) << "Creating OpenGL renderer " << this << " ...";

  // Bind the context to the current thread
  glfwMakeContextCurrent(window_->getGLFWwindow());

  // Load function lazily
  glbinding::Binding::initialize(false);

  // Set debugging callbacks
  if(RenderSystem::getSingleton().debugMode()) {

    // Enable error checking
    using namespace glbinding;
    setCallbackMaskExcept(CallbackMask::After | CallbackMask::ParametersAndReturnValue,
                          {"glGetError"});
    setAfterCallback([](const FunctionCall& call) {
      const auto error = glGetError();
      if(error != GL_NO_ERROR)
        LOG(ERROR) << "GL_ERROR: " << error << ": " << functionCallToString(call);
    });

    // Enable logging
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebugLogging, nullptr);
  }

  LOG(INFO) << "glbinding: " << GLBINDING_VERSION;
  LOG(INFO) << "GL version: " << glbinding::ContextInfo::version().toString();
  LOG(INFO) << "GL vendor: " << glbinding::ContextInfo::vendor();
  LOG(INFO) << "GL renderer: " << glbinding::ContextInfo::renderer();

  // Initialize OpenGL related managers
  stateCacheManager_ = std::make_unique<GLStateCacheManager>();
  shaderManager_ = std::make_unique<GLShaderManager>();
  programManager_ = std::make_unique<GLProgramManager>();

  LOG(INFO) << "Done creating OpenGL renderer " << this;
}

GLRenderer::~GLRenderer() {
  LOG(INFO) << "Terminating OpenGL renderer " << this << " ... ";

  glfwMakeContextCurrent(window_->getGLFWwindow());

  // Destroy all remaining shaders, programs, textures and buffers
  defaultVertexShader_.reset();
  defaultFragmentShader_.reset();
  defaultProgram_.reset();

  programManager_.reset();
  shaderManager_.reset();
  glbinding::Binding::releaseCurrentContext();

  LOG(INFO) << "Done terminating OpenGL renderer " << this << " ... ";
}

void GLRenderer::render() {
//  Viewport* viewport = target_->getViewport();
//  Camera* camera = viewport->getCamera();
//  DrawCommandList* drawCommandList = target_->getDrawCommandList().get();

//  // Compute the projction matrix
//  glm::mat4 matProj = glm::perspective(camera->getFieldOfViewY(), camera->getAspectRatio(),
//                                       camera->getZNearClipping(), camera->getZFarClipping());
  
//  // Compute camera view matrix
//  glm::mat4 matView = glm::lookAt(camera->getEye(), camera->getCenter(), camera->getUp());

//  // Precompute view projection matrix
//  glm::mat4 matViewProj = matProj * matView; 
  
//  // Clear the screen
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//  // Start rendering
//  DrawCommand* drawCommand = nullptr;
//  for(drawCommand = drawCommandList->start(); drawCommand != nullptr;
//      drawCommand = drawCommandList->next()) {
    
//    GLProgram* program = dyn_cast<GLProgram>(drawCommand->getProgram());    
    
//    // Compute the full model view projection matrix
//    glm::mat4 u_ModelViewProjection = matViewProj * drawCommand->getModelMatrix();

//    // Update the OpenGL state-machine
//    stateCacheManager_->setRenderState(drawCommand->getRenderState());
          
//    // Set the uniforms
//    program->setUniformVariable("u_ModelViewProjection", u_ModelViewProjection);
//    stateCacheManager_->setProgram(program);    
    
//    // Bind and draw the buffers
//    stateCacheManager_->draw(drawCommand);
//  }
  
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders

	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do{

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(3);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window_->getGLFWwindow());
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window_->getGLFWwindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window_->getGLFWwindow()) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(3);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

GLShaderManager* GLRenderer::getShaderManager() { return shaderManager_.get(); }

GLProgramManager* GLRenderer::getProgramManager() { return programManager_.get(); }

GLStateCacheManager* GLRenderer::getStateCache() { return stateCacheManager_.get(); }

void GLRenderer::loadDefaultShaders(const std::shared_ptr<File>& defaultVertexShaderFile,
                                    const std::shared_ptr<File>& defaultFragmentShaderFile) {
  auto& rsys = RenderSystem::getSingleton();

  defaultVertexShader_ = rsys.loadShader(window_, Shader::ST_Vertex, defaultVertexShaderFile);
  defaultFragmentShader_ = rsys.loadShader(window_, Shader::ST_Fragment, defaultFragmentShaderFile);
  defaultProgram_ = rsys.createProgram(window_, {defaultVertexShader_, defaultFragmentShader_});
}

const std::shared_ptr<Shader>& GLRenderer::getDefaultVertexShader() const {
  return defaultVertexShader_;
}

const std::shared_ptr<Shader>& GLRenderer::getDefaultFragmentShader() const {
  return defaultFragmentShader_;
}

const std::shared_ptr<Program>& GLRenderer::getDefaultProgram() const { return defaultProgram_; }

} // namespace render

} // namespace sequoia
