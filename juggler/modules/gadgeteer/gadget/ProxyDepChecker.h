/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#ifndef _GADGET_PROXY_DEP_CHECKER_H_
#define _GADGET_PROXY_DEP_CHECKER_H_

#include <gadget/gadgetConfig.h>

// Dependency checker includes
#include <jccl/RTRC/DependencyManager.h>
#include <jccl/RTRC/DepChecker.h>


namespace gadget
{

/** Dependency checker for Proxies. */
class ProxyDepChecker : public jccl::DepChecker
{
public:
   ProxyDepChecker()
   {;}

   /**
    * Returns a string name of the checker.
    * This is used to output messages in checker listings.
    */
   virtual std::string getCheckerName()
   { return std::string("gadget::ProxyChecker Checker"); }

   // We can handle only keyboard configuration information.
   virtual bool canHandle(jccl::ConfigChunkPtr chunk);

   /**
    * Are the dependencies satisfied?
    * Defaults to all being handled for it.
    */
   virtual bool depSatisfied(jccl::ConfigChunkPtr chunk)
   {
      return true;
   }

   /** Writes out the dependencies to the vprDEBUG macro. */
   virtual void debugOutDependencies (jccl::ConfigChunkPtr chunk,
                                      int dbg_lvl = vprDBG_WARNING_LVL)
   {
      jccl::DepChecker::debugOutDependencies(chunk,dbg_lvl);
      vprDEBUG_NEXT_BEGIN(vprDBG_ALL,dbg_lvl)
         << "Proxies should never have dependencies!!!" << vprDEBUG_FLUSH;
   }
};

};

#endif
