
/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <SNX heading END do not edit this line> ******************/
/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/




/* Generated by Together */

#ifndef SOUND_INFO_DATA
#define SOUND_INFO_DATA
#include <string>
#include <vector>
#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>

namespace snx
{

/**
 * info struct that describes one sound entry.
 * typically, you will fill this out and pass it to a SoundHandle object
 * to configure your sound.
 * @ingroup SonixAPI
 */
struct SoundInfo
{
   SoundInfo() : alias(), 
                 datasource( FILESYSTEM ), 
                 filename(),
                 ambient( false ),
                 retriggerable( false ),
                 repeat( 1 ),
                 pitchbend( 1.0f ),
                 cutoff( 1.0f ),
                 volume( 1.0f ),
                 triggerOnNextBind( false ), 
                 repeatCountdown( 0 ),
                 streaming( true )
   {
      //position.makeIdent();
      position[0] = 0.0f;
      position[1] = 0.0f;
      position[2] = 0.0f;
   }
   
         
   /// name of the sound.  this is the "handle", or how you refer to the sound.         
   std::string alias;

   enum DataSource
   {
      FILESYSTEM, DATA_16_MONO, DATA_8_MONO
   };
      
   /// which of the following resources to use...
   DataSource datasource; 

   /** source of the sound...
    * for audioworks, the file should be .wav 11025hz mono
    * for openal, the file should be .wav mono
    */
   std::string filename;

   /// 3D position
   gmtl::Vec3f position;
   
   /// is the sound ambient (true) or positional (false)?
   bool ambient;        
   
   /// can the sound be retriggered while playing?
   bool retriggerable;  
   
   /// number of times to repeat (static), -1 is infinite
   int repeat;          

   /// from 0 to 1.  0 is not a legal value.
   float pitchbend;

   /// from 0 to 1.
   float cutoff;

   /// from 0 to 1.
   float volume;

   /// should we stream the sound from disk?
   bool streaming;

public:
   /// do not use. internal use only
   bool triggerOnNextBind;
   /// do not use. internal use only
   int repeatCountdown; // number of times left to repeat (changes during play)
};

} // end namespace

#endif //SOUND_INFO_DATA
