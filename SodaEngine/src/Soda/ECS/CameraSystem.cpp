#include "SD_PCH.h"

#include "Soda/Tools/Logger.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "CameraSystem.h"

#include "../_Main/App.h"


namespace Soda
{
    // i dont know if its a good idea to store the previous values of
    // the cam in a seperate struct which is only for one use
    // but who the fuck cares, it works so fuck it.
    struct CommonCameraData
    {
        float AspectRatio;
    };

    inline CommonCameraData& GetCommonCamData()
    {
        static CommonCameraData commonCamData;
        return commonCamData;
    }


    CameraSystem::CameraSystem()
    {
        SetViewport();
    }

    void CameraSystem::SetOrthoCamera(float size, float nearPlane, float farPlane)
    {
        m_OrthoCamSize = size;
        m_OrthoNearPlane = nearPlane;
        m_OrthoFarPlane = farPlane;

        RecalculateMatrix();
    }
    void CameraSystem::SetPersCamera(float FOV, float nearPlane, float farPlane)
    {
        m_PerspectiveFOV = FOV;
        m_PersNearPlane = nearPlane;
        m_PersFarPlane = farPlane;

        RecalculateMatrix();
    }

    void CameraSystem::SetViewport(uint32_t width, uint32_t height)
    {
        SD_ENGINE_ASSERT((width > 0 && height > 0),
            "0 Width And Height Gives 0 Aspect Ratio"
        );
        GetCommonCamData().AspectRatio = (float)width / (float)height;
        m_AspectRatio = GetCommonCamData().AspectRatio;
        RecalculateMatrix();
    }
    void CameraSystem::SetViewport()
    {
        m_AspectRatio = GetCommonCamData().AspectRatio;
        RecalculateMatrix();
    }


    void CameraSystem::RecalculateMatrix()
    {
        if(m_CameraType == CameraType::Orthographic)
        {
            float leftSide = -m_OrthoCamSize * m_AspectRatio * 0.5f;
            float rightSize = m_OrthoCamSize * m_AspectRatio * 0.5f;
            float upSide = m_OrthoCamSize * 0.5f;
            float bottomSide = -m_OrthoCamSize * 0.5f;

            m_Projection = glm::ortho(leftSide, rightSize, bottomSide, upSide, m_OrthoNearPlane, m_OrthoFarPlane);
        }
        else
        {
            m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PersNearPlane, m_PersFarPlane);
        }
    }
}