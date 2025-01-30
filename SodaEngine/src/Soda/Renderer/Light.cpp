#include "SD_PCH.h"

#include "Light.h"

namespace Soda
{
Light *Light::SetLightType(LightType type, const Ref<Shader> &shader)
{
  switch(type)
  {
  case LightType::Directional:
    return new DirectionalLight(shader);
  case LightType::Point:
    return new PointLight(shader);
  case LightType::Spot:
    return new SpotLight(shader);
  }

  SD_ENGINE_ASSERT(!nullptr, "Invalid Light Type :O");
  return nullptr;
}

//*** Directional Light ***//
void DirectionalLight::UpdateInformation(const Ref<Shader> &shader)
{
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformVec3(
      "u_DirLight.direction", m_Direction);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformVec3(
      "u_DirLight.color", m_Color);
}
void DirectionalLight::UpdateSettings(const Ref<Shader> &shader)
{
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_DirLight.ambientStrength", m_Settings.AmbientStrength);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_DirLight.specularStrength", m_Settings.SpecularStrength);
}

//*** Point Light ***//
void PointLight::UpdateInformation(const Ref<Shader> &shader)
{
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformVec3(
      "u_PointLight.position", m_Position);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformVec3(
      "u_PointLight.color", m_Color);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformInt(
      "u_PointLight.nLights", m_nLights);
}
void PointLight::UpdateSettings(const Ref<Shader> &shader)
{
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_PointLight.ambientStrength", m_Settings.AmbientStrength);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_PointLight.specularStrength", m_Settings.SpecularStrength);
}
void PointLight::UpdateCutOffAndAttenuation(const Ref<Shader> &shader)
{
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_PointLight.constant", m_Attenuation.Constant);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_PointLight.linear", m_Attenuation.Linear);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_PointLight.quadratic", m_Attenuation.Quadratic);
}

//*** Spot Light ***//
void SpotLight::UpdateInformation(const Ref<Shader> &shader)
{
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformVec3(
      "u_SpotLight.position", m_Position);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformVec3(
      "u_SpotLight.direction", m_Direction);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformVec3(
      "u_SpotLight.color", m_Color);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformInt(
      "u_SpotLight.nLights", m_nLights);
}
void SpotLight::UpdateSettings(const Ref<Shader> &shader)
{
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_SpotLight.ambientStrength", m_Settings.AmbientStrength);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_SpotLight.specularStrength", m_Settings.SpecularStrength);
}
void SpotLight::UpdateCutOffAndAttenuation(const Ref<Shader> &shader)
{
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_SpotLight.cutOff", m_CutOff.Inner);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_SpotLight.outerCutOff", m_CutOff.Outer);

  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_SpotLight.constant", m_Attenuation.Constant);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_SpotLight.linear", m_Attenuation.Linear);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformFloat(
      "u_SpotLight.quadratic", m_Attenuation.Quadratic);
}
} // namespace Soda