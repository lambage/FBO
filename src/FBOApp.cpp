#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include <vector>
#include "GLShaderProgram.h"
#include "GLFrameBufferTexture.h"
#include "GLRectangle.h"
#include "GLTriangle.h"
#include "GLMatrix.h"

using namespace ci;
using namespace ci::app;

class FBOApp : public App {
public:
    void setup() override;
    void update() override;
	void draw() override;



private:
    maxlife::GLShaderProgramRef m_final_program;
    maxlife::GLFrameBufferTextureRef m_fbo;
    maxlife::GLShapeRef m_rectangle;
    maxlife::GLShapeRef m_triangle;
    maxlife::GLMatrix m_matrix;

    void FinalRender(unsigned int texture_id);
    unsigned int RenderToTexture();
};

void FBOApp::setup()
{
    m_fbo = maxlife::GLFrameBufferTexture::Create();
    
    const GLchar*  vertex_shader = " #version 330 core \n \
        uniform mat4	ModelViewProjectMatrix; \n \
        in vec4			ciPosition; \n \
        in vec2			ciTexCoord0; \n \
        out vec2		TexCoord0; \n \
        \
        void main(void) { \
            gl_Position = ModelViewProjectMatrix * ciPosition; \n \
            TexCoord0 = ciTexCoord0; \n \
        }";

    const GLchar*  fragment_shader = " #version 330 core \n \
        out vec4			oColor; \n \
        in vec2				TexCoord0; \n \
        uniform sampler2D   RenderedTexture; \n \
        void main(void) { \n \
            oColor = texture(RenderedTexture, TexCoord0); \n \
        }";

    m_final_program = maxlife::GLShaderProgram::Create(vertex_shader, fragment_shader);
    m_rectangle = maxlife::GLRectangle::Create();
    m_matrix = maxlife::GLMatrix::Identity();

    m_triangle = maxlife::GLTriangle::Create();
}

void FBOApp::update()
{
}

void FBOApp::draw()
{
    auto texture_id = RenderToTexture();
    FinalRender(texture_id);
}

unsigned int FBOApp::RenderToTexture()
{
    auto scoped_fbt = maxlife::ScopedFrameBufferTexture(m_fbo);
    glViewport(0, 0, 1920, 1024);
    
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (m_triangle)
    {
        m_triangle->Draw();
    }

    return m_fbo->GetTextureID();
}

void FBOApp::FinalRender(unsigned int texture_id)
{
    auto window_size = app::getWindowSize();
    glViewport(0, 0, window_size.x / 2, window_size.y / 2);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto scoped_program = maxlife::ScopedGLProgram(m_final_program);
   
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    scoped_program()->Uniform1i("RenderedTexture", 0);

    scoped_program()->UniformMatrix("ModelViewProjectMatrix", m_matrix);

    if (m_rectangle)
    {
        m_rectangle->Draw();
    }

}

CINDER_APP( FBOApp, RendererGl )
