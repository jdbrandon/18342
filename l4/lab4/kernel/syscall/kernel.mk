SYSCALL_OBJS := io.o proc.o time.o swi_handler.o c_swi.o
SYSCALL_OBJS := $(SYSCALL_OBJS:%=$(KDIR)/syscall/%)

KOBJS += $(SYSCALL_OBJS)
