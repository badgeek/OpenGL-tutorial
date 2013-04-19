#include "matrix.h"

Matrix3x3::Matrix3x3()
{
    for(int i = 0; i < 9; i++)
    {
        mat[i] = 0.0;
    }
}


Matrix3x3::Matrix3x3(float* _mat)
{
    for(int i = 0; i < 9; i++)
    {
        mat[i] = _mat[i];
    }
}


Matrix3x3 Matrix3x3::operator*(Matrix3x3& _mat)
{
    float mul[9];
    float sum = 0.0;
    for (int x = 0 ; x < 3 ; x++) {
        for(int i = 0; i < 3; i++)
        {
            for (int k = 0; k < 3 ; k++) {
                sum += mat[k + (x*3)] * _mat.mat[i+(k*3)];
            }
            mul[((3*x) + i)] = sum;
            sum = 0.0;
        }
    }
    return Matrix3x3(mul);
}


Matrix3x3 Matrix3x3::operator*(float _mult)
{
    float sum[9];
    for(int i = 0; i < 9; i++)
    {
        sum[i] =  mat[i] * _mult;
    }
    return Matrix3x3(sum);
}

void Matrix3x3::print()
{
    for(int i = 0; i < 3; i++)
    {
        for (int k = 0; k < 3 ; k++) {
            printf(" %f ", mat[k + (i*3)]);
        }
        printf("\n");
    }
}

void Matrix3x3::mult(float _mult)
{
    for(int i = 0; i < 9; i++)
    {
        mat[i] =  mat[i] * _mult;
    }
}


void Matrix3x3::transpose()
{
    float tmp[9];
    memcpy(tmp, mat, 9 * sizeof(float));
    for(int i = 0; i < 3; i++)
    {
        for (int k = 0; k < 3 ; k++) {
            tmp[i+(k*3)] = mat[k + (i*3)];
        }
    }
    for(int k = 0; k < 9; k++)
    {
        mat[k] = tmp[k];
    }
}


float Matrix3x3::determinant()
{
    float minor[3];
    minor[0] = getMinor(mat[4], mat[5],mat[7], mat[8]);
    minor[1] = getMinor(mat[3], mat[5],mat[6], mat[8]);
    minor[2] = getMinor(mat[3], mat[4],mat[6], mat[7]);
    return (mat[0] * minor[0]) - (mat[1] * minor[1]) + (mat[2] * minor[2]);
}

Matrix3x3& Matrix3x3::inverse()
{
    //0 1 2  + - +
    //3 4 5  - + -
    //6 7 8  + - +

    //m0 m1 m2
    //m3 m4 m5
    //m6 m7 m8

    float minor[9];

    minor[0] = getMinor(mat[4], mat[5],mat[7], mat[8]);
    minor[1] = -getMinor(mat[3], mat[5],mat[6], mat[8]);
    minor[2] = getMinor(mat[3], mat[4],mat[6], mat[7]);

    float determinant = (mat[0] * minor[0]) - (mat[1] * minor[1]) + (mat[2] * minor[2]);

    minor[3] = -getMinor(mat[1], mat[2],mat[7], mat[8]);
    minor[4] = getMinor(mat[0], mat[2],mat[6], mat[8]);
    minor[5] = -getMinor(mat[0], mat[1],mat[6], mat[7]);

    minor[6] = getMinor(mat[1], mat[2],mat[4], mat[5]);
    minor[7] = -getMinor(mat[0], mat[2],mat[3], mat[5]);
    minor[8] = getMinor(mat[0], mat[1],mat[3], mat[4]);

    mat[0] = minor[0];
    mat[1] = minor[3];
    mat[2] = minor[6];
    mat[3] = minor[1];
    mat[4] = minor[4];
    mat[5] = minor[7];
    mat[6] = minor[2];
    mat[7] = minor[5];
    mat[8] = minor[8];

    this->mult(1.0/determinant);

    return *this;

}

float Matrix3x3::getMinor(float mat0, float mat1, float mat2, float mat3)
{
    return ( mat0 * mat3 - mat1 * mat2 );
}

Matrix4x4::Matrix4x4()
{
    for(int i = 0; i < 16; i++)
    {
        mat[i] = 0.0;
    }
}


Matrix4x4::Matrix4x4(float* _mat)
{
    for(int i = 0; i < 16; i++)
    {
        mat[i] = _mat[i];
    }
}

Matrix4x4 Matrix4x4::operator+(Matrix4x4& _mat)
{
    float sum[16];
    for(int i = 0; i < 16; i++)
    {
        sum[i] = _mat.mat[i] + mat[i];
    }
    return Matrix4x4(sum);
}

Matrix4x4 Matrix4x4::operator*(float _mult)
{
    float sum[16];
    for(int i = 0; i < 16; i++)
    {
        sum[i] =  mat[i] * _mult;
    }
    return Matrix4x4(sum);
}


void Matrix4x4::mult(float _mult)
{
    for(int i = 0; i < 16; i++)
    {
        mat[i] =  mat[i] * _mult;
    }
}


float*  Matrix4x4::getGlMat()
{
    for(int i = 0; i < 4; i++)
    {
        for (int k = 0; k < 4 ; k++) {
            glmat[k + (i*4)] =   mat[i+(k*4)];
        }
    }
    return glmat;
}

void Matrix4x4::setIdentity()
{
    mat[0] = 1;
    mat[1] = 0;
    mat[2] = 0;
    mat[3] = 0;

    mat[4] = 0;
    mat[5] = 1;
    mat[6] = 0;
    mat[7] = 0;

    mat[8] = 0;
    mat[9] = 0;
    mat[10] = 1;
    mat[11] = 0;

    mat[12] = 0;
    mat[13] = 0;
    mat[14] = 0;
    mat[15] = 1;


}

Matrix4x4 Matrix4x4::operator*(Matrix4x4& _mat)
{
    float mul[16];
    float sum = 0.0;
    for (int x = 0 ; x < 4 ; x++) {
        for(int i = 0; i < 4; i++)
        {
            for (int k = 0; k < 4 ; k++) {
                sum += mat[k + (x*4)] * _mat.mat[i+(k*4)];
            }
            mul[((4*x) + i)] = sum;
            sum = 0.0;
        }
    }
    return Matrix4x4(mul);
}

void Matrix4x4::print()
{
    for(int i = 0; i < 4; i++)
    {
        for (int k = 0; k < 4 ; k++) {
            printf(" %f ", mat[k + (i*4)]);
        }
        printf("\n");
    }
}


float Matrix4x4::getDeterminant()
{
    return
        mat[0] * getCofactor(mat[5], mat[6], mat[7], mat[9], mat[10], mat[11], mat[13], mat[14], mat[15]) -
        mat[1] * getCofactor(mat[4], mat[6], mat[7], mat[8], mat[10], mat[11], mat[12], mat[14], mat[15]) +
        mat[2] * getCofactor(mat[4], mat[5], mat[7], mat[8], mat[9], mat[11], mat[12], mat[13], mat[15]) -
        mat[3] * getCofactor(mat[4], mat[5], mat[6], mat[8], mat[9], mat[10], mat[12], mat[13], mat[14]);

}

float Matrix4x4::getCofactor(float mat1, float mat2, float mat3, float mat4, float mat5, float mat6, float mat7, float mat8, float mat9)
{
    return
        mat1 * ( mat5 * mat9 - mat6 * mat8 ) -
        mat2 * ( mat4 * mat9 - mat6 * mat7 ) +
        mat3 * ( mat4 * mat8 - mat5 * mat7 );
}

Matrix4x4& Matrix4x4::transpose()
{
    float tmp[16];
    memcpy(tmp, mat, 16 * sizeof(float));
    for(int i = 0; i < 4; i++)
    {
        for (int k = 0; k < 4 ; k++) {
            tmp[i+(k*4)] = mat[k + (i*4)];
        }
    }
    for(int k = 0; k < 16; k++)
    {
        mat[k] = tmp[k];
    }

    return *this;
}


Matrix4x4& Matrix4x4::inverse()
{

    //if affine matrix do affine inverse
    if ( mat[12] == 0.0f && mat[13] == 0.0f && mat[14] == 0.0f && mat[15] == 1.0f)
    {
        float mat3[9];

        //    0    1    2
        //    3    4    5
        //    6    7    8

        //    0    1    2    3
        //    4    5    6    7
        //    8    9    10   11
        //    12   13   14   15

        mat3[0] = mat[0];
        mat3[1] = mat[1];
        mat3[2] = mat[2];
        mat3[3] = mat[4];
        mat3[4] = mat[5];
        mat3[5] = mat[6];
        mat3[6] = mat[8];
        mat3[7] = mat[9];
        mat3[8] = mat[10];

        Matrix3x3 res(mat3);
        res.inverse();

        mat[0] = res.mat[0];
        mat[1] = res.mat[1];
        mat[2] = res.mat[2];
        mat[4] = res.mat[3];
        mat[5] = res.mat[4];
        mat[6] = res.mat[5];
        mat[8] = res.mat[6];
        mat[9] = res.mat[7];
        mat[10] = res.mat[8];

        mat[3] =  -((res.mat[0] * mat[3]) +  (res.mat[1] * mat[7]) + (res.mat[2] * mat[11]));
        mat[7] =  -((res.mat[3] * mat[3]) +  (res.mat[4] * mat[7]) + (res.mat[5] * mat[11]));
        mat[11] = -((res.mat[6] * mat[3]) +  (res.mat[7] * mat[7]) + (res.mat[8] * mat[11]));

    }
    return *this;

}


//implementasi sebelumnya error
Matrix4x4& Matrix4x4::translate(float x, float y, float z)
{
    mat[0] += mat[12]*x;   mat[1] += mat[13]*x;   mat[2] += mat[14]*x;   mat[3] += mat[15]*x;
    mat[4] += mat[12]*y;   mat[5] += mat[13]*y;   mat[6] += mat[14]*y;   mat[7] += mat[15]*y;
    mat[8] += mat[12]*z;   mat[9] += mat[13]*z;   mat[10]+= mat[14]*z;   mat[11]+= mat[15]*z;
    return *this;
}

Matrix4x4& Matrix4x4::rotateX(float _angle)
{
    float c = cosf(_angle * DEG2RAD);
    float s = sinf(_angle * DEG2RAD);

    float m4 = mat[4],  m5 = mat[5],  m6 = mat[6],  m7 = mat[7],  
		  m8 = mat[8],  m9 = mat[9],  m10 = mat[10],  m11 = mat[11];

    mat[4] = m4 * c + m8  * -s;
    mat[5] = m5 * c + m9  * -s;
    mat[6] = m6 * c + m10 * -s;
    mat[7] = m7 * c + m11 * -s;
    mat[8] = m4 * s + m8  *  c;
    mat[9] = m5 * s + m9  *  c;
    mat[10]= m6 * s + m10 *  c;
    mat[11]= m7 * s + m11 *  c;
    return *this;
}

Matrix4x4& Matrix4x4::rotateY(float _angle)
{
    float c = cosf(_angle * DEG2RAD);
    float s = sinf(_angle * DEG2RAD);

    float m0 = mat[0],  m1 = mat[1],  m2 = mat[2],  m3 = mat[3],  
		  m8 = mat[8],  m9 = mat[9],  m10 = mat[10],  m11 = mat[11];

    mat[0] = m0 *  c + m8  * s;
    mat[1] = m1 *  c + m9  * s;
    mat[2] = m2 *  c + m10 * s;
    mat[3] = m3 *  c + m11 * s;
    mat[8] = m0 * -s + m8  * c;
    mat[9] = m1 * -s + m9  * c;
    mat[10] = m2 * -s + m10 * c;
    mat[11] = m3 * -s + m11 * c;

    return *this;
}

Matrix4x4& Matrix4x4::rotateZ(float _angle)
{
    float c = cosf(_angle * DEG2RAD);
    float s = sinf(_angle * DEG2RAD);

    float m0 = mat[0],  m1 = mat[1],  m2 = mat[2],  m3 = mat[3],
          m4 = mat[4],  m5 = mat[5],  m6 = mat[6],  m7 = mat[7];

    mat[0] = m0 *  c + m4  * -s;
    mat[1] = m1 *  c + m5  * -s;
    mat[2] = m2 *  c + m6  * -s;
    mat[3] = m3 *  c + m7  * -s;
    mat[4] = m0 *  s + m4  *  c;
    mat[5] = m1 *  s + m5  *  c;
    mat[6]= m2 *  s + m6  *  c;
    mat[7]= m3 *  s + m7  *  c;

    return *this;
}

Matrix4x4& Matrix4x4::operator*=(Matrix4x4& _mat)
{
	float mul[16];
    float sum = 0.0;
    for (int x = 0 ; x < 4 ; x++) {
        for(int i = 0; i < 4; i++)
        {
            for (int k = 0; k < 4 ; k++) {
                sum += mat[k + (x*4)] * _mat.mat[i+(k*4)];
            }
            mul[((4*x) + i)] = sum;
            sum = 0.0;
        }
    }
	for (int _i = 0 ; _i<16; _i++)
	{
		mat[_i] = mul[_i];
	}
	return *this;
}	

Vector3 Matrix4x4::operator*(Vector3& _vec)
{
	float res[4];

	res[0] = mat[0]*_vec.x +  mat[1]*_vec.y +  mat[2]*_vec.z +  mat[3]  *_vec.w;
	res[1] = mat[4]*_vec.x +  mat[5]*_vec.y +  mat[6]*_vec.z +  mat[7]  *_vec.w;
	res[2] = mat[8]*_vec.x +  mat[9]*_vec.y +  mat[10]*_vec.z + mat[11] *_vec.w;
	res[3] = mat[12]*_vec.x + mat[13]*_vec.y + mat[14]*_vec.z + mat[15] *_vec.w;
	
	Vector3 vec_res(res[0], res[1], res[2], res[3]);

	return vec_res;
}

std::ostream& operator<<(std::ostream& os, Matrix4x4& m)
{
    os << m.mat[0] << "\t" <<  m.mat[1] << "\t" <<  m.mat[2] << "\t" <<  m.mat[3] << std::endl
       << m.mat[4] << "\t" <<  m.mat[5] << "\t" <<  m.mat[6] << "\t" <<  m.mat[7]<< std::endl
       << m.mat[8] << "\t" <<  m.mat[9] << "\t" <<  m.mat[10] << "\t" <<  m.mat[11]<< std::endl
       << m.mat[12] << "\t" <<  m.mat[13] << "\t" <<  m.mat[14] << "\t" <<  m.mat[15]<< std::endl;
    return os;
}


Vector3::Vector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
	w = 1;
}


Vector3::Vector3(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void Vector3::print()
{
	printf("%f\n%f\n%f\n", x,y,z);
}


Vector3 Vector3::cross(Vector3& _vec)
{
	float res[3];
	res[0] = y * _vec.z - z * _vec.y;	
	res[1] = -(x * _vec.z - z * _vec.x);	 	
	res[2] = x * _vec.y - y * _vec.x;	

	return Vector3(res[0], res[1], res[2], 1);
}
