/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _CLUSTER_SERIALIZABLE_DATA_H
#define _CLUSTER_SERIALIZABLE_DATA_H

#include <gadget/gadgetConfig.h>
#include <vpr/IO/SerializableObject.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/Util/AttributeMapBase.h>

namespace cluster
{

/** \class SerializableData SerializableData.h cluster/SerializableData.h
 *
 * Base type for serializable data.
 */
class SerializableData : public vpr::AttributeMapBase
{
public:
   SerializableData()
   {
      ;
   }
   virtual ~SerializableData()
   {
      ;
   }

   virtual vpr::ReturnStatus writeObject(vpr::BufferObjectWriter* writer) = 0;
   virtual vpr::ReturnStatus readObject(vpr::BufferObjectReader* reader) = 0;
};

} // end namespace gadget

#endif