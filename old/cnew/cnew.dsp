# Microsoft Developer Studio Project File - Name="cnew" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=cnew - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cnew.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cnew.mak" CFG="cnew - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cnew - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "cnew - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Work/c", CAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cnew - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D YYDEBUG=1 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "cnew - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D YYDEBUG=1 /D HEAP_DBG=0 /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "cnew - Win32 Release"
# Name "cnew - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\misc\ARENA.C
# End Source File
# Begin Source File

SOURCE=.\misc\PAGEHEAP.C
# End Source File
# Begin Source File

SOURCE=.\misc\xheap.C
# End Source File
# End Group
# Begin Group "port"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\port\WINTRACE.C
# End Source File
# End Group
# Begin Source File

SOURCE=.\actsym.c
# End Source File
# Begin Source File

SOURCE=".\C-TYPES.C"
# End Source File
# Begin Source File

SOURCE=.\C.Y
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\C_TAB.C
# End Source File
# Begin Source File

SOURCE=.\CMAIN.C
# End Source File
# Begin Source File

SOURCE=.\comp.c
# End Source File
# Begin Source File

SOURCE=.\CSYMS.C
# End Source File
# Begin Source File

SOURCE=.\CWORDS.C
# End Source File
# Begin Source File

SOURCE=.\DECL.C
# End Source File
# Begin Source File

SOURCE=.\PARSETYP.C
# End Source File
# Begin Source File

SOURCE=.\SCAN.C
# End Source File
# Begin Source File

SOURCE=.\SCOPE.C
# End Source File
# Begin Source File

SOURCE=.\STRINGS.C
# End Source File
# Begin Source File

SOURCE=.\SYMTAB.C
# End Source File
# Begin Source File

SOURCE=.\tconf.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\C-HCONF.H"
# End Source File
# Begin Source File

SOURCE=".\C-TCONF.H"
# End Source File
# Begin Source File

SOURCE=".\C-TREE.DEF"
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=".\C-TREE.H"
# End Source File
# Begin Source File

SOURCE=".\C-TYPES.H"
# End Source File
# Begin Source File

SOURCE=.\C_TAB.H
# End Source File
# Begin Source File

SOURCE=.\COMP.H
# End Source File
# Begin Source File

SOURCE=.\conststr.H
# End Source File
# Begin Source File

SOURCE=.\DECL.H
# End Source File
# Begin Source File

SOURCE=.\DECLSPEC.C
# End Source File
# Begin Source File

SOURCE=.\DECLSPEC.H
# End Source File
# Begin Source File

SOURCE=.\exprcode.h
# End Source File
# Begin Source File

SOURCE=.\misc\FIXALLOC.H
# End Source File
# Begin Source File

SOURCE=.\PARSETYP.H
# End Source File
# Begin Source File

SOURCE=.\SCANTOK.H
# End Source File
# Begin Source File

SOURCE=.\SYMTAB.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Doc files"

# PROP Default_Filter "*.txt"
# Begin Source File

SOURCE=.\design.txt
# End Source File
# Begin Source File

SOURCE=.\FILES.TXT
# End Source File
# Begin Source File

SOURCE=.\LOG.TXT
# End Source File
# Begin Source File

SOURCE=.\NOTES.TXT
# End Source File
# Begin Source File

SOURCE=.\OPTNOTES.TXT
# End Source File
# Begin Source File

SOURCE=.\todo.txt
# End Source File
# End Group
# End Target
# End Project
