//
// Created by phatt on 2/9/25.
//
#pragma once

#include "EnginePCH.h"
#include "Event.h"

namespace Engine
{

    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y)
            : m_MouseX(x), m_MouseY(y) {}

        inline float GetX() const { return m_MouseX; }
        inline float GetY() const { return m_MouseY; }

        std::string ToString() const override
        {
            return std::format("MouseMovedEvent: x={}, y={}", m_MouseX, m_MouseY);
        }

        EVENT_CLASS_TYPE(EventType::MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
    private:
        float m_MouseX, m_MouseY;
    };


    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {}

        inline float GetXOffset() const { return m_XOffset; }
        inline float GetYOffset() const { return m_YOffset; }

        EVENT_CLASS_TYPE(EventType::MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
    private:
        float m_XOffset, m_YOffset;
    };


    class MouseButtonEvent : public Event
    {
    public:
        inline int GetMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
    protected:
        MouseButtonEvent(int button)
            : m_Button(button) {}

        int m_Button;
    };


    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override
        {
            return std::format("MouseButtonPressed: {}", m_Button);
        }

        EVENT_CLASS_TYPE(EventType::MouseScrolled)
    };


    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override
        {
            return std::format("MouseButtonReleasedEvent: {}", m_Button);
        }

        EVENT_CLASS_TYPE(EventType::MouseScrolled)
    };


}
