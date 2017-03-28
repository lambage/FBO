#pragma once

#include <memory>

namespace maxlife
{
    class GLFrameBufferTexture;
    using GLFrameBufferTextureRef = std::shared_ptr<GLFrameBufferTexture>;

    class GLFrameBufferTexture
    {
    public:
        GLFrameBufferTexture();
        ~GLFrameBufferTexture();

        void Bind();
        unsigned int GetTextureID() const;

        static GLFrameBufferTextureRef Create();
    private:
        struct Impl;
        std::unique_ptr<GLFrameBufferTexture::Impl> m_impl;
    };

    struct ScopedFrameBufferTexture
    {
        ScopedFrameBufferTexture(GLFrameBufferTextureRef fbt);
        ~ScopedFrameBufferTexture();

        GLFrameBufferTextureRef operator()();
    private:
        GLFrameBufferTextureRef m_fbt_ref;
    };
}
