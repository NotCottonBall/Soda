#pragma once

// this is the main file for Events.
// it is where the infamous Event class resides

#include "SD_PCH.h"

#include "Soda/_Main/Core.h"

namespace Soda
{
enum class EventType
{
  None = 0,
  WindowClose,
  WindowResize,
  WindowFocus,
  WindowNoFocus,
  WindowMove,
  AppTick,
  AppUpdate,
  AppRender,
  KeyPress,
  KeyRelease,
  KeyTyped,
  MouseClick,
  MouseRelease,
  MouseMove,
  MouseScroll
};

// so yea... we use BIT() here to make it easier for us to check if something is
// in a category without bools and shit
enum EventCategory
{
  None = 0,
  EventCategory_App = BIT(0),
  EventCategory_Input = BIT(1),
  EventCategory_Keyboard = BIT(2),
  EventCategory_Mouse = BIT(3),
  EventCategory_MouseButton = BIT(4)
};

// the Event class will carry the basic abrivations for the Events like its
// Name, what category it is, and some other stuff
class Event
{
  friend class EventDispatcher;

public:
  virtual EventType GetEventType() const = 0;
  virtual const char *GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;
  virtual std::string ToString() const { return GetName(); }

  inline bool IsInCategory(EventCategory category)
  {
    return GetCategoryFlags() & category;
  }

  bool m_Handled = false;
};

// who wants to type all the Event methods in each event we make dude... common
// thats why we made this define. To make life easier
#define EVENT_CLASS_TYPE(type)                                                 \
  static EventType GetStaticType() { return EventType::type; }                 \
  virtual EventType GetEventType() const override { return GetStaticType(); }  \
  virtual const char *GetName() const override { return #type; }

// same here... no typing big things... we just need the define
#define EVENT_CLASS_CATEGORY(category)                                         \
  virtual int GetCategoryFlags() const override { return category; }

// this class dispatches the Events with the right function to execute
class EventDispatcher
{
public:
  EventDispatcher(Event &event) : m_Event(event) {}

  // F will be deduced by the compiler
  template <typename T, typename F> bool Dispatch(const F &func)
  {
    if(m_Event.GetEventType() == T::GetStaticType())
    {
      m_Event.m_Handled = func(static_cast<T &>(m_Event));
      return true;
    }

    return false;
  }

private:
  Event &m_Event;
};

inline std::ostream &operator<<(std::ostream &os, const Event &e)
{
  return os << e.ToString();
}
} // namespace Soda