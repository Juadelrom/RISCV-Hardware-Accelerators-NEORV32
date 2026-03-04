# Modify this variable to fit your NEORV32 setup (neorv32 home folder)
NEORV32_HOME ?= ../../..
MARCH ?= rv32im_zicsr_zifencei
USER_FLAGS += -ggdb -gdwarf-3
EFFORT = -Os
# Asignación Códigos Fuente para Medida Ciclos de Reloj
APP_SRC = main.c neorv32_aes128_acelerador.c
USER_FLAGS += -Wl,--defsym,__neorv32_rom_size=16k
USER_FLAGS += -Wl,--defsym,__neorv32_ram_size=16k
#Nuevo tiempo de simulación -> 100 ms
#GHDL_RUN_FLAGS ?= --stop-time=100ms

include $(NEORV32_HOME)/sw/common/common.mk


