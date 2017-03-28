#include "GLMatrix.h"


namespace maxlife
{
    GLMatrix::GLMatrix()
        : data {  //initialize as identity matrix
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
          }
    {
    }


    GLMatrix::~GLMatrix()
    {
    }

    GLMatrix GLMatrix::Identity()
    {
        return GLMatrix();
    }
}