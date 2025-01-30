#pragma once

// this is where all the keyboard events reside

#include "Events.h"

namespace Soda
{
// first we make a base keyEvent class
// this will be inherited by our both KeyPressed event and KeyRelease Event
class KeyEvent : public Event
{
public:
  // this is the define i talked about in Events.h
  // they make life easier by setting the Event type and other stuff
  // you can see what this define exactly does in the Events.h file
  EVENT_CLASS_CATEGORY(EventCategory_Keyboard | EventCategory_Input)

  inline int GetKeyCode() const { return m_KeyCode; }

protected:
  // this constructor makes sure that the KeyEvent can be only
  // constructed/created in a class that inherits the KeyEvent Class
  KeyEvent(int keycode) : m_KeyCode(keycode) {}

  int m_KeyCode;
};

class KeyPressEvent : public KeyEvent
{
public:
  // this is the other define that makes your life easy by typing less shit
  // you can see what this does in the Events.h file
  EVENT_CLASS_TYPE(KeyPress)

  KeyPressEvent(int keycode, int Reapeter)
      : KeyEvent(keycode), m_Repeater(Reapeter)
  {
  }

  inline int GetReapeter() const { return m_Repeater; }

  std::string ToString() const override
  {
    std::stringstream msg;
    msg << "KeyPressedEvent: " << m_KeyCode << " (" << m_Repeater
        << " repeats)";
    return msg.str();
  }

private:
  int m_Repeater;
};

class KeyReleaseEvent : public KeyEvent
{
public:
  EVENT_CLASS_TYPE(KeyRelease)

  KeyReleaseEvent(int keycode) : KeyEvent(keycode) {}

  std::string ToString() const override
  {
    std::stringstream msg;
    msg << "KeyReleasedEvent: " << m_KeyCode;
    return msg.str();
  }
};

class KeyTypeEvent : public KeyEvent
{
public:
  EVENT_CLASS_TYPE(KeyTyped)

  KeyTypeEvent(int typedValue) : KeyEvent(typedValue) {}

  std::string ToString() const override
  {
    std::stringstream msg;
    msg << "KeyTypedEvent: " << m_KeyCode;
    return msg.str();
  }
};
} // namespace Soda