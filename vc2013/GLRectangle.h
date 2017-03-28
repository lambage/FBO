#pragma once

#include "GLShape.h"
#include <memory>

namespace maxlife
{
    class GLRectangle : public GLShape
    {
    public:
        GLRectangle();
        ~GLRectangle();

        virtual void Draw() const;

        static GLShapeRef Create();

    private:
        struct Impl;
        std::unique_ptr<GLRectangle::Impl> m_impl;
    };
}

