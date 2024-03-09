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

  Matrix4G   Matrix4G::operator+(const Matrix4G& m) c