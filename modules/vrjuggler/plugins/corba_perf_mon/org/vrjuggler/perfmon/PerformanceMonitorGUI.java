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

package org.vrjuggler.perfmon;

import java.util.List;
import java.util.ArrayList;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import org.omg.CORBA.BAD_PARAM;
import org.vrjuggler.tweek.event.*;
import org.vrjuggler.tweek.net.*;
import org.vrjuggler.tweek.net.corba.*;
import org.vrjuggler.jccl.config.*;
import tweek.*;
import vrj.*;


import org.jfree.chart.*;
import org.jfree.chart.axis.*;
import org.jfree.chart.plot.*;
import org.jfree.data.*;
import org.jfree.data.time.*;

import java.io.*;
import java.util.*;
import org.vrjuggler.jccl.config.io.*;

public class PerformanceMonitorGUI extends JPanel
   implements CommunicationListener, TweekFrameListener
{
   private PerformanceMonitorSubject mPerformanceMonitorSubject = null;
   private PerformanceMonitorObserverImpl mPerfMonObserver = null;
   ChartPanel mChartPanel = null;
   BorderLayout borderLayout1 = new BorderLayout();
   double lastValue = 100.0;
   JPanel jPanel1 = new JPanel();

   Integer value = new Integer( 1000 );
   Integer min = new Integer( 50 );
   Integer max = new Integer( 10000 );
   Integer step = new Integer( 50 );
   SpinnerNumberModel mSpinnerModel = new SpinnerNumberModel( value, min, max, step );

   JSpinner mJSpinner = new JSpinner( mSpinnerModel );

   ///The thread that handles updating the GUI.
   private Thread mUpdaterThread = null;

   public PerformanceMonitorGUI()
   {
      TimeSeriesCollection dataset = new TimeSeriesCollection( );
      JFreeChart chart = createChart( dataset );
      mChartPanel = new ChartPanel( chart );

      jbInit();
      mUpdaterThread = new Thread( new Updater( dataset, mSpinnerModel, mPerfMonObserver ) );
      
      System.out.println("PerformanceMonitor started");
   }

   void jbInit ()
   {
       this.setLayout( borderLayout1 );
       mChartPanel.setPreferredSize( new java.awt.Dimension( 500, 270 ) );
       this.add( mChartPanel, BorderLayout.CENTER );
       this.add( jPanel1, BorderLayout.SOUTH );
       jPanel1.add( mJSpinner, null );
  }

   // To change the refresh rate of data
   void getSleepTime ()
   {
      //return mSleepTime;
   }
   
   private JFreeChart createChart ( XYDataset dataset )
   {
      JFreeChart result = ChartFactory.createTimeSeriesChart(
         "Performance Monitoring Statistics",
         "Time",
         "Sample",
         dataset,
         true,
         true,
         false);
    XYPlot plot = result.getXYPlot();
    ValueAxis axis = plot.getDomainAxis();
    axis.setAutoRange( true );
    axis.setFixedAutoRange( 60000.0 ); // 60 seconds
    axis = plot.getRangeAxis();
    axis.setRange( 0.0, 10.0 );
    return result;
   }

   /**
    * Implements the Tweek CommunicationListener interface needed for being
    * informed of connections and disconnections with remote CORBA servers.
    */
   public void connectionOpened(CommunicationEvent e)
   {
      System.out.println("[DBG] PerfMon Connection Opened.");
      // The first thing to do is get the CORBA service object from the
      // event.  We need this so we know to whom we are are connecting.  Once
      // we have the CORBA service, we get its Subject Manager since that's
      // what contains the actual subjects we need.
      CorbaService corba_service = e.getCorbaService();
      SubjectManager mgr         = corba_service.getSubjectManager();

      Subject subject = mgr.getSubject("CorbaPerfPlugin");

      // Try to narrow the Subjet object to a SliderSubject object.  If this
      // fails, it throws a CORBA BAD_PARAM exception.  In that case, we open
      // a dialog box saying that the narrowing failed.
      try
      {
         PerformanceMonitorSubject mPerformanceMonitorSubject =
            PerformanceMonitorSubjectHelper.narrow(subject);
      }
      catch (BAD_PARAM narrow_ex)
      {
         mPerformanceMonitorSubject = null;
      }
      if (mPerformanceMonitorSubject == null)
      {
         JOptionPane.showMessageDialog(null,
                                       "Failed to narrow subject to CorbaPerfPlugin",
                                       "CorbaPerfPlugin Narrow Error",
                                       JOptionPane.ERROR_MESSAGE);
      }
      //if (CommunicationEvent.CONNECT == e.getType())
      // Ensure that slide_subject is a valid object just to be safe.
      if (mPerformanceMonitorSubject != null)
      {
         // First, we need a Java object that implements the Observer.  That
         // object must be registered with the Java CORBA service.
         mPerfMonObserver = new PerformanceMonitorObserverImpl(mPerformanceMonitorSubject);
         //mManipulationPanel.setObserver (mObserver);
         
         corba_service.registerObject(mPerfMonObserver, "PerformanceMonitorObserver");

         // Now that the observer is registered, we can attach it to the
         // subject.  The subject needs to know who its observers are so
         // that it can notify them of updates.
         mPerformanceMonitorSubject.attach(mPerfMonObserver._this());
      
         ///Start the updater thread.
         mUpdaterThread.start();
      }
   }

   /**
    * Catch an event when we lose a CORBA connection that a RTRDDataSource is
    * using.
    */
   public void connectionClosed(CommunicationEvent e)
   {
      //TODO: Find a way to close the RTRCDataSource that has lost its connection.
      disconnect();
   }

   private void disconnect()
   {
      if ( mPerfMonObserver != null )
      {
         mPerfMonObserver.detach();
         mPerfMonObserver = null;
      }
   }
   
   /**
    * Catch a frame close event to cleanly shutdown our CORBA subjects.
    */
   public void frameStateChanged (TweekFrameEvent e)
   {
      if ( e.getType() == TweekFrameEvent.FRAME_CLOSE )
      {
         disconnect();
      }
   }

}
