/********************************************************************************
    LibVideoGfx - video processing library
    Copyright (C) 2002  Dirk Farin

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ********************************************************************************/

#include "libvideogfx/graphics/geometry/matrix.hh"
#include <math.h>
#include <iomanip>
#include <assert.h>

namespace videogfx {
  using namespace std;

  Matrix4G::Matrix4G()
  {
    d_rows=d_columns=0;
  }

  Matrix4G::Matrix4G(const Matrix4G& m)
  {
    d_rows=m.d_rows;
    d_columns=m.d_columns;
    for (int r=0;r<d_rows;r++)
      for (int c=0;c<d_columns;c++)
	d_mat[r][c] = m[r][c];
  }

  Matrix4G::Matrix4G(int rows,int columns)
  {
    d_rows=rows;
    d_columns=columns;

    for (int r=0;r<d_rows;r++)
      for (int c=0;c<d_columns;c++)
	d_mat[r][c] = ((r==c) ? 1 : 0);
  }

  Matrix4G   Matrix4G::operator+(const Matrix4G& m) const
  {
    assert(d_rows    == m.d_rows); //    "matrix dimensions differ (rows)");
    assert(d_columns == m.d_columns); // "matrix dimensions differ (columns)");

    Matrix4G mat(d_rows,d_columns);

    for (int r=0;r<d_rows;r++)
      for (int c=0;c<d_columns;c++)
	mat.d_mat[r][c] = d_mat[r][c] + m.d_mat[r][c];

    return mat;
  }

  Matrix4G   Matrix4G::operator-(const Matrix4G& m) const
  {
    assert(d_rows    == m.d_rows); //    "matrix dimensions differ (rows)");
    assert(d_columns == m.d_columns); // "matrix dimensions differ (columns)");

    Matrix4G mat(d_rows,d_columns);

    for (int r=0;r<d_rows;r++)
      for (int c=0;c<d_columns;c++)
	mat.d_mat[r][c] = d_mat[r][c] - m.d_mat[r][c];

    return mat;
  }

  Matrix4G   Matrix4G::operator*(const Matrix4G& b) const
  {
    const Matrix4G& a = *this;

    assert(a.d_columns == b.d_rows); // "matrix sizes are incompatible for multiplication");

    Matrix4G mat(a.d_rows,b.d_columns);

    for (int i=0;i<a.d_rows;i++)
      for (int j=0;j<b.d_columns;j++)
	{
	  double sum=0.0;
	  for (int k=0;k<a.d_columns;k++)
	    {
	      sum += a.d_mat[i][k] * b.d_mat[k][j];
	    }

	  mat.d_mat[i][j] = sum;
	}

    return mat;
  }

  Matrix4G Matrix4G::operator*(double factor) const
  {
    Matrix4G mat(d_rows,d_columns);

    for (int i=0;i<d_rows;i++)
      for (int j=0;j<d_columns;j++)
	{
	  mat.d_mat[i][j] = d_mat[i][j] * factor;
	}

    return mat;
  }

  Matrix4G Matrix4G::operator/(double factor) const
  {
    //AssertDescr(factor != 0.0, "Cannot divide matrix through zero.");

    Matrix4G mat(d_rows,d_columns);

    for (int i=0;i<d_rows;i++)
      for (int j=0;j<d_columns;j++)
	{
	  mat.d_mat[i][j] = d_mat[i][j] / factor;
	}

    return mat;
  }

  const Matrix4G& Matrix4G::operator=(const Matrix4G& m)
  {
    d_rows    = m.d_rows;
    d_columns = m.d_columns;

    assert(d_rows<=4);
    assert(d_columns<=4);

    for (int i=0;i<d_rows;i++)
      for (int j=0;j<d_columns;j++)
	d_mat[i][j] = m.d_mat[i][j];

    return *this;
  }

  Matrix4G Matrix4G::Transpose() const
  {
    Matrix4G mat(d_columns, d_rows);

    for (int i=0;i<d_rows;i++)
      for (int j=0;j<d_columns;j++)
	mat.d_mat[j][i] = d_mat[i][j];

    return mat;
  }

  double   Matrix4G::Det() const
  {
    if (d_rows==3 && d_columns==3)
      {
	return (d_mat[0][0]*(d_mat[1][1]*d_mat[2][2] - d_mat[1][2]*d_mat[2][1]) +
		d_mat[0][1]*(d_mat[1][2]*d_mat[2][0] - d_mat[2][2]*d_mat[1][0]) +
		d_mat[0][2]*(d_mat[1][0]*d_mat[2][1] - d_mat[2][0]*d_mat[1][1]));
      }
    else if (d_rows==2 && d_columns==2)
      {
	return (d_mat[0][0]*d_mat[1][1]) - (d_mat[0][1]*d_mat[1][0]);
      }
    else
      {
	assert(0); // not implemented
      }
  }

  double   Matrix4G::Trace() const
  {
    assert(d_columns == d_rows); // "matrix sizes are incompatible for multiplication");

    double sum=0.0;
    for (int i=0; i<d_rows; i++)
      sum += d_mat[i][i];

    return sum;
  }

  /* Helper function for 4x4 matrix inverse. */
  inline void InverseInplace2x2(double m[4])
  {
    double det = 1.0/(m[0]*m[3]-m[1]*m[2]);
    m[1] = -m[1];
    m[2] = -m[2];
    swap(m[0],m[3]);

    for (int i=0;i<4;i++) m[i] *= det;
  }

  /* Helper function for 4x4 matrix inverse. */
  /* c = a*b */
  inline void Mult2x2(double c[4],double a[4],double b[4])
  {
    c[0] = a[0]*b[0] + a[1]*b[2];
    c[1] = a[0]*b[1] + a[1]*b[3];
    c[2] = a[2]*b[0] + a[3]*b[2];
    c[3] = a[2]*b[1] + a[3]*b[3];
  }

  Matrix4G Matrix4G::Inverse() const
  {
    assert(d_rows == d_columns); // "matrix must be square for inverse computation");

    Matrix4G inv(d_rows,d_rows);

    if (d_rows==1)
      {
	inv[0][0] = 1.0/d_mat[0][0];

	return inv;
      }
    else if (d_rows==2)
      {
	double det = d_mat[0][0]*d_mat[1][1] - d_mat[0][1]*d_mat[1][0];
	assert(det != 0.0);

	inv[0][0] =  d_mat[1][1]/det;
	inv[0][1] = -d_mat[0][1]/det;
	inv[1][0] = -d_mat[1][0]/det;
	inv[1][1] =  d_mat[0][0]/det;

	return inv;
 