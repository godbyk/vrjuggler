/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


/////////////////////////////////////////////////////////////////////////////
// vjKeyboardWin32.h
//
// Keyboard input device for win32
//
// History:
//
// Andy Himberger:    v0.0 - 12-1-97 - Inital version
// Allen Bierbaum:    v1.0 -  7-23-99 - Refactored to use new keyboard method
////////////////////////////////////////////////////////////////////////////
#ifndef _vj_KEYBOARD_H_
#define _vj_KEYBOARD_H_

#include <vjConfig.h>


// Get windows stuff
#include <windows.h>
#include <commctrl.h>

#include <Input/vjInput/vjKeyboard.h>
#include <Threads\vjThread.h>


class vjKeyboardWin32 : public vjKeyboard
{
public:
	
   vjKeyboardWin32()
   {
      for (int i =0; i < 256; i++)
         m_realkeys[i] = m_keys[i] = m_framekeys[i] = 0;
      
      m_realkeys[0] = m_keys[0] = m_framekeys[0] = 1;      
   }
   ~vjKeyboardWin32() { stopSampling();}


   virtual bool config(vjConfigChunk* c);

   /* Pure Virtuals required by vjInput */
   int startSampling();
   int stopSampling();
   int sample() { return 1;}
   void updateData();

   /* vjInput virtual functions
    *
    *  virtual functions that inherited members should
    *  override but are not required to
    */
   char* getDeviceName() { return "vjKeyboardWin32";}
   static std::string getChunkType() { return std::string("Keyboard");}


   // returns the number of times the key was pressed during the
   // last frame, so you can put this in an if to check if was
   // pressed at all, or if you are doing processing based on this
   // catch the actual number..
   int isKeyPressed(int vjKey)
   {  return m_keys[vjKey];}

   virtual int keyPressed(int keyId)
   { return isKeyPressed(keyId); }

   virtual bool modifierOnly(int modKey)
   { return onlyModifier(modKey); }

   void createWindowWin32 ();
	void updKeys(	UINT message,	UINT wParam, LONG lParam);

public:
   HINSTANCE	m_hInst;
	HWND		   m_hWnd;

private:
   /** @name Private functions for processing input data */
   /* Private functions for processing input data */
 	  //@{
   int onlyModifier(int);


   /** @name Windows utility functions */
   //@{
   int VKKeyTovjKey(int vkKey);
   char* checkArgs(char* look_for);

	BOOL MenuInit (HINSTANCE hInstance);
   //@}


   int		m_screen, m_x, m_y;
   unsigned int m_width,m_height;
   
   // Use m_keys to know how many times each key was KEYDOWN during the loop
   // m_realkeys is the current up/down state of each key
   // m_framekeys is a buffer of the keys that have been pressed
   // since the last updateData.  It creates a non-thread accurate buffer
   int m_keys[256];        //: Last frames state, set by the updateData
   int m_framekeys[256];   //: The keyboard state during a frame of update, without KeyUp events included
   int m_realkeys[256];    //: The real keyboard state, all events processed

   float m_mouse_sensitivity;
   
	int oldx,oldy,newx,newy;  // Mousemove variables
};

#endif
