# SPDX-License-Identifier: GPL-2.0-or-later
#
# Makefile for the surface-shutdown driver
#

sources := dkms.conf
sources += Makefile
sources += src/Kconfig
sources += src/Makefile
sources += src/surface_shutdown.c

KVERSION ?= $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD)/src CONFIG_SURFACE_SHUTDOWN=m modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD)/src CONFIG_SURFACE_SHUTDOWN=m clean

check:
	$(KDIR)/scripts/checkpatch.pl -f -q --no-tree --strict --ignore EMBEDDED_FILENAME $(sources)

dkms-install: $(sources)
	./dkms.sh install

dkms-uninstall:
	./dkms.sh uninstall
