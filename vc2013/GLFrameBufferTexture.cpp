#include "GLFrameBufferTexture.h"

#ifdef ALOGE
#include <GLES2/gl2.h>
#else
#include <stdexcept>
#include "glload/gl_core.h"
#endif

namespace maxlife
{
    struct GLFrameBufferTexture::Impl
    {
        GLuint fbo;
        GLuint texture;
    };

    GLFrameBufferTexture::GLFrameBufferTexture()
        : m_impl(std::make_unique<GLFrameBufferTexture::Impl>())
    {
        glGenFramebuffers(1, &m_impl->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_impl->fbo);

        glGenTextures(1, &m_impl->texture);
        glBindTexture(GL_TEXTURE_2D, m_impl->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_impl->texture, 0);
        GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, DrawBuffers);
    }


    GLFrameBufferTexture::~GLFrameBufferTexture()
    {
        glDeleteTextures(1, &m_impl->texture);
        glDeleteFramebuffers(1, &m_impl->fbo);
    }

    void GLFrameBufferTexture::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_impl->fbo);
    }

    unsigned int GLFrameBufferTexture::GetTextureID() const
    {
        return m_impl->texture;
    }

    GLFrameBufferTextureRef GLFrameBufferTexture::Create()
    {
        return std::make_shared<GLFrameBufferTexture>();
    }

    ScopedFrameBufferTexture::ScopedFrameBufferTexture(GLFrameBufferTextureRef fbt)
    {
        if (fbt != nullptr)
        {
            m_fbt_ref = fbt;
            m_fbt_ref->Bind();
        }
    }

    ScopedFrameBufferTexture::~ScopedFrameBufferTexture()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLFrameBufferTextureRef ScopedFrameBufferTexture::operator()()
    {
        return m_fbt_ref;
    }
}