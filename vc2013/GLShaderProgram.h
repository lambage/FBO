#pragma once

#include <memory>

namespace maxlife
{
    class GLMatrix;

    class GLShaderProgram;
    using GLShaderProgramRef = std::shared_ptr<GLShaderProgram>;

    class GLShaderProgram
    {
    public:
        GLShaderProgram(const char * vertex_shader_source, const char * fragment_shader_source);
        ~GLShaderProgram();

        unsigned int GetProgramID() const;
        void Uniform1i(const char * name, int v0);
        void UniformMatrix(const char * name, const GLMatrix& value);

        static GLShaderProgramRef Create(const char * vertex_shader_source, const char * fragment_shader_source);
    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    struct ScopedGLProgram
    {
        ScopedGLProgram(const GLShaderProgramRef& program);
        ~ScopedGLProgram();

        GLShaderProgramRef operator()();
    private:
        GLShaderProgramRef m_program_ref;
    };
}
