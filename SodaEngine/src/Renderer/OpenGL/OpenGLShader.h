#pragma once

#include "Renderer/Shaderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

// EXTREMELY TEMPORARY
typedef unsigned int GLenum;

namespace Soda
{
class OpenGLShader : public Shader
{
public:
  OpenGLShader(const std::string &filepath);
  OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc);
  virtual ~OpenGLShader();

  void Bind() override;
  void Unbind() override;

  std::string ReadFile(const std::string &filepath);
  std::unordered_map<GLenum, std::string> ProcessSource(
      const std::string &source);
  void CompileShader(
      const std::unordered_map<GLenum, std::string> &shaderSources);

  // these are the calls from the shaderer.h file that will call the OpenGL
  // specific functions
  virtual void SetUniformShort(const std::string &name, short value) override;
  virtual void SetUniformInt(const std::string &name, int value) override;
  virtual void SetUniformUInt(const std::string &name, uint32_t value) override;
  virtual void SetUniformIntArray(const std::string &name, int *values,
                                  uint32_t count) override;
  virtual void SetUniformFloat(const std::string &name, float value) override;

  virtual void SetUniformVec2(const std::string &name,
                              const glm::vec2 &value) override;
  virtual void SetUniformVec3(const std::string &name,
                              const glm::vec3 &value) override;
  virtual void SetUniformVec4(const std::string &name,
                              const glm::vec4 &value) override;

  virtual void SetUniformMat4(const std::string &name,
                              const glm::mat4 &value) override;

  // these are the OpenGL functions that will upload given data to the GPU
  void UploadUniformShort(const std::string &name, short value);
  void UploadUniformInt(const std::string &name, int value);
  void UploadUniformIntArray(const std::string &name, int *values,
                             uint32_t count);
  void UploadUniformFloat(const std::string &name, float value);

  void UploadUniformVec2(const std::string &name, const glm::vec2 &vector);
  void UploadUniformVec3(const std::string &name, const glm::vec3 &vector);
  void UploadUniformVec4(const std::string &name, const glm::vec4 &vector);

  void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix);

private:
  uint32_t m_ShaderID;
};
} // namespace Soda