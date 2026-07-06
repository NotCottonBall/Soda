#pragma once
#include <string>

// Forward Declares
struct SDL_Window;

namespace SodaEngine
{
class Window
{
private:
  SDL_Window *m_Window;

  int m_Width;
  int m_Height;
  std::string m_WindowName;

public:
  Window(int width, int height, std::string name);
  ~Window();
  bool Initialize();

  void Resize(int width, int height);
  void Rename(std::string name);

  // Getters
  SDL_Window *Get() { return m_Window; }
  float GetWidth() { return m_Width; }
  float GetHeight() { return m_Height; }
};
} // namespace SodaEngine
