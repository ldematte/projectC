

ALL : vm

CLEAN :
	rm -f *o
	rm -f vm
	rm -f core*


CPP=g++
CPP_PROJ= -Wall -O2 -I "../" -D "NDEBUG" -c 

LINK32=g++
LINK32_FLAGS= -o smc 
LINK32_OBJS= \
	ClassTable.o  \
	main.o \
	VirtualMachine.o \
	CodeLoader.o \
	RTStringTable.o \
	Exports.cpp \
	Operand.cpp \
	RTSysExportTable.o \
	type.o \
	Instruction.o \
	NativeCall.o

.SUFFIXES  = .o .cpp

type.o:
	$(CPP) $(CPP_PROJ) ../type.cpp -o type.o
	
Instruction.o:
	$(CPP) $(CPP_PROJ) ../Instruction.cpp -o Instruction.o

.cpp.o:
	$(CPP) $(CPP_PROJ) $< -o $@

vm : $(OUTDIR) $(LINK32_OBJS)
	$(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)


