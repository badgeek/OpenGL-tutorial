#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cmath>

const float DEG2RAD = 3.141593f / 180;

class Vector3
{
	public:
	float x,y,z,w;
	Vector3(float _x, float _y, float _z);
	Vector3(float _x, float _y, float _z, float _w);
	Vector3 cross(Vector3& _vec);		
	void print();
};

class Matrix3x3
{
    public:
    float mat[9];
    
    Matrix3x3(); 
    Matrix3x3(float* _mat);
    
	Matrix3x3 operator*(Matrix3x3& _mat);
    Matrix3x3 operator*(float _mult);
    Matrix3x3& inverse();

    float determinant();
    float getMinor(float mat0, float mat1, float mat2, float mat3);
    void transpose();
    void mult(float _mult);  
    void print();
};


class Matrix4x4
{
    public:
    float mat[16];
    float glmat[16];
    
    
    Matrix4x4();
    Matrix4x4(float* _mat);
    
	Matrix4x4 operator+(Matrix4x4& _mat);
    Matrix4x4 operator*(float _mult);
    Matrix4x4 operator*(Matrix4x4& _mat);	

	Vector3 operator*(Vector3& _vec);	
	
    Matrix4x4& operator*=(Matrix4x4& _mat);	
	
    Matrix4x4& inverse();
    Matrix4x4& transpose();	
	Matrix4x4& translate(float x, float y, float z);
	Matrix4x4& rotateX(float _angle);
 	Matrix4x4& rotateY(float _angle);
	Matrix4x4& rotateZ(float _angle);


    void mult(float _mult);
    void setIdentity();	
    float*  getGlMat();
    float getDeterminant();
    float getCofactor(float mat1, float mat2, float mat3, float mat4, float mat5, float mat6, float mat7, float mat8, float mat9);
    void print();
	friend std::ostream& operator<<(std::ostream& os, Matrix4x4& m);
    private:
    
};


