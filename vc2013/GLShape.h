#pragma once

#include <memory>

namespace maxlife
{
    class GLShape
    {
    public:
        GLShape();
        virtual ~GLShape();

        virtual void Draw() const = 0;
    };

    using GLShapeRef = std::shared_ptr<GLShape>;

}
