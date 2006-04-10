

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001, 2002
 *   by Iowa State University
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

#include <iostream>
#include <vector>

#include "TextTestResult.h"
#include "ExampleTestCase.h"


using namespace std;

typedef pair<string, Test *>           mapping;
typedef vector<pair<string, Test *> >   mappings;

class TestRunner
{
protected:
    bool                                m_wait;
    vector<pair<string,Test *> >        m_mappings;

public:
	            TestRunner    () : m_wait (false) {}
                ~TestRunner   ();

    void        run           (int ac, char **av);
    void        addTest       (string name, Test *test)
    { m_mappings.push_back (mapping (name, test)); }

protected:
    void        run (Test *test);
    void        printBanner ();

};



void TestRunner::printBanner ()
{
    cout << "Usage: driver [ -wait ] testName, where name is the name of a test case class" << endl;
}


void TestRunner::run (int ac, char **av)
{
    string  testCase;
    int     numberOfTests = 0;

    for (int i = 1; i < ac; i++) {

        if (string (av [i]) == "-wait") {
            m_wait = true;
            continue;
        }

        testCase = av [i];

        if (testCase == "") {
            printBanner ();
            return;
        }

        Test *testToRun = NULL;

        for (mappings::iterator it = m_mappings.begin ();
                it != m_mappings.end ();
                ++it) {
            if ((*it).first == testCase) {
                testToRun = (*it).second;
                run (testToRun);

            }
        }

        numberOfTests++;

        if (!testToRun) {
            cout << "Test " << testCase << " not found." << endl;
            return;

        } 


    }

    if (numberOfTests == 0) {
        printBanner ();
        return;        
    }

    if (m_wait) {
        cout << "<RETURN> to continue" << endl;
        cin.get ();

    }


}


TestRunner::~TestRunner ()
{
    for (mappings::iterator it = m_mappings.begin ();
             it != m_mappings.end ();
             ++it)
        delete it->second;

}


void TestRunner::run (Test *test)
{
    TextTestResult  result;

    test->run (&result);

    cout << "Test::run(TextTestResult): "<< endl;
}


int main (int ac, char **av)
{
    TestRunner runner;

    runner.addTest ("ExampleTestCase", ExampleTestCase::suite ());
    runner.run (ac, av);

    return 0;
}

