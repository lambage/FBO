#include "GLTriangle.h"
#include "GLShaderProgram.h"
#include "GLMatrix.h"

#include <array>

#ifdef ALOGE
#include <GLES2/gl2.h>
#else
#include <stdexcept>
#include "glload/gl_core.h"
#endif

namespace maxlife
{
    struct VertexFormat
    {
        std::array<float, 3> position;
        std::array<float, 4> color;

        VertexFormat(const std::array<float, 3>& position, const std::array<float, 4>& color)
            : position(position)
            , color(color)
        {
        }
    };

    struct GLTriangle::Impl
    {
        GLShaderProgramRef program;
        std::array<VertexFormat, 3> vertices;

        unsigned int vao;
        unsigned int vbo;

        Impl()
            : vertices({ { {{0.5f, -0.5f, 0.0f},{ 1.0f, 0.0f, 0.0f, 1.0f}},
            {{ -0.5f, -0.5, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f }},
            {{ 0.5f, 0.5f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f }} } })
        {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)12);
        }

        ~Impl()
        {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
        }
    };

    GLTriangle::GLTriangle()
        : m_impl(std::make_unique<GLTriangle::Impl>())
    {
        const GLchar*  vertex_shader = " #version 330 core \n \
        uniform mat4	ModelViewProjectMatrix; \n \
        layout (location = 0) in vec3			inPosition; \n \
        layout (location = 1) in vec4		inColor; \n \
        out vec4    Color; \n \
        \
        void main(void) { \
            gl_Position = ModelViewProjectMatrix * vec4(inPosition, 1); \n \
            Color = inColor; \n \
        }";

        const GLchar*  fragment_shader = " #version 330 core \n \
        out vec4			oColor; \n \
        in vec2				TexCoord0; \n \
        in vec4   Color; \n \
        void main(void) { \n \
            oColor = Color; \n \
        }";

        m_impl->program = GLShaderProgram::Create(vertex_shader, fragment_shader);
    }


    GLTriangle::~GLTriangle()
    {
    }

    void GLTriangle::Draw() const
    {
        auto scoped_program = maxlife::ScopedGLProgram(m_impl->program);

        scoped_program()->UniformMatrix("ModelViewProjectMatrix", GLMatrix::Identity());

        glBindVertexArray(m_impl->vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_impl->vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)12);

        // Draw call
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    GLShapeRef GLTriangle::Create()
    {
        return std::shared_ptr<GLShape>(std::make_shared<GLTriangle>());
    }
}