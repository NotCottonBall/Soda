#include "SD_PCH.h"

#include "Soda/ECS/Components.h"
#include "Soda/ECS/Object.h"
#include "Soda/Tools/SpriteSheet.h"
#include "Soda/_Main/Core.h"

#include "Renderer2D.h"
#include "Soda/Renderer/Render.h"

#include "Soda/Renderer/VertexArray.h"
#include "Soda/Renderer/Shaderer.h"
#include "Soda/Renderer/Texture.h"
#include "glm/gtx/dual_quaternion.hpp"


// @FIXME: with AMD GPUs the rendering doesnt work well
// that is because texture arrays need to be uniform something something
// using a UBO (Uniform Buffer Object) or bindless
// should work i think, so do that later
// NOTE: if that don't work, then fuck openGL, use Vulkan or DirectX
namespace Soda
{
    struct QuadVertex
    {
        glm::vec4 position;
        glm::vec4 color;
        glm::vec2 texCoords;
        float texIndex;
        float textureScale;
    };

    struct QuadInfo
    {
        const uint32_t m_maxQuads = 10000;
        const uint32_t m_maxVertices = m_maxQuads * 4;
        const uint32_t m_maxIndices = m_maxQuads * 6;
        static const uint32_t m_maxTextureCount = 32;

        Ref<VertexArray> m_VA;
        Ref<VertexBuffer> m_VB;
        Ref<Shader> m_Shader;
        Ref<Texture2D> m_DefaultTexture;

        QuadVertex *m_QuadVertexStart = nullptr, *m_QuadVertexPtr = nullptr;
        uint32_t m_IndicesCount = 0;

        std::array<Ref<Texture2D>, m_maxTextureCount> m_TextureSlots;
        uint32_t m_TextureIndex = 1;

        glm::vec4 m_VertexPositions[4];
    };

    static QuadInfo m_QuadInfo;

    Renderer2D::RendererStats m_RendererStats;



    void Renderer2D::Init()
    {
        m_QuadInfo.m_VA.reset(VertexArray::Create());

        // setting up the vertex buffer
        m_QuadInfo.m_VB = VertexBuffer::Create(m_QuadInfo.m_maxVertices * sizeof(QuadVertex));
        m_QuadInfo.m_VB->Bind();
        m_QuadInfo.m_VB->SetLoadout({
            { "a_position",  ShaderDataType::Vec4  },
            { "a_color",     ShaderDataType::Vec4  },
            { "a_texCoords", ShaderDataType::Vec2  },
            { "a_texIndex",  ShaderDataType::Float },
            { "a_texScale",  ShaderDataType::Float }
            });
        m_QuadInfo.m_VA->AddVertexBuffer(m_QuadInfo.m_VB);

        m_QuadInfo.m_QuadVertexStart = new QuadVertex[m_QuadInfo.m_maxVertices];


        // setting up the indices
        uint32_t* boxIndices = new uint32_t[m_QuadInfo.m_maxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < m_QuadInfo.m_maxIndices; i += 6)
        {
            boxIndices[i + 0] = offset + 0;
            boxIndices[i + 1] = offset + 1;
            boxIndices[i + 2] = offset + 2;

            boxIndices[i + 3] = offset + 2;
            boxIndices[i + 4] = offset + 3;
            boxIndices[i + 5] = offset + 0;

            offset += 4;
        }
        Ref<IndexBuffer> m_IB = IndexBuffer::Create(boxIndices, m_QuadInfo.m_maxIndices);
        m_IB->Bind();
        m_QuadInfo.m_VA->AddIndexBuffer(m_IB);
        delete[] boxIndices;


        // other stuff
        m_QuadInfo.m_DefaultTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        m_QuadInfo.m_DefaultTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int textures[m_QuadInfo.m_maxTextureCount];
        for (uint32_t i = 0; i < m_QuadInfo.m_maxTextureCount; i++)
        {
            textures[i] = i;
        }

        m_QuadInfo.m_Shader.reset(Shader::Create("SodaCan/assets/shaders/Shader2D.glsl"));
        m_QuadInfo.m_Shader->Bind();
        m_QuadInfo.m_Shader->SetUniformIntArray("u_Textures", textures, m_QuadInfo.m_maxTextureCount);


        m_QuadInfo.m_TextureSlots[0] = m_QuadInfo.m_DefaultTexture;

        m_QuadInfo.m_VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        m_QuadInfo.m_VertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        m_QuadInfo.m_VertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        m_QuadInfo.m_VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::Shutdown()
    { }

    void Renderer2D::Setup()
    {
        m_QuadInfo.m_QuadVertexPtr = m_QuadInfo.m_QuadVertexStart;

        m_QuadInfo.m_IndicesCount = 0;
        m_QuadInfo.m_TextureIndex = 1;
    }

    void Renderer2D::StartScene(const OrthoCamera& camera)
    {
        m_QuadInfo.m_Shader->Bind();
        m_QuadInfo.m_Shader->SetUniformMat4("u_PVMat", camera.GetProjectionViewMat());

        Setup();
    }
    void Renderer2D::StartScene(const TheCamera& camera, const glm::mat4& transform)
    {
        glm::mat4 m_ViewProjectionMat = camera.GetProjection() * glm::inverse(transform);

        m_QuadInfo.m_Shader->Bind();
        m_QuadInfo.m_Shader->SetUniformMat4("u_PVMat", camera.GetProjection() * glm::inverse(transform));

        Setup();
    }

    void Renderer2D::StopScene()
    {
        // track all the vertices and send it to the gpu then do the draw call
        uint32_t dataSize = (uint8_t*)m_QuadInfo.m_QuadVertexPtr - (uint8_t*)m_QuadInfo.m_QuadVertexStart;
        m_QuadInfo.m_VB->SetData(m_QuadInfo.m_QuadVertexStart, dataSize);

        DrawBatch();
    }

    void Renderer2D::DrawBatch()
    {
        for(uint32_t i = 0; i < m_QuadInfo.m_TextureIndex; i++)
        {
            m_QuadInfo.m_TextureSlots[i]->Bind(i);
        }
    
        RenderCommand::DrawThis(m_QuadInfo.m_VA, m_QuadInfo.m_IndicesCount);
        m_RendererStats.noOfDrawCalls++;
    }


    // draw the quads with matrices
    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        if(m_QuadInfo.m_IndicesCount >= m_QuadInfo.m_maxIndices)
        {
            StopScene();
            Setup();
        }

        const float textureIndex = 0.0f;

        // tex coords
        glm::vec2 texCoords[4] = {{0.0f, 0.0f}, {1.0, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

        // we pass through the mem block of each QuadVertex and set their attributes;
        // also texScale doesnt even matter but we are setting it just because
        // we dont wanna multiply the out Color with a random value in the frag shader
        for(int i = 0; i < 4; i++)
        {
            m_QuadInfo.m_QuadVertexPtr->position = transform * m_QuadInfo.m_VertexPositions[i];
            m_QuadInfo.m_QuadVertexPtr->color = color;
            m_QuadInfo.m_QuadVertexPtr->texCoords = texCoords[i];
            m_QuadInfo.m_QuadVertexPtr->texIndex = textureIndex;
            m_QuadInfo.m_QuadVertexPtr->textureScale = 1.0f;
            m_QuadInfo.m_QuadVertexPtr++;
        }
    
        m_QuadInfo.m_IndicesCount += 6;
        m_RendererStats.noOfQuads++;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, float texScale)
    {
        if(m_QuadInfo.m_IndicesCount >= m_QuadInfo.m_maxIndices)
        {
            StopScene();
            Setup();
        }

        float textureIndex = 0.0f;

        for(uint32_t i = 1; i < m_QuadInfo.m_TextureIndex; i++)
        {
            if(*m_QuadInfo.m_TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if(textureIndex == 0.0f)
        {
            textureIndex = (float)m_QuadInfo.m_TextureIndex;
            m_QuadInfo.m_TextureSlots[m_QuadInfo.m_TextureIndex] = texture;
            m_QuadInfo.m_TextureIndex++;
        }

        glm::vec2 texCoords[4] = {{0.0f, 0.0f}, {1.0, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

        // we pass through the mem block of each QuadVertex and set their attributes
        for(int i = 0; i < 4; i++)
        {
            m_QuadInfo.m_QuadVertexPtr->position = transform * m_QuadInfo.m_VertexPositions[i];
            m_QuadInfo.m_QuadVertexPtr->color = color;
            m_QuadInfo.m_QuadVertexPtr->texCoords = texCoords[i];
            m_QuadInfo.m_QuadVertexPtr->texIndex = textureIndex;
            m_QuadInfo.m_QuadVertexPtr->textureScale = texScale;
            m_QuadInfo.m_QuadVertexPtr++;
        }

        m_QuadInfo.m_IndicesCount += 6;
        m_RendererStats.noOfQuads++;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SpriteSheetTexture>& spriteSheetTexture, const glm::vec4& color, float texScale)
    {
        Ref<Texture2D> texture = spriteSheetTexture->GetTexture();
        const glm::vec2* texCoords = spriteSheetTexture->GetSpriteCoords();

        if(m_QuadInfo.m_IndicesCount >= m_QuadInfo.m_maxIndices)
        {
            StopScene();
            Setup();
        }

        float textureIndex = 0.0f;

        for(uint32_t i = 1; i < m_QuadInfo.m_TextureIndex; i++)
        {
            if(*m_QuadInfo.m_TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if(textureIndex == 0.0f)
        {
            textureIndex = (float)m_QuadInfo.m_TextureIndex;
            m_QuadInfo.m_TextureSlots[m_QuadInfo.m_TextureIndex] = texture;
            m_QuadInfo.m_TextureIndex++;
        }

        // we pass through the mem block of each QuadVertex and set their attributes
        for(int i = 0; i < 4; i++)
        {
            m_QuadInfo.m_QuadVertexPtr->position = transform * m_QuadInfo.m_VertexPositions[i];
            m_QuadInfo.m_QuadVertexPtr->color = color;
            m_QuadInfo.m_QuadVertexPtr->texCoords = texCoords[i];
            m_QuadInfo.m_QuadVertexPtr->texIndex = textureIndex;
            m_QuadInfo.m_QuadVertexPtr->textureScale = texScale;
            m_QuadInfo.m_QuadVertexPtr++;
        }

        m_QuadInfo.m_IndicesCount += 6;
        m_RendererStats.noOfQuads++;
    }


    // for normal quads (the ones that dont rotate)
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                              glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});

        DrawQuad(transform, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<Texture2D>& texture, const glm::vec4& color, float texScale)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                    glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});

        DrawQuad(transform, texture, color, texScale);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<SpriteSheetTexture>& spriteSheetTexture, const glm::vec4& color, float texScale)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                              glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});

        DrawQuad(transform, spriteSheetTexture, color, texScale);
    }


    // for rotated quads //
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float& rotation, const glm::vec2& scale, const glm::vec4& color)
    {        
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                              glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});

        DrawQuad(transform, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float& rotation, const glm::vec2& scale, const Ref<Texture2D>& texture, const glm::vec4& color, float texScale)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                              glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});

        DrawQuad(transform, texture, color, texScale);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float& rotation, const glm::vec2& scale, const Ref<SpriteSheetTexture>& spriteSheetTexture, const glm::vec4& color, float texScale)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                              glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});

        DrawQuad(transform, spriteSheetTexture, color, texScale);  
    }


    void Renderer2D::ResetRendererStats()
    { memset(&m_RendererStats, 0, sizeof(RendererStats)); }

    const Renderer2D::RendererStats& Renderer2D::GetRendererStats()
    { return m_RendererStats; }

}