#pragma once

// This is our Platform Specific Window Class
// For Windows

#include "Soda/Window.h"

#include "GLFW/glfw3.h"


namespace Soda
{
	// this is our window window class which inherits the SodaWindow (Default properties of a window)
	class WinWindow : public SodaWindow
	{
	public:
		// this constructor will take Info of a window (defaults to WindowInfo default values if Info is not provided)
		WinWindow(const WindowInfo& windowInfo);
		virtual ~WinWindow();

		// each platform has it's own Update functions which has a specific set of instructions
		void OnUpdate() override;

		inline unsigned int GetWindowWidth() const override
		{ return m_WindowData.Width; }
		inline unsigned int GetWindowHeight() const override
		{ return m_WindowData.Height; }

		// and the platform will also have its own callback function
		void SetCallbackFn(const EventCallbackFn& CallbackFn) override
		{ m_WindowData.CallbackFn = CallbackFn; }

		void SetVSync(bool status) override;
		bool GetVSyncStatus() const override;


	private:
		// Init window is where we initialize our window by giving it the given details, setting Propeties like VSync.
		// and we also setup our OpenGL stuff like the context
		virtual void InitWindow(const WindowInfo& windowInfo);
		// window deastruction process
		virtual void CloseWindow();
	private:
		GLFWwindow* m_Window;

		// we have a struct for this just to organize the properties of our window
		// it looks neat... right
		struct WindowData
		{
			std::string Name;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn CallbackFn;
		};
		WindowData m_WindowData;
	};
}