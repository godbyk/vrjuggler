/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _VJ_STD_CAVE_NAVIGATOR_H_
#define _VJ_STD_CAVE_NAVIGATOR_H_


//#include <Kernel/vjKernel.h>
#include <Math/vjCoord.h>
#include <Kernel/vjDebug.h>

#include <velocityNav.h>

//: This is a "cave style" navigation matrix
//  Provided that you call update once per frame, it should 
//  provide back to you a matrix that can be used to navigate a scene.
//  The matrix is in the OpenGL form, 
//  For performer, convert this matrix, 
//  and copy it to a DCS node once per frame as well.
class vjStdCaveNavigator : public vjMatrix
{
public:
   vjStdCaveNavigator();

   velocityNav* getNavigator()
   { return &mVNav; }
   
   //: call once per frame (pre or post frame, not intra frame)
   virtual int update();
   
   //: tell the navigator to rotate
   //  Rotation is based on the current setMatrix call
   virtual void rotate( const bool& state = true )
   {
      mNowRotating = state;
   }   
   
   //: tell the navigator to accelerate
   //  Acceleration is based on the current setMatrix call
   virtual void accelerate( const bool& state = true )
   {
      mNowTranslating = state;
   }
   
   //: tell the navigator to stop
   virtual void stop()
   {
      //TODO
   }
   
   //: tell the navigator to reverse acceleration
   virtual void reverse()
   {
      //TODO
   }
   
   //: tell the navigator to brake
   virtual void brake()
   {
      //TODO
   }
   
   //: tell the navigator to reset its matrix to origin
   virtual void reset()
   {
      //TODO
   }
   
   //: tell the navigator the matrix that reset() uses as it's origin.
   virtual void setOrigin( const vjMatrix& matrix )
   {
      //TODO
   }
   
   //: tell the navigator what the pointing device's matrix is.
   //  you can usually do this to get that matrix
   //  vjMatrix* wand_mat = mWand->getData();
   virtual void setMatrix( const vjMatrix& matrix )
   {
      mDeviceMatrix = matrix;
   }   
   
private:
   velocityNav          mVNav;      // My navigator
   //vjKernel* mKern;        // The Kernel

protected:
   vjMatrix mDeviceMatrix;
   bool     mNowTranslating;
   bool     mNowRotating;
};


#endif
