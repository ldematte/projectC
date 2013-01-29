# Microsoft Developer Studio Project File - Name="compiler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=compiler - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "compiler.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "compiler.mak" CFG="compiler - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "compiler - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "compiler - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "compiler - Win32 Release"

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
# ADD CPP /nologo /W4 /GX /O2 /I "../" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "COMPILER_ENVIRONMENT" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "compiler___Win32_Debug"
# PROP BASE Intermediate_Dir "compiler___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "compiler___Win32_Debug"
# PROP Intermediate_Dir "compiler___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W4 /Gm /GR /GX /ZI /Od /I "../" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "COMPILER_ENVIRONMENT" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCMT" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "compiler - Win32 Release"
# Name "compiler - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CodeEmitter.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ICodeEmitter.cpp
# End Source File
# Begin Source File

SOURCE=..\Instruction.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Native.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Preprocessor.cpp
# End Source File
# Begin Source File

SOURCE=.\ScopeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SourceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StringTable.cpp
# End Source File
# Begin Source File

SOURCE=.\symbol.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolTable.cpp
# End Source File
# Begin Source File

SOURCE=.\SyntaxTree.cpp
# End Source File
# Begin Source File

SOURCE=..\type.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CodeEmitter.h
# End Source File
# Begin Source File

SOURCE=..\CodeHeader.h
# End Source File
# Begin Source File

SOURCE=..\common.h
# End Source File
# Begin Source File

SOURCE=..\constants.h
# End Source File
# Begin Source File

SOURCE=.\ErrorManager.h
# End Source File
# Begin Source File

SOURCE=.\hash.h
# End Source File
# Begin Source File

SOURCE=.\ICodeEmitter.h
# End Source File
# Begin Source File

SOURCE=..\Instruction.h
# End Source File
# Begin Source File

SOURCE=.\lexer.h
# End Source File
# Begin Source File

SOURCE=.\MyStack.h
# End Source File
# Begin Source File

SOURCE=.\Native.h
# End Source File
# Begin Source File

SOURCE=..\NativeFunction.h
# End Source File
# Begin Source File

SOURCE=..\Object.h
# End Source File
# Begin Source File

SOURCE=..\Operand.h
# End Source File
# Begin Source File

SOURCE=.\OutputManager.h
# End Source File
# Begin Source File

SOURCE=.\Preprocessor.h
# End Source File
# Begin Source File

SOURCE=.\ScopeManager.h
# End Source File
# Begin Source File

SOURCE=..\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\SourceManager.h
# End Source File
# Begin Source File

SOURCE=.\StringTable.h
# End Source File
# Begin Source File

SOURCE=.\symbol.h
# End Source File
# Begin Source File

SOURCE=..\symbolP.h
# End Source File
# Begin Source File

SOURCE=.\SymbolTable.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxTree.h
# End Source File
# Begin Source File

SOURCE=..\type.h
# End Source File
# Begin Source File

SOURCE=.\Vtable.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lexer.cpp
# End Source File
# Begin Source File

SOURCE=.\minicpp.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\minicpp.lex
# End Source File
# Begin Source File

SOURCE=.\minicpp2.y
# End Source File
# End Target
# End Project
