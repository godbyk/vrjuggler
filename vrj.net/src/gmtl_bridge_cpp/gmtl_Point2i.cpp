/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

// Generated from Revision: 1.60 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Point.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Point2i* gmtl_Point_int_2__Point__()
   {
      gmtl::Point2i* obj = new gmtl::Point2i();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2i* gmtl_Point_int_2__Point__gmtl_Point2i(const gmtl::Point2i* p0)
   {
      const gmtl::Point2i& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Point2i* obj = new gmtl::Point2i(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2i* gmtl_Point_int_2__Point__gmtl_VecBase_int_2(const gmtl::VecBase<int,2>* p0)
   {
      const gmtl::VecBase<int,2>& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Point2i* obj = new gmtl::Point2i(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2i* gmtl_Point_int_2__Point__int_int(const int& p0, const int& p1)
   {
      gmtl::Point2i* obj = new gmtl::Point2i(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2i* gmtl_Point_int_2__Point__int_int_int(const int& p0, const int& p1, const int& p2)
   {
      gmtl::Point2i* obj = new gmtl::Point2i(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2i* gmtl_Point_int_2__Point__int_int_int_int(const int& p0, const int& p1, const int& p2, const int& p3)
   {
      gmtl::Point2i* obj = new gmtl::Point2i(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Point2i(gmtl::Point2i* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Point2i


