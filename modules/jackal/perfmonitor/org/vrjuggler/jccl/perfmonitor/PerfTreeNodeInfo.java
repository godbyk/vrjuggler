/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

package VjComponents.PerfMonitor;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import VjComponents.PerfMonitor.LabeledPerfDataCollector; 

public class PerfTreeNodeInfo implements ActionListener {

    protected class LabeledPanelButton extends JButton {
        public LabeledPerfDataCollector collector;
        public LabeledPerfDataCollector.IndexInfo index_info;

        public LabeledPanelButton (LabeledPerfDataCollector _collector, 
                                   LabeledPerfDataCollector.IndexInfo _ii, 
                                   String text) {
            super (text);
            collector = _collector;
            index_info = _ii;
        }
    }

    public String sublabel;
    public LabeledPerfDataCollector.IndexInfo ii; // null for folders
    protected LabeledPerfDataCollector mCollector;
    protected JComponent mComponent;
    protected JLabel mValueLabel;
    protected JButton mGraphButton;

    public PerfTreeNodeInfo (String _sublabel, LabeledPerfDataCollector.IndexInfo _ii, LabeledPerfDataCollector col) {
	sublabel = _sublabel;
	ii = _ii;
	mCollector = col;

        mComponent = new JPanel();
        mComponent.setLayout (new BoxLayout (mComponent, BoxLayout.X_AXIS));

	if (ii != null) {
	    JLabel l = new JLabel(_sublabel);
	    mComponent.add (l);
            mComponent.add (Box.createHorizontalGlue());
	    mValueLabel = new JLabel (padFloat(ii.getAverage()/1000.0), JLabel.RIGHT);
	    mComponent.add (mValueLabel);
	    mGraphButton = new LabeledPanelButton (col, ii, "Graph");
	    mGraphButton.setActionCommand ("Graph");
	    mGraphButton.addActionListener (this);
	    //b.addActionListener (PerfAnalyzerPanel.this);
	    Insets insets = new Insets (1,1,1,1);
	    mGraphButton.setMargin(insets);
	    mComponent.add (mGraphButton);
	}
	else {
	    mComponent.add(new JLabel ("<html><h2><i>" + sublabel + "</i></h2></html>"));
            mComponent.add (Box.createHorizontalGlue());
	    mGraphButton = new LabeledPanelButton (col, null, "Graph");
	    mGraphButton.setActionCommand ("Graph");
	    mGraphButton.addActionListener (this);
	    //b.addActionListener (PerfAnalyzerPanel.this);
	    Insets insets = new Insets (1,1,1,1);
	    mGraphButton.setMargin(insets);
	    mComponent.add (mGraphButton);
	}
    }

    public String toString () {
	return sublabel;
    }
    
    public JComponent getComponent() {
	return mComponent;
    }

    public LabeledPerfDataCollector.IndexInfo getIndexInfo() {
	return ii;
    }

    public LabeledPerfDataCollector getCollector() {
	return mCollector;
    }

    public JButton getGraphButton () {
	return mGraphButton;
    }

    public void update() {
	if (ii != null) {
	    if (mValueLabel != null)
		mValueLabel.setText (padFloat(ii.getAverage()/1000.0));
	}
    }

    /** Utility method for various printing routines. */
    private String padFloat (double f) {
	// reformats f to a string w/ 3 places after decimal
	String s = Double.toString(f);
	if (s.indexOf('E') != -1)
	    return s;
	int i = s.lastIndexOf('.');
	if ((i >= 0) && (i+5 < s.length()))
	    s = s.substring (0, i + 5);
	return s;
    }

    public void actionPerformed (ActionEvent e) {
	System.out.println ("actionevent...");
    }

}
