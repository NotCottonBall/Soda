#pragma once

// this Window file is the base of all windows. Linux, Windows or Mac window
// files are seperate and we specify what should happen in each one of those
// platforms in its own file the specific platform files are children of the
// SodaWindow class and inherit the properties from WindowInfo struct

#include "SD_PCH.h"

#include "Core/Core.h"
#include "Core/Events/Events.h"

namespace Soda
{
// this is the Window Data we want our window to use
// they have default values if we dont spesify what we want
struct WindowInfo
{
  std::string Name;
  unsigned int Width;
  unsigned int Height;

  WindowInfo(const std::string &p_Name = "Unnamed Soda Application",
             unsigned int p_Width = 1600, unsigned int p_Height = 900)
      : Name(p_Name), Width(p_Width), Height(p_Height)
  {
  }
};

// this is the class that is inherited by each of our platform window.
class SodaWindow
{
public:
  // the Callback function we want to run when we get input in the window
  using EventCallbackFn = std::function<void(Event &)>;

  virtual ~SodaWindow() {}

  // what happenes every update (each frame)
  virtual void OnUpdate() = 0;

  virtual unsigned int GetWindowWidth() const = 0;
  virtual unsigned int GetWindowHeight() const = 0;

  virtual void SetCallbackFn(const EventCallbackFn &CallbackFn) = 0;
  virtual void SetVSync(bool status) = 0;
  virtual bool GetVSyncStatus() const = 0;

  virtual void ShowCursor(bool status) = 0;

  // this is to get the Window of out app
  virtual void *GetTheWindow() const = 0;

  // this function creates the Window.
  // We create the window inside specific platform class where we return the
  // platform's Window
  static SodaWindow *Create(const WindowInfo &props = WindowInfo());
};
} // namespace Soda