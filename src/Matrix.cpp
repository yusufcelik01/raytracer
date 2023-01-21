#include "Matrix.hpp"
#include <iostream>


Matrix::Matrix()
{
    rows = 4; 
    columns = 4;

    data = new float*[rows];
    for(size_t i = 0; i < rows; ++i)
    {
        data[i] = new float[columns];
    }
}

Matrix::~Matrix()
{
    for(size_t i = 0; i < rows; ++i)
    {
        delete[] data[i];
    }
    delete[] data;
}

Matrix::Matrix(size_t r, size_t c)
{
    rows = r; 
    columns = c;

    data = new float*[rows];
    for(size_t i = 0; i < rows; ++i)
    {
        data[i] = new float[columns];
        for(size_t j = 0; j < columns; j++)
        {
            data[i][j] = 0.f;
        }
    }
}

Matrix::Matrix(const Matrix& rhs)
{
    rows = rhs.rows; 
    columns = rhs.columns;

    data = new float*[rows];
    for(size_t i = 0; i < rows; ++i)
    {
        data[i] = new float[columns];
        for(size_t j = 0; j < columns; j++)
        {
            data[i][j] = rhs.data[i][j];
        }
    }
}

vec2f Matrix::getSize() const
{
    return vec2f(rows, columns);
}

Matrix Matrix::operator*(const Matrix& rhs) const
{
    if(this->columns != rhs.rows)
    {
        std::cerr << "invalid matrix multiplication" << std::endl;
        exit(-1);
    }
    Matrix result(rows, rhs.columns);
    for(size_t i = 0; i < this->rows; i++) {
        for(size_t j = 0; j < rhs.columns; j++) {
            float sum = 0.f;
            for(size_t k = 0; k < this->columns; k++) {
                sum += data[i][k] * rhs.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }
    return result; 
}

Matrix& Matrix::operator=(const Matrix& rhs)
{
    this->~Matrix();//free memory
                    
    rows = rhs.rows; 
    columns = rhs.columns;

    data = new float*[rows];
    for(size_t i = 0; i < rows; ++i)
    {
        data[i] = new float[columns];
        for(size_t j = 0; j < columns; j++)
        {
            data[i][j] = rhs.data[i][j];
        }
    }
    return *this;
}

Matrix Matrix::operator*(float c) const
{
    Matrix result(*this);
    
    for(size_t i = 0; i < rows; ++i)
    {
        for(size_t j = 0; j < columns; j++)
        {
            result.data[i][j] *= c;
        }
    }
    return result;
}

vec3f Matrix::operator*(const vec3f& rhs) const
{
    if(columns != 3 || rows !=3){
        std::cerr << "invalid matrix multiplication" << std::endl;
        exit(-1);
    }
    float result[3];

    for(size_t i = 0; i < 3; i++)
    {
        result[i] = data[i][0] * rhs.x
                   + data[i][1] * rhs.y
                   + data[i][2] * rhs.z;        
    }
    return vec3f(result[0], result[1], result[2]); 
}

Matrix Matrix::inverse() const
{
    if(rows != columns){
        std::cerr << "Tried to invert non-square matrix quiting"
            << std::endl;
        exit(-1);
    }
    if(rows == 2) 
    {
        Matrix inv(2,2);
        float a,b,c,d;
        a = data[0][0];
        b = data[0][1];
        c = data[1][0];
        d = data[1][1];
        
        inv.data[0][0] = d;
        inv.data[0][1] = -b;
        inv.data[1][0] = -c;
        inv.data[1][1] = a;

        return inv * (1.f/(a*d - b*c));
    }
    //else if( rows == 3)
    //{

    //}
    else
    {
        std::cerr << "trying to invert an unsupportted matrix size" << std::endl;
        exit(-1);
    }

}
