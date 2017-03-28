#pragma once
namespace maxlife
{
    class GLMatrix
    {
    public:
        GLMatrix();
        ~GLMatrix();

        const float* operator()() const { return data; }

        static GLMatrix Identity();
    
    private:
        float data[16];
    };
}

