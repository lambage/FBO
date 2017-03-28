#pragma once

#include "GLShape.h"
#include <memory>

namespace maxlife
{
    class GLTriangle : public GLShape
    {
    public:
        GLTriangle();
        ~GLTriangle();

        virtual void Draw() const;

        static GLShapeRef Create();

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
