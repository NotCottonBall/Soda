#pragma once

// this is where all the application related events reside

#include "Events.h"

namespace Soda
{
class WindowResizeEvent : public Event
{
public:
  // these are those defines i talked about in Events.h
  // they make life easier by setting the Event type and other stuff
  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategory_App)

  WindowResizeEvent(unsigned int wWidth, unsigned int wHeight)
      : m_windowWidth(wWidth), m_windowHeight(wHeight)
  {
  }

  inline unsigned int GetWindowWidth() const { return m_windowWidth; }
  inline unsigned int GetWindowHeight() const { return m_windowHeight; }

  // we have the ToString to print out what the event did when it was called
  std::string ToString() const override
  {
    std::stringstream msg;
    msg << "Window Resized To (Width: " << m_windowWidth
        << ", Height: " << m_windowHeight << ")";
    return msg.str();
  }

private:
  unsigned int m_windowWidth, m_windowHeight;
};

class WindowCloseEvent : public Event
{
public:
  EVENT_CLASS_TYPE(WindowClose)
  EVENT_CLASS_CATEGORY(EventCategory_App)

  WindowCloseEvent() {}

  std::string ToString() const override
  {
    std::stringstream msg;
    msg << "Window Closed :(";
    return msg.str();
  }
};

class AppTickEvent : public Event
{
public:
  EVENT_CLASS_TYPE(AppTick)
  EVENT_CLASS_CATEGORY(EventCategory_App)

  AppTickEvent() {}
};

class AppUpdateEvent : public Event
{
public:
  EVENT_CLASS_TYPE(AppUpdate)
  EVENT_CLASS_CATEGORY(EventCategory_App)

  AppUpdateEvent() {}
};

class AppRenderEvent : public Event
{
public:
  EVENT_CLASS_TYPE(AppRender)
  EVENT_CLASS_CATEGORY(EventCategory_App)

  AppRenderEvent() {}
};
} // namespace Soda