//
// Created by phatt on 2/11/25.
//
#pragma once

namespace Engine
{
    class Timestep
    {
    public:
        Timestep(float&& time = 0.0f)
            : m_Time(time)
        {}

        float GetSeconds() const { return m_Time; }
        float GetMilliseconds() const { return 1000.f * m_Time; }

        // implicit cast to float
        operator float() const { return m_Time; }

    private:
        float m_Time;
    };
}

