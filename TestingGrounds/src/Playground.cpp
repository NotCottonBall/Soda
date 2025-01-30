#include "Soda.h"
#include "_Main/Start.h"

#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "2DGround.h"

// class SampleLayer : public Soda::Layer
// {
// public:
// 	SampleLayer()
// 		: Layer("Sample"), m_Camera(1280.0f / 720.0f, true)
// 	{
// 		m_VA.reset(Soda::VertexArray::Create());
//
// 		//*** Obj 1 ***//
//
// 		float vertices[3 * 6]
// 		{
// 			 0.0f,  0.5f, 0.0f,		1.0f,  0.25f, 0.25f, // 0
// 			-0.5f, -0.5f, 0.0f,		0.25f, 1.0f,  0.25f, // 1
// 			 0.5f, -0.5f, 0.0f,		0.25f, 0.25f, 1.0f, // 2
// 		};
// 		int indices[3]
// 		{
// 			0, 1, 2
// 		};
//
// 		std::shared_ptr<Soda::VertexBuffer> m_VB;
// 		m_VB.reset(Soda::VertexBuffer::Create(vertices, sizeof(vertices)));
// 		m_VB->Bind();
//
// 		std::shared_ptr<Soda::IndexBuffer> m_IB;
// 		m_IB.reset(Soda::IndexBuffer::Create(indices, sizeof(indices) /
// sizeof(int))); 		m_IB->Bind();
//
//
//
// 		Soda::BufferLoadout loadout = {
// 			{ "a_position", Soda::ShaderDataType::Vec3 },
// 			{ "a_color", Soda::ShaderDataType::Vec3 }
// 		};
//
// 		m_VB->SetLoadout(loadout);
//
// 		m_VA->AddVertexBuffer(m_VB);
// 		m_VA->AddIndexBuffer(m_IB);
//
// 		//***/ Obj 1 /***//
//
//
// 		//*** Obj 2 ***//
//
// 		m_squareVA.reset(Soda::VertexArray::Create());
//
// 		float squareVertices[4 * 5]
// 		{
// 			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,
// 			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
// 			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
// 			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f
//
// 		};
// 		int squareIndices[6]
// 		{
// 			0, 1, 2, 0, 2, 3
// 		};
//
// 		Soda::BufferLoadout SquareLoadout = {
// 			{ "a_squarePosition", Soda::ShaderDataType::Vec3 },
// 			{ "a_squareTexCoord", Soda::ShaderDataType::Vec2 }
// 		};
//
// 		std::shared_ptr<Soda::VertexBuffer> m_squareVB;
// 		m_squareVB.reset(Soda::VertexBuffer::Create(squareVertices,
// sizeof(squareVertices))); 		m_squareVB->Bind();
//
// 		std::shared_ptr<Soda::IndexBuffer> m_squareIB;
// 		m_squareIB.reset(Soda::IndexBuffer::Create(squareIndices, 24));
// 		m_squareIB->Bind();
//
// 		m_squareVB->SetLoadout(SquareLoadout);
//
// 		m_squareVA->AddVertexBuffer(m_squareVB);
// 		m_squareVA->AddIndexBuffer(m_squareIB);
//
// 		//***/ Obj 2 /***//
//
//
// 		//*** shader ***//
// 			// vertx. shdr.
// 		std::string vrtxShdr = R"(
// 			#version 410 core
//
// 			layout(location = 0) in vec3 a_vrtxPosition;
// 			layout(location = 1) in vec3 a_color;
// 			out vec3 o_objColor;
//
// 			uniform mat4 u_PVMat;
// 			uniform mat4 u_ModelMat;
//
// 			void main()
// 			{
// 				gl_Position = u_PVMat * u_ModelMat * vec4(a_vrtxPosition, 1.0);
// 				o_objColor = a_color;
// 			}
// 		)";
//
// 		std::string fragShdr = R"(
// 			#version 410 core
//
// 			layout(location = 0) out vec4 Color;
// 			in vec3 o_objColor;
//
// 			uniform vec4 u_color;
//
// 			void main()
// 			{
// 				Color = u_color;
// 			}
// 		)";
//
// 		m_BasicShader.reset(Soda::Shader::Create(vrtxShdr, fragShdr));
//
//
// 		// vertx. shdr.
//
// 		std::string cvs = R"(
// 			#version 410 core
//
// 			layout(location = 0) in vec3 a_vrtxPosition;
// 			layout(location = 1) in vec3 a_color;
// 			out vec3 o_objColor;
//
// 			uniform mat4 u_PVMat;
// 			uniform mat4 u_ModelMat;
//
// 			void main()
// 			{
// 				gl_Position = u_PVMat * u_ModelMat * vec4(a_vrtxPosition, 1.0);
// 				o_objColor = a_color;
// 			}
// 		)";
//
// 		std::string cfs = R"(
// 			#version 410 core
//
// 			layout(location = 0) out vec4 Color;
// 			in vec3 o_objColor;
//
// 			void main()
// 			{
// 				Color = vec4(o_objColor, 1.0f);
// 			}
// 		)";
//
// 		m_positionColor.reset(Soda::Shader::Create(cvs, cfs));
//
//
//
// 		m_TextureShader.reset(Soda::Shader::Create("assets/shaders/Texture.glsl"));
//
//
// 		m_Texture = Soda::Texture2D::Create("assets/textures/GingerCat.png");
//
// 		std::dynamic_pointer_cast<Soda::OpenGLShader>(m_BasicShader)->Bind();
// 		std::dynamic_pointer_cast<Soda::OpenGLShader>(m_TextureShader)->SetUniformInt("u_Texture",
// 0);
// 	}
//
//
// 	void OnEvent(Soda::Event& event) override
// 	{
// 		m_Camera.OnEvent(event);
//
// 		if(event.GetEventType() == Soda::EventType::KeyPress)
// 		{
// 			Soda::KeyPressEvent& keyEvent = (Soda::KeyPressEvent&)event;
//
// 			if (Soda::Input::IsKeyPressed(SD_KEY_PERIOD))
// 				camSpeed += 5.0f;
// 			else if (Soda::Input::IsKeyPressed(SD_KEY_COMMA) && camSpeed > 5.0)
// 				camSpeed -= 5.0f;
// 		}
// 	}
//
// 	void OnImGuiUpdate()
// 	{
// 		ImGui::Begin("Stats");
// 		ImGui::Text("FPS: %d", FPS);
// 		ImGui::End();
//
//
// 		ImGui::Begin("Camera");
// 		ImGui::Text("Transform");
// 		// ImGui::DragFloat2("Camera Position",
// &m_Camera.GetCamera().GetPosition().x, 0.1f);
// 		// ImGui::DragFloat("Camera Rotation",
// &m_Camera.GetCamera().GetRotation(), 0.1f); 		ImGui::Text("");
// 		ImGui::Text("Properties");
// 		ImGui::DragFloat("Camera Speed", &camSpeed, 0.1f);
// 		ImGui::End();
//
//
// 		ImGui::Begin("Scene");
// 		ImGui::Text("Transform");
// 		ImGui::DragFloat2("Box Position", &boxPosition.x, 0.1f);
// 		ImGui::DragFloat("Box Rotation", &boxRotation, 0.1f);
// 		ImGui::DragFloat2("Box Scale", &boxScale.x, 0.1f);
// 		ImGui::Text("");
// 		ImGui::Text("Box Color");
// 		ImGui::ColorEdit4("Box Color", &color.x);
// 		ImGui::Text("Triangle Color");
// 		ImGui::Text("");
// 		ImGui::Text("Texture");
// 		ImGui::Image((void*)m_Texture->GetTextureID(), ImVec2(100, 100));
// 		ImGui::Text(m_Texture->GetFilePath().c_str());
// 		ImGui::End();
//
//
// 		ImGui::Begin("Player");
// 		ImGui::Text("Transform");
// 		ImGui::DragFloat2("Player Position", &playerPos.x, 0.1f);
// 		ImGui::DragFloat("Player Rotation", &playerRotation, 0.1f);
// 		ImGui::DragFloat2("Player Scale", &playerScale.x, 0.1f);
// 		ImGui::Text("");
// 		ImGui::Text("Properties");
// 		ImGui::DragFloat("Player Speed", &playerSpeed, 0.1f);
// 		ImGui::End();
// 	}
//
// 	void OnUpdate(Soda::Timestep dt) override
// 	{
// 		FPS = 1.0f / dt;
//
// 		m_Camera.OnUpdate(dt);
//
// 		boxTransform =
// 			glm::translate(glm::mat4(1.0f), boxPosition) *
// 			glm::rotate(glm::mat4(1.0f), glm::radians(boxRotation), glm::vec3(0,
// 0, 1)) * 			glm::scale(glm::mat4(1.0f), boxScale);
//
// 		playerTransform =
// 			glm::translate(glm::mat4(1.0f), playerPos) *
// 			glm::rotate(glm::mat4(1.0f), glm::radians(playerRotation),
// glm::vec3(0, 0, 1)) * 			glm::scale(glm::mat4(1.0f), playerScale);
//
//
// 		// player movement
// 		if (Soda::Input::IsKeyPressed(SD_KEY_UP))
// 		{
// 			playerPos.y += 0.1 * playerSpeed * dt;
// 			playerRotation = 0.0f;
// 		}
// 		else if (Soda::Input::IsKeyPressed(SD_KEY_DOWN))
// 		{
// 			playerPos.y -= 0.1 * playerSpeed * dt;
// 			playerRotation = 180.0f;
// 		}
//
// 		if (Soda::Input::IsKeyPressed(SD_KEY_LEFT))
// 		{
// 			playerPos.x -= 0.1 * playerSpeed * dt;
// 			playerRotation = 90.0f;
// 		}
// 		else if (Soda::Input::IsKeyPressed(SD_KEY_RIGHT))
// 		{
// 			playerPos.x += 0.1 * playerSpeed * dt;
// 			playerRotation = -90.0f;
// 		}
//
//
//
// 		Soda::Renderer::StartScene(m_Camera.GetCamera());
// 		{
// 			Soda::RenderCommand::ClearScreen({ 0.1f, 0.1f, 0.1f, 1.0f });
//
// 			Soda::Renderer::PushThis(m_squareVA, m_BasicShader, boxTransform);
// 			std::dynamic_pointer_cast<Soda::OpenGLShader>(m_BasicShader)->SetUniformVec4("u_color",
// color); 			Soda::Renderer::PushThis(m_VA, m_positionColor);
//
// 			m_Texture->Bind();
// 			Soda::Renderer::PushThis(m_squareVA, m_TextureShader,
// playerTransform);
// 		}
// 		Soda::Renderer::StopScene();
// 	}
//
// private:
// 	Soda::OrthoCameraController m_Camera;
//
// 	std::shared_ptr<Soda::Shader> m_positionColor;
// 	std::shared_ptr<Soda::Shader> m_BasicShader;
// 	std::shared_ptr<Soda::Shader> m_TextureShader;
//
// 	std::shared_ptr<Soda::Texture2D> m_Texture;
//
// 	std::shared_ptr<Soda::VertexArray> m_VA;
// 	std::shared_ptr<Soda::VertexArray> m_squareVA;
//
// private:
// 	uint32_t FPS = 0.0f;
//
// 	float camRotationSpeed = 0.0f;
// 	float camSpeed = 40.0f;
//
// 	glm::vec3 boxPosition = { 0.7f, 0.5f, 0.0f };
// 	float boxRotation = 0.0f;
// 	glm::vec3 boxScale = glm::vec3(1.5);
// 	glm::mat4 boxTransform;
//
// 	glm::vec4 color = { 0.9f, 0.6f, 0.2f, 1.0f };
//
//
// 	glm::vec3 playerPos = { -1.0f, 0.0f, 0.0f };
// 	float playerRotation = 0.0f;
// 	glm::vec3 playerScale = { 0.5f, 0.5f, 0.5f };
// 	glm::mat4 playerTransform;
//
// 	float playerSpeed = 20.0f;
// };

// class ThreeDLayer : public Soda::Layer
// {
// public:
// 	ThreeDLayer()
// 		: Layer("3D"), m_Camera(45.0f, 1920.0 / 1080.0, 0.1f, 100.0f)
// 	{
// 		//*** Cube ***//
// 		m_CubeVA.reset(Soda::VertexArray::Create());
//
// 		float vertices[] = {
// 			// pos                  // color				   // texCoords //
// normals
// 			// front
// 			-0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f,
// 0.0f,  1.0f, // 0 			 0.5f, -0.5f, 0.5f,    1.0f, 1.0f, 1.0f, 1.0f,
// 1.0f, 0.0f,     0.0f,  0.0f,  1.0f, // 1 			 0.5f,  0.5f,
// 0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f,     0.0f,  0.0f,  1.0f, // 2
// 			-0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,
// 0.0f,  1.0f, // 3
//
// 			// back
// 			-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f,
// 0.0f, -1.0f, // 4 			 0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,
// 1.0f, 0.0f,     0.0f,  0.0f, -1.0f, // 5 			 0.5f,  0.5f,
// -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f,     0.0f,  0.0f, -1.0f, // 6
// 			-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,
// 0.0f, -1.0f, // 7
//
// 			// left
// 			-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f,
// -1.0f,  0.0f,  0.0f, // 8 			-0.5f, -0.5f,
// 0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f,    -1.0f,  0.0f,  0.0f, // 9
// 			-0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f,
// -1.0f,  0.0f,  0.0f, // 10 			-0.5f,  0.5f,
// -0.5f,    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,    -1.0f,  0.0f,  0.0f, // 11
//
// 			// right
// 			 0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f,
// 0.0f,     1.0f,  0.0f,  0.0f, // 12 			 0.5f, -0.5f,
// 0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f,     1.0f,  0.0f,  0.0f, // 13
// 			 0.5f,  0.5f,
// 0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f,     1.0f,  0.0f,  0.0f, // 14
// 			 0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,
// 0.0f, 1.0f,     1.0f,  0.0f,  0.0f, // 15
//
// 			 // top
// 			 -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f,
// 0.0f,  1.0f, 0.0f, // 16 			  0.5f,  0.5f,
// 0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f,     0.0f,  1.0f, 0.0f, // 17
// 			  0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f,
// 0.0f,  1.0f, 0.0f, // 18 			 -0.5f,  0.5f,
// -0.5f,    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,     0.0f,  1.0f, 0.0f, // 19
//
// 			 // bottom
// 			 -0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f,
// 0.0f, -1.0f, 0.0f, // 20 			  0.5f, -0.5f,
// 0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f,     0.0f, -1.0f, 0.0f, // 21
// 			  0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f,
// 0.0f, -1.0f, 0.0f, // 22 			 -0.5f, -0.5f,
// -0.5f,    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,     0.0f, -1.0f, 0.0f, // 23
// 		};
//
// 		int indices[] = {
// 			// front
// 			0, 1, 2,
// 			2, 3, 0,
//
// 			// back
// 			4, 5, 6,
// 			6, 7, 4,
//
// 			// left
// 			8, 9, 10,
// 			10, 11, 8,
//
// 			// right
// 			12, 13, 14,
// 			14, 15, 12,
//
// 			// top
// 			16, 17, 18,
// 			18, 19, 16,
//
// 			// bottom
// 			20, 21, 22,
// 			22, 23, 20
// 		};
//
// 		m_CubeVB.reset(Soda::VertexBuffer::Create(vertices, sizeof(vertices)));
// 		m_CubeVB->Bind();
// 		m_CubeIB.reset(Soda::IndexBuffer::Create(indices, sizeof(indices)));
// 		m_CubeIB->Bind();
//
// 		Soda::BufferLoadout loadout = {
// 			{ "a_position", Soda::ShaderDataType::Vec3 },
// 			{ "a_color", Soda::ShaderDataType::Vec4 },
// 			{ "a_texCoords", Soda::ShaderDataType::Vec2 },
// 			{ "a_normal", Soda::ShaderDataType::Vec3 }
// 		};
//
// 		m_CubeVB->SetLoadout(loadout);
//
// 		m_CubeVA->AddVertexBuffer(m_CubeVB);
// 		m_CubeVA->AddIndexBuffer(m_CubeIB);
// 		//***// Cube //***//
//
//
// 		//*** Light Cube ***//
// 		m_LightCubeVA.reset(Soda::VertexArray::Create());
//
// 		float lightCube[] = {
// 			// pos
// 			// front
// 			-0.5f, -0.5f,  0.5f, // 0
// 			 0.5f, -0.5f,  0.5f, // 1
// 			 0.5f,  0.5f,  0.5f, // 2
// 			-0.5f,  0.5f,  0.5f, // 3
//
// 			// back
// 			-0.5f, -0.5f, -0.5f, // 4
// 			 0.5f, -0.5f, -0.5f, // 5
// 			 0.5f,  0.5f, -0.5f, // 6
// 			-0.5f,  0.5f, -0.5f, // 7
// 		};
//
// 		int lightIndices[] = {
// 			// front
// 			0, 1, 2,
// 			2, 3, 0,
//
// 			// back
// 			4, 5, 6,
// 			6, 7, 4,
//
// 			// left
// 			8, 9, 10,
// 			10, 11, 8,
//
// 			// right
// 			12, 13, 14,
// 			14, 15, 12,
// 		};
//
//
// 		m_Shader.reset(Soda::Shader::Create("assets/shaders/Shader.glsl"));
// 		std::dynamic_pointer_cast<Soda::OpenGLShader>(m_Shader)->Bind();
//
// 		m_BoxDiffTexture =
// Soda::Texture2D::Create("assets/textures/WoodenContainer_diff.png");
// 		m_BoxDiffTexture->Bind(0);
// 		m_BoxSpecTexture =
// Soda::Texture2D::Create("assets/textures/WoodenContainer_spec.png");
// 		m_BoxSpecTexture->Bind(1);
//
// 		std::dynamic_pointer_cast<Soda::OpenGLShader>(m_Shader)->SetUniformInt("u_DiffuseTexture",
// 0);
// 		std::dynamic_pointer_cast<Soda::OpenGLShader>(m_Shader)->SetUniformInt("u_SpecularTexture",
// 1);
//
// 		m_DirectionalLight.reset(Soda::Light::SetLightType(Soda::LightType::Directional,
// m_Shader));
//
// 	}
//
// 	void OnImGuiUpdate() override
// 	{
// 		ImGui::Begin("FPS");
// 		{
// 			ImGui::Text("FPS: %d", (int)fps);
// 		}
// 		ImGui::End();
//
// 		ImGui::Begin("Stats");
// 		{
// 			ImGui::DragFloat3("Cube Position", &m_CubePosition.x, 0.1f);
// 			ImGui::DragFloat3("Cube Rotation", &m_CubeRotation.x, 0.1f);
// 			ImGui::DragFloat3("Cube Scale", &m_CubeScale.x, 0.1f);
// 			ImGui::ColorEdit4("Cube Color", &m_CubeColor.x);
// 			ImGui::Text("");
// 			ImGui::DragFloat("Camera Speed", &camSpeed, 0.1f);
// 		}
// 		ImGui::End();
//
// 		ImGui::Begin("Lights");
// 		{
// 			ImGui::Text("Directional Light");
// 			ImGui::DragFloat3("Direction", &dirLightDirection.x, 0.1f);
// 			ImGui::ColorEdit3("Color", &dirLightColor.x);
// 		}
// 		ImGui::End();
// 	}
//
// 	void OnEvent(Soda::Event& event) override
// 	{
// 		if(event.GetEventType() == Soda::EventType::MouseClick)
// 		{
// 			if (Soda::Input::IsMouseClicked(SD_MOUSE_BUTTON_RIGHT))
// 			{
// 				isMovable = 1;
// 				window.ShowCursor(false);
// 			}
// 		}
// 		else if (event.GetEventType() == Soda::EventType::MouseRelease)
// 		{
// 			if (!Soda::Input::IsMouseClicked(SD_MOUSE_BUTTON_RIGHT))
// 			{
// 				isMovable = 0;
// 				window.ShowCursor(true);
// 			}
// 		}
//
// 		if (event.GetEventType() == Soda::EventType::MouseScroll)
// 		{
// 			Soda::MouseScrollEvent& scrollEvent =
// (Soda::MouseScrollEvent&)event;
//
// 			if(scrollEvent.GetWheelOffsetY() > 0 && camSpeed <= 500.0f)
// 				camSpeed += 50.0f;
// 			else if (scrollEvent.GetWheelOffsetY() < 0 && camSpeed >= 50.0f)
// 				camSpeed -= 50.0f;
// 		}
//
// 		if (event.GetEventType() == Soda::EventType::MouseMove)
// 		{
// 			Soda::MouseMoveEvent& mouseEvent = (Soda::MouseMoveEvent&)event;
//
// 			if (isMovable > 0)
// 			{
// 				if (isMovable <= 1)
// 				{
// 					lastX = mouseEvent.GetMouseX();
// 					lastY = mouseEvent.GetMouseY();
//
// 					isMovable++;
// 				}
//
// 				float xOffset = mouseEvent.GetMouseX() - lastX;
// 				float yOffset = lastY - mouseEvent.GetMouseY();
//
// 				lastX = mouseEvent.GetMouseX();
// 				lastY = mouseEvent.GetMouseY();
//
// 				xOffset *= camSensi;
// 				yOffset *= camSensi;
//
// 				yaw += xOffset;
// 				pitch += yOffset;
//
// 				if (pitch > 89.0f)
// 					pitch = 89.0f;
// 				else if (pitch < -89.0f)
// 					pitch = -89.0f;
//
// 				m_Camera.SetTarget(glm::vec3(
// 					cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
// 					sin(glm::radians(pitch)),
// 					sin(glm::radians(yaw)) * cos(glm::radians(pitch))
// 				));
// 			}
// 		}
// 	}
//
// 	void OnUpdate(Soda::Timestep dt) override
// 	{
// 		fps = 1.0f / dt;
//
//
// 		m_CubeTransform = glm::translate(glm::mat4(1.0f), m_CubePosition) *
// 			glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation.x),
// glm::vec3(1, 0, 0)) * 			glm::rotate(glm::mat4(1.0f),
// glm::radians(m_CubeRotation.y), glm::vec3(0, 1, 0)) *
// 			glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation.z),
// glm::vec3(0, 0, 1)) * 			glm::scale(glm::mat4(1.0f), m_CubeScale);
//
// 		m_DirectionalLight->SetDirection(dirLightDirection);
// 		m_DirectionalLight->SetColor(dirLightColor);
//
//
// 		if (isMovable > 0)
// 		{
// 			if (Soda::Input::IsKeyPressed(SD_KEY_W))
// 			{
// 				camPosition += (camSpeed * dt) * m_Camera.GetTarget();
// 			}
// 			else if (Soda::Input::IsKeyPressed(SD_KEY_S))
// 			{
// 				camPosition -= (camSpeed * dt) * m_Camera.GetTarget();
// 			}
//
// 			if (Soda::Input::IsKeyPressed(SD_KEY_A))
// 			{
// 				camPosition -= ((camSpeed - 10) * dt) *
// glm::normalize(glm::cross(m_Camera.GetTarget(), glm::vec3(0.0f, 1.0f,
// 0.0f)));
// 			}
// 			else if (Soda::Input::IsKeyPressed(SD_KEY_D))
// 			{
// 				camPosition += ((camSpeed - 10) * dt) *
// glm::normalize(glm::cross(m_Camera.GetTarget(), glm::vec3(0.0f, 1.0f,
// 0.0f)));
// 			}
//
// 			if (Soda::Input::IsKeyPressed(SD_KEY_Q))
// 			{
// 				camPosition -= (camSpeed * dt) * glm::vec3(0.0f, 1.0f, 0.0f);
// 			}
// 			else if (Soda::Input::IsKeyPressed(SD_KEY_E))
// 			{
// 				camPosition += (camSpeed * dt) * glm::vec3(0.0f, 1.0f, 0.0f);
// 			}
// 		}
//
// 		m_Camera.SetPosition(camPosition);
// 		m_Camera.SetRotation(camRotation);
//
// 		Soda::Renderer::StartScene(m_Camera);
// 		{
// 			Soda::RenderCommand::ClearScreen(glm::vec4(0.0f));
//
//
// 			m_BoxDiffTexture->Bind(0);
// 			m_BoxSpecTexture->Bind(1);
// 			Soda::Renderer::PushThis(m_CubeVA, m_Shader, m_CubeTransform);
// 		}
// 		Soda::Renderer::StopScene();
// 	}
//
// private:
// 	Soda::SodaWindow& window = Soda::App::Get().GetWindow();
//
// 	Soda::PerspectiveCamera m_Camera;
//
// 	std::shared_ptr<Soda::Shader> m_Shader;
//
// 	std::shared_ptr<Soda::Light> m_DirectionalLight;
//
//
// 	std::shared_ptr<Soda::Texture2D> m_BoxDiffTexture;
// 	std::shared_ptr<Soda::Texture2D> m_BoxSpecTexture;
//
// 	std::shared_ptr<Soda::VertexArray> m_CubeVA;
// 	std::shared_ptr<Soda::VertexBuffer> m_CubeVB;
// 	std::shared_ptr<Soda::IndexBuffer> m_CubeIB;
//
// 	std::shared_ptr<Soda::VertexArray> m_LightCubeVA;
// 	std::shared_ptr<Soda::VertexBuffer> m_LightCubeVB;
// 	std::shared_ptr<Soda::IndexBuffer> m_LightCubeIB;
//
// private:
// 	glm::vec3 m_CubePosition = { 0.0f, 0.0f, 0.0f };
// 	glm::vec3 m_CubeRotation = { 0.0f, 0.0f, 0.0f };
// 	glm::vec3 m_CubeScale = { 1.0f, 1.0f, 1.0f };
// 	glm::mat4 m_CubeTransform;
//
// 	glm::vec4 m_CubeColor = { 0.9f, 0.6f, 0.2f, 1.0f };
//
//
// 	glm::vec3 m_LightCubePosition = { 0.0f, 1.0f, 2.0f };
// 	float m_LightCubeRotation = 45.0f;
// 	glm::vec3 m_LightCubeScale = { 1.0f, 1.0f, 1.0f };
// 	glm::mat4 m_LightCubeTransform;
//
// 	glm::vec4 m_LightCubeColor = { 1.0f, 1.0f, 1.0f, 1.0f };
//
//
// 	glm::vec3 dirLightDirection = { -0.2f, -1.0f, -0.3f };
// 	glm::vec3 dirLightColor = { 1.0f, 1.0f, 1.0f };
//
//
// 	glm::vec3 camPosition = { 0.0f, 0.0f, 3.0f };
// 	float camRotation = 0.0f;
// 	float camSpeed = 20.0f;
// 	float camSensi = 0.1f;
//
// 	float lastX = 0.0f;
// 	float lastY = 0.0f;
// 	float yaw = -90.0f;
// 	float pitch = 0.0f;
//
// 	float lightInensity = 1.0f;
//
//
// 	char isMovable = 0;
//
// 	float fps = 0.0f;
// };

class Playground : public Soda::App
{
public:
  Playground() { PushLayer(new Test2D()); }

  ~Playground() {}
};

Soda::App *Soda::CreateApp() { return new Playground(); }