FOLDER_ASSEMBLER = ./Assembler
FOLDER_PROCESSOR = ./Processor
FILE_CMD = Assembler/source/cpu_commands.txt

dir_asm = ../assembler.exe
dir_proc = ../processor.exe
compile ?= all
start ?= all

start_proc = ./processor.exe
start_asm = ./assembler.exe

dir_build_asm = Assembler/build
dir_build_proc = Processor/build

ASM = asm
PROC = proc

.PHONY = all, run


ifeq ($(compile),$(ASM))
all::
	make -C $(FOLDER_ASSEMBLER) dir=$(dir_asm)
else
ifeq ($(compile), $(PROC))
all::
	make -C $(FOLDER_PROCESSOR) dir=$(dir_proc)
else
all::
	make -C $(FOLDER_ASSEMBLER) dir=$(dir_asm)
	make -C $(FOLDER_PROCESSOR) dir=$(dir_proc)
endif
endif


clean:
	rm -rf $(dir_build_asm)
	rm -rf $(dir_build_proc)
	rm -rf $(FILE_CMD)
	rm -rf $(start_asm)
	rm -rf $(start_proc)


ifeq ($(start), $(ASM))
run::
	$(start_asm)
else
ifeq ($(start), $(PROC))
run::
	$(start_proc)
else
run::
	$(start_asm)
	$(start_proc)
endif
endif



