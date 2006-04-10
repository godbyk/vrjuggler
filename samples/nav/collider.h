/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001, 2002
 *   by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
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

#include <Math/vjVec3.h>

#ifndef _COLLIDER_H_
#define _COLLIDER_H_

//: Base class for collision detection
//
// All it does is test movements to see if they are valid.
//  Any motion (nav) model can make use of a collider to
// check for collisions and to modify the motion
//
class collider
{
public:

   //: test a movement
   //!ARGS: curPos - The current position we are at
   //!ARGS: delta - The amount we want to move
   //!ARGS: correction - The amount to correct the movement so that we do not collide
   //!ARGS: curPosWithDelta - Does the curPos already have delta added in?
   //!RETURNS: true - There was a hit.
   virtual bool testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction, bool curPosWithDelta = false) = 0;

   // Did it collide in the last frame
   // API specific functions could return more info about the collision
   bool didCollide()
   { return mDidCollide; }

protected:
   void setDidCollide(bool val)
   { mDidCollide = val; }

   bool mDidCollide;
};

#endif