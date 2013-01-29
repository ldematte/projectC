# Microsoft Developer Studio Generated NMAKE File, Based on compiler.dsp
!IF "$(CFG)" == ""
CFG=compiler - Win32 Debug
!MESSAGE No configuration specified. Defaulting to compiler - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "compiler - Win32 Release" && "$(CFG)" != "compiler - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "compiler - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\compiler.exe"


CLEAN :
	-@erase "$(INTDIR)\CodeEmitter.obj"
	-@erase "$(INTDIR)\ErrorManager.obj"
	-@erase "$(INTDIR)\ICodeEmitter.obj"
	-@erase "$(INTDIR)\Instruction.obj"
	-@erase "$(INTDIR)\lexer.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\minicpp.obj"
	-@erase "$(INTDIR)\Native.obj"
	-@erase "$(INTDIR)\OutputManager.obj"
	-@erase "$(INTDIR)\Preprocessor.obj"
	-@erase "$(INTDIR)\ScopeManager.obj"
	-@erase "$(INTDIR)\SourceManager.obj"
	-@erase "$(INTDIR)\StringTable.obj"
	-@erase "$(INTDIR)\symbol.obj"
	-@erase "$(INTDIR)\SymbolTable.obj"
	-@erase "$(INTDIR)\SyntaxTree.obj"
	-@erase "$(INTDIR)\type.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\compiler.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W4 /GX /O2 /I "../" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "COMPILER_ENVIRONMENT" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\compiler.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\compiler.pdb" /machine:I386 /out:"$(OUTDIR)\compiler.exe" 
LINK32_OBJS= \
	"$(INTDIR)\CodeEmitter.obj" \
	"$(INTDIR)\ErrorManager.obj" \
	"$(INTDIR)\ICodeEmitter.obj" \
	"$(INTDIR)\Instruction.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\Native.obj" \
	"$(INTDIR)\OutputManager.obj" \
	"$(INTDIR)\Preprocessor.obj" \
	"$(INTDIR)\ScopeManager.obj" \
	"$(INTDIR)\SourceManager.obj" \
	"$(INTDIR)\StringTable.obj" \
	"$(INTDIR)\symbol.obj" \
	"$(INTDIR)\SymbolTable.obj" \
	"$(INTDIR)\SyntaxTree.obj" \
	"$(INTDIR)\type.obj" \
	"$(INTDIR)\lexer.obj" \
	"$(INTDIR)\minicpp.obj"

"$(OUTDIR)\compiler.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

OUTDIR=.\compiler___Win32_Debug
INTDIR=.\compiler___Win32_Debug
# Begin Custom Macros
OutDir=.\compiler___Win32_Debug
# End Custom Macros

ALL : "$(OUTDIR)\compiler.exe"


CLEAN :
	-@erase "$(INTDIR)\CodeEmitter.obj"
	-@erase "$(INTDIR)\ErrorManager.obj"
	-@erase "$(INTDIR)\ICodeEmitter.obj"
	-@erase "$(INTDIR)\Instruction.obj"
	-@erase "$(INTDIR)\lexer.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\minicpp.obj"
	-@erase "$(INTDIR)\Native.obj"
	-@erase "$(INTDIR)\OutputManager.obj"
	-@erase "$(INTDIR)\Preprocessor.obj"
	-@erase "$(INTDIR)\ScopeManager.obj"
	-@erase "$(INTDIR)\SourceManager.obj"
	-@erase "$(INTDIR)\StringTable.obj"
	-@erase "$(INTDIR)\symbol.obj"
	-@erase "$(INTDIR)\SymbolTable.obj"
	-@erase "$(INTDIR)\SyntaxTree.obj"
	-@erase "$(INTDIR)\type.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\compiler.exe"
	-@erase "$(OUTDIR)\compiler.ilk"
	-@erase "$(OUTDIR)\compiler.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W4 /Gm /GR /GX /ZI /Od /I "../" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "COMPILER_ENVIRONMENT" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\compiler.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\compiler.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT" /out:"$(OUTDIR)\compiler.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\CodeEmitter.obj" \
	"$(INTDIR)\ErrorManager.obj" \
	"$(INTDIR)\ICodeEmitter.obj" \
	"$(INTDIR)\Instruction.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\Native.obj" \
	"$(INTDIR)\OutputManager.obj" \
	"$(INTDIR)\Preprocessor.obj" \
	"$(INTDIR)\ScopeManager.obj" \
	"$(INTDIR)\SourceManager.obj" \
	"$(INTDIR)\StringTable.obj" \
	"$(INTDIR)\symbol.obj" \
	"$(INTDIR)\SymbolTable.obj" \
	"$(INTDIR)\SyntaxTree.obj" \
	"$(INTDIR)\type.obj" \
	"$(INTDIR)\lexer.obj" \
	"$(INTDIR)\minicpp.obj"

"$(OUTDIR)\compiler.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("compiler.dep")
!INCLUDE "compiler.dep"
!ELSE 
!MESSAGE Warning: cannot find "compiler.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "compiler - Win32 Release" || "$(CFG)" == "compiler - Win32 Debug"
SOURCE=.\CodeEmitter.cpp

"$(INTDIR)\CodeEmitter.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ErrorManager.cpp

"$(INTDIR)\ErrorManager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ICodeEmitter.cpp

"$(INTDIR)\ICodeEmitter.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\Instruction.cpp

"$(INTDIR)\Instruction.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Native.cpp

"$(INTDIR)\Native.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OutputManager.cpp

"$(INTDIR)\OutputManager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Preprocessor.cpp

"$(INTDIR)\Preprocessor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ScopeManager.cpp

"$(INTDIR)\ScopeManager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SourceManager.cpp

"$(INTDIR)\SourceManager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StringTable.cpp

"$(INTDIR)\StringTable.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\symbol.cpp

"$(INTDIR)\symbol.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SymbolTable.cpp

"$(INTDIR)\SymbolTable.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SyntaxTree.cpp

"$(INTDIR)\SyntaxTree.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\type.cpp

"$(INTDIR)\type.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lexer.cpp

"$(INTDIR)\lexer.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\minicpp.cpp

"$(INTDIR)\minicpp.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

