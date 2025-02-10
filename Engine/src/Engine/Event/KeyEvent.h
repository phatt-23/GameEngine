//
// Created by phatt on 2/9/25.
//
#pragma once

#include "Event.h"

namespace Engine {

    class KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);
    protected:
        KeyEvent(int keyCode)
            : m_KeyCode(keyCode) {}

        int m_KeyCode;
    };


    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keyCode, int repeatCount)
            : KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

        inline int GetRepeatCount() const { return m_RepeatCount; }

        std::string ToString() const override
        {
            return std::format("KeyPressedEvent: keyCode={}, repeatCount={}",
                               m_KeyCode, m_RepeatCount);
        }

        EVENT_CLASS_TYPE(EventType::KeyPressed)
    private:
        int m_RepeatCount;
    };


    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keyCode)
            : KeyEvent(keyCode) {}

        std::string ToString() const override
        {
            return std::format("KeyReleasedEvent: keyCode={}", m_KeyCode);
        }

        EVENT_CLASS_TYPE(EventType::KeyReleased)
    };


    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(int keyCode)
            : KeyEvent(keyCode) {}

        std::string ToString() const override
        {
            return std::format("KeyTypedEvent: keyCode={}", m_KeyCode);
        }

        EVENT_CLASS_TYPE(EventType::KeyTyped)
    };

}
