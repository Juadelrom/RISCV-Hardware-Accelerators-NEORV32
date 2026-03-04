# Modify this variable to fit your NEORV32 setup (neorv32 home folder)
NEORV32_HOME ?= ../../..
MARCH ?= rv32im_zicsr_zifencei
USER_FLAGS += -ggdb -gdwarf-3
EFFORT = -Os
USER_FLAGS += -Wl,--defsym,__neorv32_rom_size=16k
USER_FLAGS += -Wl,--defsym,__neorv32_ram_size=16k

include $(NEORV32_HOME)/sw/common/common.mk


