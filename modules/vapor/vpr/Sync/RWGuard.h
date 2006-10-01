/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_RWGuard_h_
#define _VPR_RWGuard_h_

#include <vpr/vprConfig.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/Sync/RWMutex.h>


namespace vpr
{

/** \class ReadGuard RWGuard.h vpr/Sync/RWGuard.h
 *
 * Read Guard wrapper.
 */
class ReadGuard
{
public:
   /**
    * Acquires the lock implicitly.
    * If \p block is 1, then use a blocking acquire.
    */
   ReadGuard(RWMutex& lock, int block = 1) : mTheLock(&lock)
   {
      mLockStatus = block ? acquire().success() : tryAcquire().success();
   }

   /** Releases the lock. */
   ~ReadGuard()
   {
      if ( mLockStatus )
      {
         mTheLock->release();
      }
   }

   /**
    * @return \c true is returned if this guard is locked.
    * @return \c false is returned if this guard is not locked.
    */
   const bool& locked()
   {
      return mLockStatus;
   }

   /** Acquires the lock. */
   vpr::ReturnStatus acquire()
   {
      vpr::ReturnStatus status = mTheLock->acquireRead();
      mLockStatus = status.success();
      return status;
   }

   /** Tries to acquire the lock. */
   vpr::ReturnStatus tryAcquire()
   {
      vpr::ReturnStatus status = mTheLock->tryAcquireRead();
      mLockStatus = status.success();
      return status;
   }

   /** Explicity releases the lock. */
   vpr::ReturnStatus release()
   {
      mLockStatus = false;
      return mTheLock->release();
   }

private:
   RWMutex* mTheLock;    /**< The lock that we are using */
   bool     mLockStatus; /**< Are we locked or not */
};

/** \class WriteGuard RWGuard.h vpr/Sync/RWGuard.h
 *
 * Write Guard wrapper.
 */
class WriteGuard
{
public:
   /**
    * Acquires the lock implicitly.
    * If \p block is 1, then use a blocking acquire.
    */
   WriteGuard(RWMutex& lock, int block = 1) : mTheLock(&lock)
   {
      mLockStatus = block ? acquire().success() : tryAcquire().success();
   }

   /** Releases the lock. */
   ~WriteGuard()
   {
      if ( mLockStatus )
      {
         mTheLock->release();
      }
   }

   /**
    * @return \c true is returned if this guard is locked.
    * @return \c false is returned if this guard is not locked.
    */
   const bool& locked()
   {
      return mLockStatus;
   }

   /** Acquires the lock. */
   vpr::ReturnStatus acquire()
   {
      vpr::ReturnStatus status = mTheLock->acquireWrite();
      mLockStatus = status.success();
      return status;
   }

   /** Tries to acquire lock. */
   vpr::ReturnStatus tryAcquire()
   {
      vpr::ReturnStatus status = mTheLock->tryAcquireWrite();
      mLockStatus = status.success();
      return status;
   }

   /** Explicity releases the lock. */
   vpr::ReturnStatus release()
   {
      mLockStatus = false;
      return mTheLock->release();
   }

private:
   RWMutex* mTheLock;    /**< The lock that we are using */
   bool     mLockStatus; /**< Are we locked or not */
};

} // End of vpr namespace


#endif
