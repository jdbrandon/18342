PROGS_TEST_OBJS := typehero.o
PROGS_TEST_OBJS := $(PROGS_TEST_OBJS:%=$(TDIR)/typehero/%)
ALL_OBJS += $(PROGS_TEST_OBJS)

$(TDIR)/bin/typehero : $(TSTART) $(PROGS_TEST_OBJS) $(TLIBC)

