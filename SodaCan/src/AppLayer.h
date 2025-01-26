#pragma once

#include "Soda.h"
#include "Panels/Panels.h"
#include "Soda/ECS/Object.h"


namespace Soda
{
    class SodaCan : public Layer
    {
    public:
        SodaCan();
        virtual ~SodaCan() = default;

        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate(Timestep dt) override;
        void OnEvent(Event& event) override;
        void OnImGuiUpdate() override;

        void OnResize(uint32_t width, uint32_t height) override;

    private:
        OrthoCameraController m_EditorCameraController;
        Ref<Framebuffer> m_Framebuffer;
    private:
        // @TODO: change the name m_Scene to m_CurentSystem or m_System
        // or the opposite
        Ref<Systems> m_Scene;
        
        Object m_Square;
        Object m_Square2;
        Object m_Square3;

        Object m_EditorCamera;
        Object m_SecondCam;

        bool m_PrimaryCam = true;

        Ref<Texture2D> m_GridTex;
        Ref<Texture2D> m_GingerCat;
        Ref<Texture2D> m_BoxTexture;

        Ref<SpriteSheetTexture> m_miniDirt;
        Ref<SpriteSheetTexture> m_miniDirtWithGrass;

        // could make a Object2D class to store all of this data
        Ref<VertexArray> m_BoxVA;
        Ref<Shader> m_Shader2D;

        glm::vec3 m_BoxPosition = { 0.0f, 0.0f, 0.1f };
        glm::vec2 m_BoxScale = { 1.0f, 1.0f };
        float m_BoxRotation = 0.0f;
        glm::vec4 m_BoxColor = { 1.0f, 1.0f, 1.0f, 1.0f };

        float m_GradFactor = 5.0f;
        float m_MulFactor = 1.0f;

    private:
        glm::vec2 m_ViewportSize = glm::vec2(0.0f);

    // Panels
    private:
        Panels m_Panels;
    private:
        bool m_IsPanelFocused = false;
        bool m_IsPanelHovered = false;

        enum Settings : char
        {
            None = 0,
            EnableRendererStats = BIT(0),
            ADD_SOMETHING_HERE  = BIT(1) // for future settings
        };
        Settings m_DefaultSettings = Settings::None;

        void toggoleSetting(Settings& defaultSetting, Settings settingToToggle)
        { defaultSetting = static_cast<Settings>(defaultSetting ^ settingToToggle); }
    };
}