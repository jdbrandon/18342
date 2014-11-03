KERNEL = $(KDIR)/kernel
KSTART = $(KDIR)/start.o

# All core kernel objects go here.  Add objects here if you need to.
KOBJS := test.o exit_user.o user_mode.o swi_handler.o irq_handler.o assert.o raise.o c_irq.o c_swi.o main.o
KOBJS := $(KOBJS:%=$(KDIR)/%)
ALL_OBJS += $(KOBJS) $(KSTART)
ALL_CLOBBERS += $(KERNEL) $(KERNEL).bin

-include $(KDIR)/arm/kernel.mk

# Put everything needed by the kernel into the final binary.
# KOBJS contains core kernel objects.
# AOBJS contains objects that are ARM dependent.
# UOBJS contains objects that are U-boot dependent.

$(KERNEL): $(KSTART) $(KOBJS) $(AOBJS) $(UOBJS)
	@echo LD $(notdir $@)
	@$(LD) -static $(LDFLAGS) -o $@ $^ $(LIBGCC)

