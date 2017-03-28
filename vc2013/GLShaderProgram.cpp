#include "GLShaderProgram.h"
#include "GLMatrix.h"

#include <map>
#ifdef ALOGE
#include <GLES2/gl2.h>
#else
#include <stdexcept>
#include "glload/gl_core.h"
#endif

namespace maxlife
{
    struct GLShaderProgram::Impl
    {
        GLuint program;
        std::map<std::string, GLint> uniform_cache;

        GLint GetUniform(const char * name)
        {
            GLint retval = -1;
            auto it = uniform_cache.find(name);
            if (it == std::end(uniform_cache))
            {
                auto location = glGetUniformLocation(program, name);
                if (location >= 0)
                {
                    uniform_cache[name] = location;
                    retval = location;
                }
            }
            else
            {
                retval = it->second;
            }
            return retval;
        }
    };

    struct GLShaderObject
    {
        GLShaderObject(GLenum shader_type, const char * shader_source)
        {
            m_shader_object = glCreateShader(shader_type);

            glShaderSource(m_shader_object, 1, &shader_source, 0);
            glCompileShader(m_shader_object);
            GLint status;
            glGetShaderiv(m_shader_object, GL_COMPILE_STATUS, &status);
            if (status != GL_TRUE) {
                // Some drivers return wrong values for GL_INFO_LOG_LENGTH
                // use a fixed size instead
                GLchar log[1024];
                glGetShaderInfoLog(m_shader_object, sizeof(log), 0, log);
#ifdef ALOGE
                ALOGE("Error while compiling shader: \n%s\n%s", shader_source, log);
#else
                throw std::runtime_error(log);
#endif
                glDeleteShader(m_shader_object);
            }
        }

        ~GLShaderObject()
        {
            glDeleteShader(m_shader_object);
        }

        GLuint GetShaderObject() const { return m_shader_object; }

    private:
        GLuint m_shader_object;
    };

    GLShaderProgram::GLShaderProgram(const char * vertex_shader_source, const char * fragment_shader_source)
        : m_impl(std::make_unique<GLShaderProgram::Impl>())
    {
        auto vertex = GLShaderObject(GL_VERTEX_SHADER, vertex_shader_source);
        auto fragment = GLShaderObject(GL_FRAGMENT_SHADER, fragment_shader_source);
        auto program_id = glCreateProgram();

        glAttachShader(program_id, vertex.GetShaderObject());
        glAttachShader(program_id, fragment.GetShaderObject());
        glLinkProgram(program_id);

        GLint status;
        glGetProgramiv(program_id, GL_LINK_STATUS, &status);
        if (status != GL_TRUE) 
        {
            GLchar log[1024];
            glGetProgramInfoLog(program_id, sizeof(log), 0, &log[0]);
            glDetachShader(program_id, vertex.GetShaderObject());
            glDetachShader(program_id, fragment.GetShaderObject());
            glDeleteProgram(program_id);

#ifdef ALOGE
            ALOGE("Error while linking shaders: %s", log);
#else
            throw std::runtime_error(std::string("Error while linking shaders: " ) + log);
#endif
        }
        else 
        {
            m_impl->program = program_id;
        }
    }

    GLShaderProgram::~GLShaderProgram()
    {
        glDeleteProgram(m_impl->program);
    }

    unsigned int GLShaderProgram::GetProgramID() const
    {
        return m_impl->program;
    }

    void GLShaderProgram::Uniform1i(const char * name, int v0)
    {
        auto location = m_impl->GetUniform(name);
        glUniform1i(location, v0);
    }

    void GLShaderProgram::UniformMatrix(const char * name, const GLMatrix&  value)
    {
        auto location = m_impl->GetUniform(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, value());
    }

    GLShaderProgramRef GLShaderProgram::Create(const char * vertex_shader_source, const char * fragment_shader_source)
    {
        return std::make_shared<GLShaderProgram>(vertex_shader_source, fragment_shader_source);
    }

    ScopedGLProgram::ScopedGLProgram(const GLShaderProgramRef & program)
    {
        if (program != nullptr)
        {
            glUseProgram(program->GetProgramID());
            m_program_ref = program;
        }
    }

    ScopedGLProgram::~ScopedGLProgram()
    {
        glUseProgram(0);
    }
    GLShaderProgramRef ScopedGLProgram::operator()()
    {
        return m_program_ref;
    }
}