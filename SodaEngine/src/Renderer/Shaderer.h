#pragma once

#include "glm/glm.hpp"

namespace Soda
{
class Shader
{
public:
  virtual ~Shader() = default;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  virtual void SetUniformShort(const std::string &name, short value) = 0;
  virtual void SetUniformInt(const std::string &name, int value) = 0;
  virtual void SetUniformUInt(const std::string &name, uint32_t value) = 0;
  virtual void SetUniformIntArray(const std::string &name, int *values,
                                  uint32_t count) = 0;
  virtual void SetUniformFloat(const std::string &name, float value) = 0;

  virtual void SetUniformVec2(const std::string &name,
                              const glm::vec2 &value) = 0;
  virtual void SetUniformVec3(const std::string &name,
                              const glm::vec3 &value) = 0;
  virtual void SetUniformVec4(const std::string &name,
                              const glm::vec4 &value) = 0;

  virtual void SetUniformMat4(const std::string &name,
                              const glm::mat4 &value) = 0;

  static Shader *Create(const std::string &filepath);
  static Shader *Create(const std::string &vertexSrc,
                        const std::string &fragmentSrc);
};
} // namespace Soda