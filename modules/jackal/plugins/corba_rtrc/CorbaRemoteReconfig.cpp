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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <jccl/Plugins/PluginConfig.h>

#include <stdlib.h>
#include <boost/concept_check.hpp>
#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>

#include <jccl/Util/Debug.h>
#include <jccl/RTRC/ConfigManager.h>
#include <corba_rtrc/RemoteReconfigSubjectImpl.h>
#include <corba_rtrc/CorbaRemoteReconfig.h>


extern "C"
{

JCCL_PLUGIN_EXPORT(jccl::RemoteReconfig*) initPlugin(jccl::ConfigManager* configMgr)
{
   return new jccl::CorbaRemoteReconfig(configMgr);
}

}

namespace jccl
{

CorbaRemoteReconfig::CorbaRemoteReconfig(jccl::ConfigManager* configMgr)
   : mConfigManager(configMgr)
   , mCorbaManager(NULL)
   , mInterface(NULL)
   , mEnabled(false)
   , mInterfaceName("CorbaRemoteReconfig")
{
   mConfigManager->addConfigElementHandler(this);
}

CorbaRemoteReconfig::~CorbaRemoteReconfig()
{
   mConfigManager->removeConfigElementHandler(this);
   mConfigManager = NULL;
   stopCorba();
}

bool CorbaRemoteReconfig::configCanHandle(jccl::ConfigElementPtr element)
{
   return element->getID() == std::string("corba_remote_reconfig");
}

bool CorbaRemoteReconfig::configAdd(jccl::ConfigElementPtr element)
{
   // If the ORB is already running, we need to shut it down first.  One big
   // reason for doing this is to release the resources (memory and so on)
   // allocated previously.
   if ( mInterface != NULL || mCorbaManager != NULL )
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_STATE_LVL)
         << "[CorbaRemoteReconfig::configAdd()] Attempting to shut down "
         << "existing CORBA instance.\n" << vprDEBUG_FLUSH;

      // stopCorba() will call disable() if we are still in the enabled state.
      stopCorba();
   }

   const std::string ns_host =
      element->getProperty<std::string>("naming_service_host");
   const vpr::Uint16 ns_port =
      element->getProperty<vpr::Uint16>("naming_service_port");
   const std::string iiop_version =
      element->getProperty<std::string>("iiop_version");

   // We'll ignore the return value for now.  startCorba() prints out enough
   // warning information on its own if something goes wrong.
   this->startCorba(ns_host, ns_port, iiop_version);

   return true;
}

bool CorbaRemoteReconfig::configRemove(jccl::ConfigElementPtr element)
{
   boost::ignore_unused_variable_warning(element);

   if ( isEnabled() )
   {
      disable();
   }

   return true;
}

vpr::ReturnStatus CorbaRemoteReconfig::init()
{
   return vpr::ReturnStatus();
}

vpr::ReturnStatus CorbaRemoteReconfig::enable()
{
   return vpr::ReturnStatus();
}

bool CorbaRemoteReconfig::isEnabled() const
{
   return mEnabled;
}

void CorbaRemoteReconfig::disable()
{
   //Make sure the corba manager has been initialized
   if ( mInterface == NULL || mCorbaManager == NULL || ! mEnabled )
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING:")
         << " [CorbaRemoteReconfig::disable()] Cannot disable interface "
         << "until it has been initialized!\n" << vprDEBUG_FLUSH;

      return;
   }

   try
   {
      // Unregister the subject (note that observers must handle this
      // disconnection).
      vpr::ReturnStatus status;

      // Attempt to un-register the subject.
      status = mCorbaManager->getSubjectManager()->unregisterSubject(mInterfaceName.c_str());

      if ( ! status.success() )
      {
         vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " [CorbaRemoteReconfig::disable()] Could not unregister subject."
            << std::endl << vprDEBUG_FLUSH;
      }
   }
   catch (...)
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING:")
         << " [CorbaRemoteReconfig::disable()] Caught an exception while "
         << "trying to unregister subject.\n" << vprDEBUG_FLUSH;
   }

   mCorbaManager->shutdown();
   mEnabled = false;
}

vpr::ReturnStatus CorbaRemoteReconfig::startCorba(const std::string& nsHost,
                                                  const vpr::Uint16 nsPort,
                                                  const std::string& iiopVer)
{
   // Create new CORBA Manager and initialize it.
   mCorbaManager = new tweek::CorbaManager;
   std::string err_msg("Error occured during initialization");
   vpr::ReturnStatus status;

   try
   {
      int dummy_int(0);

      // Attempt to initialize the CORBA Manager.
      status = mCorbaManager->init("corba_rtrc", dummy_int, NULL, nsHost,
                                   nsPort, iiopVer);

      // Test to see if init succeeded.
      if ( status.success() )
      {
         try
         {
            // Attempt to create the Subject Manager.
            status = mCorbaManager->createSubjectManager();

            if ( status.success() )
            {
               // Customize the Subject Manager information a little to help
               // users.
               mCorbaManager->getSubjectManager()->addInfoItem("Usage",
                                                               "Config Manager");

               // Create an intstance of our subject.
               mInterface = new RemoteReconfigSubjectImpl;

               // Register the subject with the CORBA Manager.
               try
               {
                  // Attempt to register the subject.
                  mCorbaManager->getSubjectManager()->registerSubject(mInterface, mInterfaceName.c_str());
                  mEnabled = true;
               }
               catch(...)
               {
                  vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
                     << clrOutBOLD(clrRED, "ERROR:")
                     << " [CorbaRemoteReconfig::startCorba()] Caught an "
                     << "exception while trying to register subject.\n"
                     << vprDEBUG_FLUSH;
                  vprDEBUG_NEXT(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
                     << "Disabling CORBA remote run-time reconfiguration.\n"
                     << vprDEBUG_FLUSH;
                  status.setCode(vpr::ReturnStatus::Fail);
               }
            }
            else
            {
               err_msg = "Tweek Subject Manager failed to initialize.";
               throw std::exception();
            }
         }
         catch(CORBA::Exception& ex)
         {
            boost::ignore_unused_variable_warning(ex);
            err_msg = "Caught a CORBA exception while creating the Subject Manager";
            throw;  // Rethrow the exception, which will be caught below
         }
      }
      else
      {
         err_msg = "Could not initialize tweek::CorbaManager.";
         throw std::exception();
      }
   }
   catch(...)
   {
      vprDEBUG(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrRED, "ERROR:")
         << " [CorbaRemoteReconfig::startCorba()] " << err_msg << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(jcclDBG_PLUGIN, vprDBG_WARNING_LVL)
         << "Disabling CORBA remote run-time reconfiguration.\n"
         << vprDEBUG_FLUSH;

      delete mCorbaManager;
      mCorbaManager = NULL;
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

void CorbaRemoteReconfig::stopCorba()
{
   //Clean up interface by disconnecting first
   if (mInterface != NULL)
   {
      if ( isEnabled() )
      {
         disable();
      }

      delete mInterface;
      mInterface = NULL;
   }

   // Clean up the corba manager and delete it.
   if (mCorbaManager != NULL)
   {
      // CORBA Manager has its own destructor cleanup.
      delete mCorbaManager;
      mCorbaManager = NULL;
   }
}

} // End of jccl namespace