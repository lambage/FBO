#include "GLRectangle.h"

#ifdef ALOGE
#include <GLES2/gl2.h>
#else
#include <stdexcept>
#include "glload/gl_core.h"
#endif

namespace maxlife
{
    struct GLRectangle::Impl 
    {
        unsigned int vao;

        GLuint m_vertex_buffer_id;
        GLuint m_texture_coordinates_buffer_id;

        float m_vertices[12];
        float m_texture_coordinates[8];
    };

    GLRectangle::GLRectangle()
        : m_impl(std::make_unique<GLRectangle::Impl>())
    {
        glGenVertexArrays(1, &m_impl->vao);
        glBindVertexArray(m_impl->vao);

        glGenBuffers(1, &m_impl->m_vertex_buffer_id);
        glGenBuffers(1, &m_impl->m_texture_coordinates_buffer_id);

        m_impl->m_vertices[0] = -1.0f; m_impl->m_vertices[1] = -1.0f, m_impl->m_vertices[2] = 0.0f;
        m_impl->m_vertices[3] =  1.0f; m_impl->m_vertices[4] = -1.0f, m_impl->m_vertices[5] = 0.0f;
        m_impl->m_vertices[6] =  1.0f; m_impl->m_vertices[7] = 1.0f,  m_impl->m_vertices[8] = 0.0f;
        m_impl->m_vertices[9] = -1.0f; m_impl->m_vertices[10] = 1.0f, m_impl->m_vertices[11] = 0.0f;

        m_impl->m_texture_coordinates[0] = 0.0f; m_impl->m_texture_coordinates[1] = 0.0f;
        m_impl->m_texture_coordinates[2] = 1.0f; m_impl->m_texture_coordinates[3] = 0.0f;
        m_impl->m_texture_coordinates[4] = 1.0f; m_impl->m_texture_coordinates[5] = 1.0f;
        m_impl->m_texture_coordinates[6] = 0.0f; m_impl->m_texture_coordinates[7] = 1.0f;

        glBindBuffer(GL_ARRAY_BUFFER, m_impl->m_vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), m_impl->m_vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_impl->m_texture_coordinates_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), m_impl->m_texture_coordinates, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    GLRectangle::~GLRectangle()
    {
        glDeleteBuffers(1, &m_impl->m_vertex_buffer_id);
        glDeleteBuffers(1, &m_impl->m_texture_coordinates_buffer_id);
    }

    void GLRectangle::Draw() const
    {
        glBindVertexArray(m_impl->vao);
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_impl->m_vertex_buffer_id);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        // 2nd attribute buffer : texture coordinates
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, m_impl->m_texture_coordinates_buffer_id);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

        // Draw call
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

    GLShapeRef GLRectangle::Create()
    {
        return std::shared_ptr<GLShape>(std::make_shared<GLRectangle>());
    }
}
