//
// Created by phatt on 2/10/25.
//
#pragma once

#include "Core/Core.h"

namespace Engine
{
    class Texture
    {
    public:
        virtual ~Texture() = default;
    
        virtual const std::string& GetName() const = 0;
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void Bind(unsigned int slot = 0) const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(void* data, unsigned int size) = 0;
    };

    class Texture2D : public Texture
    {
    public:
        virtual ~Texture2D() override = default;

        static Ref<Texture2D> Create(const std::string& path);
        static Ref<Texture2D> Create(const std::string& name, const std::string& path);
        static Ref<Texture2D> Create(const std::string& name, unsigned int width, unsigned int height);
    };

}
