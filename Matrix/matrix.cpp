#include <iostream>
#include <stdio.h>

class Matrix3x3
{
public:
      float mat[9];

      Matrix3x3()
      {
        for(int i = 0; i < 9; i++)
        {
            mat[i] = 0.0;
        }
      } 
    
    
      Matrix3x3(float* _mat)
      {
        for(int i = 0; i < 9; i++)
        {
            mat[i] = _mat[i];
        }
      }


    Matrix3x3 operator*(Matrix3x3& _mat)
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


      Matrix3x3 operator*(float _mult)
      {
        float sum[9];
        for(int i = 0; i < 9; i++)
        {
            sum[i] =  mat[i] * _mult;
        }
        return Matrix3x3(sum);
      }

      void print()
        {
            for(int i = 0; i < 3; i++)
            {
                for (int k = 0; k < 3 ; k++) {
                    printf(" %f ", mat[k + (i*3)]);
                }            
                printf("\n");
            }
        }

          void mult(float _mult)
          {
            for(int i = 0; i < 9; i++)
            {
                mat[i] =  mat[i] * _mult;
            }
          }


          void transpose()
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


      float determinant()
      {
        float minor[3];
         minor[0] = getMinor(mat[4], mat[5],mat[7], mat[8]);
         minor[1] = getMinor(mat[3], mat[5],mat[6], mat[8]);
         minor[2] = getMinor(mat[3], mat[4],mat[6], mat[7]);
        return (mat[0] * minor[0]) - (mat[1] * minor[1]) + (mat[2] * minor[2]);
      }
    
      Matrix3x3 inverse()
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
                         
            Matrix3x3 res(minor);
            res.transpose();
            res.mult(1.0/determinant);
            
            return res;
            
      }    
    
      float getMinor(float mat0, float mat1, float mat2, float mat3)
      {
        return ( mat0 * mat3 - mat1 * mat2 );
      }

};


class Matrix4x4
{
  public:
  float mat[16];
  float glmat[16];


  Matrix4x4()
  {
    for(int i = 0; i < 16; i++)
    {
        mat[i] = 0.0;
    }
  }  


  Matrix4x4(float* _mat)
  {
    for(int i = 0; i < 16; i++)
    {
        mat[i] = _mat[i];
    }
  }

  Matrix4x4 operator+(Matrix4x4& _mat)
  {
    float sum[16];
    for(int i = 0; i < 16; i++)
    {
        sum[i] = _mat.mat[i] + mat[i];
    }
    return Matrix4x4(sum);
  }

  Matrix4x4 operator*(float _mult)
  {
    float sum[16];
    for(int i = 0; i < 16; i++)
    {
        sum[i] =  mat[i] * _mult;
    }
    return Matrix4x4(sum);
  }


  void mult(float _mult)
  {
    for(int i = 0; i < 16; i++)
    {
        mat[i] =  mat[i] * _mult;
    }
  }


float*  getGlMat()
{
    for(int i = 0; i < 4; i++)
    {
            for (int k = 0; k < 4 ; k++) {    
             glmat[k + (i*4)] =   mat[i+(k*4)]; 
            }       
    }    
    return glmat;
}

void setIdentity()
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

Matrix4x4 operator*(Matrix4x4& _mat)
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
  
  void print()
    {
        for(int i = 0; i < 4; i++)
        {
            for (int k = 0; k < 4 ; k++) {
                printf(" %f ", mat[k + (i*4)]);
            }            
            printf("\n");
        }
    }
    
    
  float getDeterminant()
  {
    return 
    mat[0] * getCofactor(mat[5], mat[6], mat[7], mat[9], mat[10], mat[11], mat[13], mat[14], mat[15]) -
    mat[1] * getCofactor(mat[4], mat[6], mat[7], mat[8], mat[10], mat[11], mat[12], mat[14], mat[15]) +
    mat[2] * getCofactor(mat[4], mat[5], mat[7], mat[8], mat[9], mat[11], mat[12], mat[13], mat[15]) -
    mat[3] * getCofactor(mat[4], mat[5], mat[6], mat[8], mat[9], mat[10], mat[12], mat[13], mat[14]);
                
  }
    
  float getCofactor(float mat1, float mat2, float mat3, float mat4, float mat5, float mat6, float mat7, float mat8, float mat9)
  {
         return 
         mat1 * ( mat5 * mat9 - mat6 * mat8 ) - 
         mat2 * ( mat4 * mat9 - mat6 * mat7 ) + 
         mat3 * ( mat4 * mat8 - mat5 * mat7 );                
  }

  void transpose()
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
  }


 Matrix4x4& inverse()
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
        
        mat3[0] = mat[0];    mat3[1] = mat[1];    mat3[2] = mat[2];
        mat3[3] = mat[4];    mat3[4] = mat[5];    mat3[5] = mat[6];
        mat3[6] = mat[8];    mat3[7] = mat[9];    mat3[8] = mat[10];
        
        Matrix3x3 tmp(mat3);
        Matrix3x3 res = tmp.inverse();
        
       mat[0] = res.mat[0]; mat[1] = res.mat[1]; mat[2] = res.mat[2];       
       mat[4] = res.mat[3]; mat[5] = res.mat[4]; mat[6] = res.mat[5];
       mat[8] = res.mat[6]; mat[9] = res.mat[7]; mat[10] = res.mat[8];
                 
       mat[3] =  -((res.mat[0] * mat[3]) +  (res.mat[1] * mat[7]) + (res.mat[2] * mat[11]));
       mat[7] =  -((res.mat[3] * mat[3]) +  (res.mat[4] * mat[7]) + (res.mat[5] * mat[11]));
       mat[11] = -((res.mat[6] * mat[3]) +  (res.mat[7] * mat[7]) + (res.mat[8] * mat[11]));
       
    this->transpose();                 
       return *this;                                                                
    }
    
 }

friend std::ostream& operator<<(std::ostream& os, Matrix4x4& m)
{
    os << m.mat[0] << "\t" <<  m.mat[1] << "\t" <<  m.mat[2] << "\t" <<  m.mat[3] << std::endl
       << m.mat[4] << "\t" <<  m.mat[5] << "\t" <<  m.mat[6] << "\t" <<  m.mat[7]<< std::endl
       << m.mat[8] << "\t" <<  m.mat[9] << "\t" <<  m.mat[10] << "\t" <<  m.mat[11]<< std::endl
       << m.mat[12] << "\t" <<  m.mat[13] << "\t" <<  m.mat[14] << "\t" <<  m.mat[15]<< std::endl; 
    return os;
}

  private:

};



using namespace std;
int main(int argc, char *argv[]) {
  

     float tes_mat[] = {1,0,3,
                       7,3,-1,
                       3,2,5};
                    
     float gl_mat[] = {1,0,0,4,
                       0,1,0,15,
                       0,0,1,6,
                       0,0,0,1};               

     float gl_mat2[] = {1,3,0,0,
                        0,4,0,0,
                        0,0,1,0,
                        0,0,0,1};               
  

double d = 3.14159265358979;
cout.precision(4);
cout << "Pi: " << fixed << d << " " << d << endl;

  //std::cout <<  "test";
 // Matrix3x3 test(tes_mat);
 // Matrix3x3 inv_test = test.inverse();
 //(inv_test * test).print();
//  inv_test.print();
}