# Microsoft Developer Studio Generated NMAKE File, Based on VirtualMachine.dsp
!IF "$(CFG)" == ""
CFG=VirtualMachine - Win32 Debug
!MESSAGE No configuration specified. Defaulting to VirtualMachine - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "VirtualMachine - Win32 Release" && "$(CFG)" != "VirtualMachine - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VirtualMachine.mak" CFG="VirtualMachine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VirtualMachine - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "VirtualMachine - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "VirtualMachine - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\VirtualMachine.exe"


CLEAN :
	-@erase "$(INTDIR)\ClassTable.obj"
	-@erase "$(INTDIR)\CodeLoader.obj"
	-@erase "$(INTDIR)\Exports.obj"
	-@erase "$(INTDIR)\Instruction.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\NativeCall.obj"
	-@erase "$(INTDIR)\Operand.obj"
	-@erase "$(INTDIR)\RTStringTable.obj"
	-@erase "$(INTDIR)\RTSysExportTable.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VirtualMachine.obj"
	-@erase "$(OUTDIR)\VirtualMachine.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W4 /GR /GX /O2 /I "../" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\VirtualMachine.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VirtualMachine.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\VirtualMachine.pdb" /machine:I386 /nodefaultlib:"LIBCMT" /out:"$(OUTDIR)\VirtualMachine.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ClassTable.obj" \
	"$(INTDIR)\CodeLoader.obj" \
	"$(INTDIR)\Exports.obj" \
	"$(INTDIR)\Instruction.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\NativeCall.obj" \
	"$(INTDIR)\Operand.obj" \
	"$(INTDIR)\RTStringTable.obj" \
	"$(INTDIR)\RTSysExportTable.obj" \
	"$(INTDIR)\VirtualMachine.obj"

"$(OUTDIR)\VirtualMachine.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "VirtualMachine - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\VirtualMachine.exe"


CLEAN :
	-@erase "$(INTDIR)\ClassTable.obj"
	-@erase "$(INTDIR)\CodeLoader.obj"
	-@erase "$(INTDIR)\Exports.obj"
	-@erase "$(INTDIR)\Instruction.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\NativeCall.obj"
	-@erase "$(INTDIR)\Operand.obj"
	-@erase "$(INTDIR)\RTStringTable.obj"
	-@erase "$(INTDIR)\RTSysExportTable.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VirtualMachine.obj"
	-@erase "$(OUTDIR)\VirtualMachine.exe"
	-@erase "$(OUTDIR)\VirtualMachine.ilk"
	-@erase "$(OUTDIR)\VirtualMachine.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W4 /Gm /GR /GX /ZI /Od /I "../" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\VirtualMachine.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VirtualMachine.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\VirtualMachine.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT" /out:"$(OUTDIR)\VirtualMachine.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ClassTable.obj" \
	"$(INTDIR)\CodeLoader.obj" \
	"$(INTDIR)\Exports.obj" \
	"$(INTDIR)\Instruction.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\NativeCall.obj" \
	"$(INTDIR)\Operand.obj" \
	"$(INTDIR)\RTStringTable.obj" \
	"$(INTDIR)\RTSysExportTable.obj" \
	"$(INTDIR)\VirtualMachine.obj"

"$(OUTDIR)\VirtualMachine.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("VirtualMachine.dep")
!INCLUDE "VirtualMachine.dep"
!ELSE 
!MESSAGE Warning: cannot find "VirtualMachine.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "VirtualMachine - Win32 Release" || "$(CFG)" == "VirtualMachine - Win32 Debug"
SOURCE=.\ClassTable.cpp

"$(INTDIR)\ClassTable.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CodeLoader.cpp

"$(INTDIR)\CodeLoader.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Exports.cpp

"$(INTDIR)\Exports.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Instruction.cpp

"$(INTDIR)\Instruction.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\NativeCall.cpp

"$(INTDIR)\NativeCall.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Operand.cpp

"$(INTDIR)\Operand.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RTStringTable.cpp

"$(INTDIR)\RTStringTable.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RTSysExportTable.cpp

"$(INTDIR)\RTSysExportTable.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\VirtualMachine.cpp

"$(INTDIR)\VirtualMachine.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

