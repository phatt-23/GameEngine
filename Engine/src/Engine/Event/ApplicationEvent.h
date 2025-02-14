//
// Created by phatt on 2/9/25.
//
#pragma once

#include "Core/Core.h"
#include "Event/Event.h"

namespace Engine
{

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_Width(width), m_Height(height) {}

        inline unsigned int GetWidth() const { return m_Width; }
        inline unsigned int GetHeight() const { return m_Height; }

        std::string ToString() const override
        {
            return std::format("WindowResizeEvent: width={}, height={}", m_Width, m_Height);
        }

        EVENT_CLASS_TYPE(EventType::WindowResize);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    private:
        unsigned int m_Width;
        unsigned int m_Height;
    };


    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        std::string ToString() const override
        {
            return "WindowCloseEvent";
        }

        EVENT_CLASS_TYPE(EventType::WindowClose);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };


    class AppTickEvent : public Event
    {
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppTick);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };


    class AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppUpdate);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };


    class AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppRender);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };

}

