#include <tweek/tweekConfig.h>

#include <string>
#include <vpr/vpr.h>
#include <vpr/System.h>

#include <tweek/Util/Debug.h>
#include <tweek/CORBA/CorbaHelpers.h>

namespace tweek
{

CosNaming::NamingContext_var getRootNamingContext(CORBA::ORB_ptr orb,
                                                  const std::string& refName)
{
#ifdef OMNIORB_VER
   // If the user does not have the OMNIORB_CONFIG environment variable set,
   // there will most likely be problems finding and/or contacting the
   // Naming Service.  To that end, print a warning saying as much when the
   // variable is not set.
   std::string temp;
   if ( vpr::System::getenv("OMNIORB_CONFIG", temp).failure() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING: OMNIORB_CONFIG not set!")
         << "  Expect problems contacting the Naming Service\n"
         << vprDEBUG_FLUSH;
   }
#endif

   CORBA::Object_var name_obj;
   CosNaming::NamingContext_var root_context;

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Requesting Name Service\n"
                                              << vprDEBUG_FLUSH;
   name_obj     = orb->resolve_initial_references(refName.c_str());
   root_context = CosNaming::NamingContext::_narrow(name_obj);

   if ( CORBA::is_nil(root_context) )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Failed to narrow Naming Service root context\n"
         << vprDEBUG_FLUSH;
   }

   return root_context;
}

CosNaming::NamingContext_var bindLocalNamingContext(CosNaming::NamingContext_ptr parentContext,
                                                    const std::string& localId)
{
   vprASSERT(! CORBA::is_nil(parentContext) && "Root context is invalid");

   std::string kind("context");
   CosNaming::Name tweek_context_name;

   tweek_context_name.length(1);
   tweek_context_name[0].id   = CORBA::string_dup(localId.c_str());
   tweek_context_name[0].kind = CORBA::string_dup(kind.c_str());

   CosNaming::NamingContext_var local_context;

   try
   {
      local_context = parentContext->bind_new_context(tweek_context_name);
   }
   catch (CosNaming::NamingContext::AlreadyBound& ex)
   {
      CORBA::Object_var temp_obj;

      temp_obj      = parentContext->resolve(tweek_context_name);
      local_context = CosNaming::NamingContext::_narrow(temp_obj);

      if ( CORBA::is_nil(local_context) )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "Failed to narrow Naming Service to local Tweek context\n"
            << vprDEBUG_FLUSH;
      }
   }

   return local_context;
}

} // End of tweek namespace
