

ALL : smc

CLEAN :
	-@erase "CodeEmitter.o"
	-@erase "ErrorManager.o"
	-@erase "ICodeEmitter.o"
	-@erase "Instruction.o"
	-@erase "lexer.o"
	-@erase "main.o"
	-@erase "minicpp.o"
	-@erase "Native.o"
	-@erase "OutputManager.o"
	-@erase "Preprocessor.o"
	-@erase "ScopeManager.o"
	-@erase "SourceManager.o"
	-@erase "StringTable.o"
	-@erase "symbol.o"
	-@erase "SymbolTable.o"
	-@erase "SyntaxTree.o"
	-@erase "type.o"
	-@erase "smc"


CPP=g++
CPP_PROJ= -Wall -O2 -I "../" -D "NDEBUG" -D "COMPILER_ENVIRONMENT" -c 

LINK32=g++
LINK32_FLAGS= -o smc 
LINK32_OBJS= \
	CodeEmitter.o \
	ErrorManager.o \
	ICodeEmitter.o \
	Instruction.o \
	main.o \
	Native.o \
	OutputManager.o \
	Preprocessor.o \
	ScopeManager.o \
	SourceManager.o \
	StringTable.o \
	symbol.o \
	SymbolTable.o \
	SyntaxTree.o \
	type.o \
	lexer.o \
	minicpp.o

.SUFFIXES  = .o .cpp

type.o:
	$(CPP) $(CPP_PROJ) ../type.cpp -o type.o
	
Instruction.o:
	$(CPP) $(CPP_PROJ) ../Instruction.cpp -o Instruction.o

.cpp.o:
	$(CPP) $(CPP_PROJ) $< -o $@

smc : $(OUTDIR) $(LINK32_OBJS)
	$(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)


