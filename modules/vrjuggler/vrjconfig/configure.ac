# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2011 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
#
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Base configure.in for VRJConfig.
# -----------------------------------------------------------------------------
# This file is "compiled" by GNU autoconf to generate the configure script
# that is actually run.
# -----------------------------------------------------------------------------

AC_INIT([VRJConfig], [1.5], [vrjuggler-devel@lists.sourceforge.net])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2011 by Iowa State University])
AC_CONFIG_SRCDIR([VRJConfig.xml])
AC_REVISION($Revision$)
AC_CONFIG_AUX_DIR([../../../share/config])
DPP_PREREQ([2.2.2])
DPP_INIT

# Generate the reconfig script first so that it's easy to run configure again
# if it fails later on.
DPP_GEN_RECONFIG

# -----------------------------------------------------------------------------
# Command-line arguments (--enable-option, --with-pkg=package_name).
# -----------------------------------------------------------------------------
# Enable versioning by default
AC_ARG_ENABLE([versioning],
              [  --disable-versioning    Disable all versioning capabilities],
              [ENABLE_VERSIONING="$enableval"], [ENABLE_VERSIONING='yes'])
# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

# Specify where Java3D is located
AC_ARG_WITH(java3d,
            [  --with-java3d=<PATH>    Path to Java3D JAR file(s) ],
            JAVA3D_JAR="$withval", JAVA3D_JAR='none')

# Specify where JOGL is located
AC_ARG_WITH(jogl,
            [  --with-jogl=<PATH>      Path to JOGL JAR file(s) ],
            JOGL_JARS="$withval", JOGL_JARS='')

# -----------------------------------------------------------------------------
# System-dependent stuff.
# -----------------------------------------------------------------------------
DPP_SYSTEM_SETUP

# -----------------------------------------------------------------------------
# Path setup.
# -----------------------------------------------------------------------------

# $srcdir is the root directory of the juggler source tree.  To get a value for
# $VJROOT_ABS, we cd there and save the value of running pwd.  Then return to
# the directory where configure is being run ($topdir).
cd "$srcdir"
VJROOT_ABS=`pwd`
cd "$VJROOT_ABS/../../.."
UNIX_JUGGLERROOT_ABS=`pwd`
cd "$topdir"

JUGGLERROOT_ABS="$UNIX_JUGGLERROOT_ABS"

# -----------------------------------------------------------------------------
# Checks for programs.
# -----------------------------------------------------------------------------

# Check for the JDK.
DPP_PATH_JAVA( , [BUILD_JAVA='Y'], [BUILD_JAVA='N'])

# If there is still no value in $JAVAC, then warn that VRJConfig will not be
# built.
if test "x$BUILD_JAVA" = "xN" ; then
   AC_MSG_WARN([*** VRJConfig requires the JDK -- it will not be built ***])
else
   DPP_JDK_VERSION([$JDK_HOME], [1.4], ,
      [AC_MSG_WARN([*** JDK 1.4 or newer required for VRJConfig ***])
       BUILD_JAVA='N'])
fi

AC_CHECK_PROG([ANT],
    [ant],
    [JAVA_HOME=\"$JDK_HOME\" \"$UNIX_JUGGLERROOT_ABS/external/ant/bin/ant\" --noconfig])
AC_ARG_VAR([ANT], [Path of ant build tool])

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_HAVE_PERL5( , , [AC_MSG_ERROR([*** Perl is required ***])])

DPP_HAVE_GNU_MAKE([3.78], ,
    [AC_MSG_ERROR([*** The build system requires GNU make 3.78 or newer ***])])
DPP_BASIC_PROGS($PLATFORM, $OS_TYPE)
DPP_PROG_INSTALL

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------

if test "x$BUILD_JAVA" = "xY" ; then
   TWEEK_PATH_JAVA([0.92.4],
      [JCCL_PATH_JAVA([0.92.11], ,
         [AC_MSG_WARN([*** JCCL Java API required for VRJConfig ***])
          BUILD_JAVA='N'])],
      [AC_MSG_WARN(*** Tweek Java API required for VRJConfig ***)
       BUILD_JAVA='N'])
fi

# --------
# Java3D.
# --------
BUILD_JAVA_3D_EDITOR='N'
if test "x$BUILD_JAVA" = "xY" ; then
   # If we are building VRJConfig, we can try to enable building the
   # Java3D-based editor(s).
   if test "x$BUILD_JAVA" = "xY" ; then
      DPP_CHECK_JAVA_STATIC_METHOD([javax.media.j3d.VirtualUniverse],
         [getJ3DThreadPriority()], ,
         [BUILD_JAVA_3D_EDITOR='Y'],
         [ AC_MSG_WARN([*** Java3D required for Java3D custom editor ***])
           BUILD_JAVA_3D_EDITOR='N' ],
         [$JAVA3D_JAR])
   fi
fi

# --------
# JOGL.
# --------
BUILD_JOGL_EDITOR='N'
if test "x$BUILD_JAVA" = "xY" ; then
   # If we are building VRJConfig, we can try to enable building the
   # JOGL-based editor(s).
   if test "x$BUILD_JAVA" = "xY" ; then
      DPP_CHECK_JAVA_STATIC_METHOD([net.java.games.jogl.Version],
         [getVersion()], ,
         [BUILD_JOGL_EDITOR='Y'],
         [ AC_MSG_WARN([*** JOGL required for JOGL custom editor ***])
           BUILD_JOGL_EDITOR='N' ],
         [$JOGL_JARS])
      if test "x$BUILD_JOGL_EDITOR" = "xY" ; then
         DPP_CHECK_JAVA_STATIC_METHOD([gleem.ManipManager],
            [getManipManager()], ,
            [BUILD_JOGL_EDITOR='Y'],
            [ AC_MSG_WARN([*** Gleem required for JOGL custom editor ***])
              BUILD_JOGL_EDITOR='N' ],
            [$JOGL_JARS])
      fi
   fi
fi

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------
DPP_INSTALLER(vrjuggler, 0644, 0755, 0755)

# -----------------------------------------------------------------------------
# Finalize substitution settings.
# -----------------------------------------------------------------------------
#

VJ_VERSION_GROK(../VERSION)

TWEEK_DATA_DIR="share/$TWEEK_DATA_SUBDIR"
JCCL_DATA_DIR="share/$JCCL_DATA_SUBDIR"

if test "x$ENABLE_VERSIONING" = "xyes" ; then
   VJ_VERSION_DOT="$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION"
   VJ_DATA_VERSION="vrjuggler-$VJ_VERSION_DOT"
   VJ_DATA_DIR="share/vrjuggler-$VJ_VERSION_DOT"
   PLUGIN_DIR_VERSION="vrjuggler-$VJ_VERSION_DOT"

# If versioning is disabled, then none of the version variables need to be
# set except for $DATA_VERSION. Its use is different than the others. The
# important thing to note is that the directory name is unversioned in this
# case.
else
   VJ_DATA_VERSION="vrjuggler"
   VJ_DATA_DIR="share/vrjuggler"
   PLUGIN_DIR_VERSION="vrjuggler"
fi

# Define the base path to the source directory using $(VJROOT_ABS) as an
# alternative to using $(srcdir).
UNIX_VJROOT_ABS="$VJROOT_ABS"

if test "x$PLATFORM" = "xDarwin" ; then
   changequote(<<, >>)
   vrjconfig_bundle_tweek_jardir=`echo $TWEEK_JARS | awk -F: '{ print $1 }' | sed -e 's/^\(.*\)\/[^/]*\.jar$/\1/'`
   vrjconfig_bundle_jccl_jardir=`echo $JCCL_JARS | awk -F: '{ print $1 }' | sed -e 's/^\(.*\)\/[^/]*\.jar$/\1/'`

   tweek_java_api_jars_bundle=`echo $TWEEK_JARS | sed -e "s|$vrjconfig_bundle_tweek_jardir/\([^/]*.jar\)|"'$JAVAROOT/\1|g'`
   tweek_ext_java_api_jars_bundle=`echo $TWEEK_EXT_JARS | sed -e 's/:/ /g' -e "s|$vrjconfig_bundle_tweek_jardir/\([^/].jar\)|\1|g"`
   changequote([, ])

   tweek_datadir=`echo $vrjconfig_bundle_tweek_jardir | sed -e 's/\(.*\)java$/\1/'`
   vrjconfig_bundle_tweek_beandir="${tweek_datadir}beans"

   jccl_datadir=`echo $vrjconfig_bundle_jccl_jardir | sed -e 's/\(.*\)java$/\1/'`
   vrjconfig_bundle_jccl_beandir="${jccl_datadir}beans"
   vrjconfig_bundle_jccl_datadir="${jccl_datadir}data"
fi

# -----------------------------------------------------------------------------
# Do Makefile substitutions.
# -----------------------------------------------------------------------------

DPP_SUBST

AC_SUBST(topdir)
AC_SUBST(UNIX_VJROOT_ABS)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(VJROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)

AC_SUBST(TWEEK_DATA_DIR)
AC_SUBST(JCCL_DATA_DIR)
AC_SUBST(VJ_DATA_VERSION)
AC_SUBST(VJ_DATA_DIR)
AC_SUBST(PLUGIN_DIR_VERSION)
AC_SUBST(ANT)
AC_SUBST(RM_LN)

AC_SUBST(BUILD_JAVA)
AC_SUBST(BUILD_JAVA_3D_EDITOR)
AC_SUBST(JAVA3D_JAR)
AC_SUBST(JOGL_JARS)

AC_SUBST(tweek_java_api_jars_bundle)
AC_SUBST(tweek_ext_java_api_jars_bundle)
AC_SUBST(vrjconfig_bundle_tweek_beandir)
AC_SUBST(vrjconfig_bundle_tweek_jardir)
AC_SUBST(vrjconfig_bundle_jccl_beandir)
AC_SUBST(vrjconfig_bundle_jccl_datadir)
AC_SUBST(vrjconfig_bundle_jccl_jardir)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------

AC_CONFIG_FILES([
   Makefile
   build.xml
   common.defs.mk
   Info.plist
   java.defs.mk
   make.defs.mk
   vrjconfig.sh
   commoneditors/build.xml
   commoneditors/Makefile
   customeditors/Makefile
   customeditors/cave/build.xml
   customeditors/cave/Makefile
   customeditors/display_window/build.xml
   customeditors/display_window/Makefile
   customeditors/flock/build.xml
   customeditors/flock/Makefile
   customeditors/intersense/build.xml
   customeditors/intersense/Makefile
   customeditors/motionstar/build.xml
   customeditors/motionstar/Makefile
   customeditors/pinchglove/build.xml
   customeditors/pinchglove/Makefile
   customeditors/proxyeditor/build.xml
   customeditors/proxyeditor/Makefile
   customeditors/surfacedisplayeditor/build.xml
   customeditors/surfacedisplayeditor/Makefile
   wizards/Makefile
   wizards/cluster/build.xml
   wizards/cluster/Makefile
   wizards/newdevice/build.xml
   wizards/newdevice/Makefile
   wizards/vrsystem/build.xml
   wizards/vrsystem/Makefile
   ])

AC_CONFIG_COMMANDS([permission], [chmod a+x vrjconfig.sh])

AC_OUTPUT
