/* Generated by Together */

#ifndef AJSTUBSOUNDIMPLEMENTATION_H
#define AJSTUBSOUNDIMPLEMENTATION_H
#include <string>
#include "aj/ajSoundImplementation.h"
#include "aj/ajSoundInfo.h"
#include "aj/ajSoundAPIInfo.h"
class ajStubSoundImplementation : public ajSoundImplementation
{
public:
   /**
    * constructor for the OpenAL implementation 
    */
   ajStubSoundImplementation() : ajSoundImplementation() {}

   /**
    * destructor for the OpenAL implementation
    */
   virtual ~ajStubSoundImplementation()
   {
   }

   /**
     * every implementation can return a new copy of itself
     */
   virtual void clone( ajSoundImplementation* &newCopy )
   {
      newCopy = new ajStubSoundImplementation;
      
      // copy state, so that we return a true "clone"
      newCopy->copy( *this );
   }
   
   /**
    * @input alias of the sound to trigger, and number of times to play
    * @preconditions alias does not have to be associated with a loaded sound.
    * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
    * @semantics Triggers a sound
    */
   virtual void trigger(const std::string & alias, const unsigned int & looping = 0)
   {
      ajSoundImplementation::trigger( alias, looping );
      // do nothing
   }

   /**
    * @semantics stop the sound
    * @input alias of the sound to be stopped
    */
   virtual void stop(const std::string & name)
   {
      ajSoundImplementation::stop( name );
      // do nothing
   }

   /**
    * set sound's 3D position 
    */
   virtual void setPosition( const std::string& alias, float x, float y, float z )
   {
      ajSoundImplementation::setPosition( alias, x, y, z );
   }

   /**
    * get sound's 3D position
    * @input alias is a name that has been associate()d with some sound data
    * @output x,y,z are returned in OpenGL coordinates.
    */
   virtual void getPosition( const std::string& alias, float& x, float& y, float& z )
   {
      ajSoundImplementation::getPosition( alias, x, y, z );
   }
   
   /**
    * set the position of the listener
    */
   virtual void setListenerPosition( const ajMatrix44& mat )
   {
      ajSoundImplementation::setListenerPosition( mat );
   }

   /**
    * get the position of the listener
    */
   virtual void getListenerPosition( ajMatrix44& mat )
   {
      ajSoundImplementation::getListenerPosition( mat );
   }
   
   /**
    * start the sound API, creating any contexts or other configurations at startup
    * @postconditions sound API is ready to go.
    * @semantics this function should be called before using the other functions in the class.
    */
   virtual void startAPI()
   {
   }
   
   /**
    * kill the sound API, deallocating any sounds, etc...
    * @postconditions sound API is ready to go.
    * @semantics this function could be called any time, the function could be called multiple times, so it should be smart.
    */
   virtual void shutdownAPI()
   {
   }   

   /**
     * query whether the API has been started or not
     * @semantics return true if api has been started, false otherwise.
     */
   virtual bool isStarted() const
   {
      return true;
   }

   /**
     * configure/reconfigure a sound
     * configure: associate a name (alias) to the description if not already done
     * reconfigure: change properties of the sound to the descriptino provided.
     * @preconditions provide an alias and a SoundInfo which describes the sound
     * @postconditions alias will point to loaded sound data
     * @semantics associate an alias to sound data.  later this alias can be used to operate on this sound data.
     */
   virtual void configure( const std::string& alias, const ajSoundInfo& description )
   {
      ajSoundImplementation::configure( alias, description );
      // do nothing
   }

   /**
     * remove a configured sound, any future reference to the alias will not
     * cause an error, but will not result in a rendered sound
     */
   virtual void remove( const std::string alias )
   {
      ajSoundImplementation::remove( alias );
      // do nothing
   }

   /**
    * clear all associate()tions.
    * @semantics any existing aliases will be stubbed. aounds will be unbind()ed
    */
   virtual void clear()
   {
   }   
   
   /**
    * bind: load (or reload) all associate()d sounds
    * @postconditions all sound associations are buffered by the sound API
    */
   virtual void bindAll()
   {
   }   

   /**
    * unbind: unload/deallocate all associate()d sounds.
    * @postconditions all sound associations are unbuffered by the sound API
    */
   virtual void unbindAll()
   {
   }

   /**
    * load/allocate the sound data this alias refers to the sound API
    * @postconditions the sound API has the sound buffered.
    */
   virtual void bind( const std::string& alias )
   {
   }   

   /**
    * unload/deallocate the sound data this alias refers from the sound API
    * @postconditions the sound API no longer has the sound buffered.
    */
   virtual void unbind( const std::string& alias )
   {
   }

   /**
    * take a time step of [timeElapsed] seconds.
    * @semantics call once per sound frame (doesn't have to be same as your graphics frame)
    * @input time elapsed since last frame
    */
   virtual void step( const float & timeElapsed )
   {
      ajSoundImplementation::step( timeElapsed );
      // do nothing
   }

private:  

   /** @link dependency */
   /*#  ajSoundInfo lnkSoundInfo; */

   /** @link dependency */
   /*#  ajSoundAPIInfo lnkajSoundAPIInfo; */
};
#endif //AJSTUBSOUNDIMPLEMENTATION_H
