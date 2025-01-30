#pragma once

#include "SD_PCH.h"

#include "glm/glm.hpp"

#include "Soda/Renderer/OpenGL/OpenGLShader.h"

namespace Soda
{
enum class LightType : char
{
  Directional = 0,
  Point = 1,
  Spot = 2
};

// light system with a light class as parent of other light tyles
class Light
{
public:
  struct Attenuation
  {
    float Constant;
    float Linear;
    float Quadratic;
  };
  struct Settings
  {
    float AmbientStrength;
    float SpecularStrength;
  };

  virtual void SetPosition(const glm::vec3 &position)
  {
    SD_ENGINE_LOG(
        "The Function SetPosition() Is Not Avaiable For The Light Type {0}",
        (char)GetLightType());
  }
  virtual void SetDirection(const glm::vec3 &direction)
  {
    SD_ENGINE_LOG(
        "The Function SetDirection() Is Not Available For The Light Type {0}",
        (char)GetLightType());
  }
  virtual void SetColor(const glm::vec3 &color) = 0;

  virtual void SetSettings(float ambientStrength, float specularStrength) = 0;

  virtual void SetAttenuation(float constant, float linear, float quad)
  {
    SD_ENGINE_LOG(
        "The Function SetAttenuation() Is Not Available For The Light Type {0}",
        (char)GetLightType());
  }
  virtual void SetCutOff(float inner, float outer)
  {
    SD_ENGINE_LOG(
        "The Function SetCutOff() Is Not Available For The Light Type {0}",
        (char)GetLightType());
  }

  virtual const glm::vec3 &GetPosition() const
  {
    SD_ENGINE_LOG(
        "The Function GetPosition() Is Not Available For The Light Type {0}",
        (char)GetLightType());
    return glm::vec3(0.0f);
  }
  virtual const glm::vec3 &GetDirection() const
  {
    SD_ENGINE_LOG(
        "The Function GetDirection() Is Not Available For The Light Type {0}",
        (char)GetLightType());
    return glm::vec3(0.0f);
  }
  virtual const glm::vec3 &GetColor() const = 0;

  virtual const std::pair<float, float> &GetSettings() const = 0;

  virtual const std::tuple<float, float, float> &GetAttenuation() const
  {
    SD_ENGINE_LOG(
        "The Function GetAttenuation() Is Not Available For The Light Type {0}",
        (char)GetLightType());
    return {0.0f, 0.0f, 0.0f};
  }
  virtual const std::pair<float, float> &GetCutOff() const
  {
    SD_ENGINE_LOG(
        "The Function GetCutOff() Is Not Available For The Light Type {0}",
        (char)GetLightType());
    return {0.0f, 0.0f};
  }

  virtual const LightType &GetLightType() const = 0;

  virtual ~Light() = default;

  static Light *SetLightType(LightType type, const Ref<Shader> &shader);

  virtual void UpdateInformation(
      const Ref<Shader> &shader) = 0; // Updates the light's nLights, position,
                                      // direction, color
  virtual void UpdateSettings(
      const Ref<Shader>
          &shader) = 0; // Updates the light's ambientStrength, specularStrength
  virtual void UpdateCutOffAndAttenuation(
      const Ref<Shader>
          &shader) = 0; // Updates the light's cutOff and attenuation

private:
  LightType m_Type;
};

//*** Directional Light ***//
class DirectionalLight : public Light
{
public:
  DirectionalLight(const Ref<Shader> &shader,
                   const glm::vec3 &direction = glm::vec3(-0.2f, -1.0f, -0.3f),
                   const glm::vec3 &color = glm::vec3(1.0f),
                   float ambientStrength = 0.1f, float specularStrength = 32.0f)
      : m_Direction(direction), m_Color(color),
        m_Settings({ambientStrength, specularStrength}), m_Shader(shader)
  {
    UpdateInformation(shader);
    UpdateSettings(shader);
  }

  virtual void SetDirection(const glm::vec3 &direction) override
  {
    m_Direction = direction;
    UpdateInformation(m_Shader);
  }
  virtual void SetColor(const glm::vec3 &color) override
  {
    m_Color = color;
    UpdateInformation(m_Shader);
  }

  virtual void SetSettings(float ambientStrength,
                           float specularStrength) override
  {
    m_Settings.AmbientStrength = ambientStrength;
    m_Settings.SpecularStrength = specularStrength;
    UpdateSettings(m_Shader);
  }

  virtual const glm::vec3 &GetDirection() const override { return m_Direction; }
  virtual const glm::vec3 &GetColor() const override { return m_Color; }

  virtual const std::pair<float, float> &GetSettings() const override
  {
    return {m_Settings.AmbientStrength, m_Settings.SpecularStrength};
  }

  virtual const LightType &GetLightType() const override
  {
    return LightType::Directional;
  }

private:
  virtual void UpdateInformation(const Ref<Shader> &shader) override;
  virtual void UpdateSettings(const Ref<Shader> &shader) override;
  virtual void UpdateCutOffAndAttenuation(const Ref<Shader> &shader) override {}

private:
  glm::vec3 m_Direction;
  glm::vec3 m_Color;

  Settings m_Settings;

  const Ref<Shader> &m_Shader;
};

//*** Point Light ***//
// TODO: add a range to the point light
class PointLight : public Light
{
public:
  PointLight(const Ref<Shader> &shader,
             const glm::vec3 &position = glm::vec3(0.0f),
             const glm::vec3 &color = glm::vec3(1.0f), float constant = 1.0f,
             float linear = 0.09f, float quad = 0.032f,
             float ambientStrength = 0.1f, float specularStrength = 32.0f)
      : m_Position(position), m_Color(color),
        m_Attenuation({constant, linear, quad}),
        m_Settings({ambientStrength, specularStrength}), m_Shader(shader)
  {
  }

  virtual void SetPosition(const glm::vec3 &position) override
  {
    m_Position = position;
    UpdateInformation(m_Shader);
  }
  virtual void SetColor(const glm::vec3 &color) override
  {
    m_Color = color;
    UpdateInformation(m_Shader);
  }

  virtual void SetSettings(float ambientStrength,
                           float specularStrength) override
  {
    m_Settings.AmbientStrength = ambientStrength;
    m_Settings.SpecularStrength = specularStrength;
    UpdateSettings(m_Shader);
  }

  virtual void SetAttenuation(float constant, float linear, float quad) override
  {
    m_Attenuation.Constant = constant;
    m_Attenuation.Linear = linear;
    m_Attenuation.Quadratic = quad;
    UpdateCutOffAndAttenuation(m_Shader);
  }

  virtual const glm::vec3 &GetPosition() const override { return m_Position; }
  virtual const glm::vec3 &GetColor() const override { return m_Color; }

  virtual const std::pair<float, float> &GetSettings() const override
  {
    return {m_Settings.AmbientStrength, m_Settings.SpecularStrength};
  }

  virtual const std::tuple<float, float, float> &GetAttenuation() const override
  {
    return {m_Attenuation.Constant, m_Attenuation.Linear,
            m_Attenuation.Quadratic};
  }

  virtual const LightType &GetLightType() const override
  {
    return LightType::Point;
  }

  const int GetNumberOfLights() const { return m_nLights; }

private:
  virtual void UpdateInformation(const Ref<Shader> &shader) override;
  virtual void UpdateSettings(const Ref<Shader> &shader) override;
  virtual void UpdateCutOffAndAttenuation(const Ref<Shader> &shader) override;

private:
  glm::vec3 m_Position;
  glm::vec3 m_Color;

  Attenuation m_Attenuation;

  Settings m_Settings;

  int m_nLights = 0;

  const Ref<Shader> &m_Shader;
};

//*** Spot Light ***//
class SpotLight : public Light
{
public:
  SpotLight(const Ref<Shader> &shader,
            const glm::vec3 &position = glm::vec3(0.0f),
            const glm::vec3 &direction = glm::vec3(0.0f, -1.0f, 0.0f),
            const glm::vec3 &color = glm::vec3(1.0f), float inner = 12.5f,
            float outer = 15.0f, float constant = 1.0f, float linear = 0.09f,
            float quad = 0.032f, float ambientStrength = 0.1f,
            float specularStrength = 32.0f)
      : m_Position(position), m_Direction(direction), m_Color(color),
        m_CutOff({inner, outer}), m_Attenuation({constant, linear, quad}),
        m_Settings({ambientStrength, specularStrength}), m_Shader(shader)
  {
  }

  virtual void SetPosition(const glm::vec3 &position) override
  {
    m_Position = position;
    UpdateInformation(m_Shader);
  }
  virtual void SetDirection(const glm::vec3 &direction) override
  {
    m_Direction = direction;
    UpdateInformation(m_Shader);
  }
  virtual void SetColor(const glm::vec3 &color) override
  {
    m_Color = color;
    UpdateInformation(m_Shader);
  }

  virtual void SetSettings(float ambientStrength,
                           float specularStrength) override
  {
    m_Settings.AmbientStrength = ambientStrength;
    m_Settings.SpecularStrength = specularStrength;
    UpdateSettings(m_Shader);
  }

  virtual void SetAttenuation(float constant, float linear, float quad) override
  {
    m_Attenuation.Constant = constant;
    m_Attenuation.Linear = linear;
    m_Attenuation.Quadratic = quad;
    UpdateCutOffAndAttenuation(m_Shader);
  }
  virtual void SetCutOff(float inner, float outer) override
  {
    m_CutOff.Inner = inner;
    m_CutOff.Outer = outer;
    UpdateCutOffAndAttenuation(m_Shader);
  }

  virtual const glm::vec3 &GetPosition() const override { return m_Position; }
  virtual const glm::vec3 &GetDirection() const override { return m_Direction; }
  virtual const glm::vec3 &GetColor() const override { return m_Color; }

  virtual const std::pair<float, float> &GetSettings() const override
  {
    return {m_Settings.AmbientStrength, m_Settings.SpecularStrength};
  }

  virtual const std::tuple<float, float, float> &GetAttenuation() const override
  {
    return {m_Attenuation.Constant, m_Attenuation.Linear,
            m_Attenuation.Quadratic};
  }
  virtual const std::pair<float, float> &GetCutOff() const override
  {
    return {m_CutOff.Inner, m_CutOff.Outer};
  }

  virtual const LightType &GetLightType() const override
  {
    return LightType::Spot;
  }

  const int GetNumberOfLights() const { return m_nLights; }

private:
  virtual void UpdateInformation(const Ref<Shader> &shader) override;
  virtual void UpdateSettings(const Ref<Shader> &shader) override;
  virtual void UpdateCutOffAndAttenuation(const Ref<Shader> &shader) override;

private:
  glm::vec3 m_Position;
  glm::vec3 m_Direction;
  glm::vec3 m_Color;

  Attenuation m_Attenuation;
  struct CutOff
  {
    float Inner;
    float Outer;
  } m_CutOff;

  Settings m_Settings;

  int m_nLights = 0;

  const Ref<Shader> &m_Shader;
};
} // namespace Soda