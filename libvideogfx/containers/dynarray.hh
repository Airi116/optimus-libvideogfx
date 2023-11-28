/********************************************************************************
  libvideogfx/containers/dynarray.hh

  purpose:
    Very simple dynamic array class.

  notes:

  to do:

  author(s):
   - Dirk Farin, dirk.farin@gmx.de

  modifications:
   29/Aug/2001 - Dirk Farin
   - first implementation
 ********************************************************************************
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

#ifndef LIBVIDEOGFX_CONTAINERS_DYNARRAY_HH
#define LIBVIDEOGFX_CONTAINERS_DYNARRAY_HH

#include "libvideogfx/types.hh"
#include <assert.h>

namespace videogfx {

  template <class T> class DynArray
  {
  public:
    DynArray();
    DynArray(int initial_size);
    DynArray(const DynArray<T>&);
    ~DynArray();

    void Clear();
    void Append(const T& t);

    /* Append a new, empty element and return a reference to it. */
    T&   AppendNewEntry() { T t; Append(t); return d_array[d_nentries-1]; }

    /* Insert the element at the specified position. */
    void Insert(int pos,const T& t);
    T    ReturnAndRemoveEntry(int n);
    void RemoveEntry(int n);
    int  AskSize() const { return d_nentries; }

    void SetEmptyValue(const T& e) { d_empty_value=e; d_empty_val_set=true; }

    const T& operator[](int n) const { return d_array[n]; }
    T& operator[](int n)
    {
      EnlargeToSize(n+1);
      if (n>=d_nentries)
	d_nentries = n+1;
      return d_array[n];
    }

    const DynArray<T> operator=(const DynArray<T>&);

  private:
    T    d_empty_value;
    bool d_empty_val_set;

    T*   d_array;
    int  d_nentries;
    int  d_size;

    void Enla