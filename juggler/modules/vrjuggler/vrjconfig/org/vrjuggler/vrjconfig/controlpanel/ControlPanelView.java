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

package org.vrjuggler.vrjconfig.controlpanel;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Frame;
import java.awt.event.*;
import java.beans.PropertyEditorManager;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;

import org.jdom.input.*;
import org.jdom.Document;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;
import org.vrjuggler.jccl.editors.net.TinyBrowser;

import org.vrjuggler.tweek.TweekCore;
import org.vrjuggler.tweek.beans.BeanPathException;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.FileLoader;
import org.vrjuggler.tweek.beans.HelpProvider;
import org.vrjuggler.tweek.beans.UndoHandler;
import org.vrjuggler.tweek.beans.XMLBeanFinder;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.tweek.text.MessageDocument;
import org.vrjuggler.tweek.wizard.*;

import org.vrjuggler.vrjconfig.VrjConfigConstants;
import org.vrjuggler.vrjconfig.ui.*;


/**
 * Provides a control panel view into a config element collection.
 */
public class ControlPanelView
   extends JPanel
   implements HelpProvider
            , FileLoader
            , UndoHandler
{
   public ControlPanelView()
   {
      mToolbar = new ControlPanelToolbar(mContext, this, this);

      // Make sure all editors are registered.
      PropertyEditorManager.registerEditor(Boolean.class, BooleanEditor.class);
      PropertyEditorManager.registerEditor(String.class, StringEditor.class);
      PropertyEditorManager.registerEditor(Integer.class, IntegerEditor.class);
      PropertyEditorManager.registerEditor(Float.class, FloatEditor.class);

      EnvironmentServiceProxy env_svc = new EnvironmentServiceProxy();

      // This needs to be the first step to ensure that all the basic services
      // and viewers get loaded.
      String default_path = env_svc.getenv("VJ_BASE_DIR") +
                            File.separator + "share" +
                            File.separator + "vrjuggler" +
                            File.separator + "beans" +
                            File.separator + "customeditors";

      MessageDocument doc = TweekCore.instance().getMessageDocument();

      try
      {
         // Get the beans in the given path and add them to the dependency
         // manager.
         XMLBeanFinder finder = new XMLBeanFinder(false);
         List beans = finder.find(default_path);
         doc.printStatusnl(beans.toString());

         TweekCore tc = TweekCore.instance();
         tc.loadBeans(beans);
      }
      catch (BeanPathException e)
      {
         doc.printWarningnl("WARNING: Invalid path " + default_path);
      }
      
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      mHelpBrowserFrame.setContentPane(mHelpBrowser);
      mHelpBrowserFrame.setSize(new java.awt.Dimension(640, 480));
      mHelpBrowserFrame.setTitle("VR Juggler Configuration Help Browser");
      mHelpBrowserFrame.validate();

      // Add forward and back buttons to the toolbar
      mToolbar.addToToolbar(Box.createHorizontalStrut(8));
      mBackBtn.setText("Back");
      mBackBtn.setEnabled(false);
      mBackBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            backClicked();
         }
      });
      mForwardBtn.setText("Forward");
      mForwardBtn.setEnabled(false);
      mForwardBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            forwClicked();
         }
      });
      mToolbar.addToToolbar(mBackBtn);
      mToolbar.addToToolbar(mForwardBtn);

      // Load the ControlPanel model from XML file.
      EnvironmentServiceProxy env_service = new EnvironmentServiceProxy();
      String controlpanel_path =
         env_service.expandEnvVars(
            "${VJ_BASE_DIR}/share/vrjuggler/data/ControlPanel.xml"
         );
      ControlPanelViewModel model =
         new ControlPanelViewModel(controlpanel_path);
      showCategoryPanel((CategoryNode)model.getRoot());
   }

   public String getHelpDescription()
   {
      return "VR Juggler Configuration Help ...";
   }

   public String getFileType()
   {
      return "VR Juggler Configuration";
   }

   public boolean canOpenMultiple()
   {
      return true;
   }

   public boolean openRequested()
   {
      return true;
   }

   public boolean canSave()
   {
      return true;
   }

   public boolean hasUnsavedChanges()
   {
      return mContext.getConfigUndoManager().getUnsavedChanges();
   }

   public boolean saveRequested()
   {
      return mToolbar.doSave();
   }

   public boolean saveAsRequested()
   {
      return mToolbar.doSaveAs();
   }

   public boolean saveAllRequested()
   {
      return mToolbar.doSave();
   }

   public boolean closeRequested()
   {
      return true;
   }

   public javax.swing.undo.UndoManager getUndoManager()
   {
      return mContext.getConfigUndoManager();
   }

   public void undoRequested()
   {
      mToolbar.doUndo();
   }

   public void redoRequested()
   {
      mToolbar.doRedo();
   }

   public int getOpenFileCount()
   {
      return 1;
   }

   public void helpRequested()
   {
      try
      {
         mHelpBrowser.setPage(
            new java.net.URL(VrjConfigConstants.HELP_URL_STR)
         );
      }
      catch(java.net.MalformedURLException ex)
      {
         /* Ignore the exception. */ ;
      }

      mHelpBrowserFrame.setVisible(true);
   }

   /**
    * Helper class for getting the config manager serivce.
    */
   private ConfigBroker getConfigBroker()
   {
      return new ConfigBrokerProxy();
   }

   private void showTypePanel(TypeNode root)
   {
      ControlPanel ctl = createPanelWithElementsOfType(root.getToken());
      ctl.setTitle(root.getTitle());
   
      ctl.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            ControlPanel control = (ControlPanel)evt.getSource();
            Object object = control.getModel().getUserObjectAt(evt.getID());
            if(null != object && object instanceof ConfigElement)
            {
               ConfigElement element = (ConfigElement)object;
               String token = element.getDefinition().getToken();
               
               List list = CustomEditorRegistry.findEditors(token);
               
               //XXX:This will be used after making findEditors -> findEditor
               //if(null != editor)
               if(null == list || list.size() == 0)
               {
                  Frame parent = 
                     (Frame) SwingUtilities.getAncestorOfClass(Frame.class,
                                                               ControlPanelView.this);
                  JOptionPane.showMessageDialog(
                     parent,
                     "No Custom Editor registered for config element type '" +
                        token + "'",
                     "VRJConfig Control Panel",
                     JOptionPane.WARNING_MESSAGE
                  );
               }
               else if(null != list && list.size() > 0)
               {
                  CustomEditor editor = (CustomEditor)list.get(0);
                  editor.setConfig(mContext, element);

                  Frame parent = 
                     (Frame) SwingUtilities.getAncestorOfClass(Frame.class,
                                                               ControlPanelView.this);
                  CustomEditorDialog dlg = new CustomEditorDialog(parent,
                                                                  editor);
                  int status = dlg.showDialog();

                  if ( status == CustomEditorDialog.CANCEL_OPTION )
                  {
                     // XXX: How do we undo all of the changes that the user
                     // made?
                  }
                  // Save the user's changes.
                  else
                  {
                     ConfigBroker broker = new ConfigBrokerProxy();
                     for ( Iterator itr = mContext.getResources().iterator();
                           itr.hasNext(); )
                     {
                        DataSource data_source = broker.get((String)itr.next());
                        if (! data_source.isReadOnly())
                        {
                           try
                           {
                              data_source.commit();
                           }
                           catch(IOException ioe)
                           {
                              JOptionPane.showMessageDialog(parent,
                                                            ioe.getMessage(),
                                                            "Error",
                                                            JOptionPane.ERROR_MESSAGE);
                           }
                        }
                     }
                  }

                  // Inform the ConfigUndoManager that we have saved changes.
                  mContext.getConfigUndoManager().saveHappened();
               }
            }
         }
      });
      pushCurrentBack(ctl);
   }

   private void showEditorPanel(EditorNode root)
   {
      System.out.println("Editor Node: " + root.getClassName());
      
      try
      {
         ClassLoader loader = getClass().getClassLoader();

         final Class jogl_editor_class = loader.loadClass( root.getClassName() );
         
         try
         {
            CustomEditor pos_editor = (CustomEditor)jogl_editor_class.newInstance();
            JDialog dlg = new JDialog(
               (Frame)SwingUtilities.getAncestorOfClass(Frame.class, ControlPanelView.this),
               "Positional Device Editor",
               true);
            
            // This editor actually edits a context, so pass null for the ConfigElement.
            pos_editor.setConfig(mContext, null);

            dlg.getContentPane().add( pos_editor.getPanel() );
            dlg.setTitle( pos_editor.getTitle() );
            dlg.pack();
            dlg.setVisible( true );
            //frame.setSize(750, 750);
            //frame.show();
         }
         catch(InstantiationException e)
         {
            System.out.println(e);
            e.printStackTrace();
         }
         catch(IllegalAccessException e)
         {
            System.out.println(e);
            e.printStackTrace();
         }
         
         /*
         mTopSectionPanel.add(mVisualizeBtn);

         mVisualizeBtn.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  try
                  {
                     CustomEditor pos_editor = (CustomEditor)jogl_editor_class.newInstance();
                     JDialog dlg = new JDialog(
                        (Frame)SwingUtilities.getAncestorOfClass(Frame.class, IntersensePanel.this),
                        "3D Visualization", true);

                     pos_editor.setConfig(mConfigContext, mConfigElement);

                     dlg.getContentPane().add((JPanel)pos_editor);
                     dlg.pack();
                     dlg.setVisible(true);
                     //frame.setSize(750, 750);
                     //frame.show();
                  }
                  catch(InstantiationException e)
                  {
                     System.out.println(e);
                     e.printStackTrace();
                  }
                  catch(IllegalAccessException e)
                  {
                     System.out.println(e);
                     e.printStackTrace();
                  }
               }
            });
         */
      }
      catch(ClassNotFoundException e)
      {
         System.out.println("*** Could not find the PositionalDeviceEditor, JOGL must not be availible. ***");
      }
   }

   private void showWizardPanel(WizardNode root)
   {
      final JDialog dialog = new JDialog();
      dialog.setSize(590, 430);
      dialog.setModal(true);
      dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
      dialog.getContentPane().setLayout(new BorderLayout());
    
      try
      {
         Wizard wizard = WizardLoader.loadWizard(root.getWizardLocation(),
            this.getClass().getClassLoader());

         Map white_board = new HashMap();
         white_board.put("context", mContext);
         
         WizardViewerBean viewer = new WizardViewerBean(white_board);
         viewer.setWizard(wizard);
         viewer.setSidebarImage(new ImageIcon(this.getClass().getClassLoader().getResource("org/vrjuggler/tweek/wizard/images/juggler_sidebar.png")));
         viewer.addWizardViewListener(new WizardViewListener()
         {
            public void wizardStarted(WizardViewEvent evt) {}

            public void wizardCancelled(WizardViewEvent evt)
            {
               dialog.setVisible(false);
               dialog.dispose();
            }

            public void wizardFinished(WizardViewEvent evt)
            {
               dialog.setVisible(false);
               dialog.dispose();
            }
         });
         dialog.getContentPane().add(viewer, BorderLayout.CENTER);
         dialog.setVisible(true);
      }
      catch(IOException ex)
      {
         System.out.println("Wizard: " + root.getWizardLocation() + " failed to load.");
         ex.printStackTrace();
      }
   }
   
   private void showCategoryPanel(CategoryNode root)
   {
      DefaultControlPanelModel model = new DefaultControlPanelModel();
      if(!root.getAllowsChildren())
      {
         System.out.println("ERROR...");
         System.out.println(root.getClass());
      }
      for(Enumeration e = root.children() ; e.hasMoreElements() ; )
      {
         ControlPanelNode node = (ControlPanelNode)e.nextElement();
         model.add(node);
      }
      ControlPanel new_control = new ControlPanel();
      new_control.setModel(model);
      new_control.setTitle(root.getTitle());
      new_control.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  ControlPanel control = (ControlPanel)evt.getSource();
                  Object value = control.getModel().getUserObjectAt(evt.getID());
                  if(value instanceof CategoryNode)
                  {
                     showCategoryPanel((CategoryNode)value);
                  }
                  else if(value instanceof TypeNode)
                  {
                     showTypePanel((TypeNode)value);
                  }
                  else if(value instanceof EditorNode)
                  {
                     showEditorPanel((EditorNode)value);
                  }
                  else if(value instanceof WizardNode)
                  {
                     showWizardPanel((WizardNode)value);
                  }
               }
            });
      pushCurrentBack(new_control);
   }

   /**
    * Displays a panel containing all of the elements that have the given
    * token.
    */
   private ControlPanel createPanelWithElementsOfType(String token)
   {
      ClassLoader loader = BeanJarClassLoader.instance();
      DefaultControlPanelModel model = new DefaultControlPanelModel();
      /*
      List devices = ConfigUtilities.getElementsWithDefinition(
                           getConfigBroker().getElements(mContext), token);
      */
      List devices = getElementsWithDefinitionAndDerived(token);
      for (Iterator itr = devices.iterator(); itr.hasNext(); )
      {
         ConfigElement element = (ConfigElement)itr.next();
         String icon_location = element.getDefinition().getIconLocation();
         icon_location = (new EnvironmentServiceProxy()).expandEnvVars(icon_location);
         try
         {
            java.net.URL url = new java.net.URL(icon_location);
            Icon device_icon = new ImageIcon(url);
            model.add(element.getName(), device_icon, element);
         }
         catch(Exception ex)
         {
            ex.printStackTrace();
         }
      }
      ControlPanel new_control = new ControlPanel();
      new_control.setModel(model);

      return new_control;
   }

   private List getElementsWithDefinitionAndDerived(String token)
   {
      List result = new ArrayList();
      List elts = getConfigBroker().getElements(mContext);
      ConfigDefinitionRepository repos = getConfigBroker().getRepository();
      List sub_defs = repos.getSubDefinitions(token); 
      for(Iterator itr = sub_defs.iterator() ; itr.hasNext() ; )
      {
         List elements = ConfigUtilities.getElementsWithDefinition(elts, (String)itr.next());
         result.addAll(elements);
      }
      
      List elements = ConfigUtilities.getElementsWithDefinition(elts, token);
      result.addAll(elements);
      
      return result;
   }

   /**
    * Handles the case when the backward navigation button is clicked.
    */
   private void backClicked()
   {
      if (mForwardStack.empty())
      {
         mForwardBtn.setEnabled(true);
      }

      // Change the current component to the one at the top of the back stack.
      swapCurComponent(mBackStack, mForwardStack);

      if (mBackStack.empty())
      {
         mBackBtn.setEnabled(false);
      }
   }

   /**
    * Handles the case when the forward navigation button is clicked.
    */
   private void forwClicked()
   {
      if (mBackStack.empty())
      {
         mBackBtn.setEnabled(true);
      }

      // Change the current component to the next panel in the forward stack.
      swapCurComponent(mForwardStack, mBackStack);

      if (mForwardStack.empty())
      {
         mForwardBtn.setEnabled(false);
      }
   }

   /**
    * Moves the current component to the top of toStack and sets the current
    * component to the top of fromStack.
    */
   private void swapCurComponent(Stack fromStack, Stack toStack)
   {
      toStack.push(mCurrentView);

      this.remove(mCurrentView);
      mCurrentView = (Component)fromStack.pop();
      this.add(mCurrentView, BorderLayout.CENTER);
      this.repaint();
   }

   /**
    * Pushes the current component to the back stack and makes the given
    * component the new current component.
    */
   private void pushCurrentBack(Component newComp)
   {
      if (mBackStack.empty())
      {
         mBackBtn.setEnabled(true);
      }

      mBackStack.push(mCurrentView);

      // Clear out the forward navigation stack because what we are now viewing
      // is what will go into the forward stack if the back button is pressed.
      mForwardStack.clear();
      mForwardBtn.setEnabled(false);

      this.remove(mCurrentView);
      mCurrentView = newComp;
      this.add(mCurrentView, BorderLayout.CENTER);
      updateUI();
      newComp.invalidate();
   }

   /**
    * Autogenerated code for the JBuilder GUI.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(mBaseLayout);
      this.add(mToolbar, BorderLayout.NORTH);
   }

   private TinyBrowser mHelpBrowser      = new TinyBrowser();
   private JFrame      mHelpBrowserFrame = new JFrame();

   //--- JBuilder GUI variables ---//
   private BorderLayout mBaseLayout = new BorderLayout();
   private ControlPanelToolbar mToolbar = null;

   /**
    * The context providing a view into the current configuration.
    */
   private ConfigContext mContext = new ConfigContext();

   /**
    * The currently viewed component.
    */
   private Component mCurrentView = new ControlPanel();

   private JButton mBackBtn = new JButton();
   private JButton mForwardBtn = new JButton();

   private Stack mBackStack = new Stack();
   private Stack mForwardStack = new Stack();
}
