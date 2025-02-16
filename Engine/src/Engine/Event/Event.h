//
// Created by phatt on 2/9/25.
//
#pragma once

#include <string>
#include "Core/Core.h"

namespace Engine
{

    enum class EventType
    {
        None = 0,
        AppTick, AppUpdate, AppRender,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
    };


    enum EventCategory : int
    {
        None = 0,
        EventCategoryApplication    = EG_BIT(1),
        EventCategoryInput          = EG_BIT(2),
        EventCategoryKeyboard       = EG_BIT(3),
        EventCategoryMouse          = EG_BIT(4),
        EventCategoryMouseButton    = EG_BIT(5),
    };


    class Event
    {
        friend class EventDispatcher;
    public:
        virtual EventType GetEventType() const = 0;
        virtual int GetCategory() const = 0;
        virtual const char* GetName() const = 0;

        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategory category) const
        {
            return GetCategory() & category;
        }

        inline bool IsHandled() const { return m_Handled; }
    protected:
        bool m_Handled = false;
    };


#define EVENT_CLASS_TYPE(type) \
    const char* GetName() const override { return #type; } \
    static EventType GetStaticType() { return type; } \
    EventType GetEventType() const override { return GetStaticType(); }


#define EVENT_CLASS_CATEGORY(category) \
    int GetCategory() const override { return category; }


    class EventDispatcher
    {
    public:
        template<typename T>
        requires std::derived_from<T, Event> 
        && requires { { T::GetStaticType() } -> std::convertible_to<EventType>; }
        using EventFn = std::function<bool(T&)>;

        EventDispatcher(Event& event)
            : m_Event(event) {}

        template<typename T>
        bool Dispatch(EventFn<T>&& fn)
        {
            if (T::GetStaticType() == m_Event.GetEventType())
            {
                // get address of m_Event // now its *
                // cast it into T* // dereference into &
                m_Event.m_Handled = fn(*(T*)&m_Event);
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };


    inline std::ostream& operator << (std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }


    inline std::string format_as(const Event& e)
    {
        return e.ToString();
    }

}
