// #pragma once

// #include "Soda.h"

// class Test2D : public Soda::Layer
// {
// public:
//   Test2D();
//   virtual ~Test2D() = default;

//   void OnAttach() override;
//   void OnDetach() override;

//   void OnUpdate(Soda::Timestep dt) override;
//   void OnEvent(Soda::Event &event) override;
//   void OnImGuiUpdate() override;

// private:
//   Soda::OrthoCameraController m_CameraController;

//   Soda::Ref<Soda::Texture2D> m_BoxTexture;

//   // could make a Object2D class to store all of this data
//   Soda::Ref<Soda::VertexArray> m_BoxVA;
//   Soda::Ref<Soda::Shader> m_Shader2D;

// private:
//   glm::vec3 m_BoxPosition = {0.0f, 0.0f, 0.0f};
//   glm::vec2 m_BoxScale = {1.0f, 1.0f};
//   float m_BoxRotation = 0.0f;
//   glm::vec4 m_BoxColor = {0.8f, 0.2f, 0.3f, 1.0f};
// };