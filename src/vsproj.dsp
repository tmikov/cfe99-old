# Microsoft Developer Studio Project File - Name="vsproj" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=vsproj - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vsproj.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vsproj.mak" CFG="vsproj - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vsproj - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "vsproj - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "vsproj - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f vsproj.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "vsproj.exe"
# PROP BASE Bsc_Name "vsproj.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "nmake /f nt86.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "out\c99c.exe"
# PROP Bsc_Name "out\c99c.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "vsproj - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f vsproj.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "vsproj.exe"
# PROP BASE Bsc_Name "vsproj.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "nmake /f nt86.mak debug=1"
# PROP Rebuild_Opt "/a"
# PROP Target_File "out\c99c.exe"
# PROP Bsc_Name "out\c99c.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "vsproj - Win32 Release"
# Name "vsproj - Win32 Debug"

!IF  "$(CFG)" == "vsproj - Win32 Release"

!ELSEIF  "$(CFG)" == "vsproj - Win32 Debug"

!ENDIF 

# Begin Group "cfe"

# PROP Default_Filter ""
# Begin Group "c"

# PROP Default_Filter ""
# Begin Group "srtuni"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cfe\c\srtuni\doit.bat
# End Source File
# Begin Source File

SOURCE=.\cfe\c\srtuni\srtuni.c
# End Source File
# Begin Source File

SOURCE=.\cfe\c\srtuni\srtuni.l
# End Source File
# Begin Source File

SOURCE=.\cfe\c\srtuni\uni.txt
# End Source File
# End Group
# Begin Group "generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cfe\c\c.output
# End Source File
# Begin Source File

SOURCE=.\cfe\c\c.tab.c
# End Source File
# Begin Source File

SOURCE=.\cfe\c\c.tab.h
# End Source File
# Begin Source File

SOURCE=.\cfe\c\c.tab.inc
# End Source File
# Begin Source File

SOURCE=.\cfe\c\c.y
# End Source File
# Begin Source File

SOURCE=.\cfe\c\c1.act
# End Source File
# Begin Source File

SOURCE=.\cfe\c\c1.tab.c
# End Source File
# Begin Source File

SOURCE=.\cfe\c\c1.y
# End Source File
# Begin Source File

SOURCE=.\cfe\c\pp_parse.tab.c
# End Source File
# Begin Source File

SOURCE=.\cfe\c\pp_parse.tab.h
# End Source File
# Begin Source File

SOURCE=.\cfe\c\pp_scan.c
# End Source File
# Begin Source File

SOURCE=.\cfe\c\scan.c
# End Source File
# Begin Source File

SOURCE=.\cfe\c\srtuni\uni.inc
# End Source File
# End Group
# Begin Source File

SOURCE=.\cfe\c\bison.simple.c
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-ast.c"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-ast.h"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-ast_ops.inc"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-decl.c"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-decl.h"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-dspecs.c"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-dspecs.h"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-dump.c"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-dump.h"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-expr.c"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-expr.h"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-fold.c"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-stmt.c"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-stmt.h"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-symtab.c"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-symtab.h"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-types.c"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-types.h"
# End Source File
# Begin Source File

SOURCE=".\cfe\c\c-types.txt"
# End Source File
# Begin Source File

SOURCE=.\cfe\c\c.yp
# End Source File
# Begin Source File

SOURCE=.\cfe\c\cfe.c
# End Source File
# Begin Source File

SOURCE=.\cfe\c\cfe_err.c
# End Source File
# Begin Source File

SOURCE=.\cfe\c\cfe_err.h
# End Source File
# Begin Source File

SOURCE=.\cfe\c\cfe_opts.c
# End Source File
# Begin Source File

SOURCE=.\cfe\c\cfe_priv.h
# End Source File
# Begin Source File

SOURCE=.\cfe\c\hexdbl.c
# End Source File
# Begin Source File

SOURCE=.\cfe\c\idtab.c
# End Source File
# Begin Source File

SOURCE=.\cfe\c\idtab.h
# End Source File
# Begin Source File

SOURCE=.\cfe\c\pp_parse.h
# End Source File
# Begin Source File

SOURCE=.\cfe\c\pp_parse.y
# End Source File
# Begin Source File

SOURCE=.\cfe\c\pp_scan.l
# End Source File
# Begin Source File

SOURCE=.\cfe\c\scan.l
# End Source File
# End Group
# Begin Group "h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cfe\h\cfe.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cfe\technote.txt
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Group "c No. 1"

# PROP Default_Filter ""
# End Group
# Begin Group "h No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\common\h\common.h
# End Source File
# End Group
# End Group
# Begin Group "target"

# PROP Default_Filter ""
# Begin Group "i386"

# PROP Default_Filter ""
# Begin Group "mach No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\target\i386\mach\machconf.c
# End Source File
# Begin Source File

SOURCE=.\target\i386\mach\machconf.h
# End Source File
# End Group
# Begin Group "cfe No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\target\i386\cfe\cfeconf.c
# End Source File
# Begin Source File

SOURCE=.\target\i386\cfe\cfeconf.h
# End Source File
# End Group
# End Group
# Begin Group "h No. 4"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\target\h\cfetarg.h
# End Source File
# Begin Source File

SOURCE=.\target\h\mach.h
# End Source File
# End Group
# Begin Group "c No. 4"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\target\c\mach.c
# End Source File
# Begin Source File

SOURCE=.\target\c\mach_priv.h
# End Source File
# End Group
# End Group
# Begin Group "ulib"

# PROP Default_Filter ""
# Begin Group "c No. 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ulib\c\clist.c
# End Source File
# Begin Source File

SOURCE=..\ulib\c\cord.c
# End Source File
# Begin Source File

SOURCE=..\ulib\c\debug.c
# End Source File
# Begin Source File

SOURCE=..\ulib\c\simgc.c
# End Source File
# Begin Source File

SOURCE=..\ulib\c\strings.c
# End Source File
# Begin Source File

SOURCE=..\ulib\c\ulibpriv.h
# End Source File
# End Group
# Begin Group "h No. 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ulib\h\clist.h
# End Source File
# Begin Source File

SOURCE=..\ulib\h\cord.h
# End Source File
# Begin Source File

SOURCE=..\ulib\h\debug.h
# End Source File
# Begin Source File

SOURCE=..\ulib\h\simgc.h
# End Source File
# Begin Source File

SOURCE=..\ulib\h\strings.h
# End Source File
# Begin Source File

SOURCE=..\ulib\h\ulibdefs.h
# End Source File
# End Group
# Begin Group "avl-0.4"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\ulib\avl-0.4\avl.c"
# End Source File
# Begin Source File

SOURCE="..\ulib\avl-0.4\avl.h"
# End Source File
# End Group
# End Group
# Begin Group "tests"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\tests\decl1.c
# End Source File
# Begin Source File

SOURCE=.\tests\decl2.c
# End Source File
# Begin Source File

SOURCE=.\tests\decl3.c
# End Source File
# Begin Source File

SOURCE=.\tests\decl4.c
# End Source File
# Begin Source File

SOURCE=.\tests\scan1.c
# End Source File
# Begin Source File

SOURCE=.\tests\scan2.c
# End Source File
# Begin Source File

SOURCE=.\tests\xattr1.c
# End Source File
# Begin Source File

SOURCE=.\tests\xdspec1.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\clean.bat
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\notes.txt
# End Source File
# Begin Source File

SOURCE=.\nt86.mak
# End Source File
# Begin Source File

SOURCE=.\precomp.h
# End Source File
# Begin Source File

SOURCE=.\scratch.txt
# End Source File
# Begin Source File

SOURCE=.\technote.txt
# End Source File
# End Target
# End Project
