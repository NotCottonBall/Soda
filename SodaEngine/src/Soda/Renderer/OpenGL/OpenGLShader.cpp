#include "SD_PCH.h"
#include "Soda/_Main/Core.h"

#include "OpenGLShader.h"

#include "glad/glad.h"

namespace Soda
{
static GLenum ShaderTypeFromString(const std::string &type)
{
  if(type == "vertex")
    return GL_VERTEX_SHADER;
  if(type == "fragment" || type == "pixel" || type == "color")
    return GL_FRAGMENT_SHADER;

  SD_ENGINE_ASSERT(false, "Unknown shader type {0}!", type);
}

OpenGLShader::OpenGLShader(const std::string &filepath)
{
  std::string source = ReadFile(filepath);
  auto shaderSources = ProcessSource(source);
  CompileShader(shaderSources);

  // // Extract name from filepath
  // auto lastSlash = filepath.find_last_of("/\\");
  // lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
  // auto lastDot = filepath.rfind('.');
  // auto count = lastDot == std::string::npos ? filepath.size() - lastSlash :
  // lastDot - lastSlash; m_Name = filepath.substr(lastSlash, count);
}

OpenGLShader::OpenGLShader(const std::string &vertexSrc,
                           const std::string &fragmentSrc)
{
  std::unordered_map<GLenum, std::string> shaderSources;

  shaderSources[GL_VERTEX_SHADER] = vertexSrc;
  shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;

  CompileShader(shaderSources);
}

void OpenGLShader::CompileShader(
    const std::unordered_map<GLenum, std::string> &shaderSources)
{
  // compiles all the files we send it
  // and then links them together
  // and then deletes the intermediate files
  // and then returns the shader program id

  m_ShaderID = glCreateProgram();

  for(auto &st : shaderSources)
  {
    GLenum type = st.first;
    const std::string &source = st.second;

    GLuint shader = glCreateShader(type);

    const GLchar *sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, 0);

    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

      glDeleteShader(shader);

      SD_ENGINE_ERROR("{0}", infoLog.data());
      SD_ENGINE_ASSERT(false, "Shader compilation failure!");
      return;
    }

    glAttachShader(m_ShaderID, shader);
  }

  GLint isLinked = 0;
  glLinkProgram(m_ShaderID);
  glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &isLinked);
  if(isLinked == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(m_ShaderID, maxLength, &maxLength, &infoLog[0]);

    glDeleteProgram(m_ShaderID);

    SD_ENGINE_ERROR("{0}", infoLog.data());
    SD_ENGINE_ASSERT(false, "Shader linking failure!");
    return;
  }

  for(auto &st : shaderSources)
    glDeleteShader(st.first);

  for(auto &st : shaderSources)
    glDetachShader(m_ShaderID, st.first);
}

std::string OpenGLShader::ReadFile(const std::string &filepath)
{
  std::string source;
  std::ifstream input(filepath, std::ios::in | std::ios::binary);
  if(input)
  {
    input.seekg(0, std::ios::end);
    source.resize(input.tellg());
    input.seekg(0, std::ios::beg);
    input.read(&source[0], source.size());
    input.close();
  }
  else
  {
    SD_ENGINE_ERROR("Could not open file '{0}'", filepath);
  }

  return source;
}

std::unordered_map<GLenum, std::string> OpenGLShader::ProcessSource(
    const std::string &source)
{
  std::unordered_map<GLenum, std::string> shaderSources;

  std::string token = "@";
  size_t pos = source.find(token);
  while(pos != std::string::npos)
  {
    size_t eol = source.find_first_of("\r\n", pos);
    SD_ENGINE_ASSERT(eol != std::string::npos, "Syntax error");

    size_t begin = pos + token.length();
    std::string type = source.substr(begin, eol - begin);
    SD_ENGINE_ASSERT(type == "vertex" || type == "fragment",
                     "Invalid shader type specified");

    size_t nextLinePos = source.find_first_not_of("\r\n", eol);
    SD_ENGINE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
    pos = source.find(token, nextLinePos);

    shaderSources[ShaderTypeFromString(type)] =
        (pos == std::string::npos)
            ? source.substr(nextLinePos)
            : source.substr(nextLinePos, pos - nextLinePos);
  }

  return shaderSources;
}

// OpenGL Uniform Setter
void OpenGLShader::SetUniformShort(const std::string &name, short value)
{
  UploadUniformShort(name, value);
}
void OpenGLShader::SetUniformInt(const std::string &name, int value)
{
  UploadUniformInt(name, value);
}
void OpenGLShader::SetUniformIntArray(const std::string &name, int *values,
                                      uint32_t count)
{
  UploadUniformIntArray(name, values, count);
}
void OpenGLShader::SetUniformFloat(const std::string &name, float value)
{
  UploadUniformFloat(name, value);
}

void OpenGLShader::SetUniformVec2(const std::string &name,
                                  const glm::vec2 &value)
{
  UploadUniformVec2(name, value);
}
void OpenGLShader::SetUniformVec3(const std::string &name,
                                  const glm::vec3 &value)
{
  UploadUniformVec3(name, value);
}
void OpenGLShader::SetUniformVec4(const std::string &name,
                                  const glm::vec4 &value)
{
  UploadUniformVec4(name, value);
}

void OpenGLShader::SetUniformMat4(const std::string &name,
                                  const glm::mat4 &value)
{
  UploadUniformMat4(name, value);
}

// OpenGL Uniform Uploaders
void OpenGLShader::UploadUniformShort(const std::string &name, short value)
{
  int location = glGetUniformLocation(m_ShaderID, name.c_str());
  SD_ENGINE_ASSERT(location != -1, "Uniform {0} doesn't exist!", name);

  glUniform1i(location, value);
}

void OpenGLShader::UploadUniformInt(const std::string &name, int value)
{
  int location = glGetUniformLocation(m_ShaderID, name.c_str());
  SD_ENGINE_ASSERT(location != -1, "Uniform {0} doesn't exist!", name);

  glUniform1i(location, value);
}
void OpenGLShader::UploadUniformIntArray(const std::string &name, int *values,
                                         uint32_t count)
{
  int location = glGetUniformLocation(m_ShaderID, name.c_str());
  SD_ENGINE_ASSERT(location != -1, "Uniform {0} doesn't exist!", name);

  glUniform1iv(location, count, values);
}
void OpenGLShader::UploadUniformFloat(const std::string &name, float value)
{
  int location = glGetUniformLocation(m_ShaderID, name.c_str());
  SD_ENGINE_ASSERT(location != -1, "Uniform {0} doesn't exist!", name);

  glUniform1f(location, value);
}

void OpenGLShader::UploadUniformVec2(const std::string &name,
                                     const glm::vec2 &vector)
{
  int location = glGetUniformLocation(m_ShaderID, name.c_str());
  SD_ENGINE_ASSERT(location != -1, "Uniform {0} doesn't exist!", name);

  glUniform2f(location, vector.x, vector.y);
}
void OpenGLShader::UploadUniformVec4(const std::string &name,
                                     const glm::vec4 &vector)
{
  int location = glGetUniformLocation(m_ShaderID, name.c_str());
  SD_ENGINE_ASSERT(location != -1, "Uniform {0} doesn't exist!", name);

  glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}
void OpenGLShader::UploadUniformVec3(const std::string &name,
                                     const glm::vec3 &vector)
{
  int location = glGetUniformLocation(m_ShaderID, name.c_str());
  SD_ENGINE_ASSERT(location != -1, "Uniform {0} doesn't exist!", name);

  glUniform3f(location, vector.x, vector.y, vector.z);
}

void OpenGLShader::UploadUniformMat4(const std::string &name,
                                     const glm::mat4 &matrix)
{
  int location = glGetUniformLocation(m_ShaderID, name.c_str());
  SD_ENGINE_ASSERT(location != -1, "Uniform {0} doesn't exist!", name);

  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(m_ShaderID); }

void OpenGLShader::Bind() { glUseProgram(m_ShaderID); }

void OpenGLShader::Unbind() { glUseProgram(0); }
} // namespace Soda
