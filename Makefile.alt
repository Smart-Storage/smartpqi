obj-m += smartpqi.o
smartpqi-objs := smartpqi_init.o smartpqi_sis.o smartpqi_sas_transport.o smartpqi_kernel_compat.o

ifeq (${KVER},)
	KVER := $(shell echo `uname -r`)
endif
KVER_SHORT := $(shell echo $(KVER) | sed -r -e 's,(^[0-9]+\.[0-9]+).*,\1,')
KVERSION := $(shell echo $(KVER) | sed -r -e 's,(^[0-9]+)\.[0-9]+.*,\1,')
KMAJOR := $(shell echo $(KVER) | sed -r -e 's,^[0-9]+\.([0-9]+).*,\1,')

KERNEL_CLASS := $(shell \
   if [ $(KVERSION) -eq 3 ]; \
   then \
      if [ $(KMAJOR) -lt 5 ]; \
      then \
         echo "KCLASS$(KVERSION)A"; \
      elif [ $(KMAJOR) -lt 11 ]; \
      then \
         echo "KCLASS$(KVERSION)B"; \
      elif [ $(KMAJOR) -lt 19 ]; \
      then \
         echo "KCLASS$(KVERSION)C"; \
      else \
         echo "KCLASS$(KVERSION)D"; \
      fi; \
   elif [ $(KVERSION) -eq 4 ]; \
   then \
      if [ $(KMAJOR) -lt 11 ]; \
      then \
         echo "KCLASS$(KVERSION)A"; \
      elif [ $(KMAJOR) -lt 14 ]; \
      then \
         echo "KCLASS$(KVERSION)B"; \
      else \
         echo "KCLASS$(KVERSION)C"; \
      fi; \
   elif [ $(KVERSION) -eq 5 ]; \
   then \
      if [ $(KMAJOR) -lt 1 ]; \
      then \
        echo "KCLASS$(KVERSION)A"; \
      elif [ $(KMAJOR) -lt 5 ]; \
      then \
        echo "KCLASS$(KVERSION)B"; \
      elif [ $(KMAJOR) -eq 5 ]; \
      then \
        echo "KCLASS$(KVERSION)C"; \
      else \
        echo "KCLASS$(KVERSION)D"; \
      fi; \
   elif [ $(KVERSION) -eq 6 ]; \
   then \
      if [ $(KMAJOR) -lt 1 ]; \
      then \
        echo "KCLASS$(KVERSION)A"; \
      else \
        echo "KCLASS$(KVERSION)B"; \
      fi; \
   fi; \
)

OSVENDOR := $(shell \
   if [ -f /etc/redhat-release ]; \
   then \
      if [ -n "`egrep 'Plow|release 9' /etc/redhat-release`" -a \
           $(KVER_SHORT) = "5.14" ]; \
      then \
         UPDATE=`grep "release 9" /etc/redhat-release | \
            sed -r -e 's,^.* release [0-9]+.([0-9]+).*,\1,'`; \
         echo "RHEL9U$${UPDATE}"; \
      elif [ -n "`egrep 'Ootpa|release 8' /etc/redhat-release`" -a \
           $(KVER_SHORT) = "4.18" ]; \
      then \
         UPDATE=`grep "release 8" /etc/redhat-release | \
            sed -r -e 's,^.* release [0-9]+.([0-9]+).*,\1,'`; \
         echo "RHEL8U$${UPDATE}"; \
      elif [ -n "`grep 'AltArch' /etc/redhat-release | grep 'release 7'`" -a \
           $(KVER_SHORT) = "4.18" ]; \
      then \
         echo "CENTOS7ALTARM"; \
      elif [ -n "`egrep 'Maipo|release 7' /etc/redhat-release`" -a \
           $(KVER_SHORT) = "3.10" ]; \
      then \
         UPDATE=`grep "release 7" /etc/redhat-release | \
            sed -r -e 's,^.* release [0-9]+.([0-9]+).*,\1,'`; \
         echo "RHEL7U$${UPDATE}"; \
      elif [ -n "`egrep 'Santiago|release 6' /etc/redhat-release`" -a \
             $(KVER_SHORT) = "2.6" ]; \
      then \
         UPDATE=`cat /etc/redhat-release | \
            sed -r -e 's,^.* release [0-9]+.([0-9]+).*,\1,'`; \
         echo "RHEL6U$${UPDATE}"; \
      else \
         echo "KERNEL"; \
      fi; \
   elif [ -f /etc/SuSE-release ]; \
   then \
      if [ -n "`grep 'Enterprise Server 12' /etc/SuSE-release`" -a \
           $(KVER_SHORT) = "3.12" -o $(KVER_SHORT) = "4.12" ]; \
      then \
         PATCHLEVEL=`grep PATCHLEVEL /etc/SuSE-release | awk '{ print $$3 }'`; \
         echo "SLES12SP$${PATCHLEVEL}"; \
      elif [ -n "`grep 'Enterprise Server 11' /etc/SuSE-release`" -a \
             $(KVER_SHORT) = "2.6" -o $(KVER_SHORT) = "3.0" ]; \
      then \
         PATCHLEVEL=`grep PATCHLEVEL /etc/SuSE-release | awk '{ print $$3 }'`; \
         echo "SLES11SP$${PATCHLEVEL}"; \
      else \
         echo "KERNEL"; \
      fi; \
   elif [ -f /etc/openEuler-release ]; \
   then \
      if [ -n "`grep 'release 20.03' /etc/openEuler-release`" -a \
           $(KVER_SHORT) = "4.19" ]; \
      then \
         echo "OEULER2003"; \
      elif [ -n "`grep 'release 22.03' /etc/openEuler-release`" -a \
             $(KVER_SHORT) = "5.10" ]; \
      then \
         echo "OEULER2203"; \
      elif [ -n "`grep 'release 24.03' /etc/openEuler-release`" -a \
             $(KVER_SHORT) = "6.6" ]; \
      then \
         echo "OEULER2403"; \
      fi; \
   elif [ -f /etc/os-release ]; \
   then \
      if [ -n "`grep 'Enterprise Server 15' /etc/os-release`" ]; \
      then \
         SVER=`grep "^VERSION_ID=" /etc/os-release | cut -d "\"" -f 2`; \
         if [ -n "`echo $$SVER | grep "\."`" ]; \
         then \
            PATCHLEVEL=`echo $$SVER | cut -d "." -f 2`; \
         else \
            PATCHLEVEL="0"; \
         fi; \
         echo "SLES15SP$${PATCHLEVEL}"; \
      elif [ -n "`grep 'VERSION="V10 (Sword)"' /etc/os-release`" -a \
             $(KVER_SHORT) = "4.19" ]; \
      then \
         echo "K10SP2"; \
      else \
         echo "KERNEL"; \
      fi; \
   else \
      echo "KERNEL"; \
   fi; \
)

COMPAT_SWITCH := $(shell \
   if [ "$(OSVENDOR)" = "KERNEL" ]; \
   then \
      echo "EXTRA_CFLAGS+=-D$(KERNEL_CLASS)"; \
   else \
      echo "EXTRA_CFLAGS+=-D$(OSVENDOR)"; \
   fi; \
)
ifeq (${KDIR},)
   KDIR := /lib/modules/$(KVER)
endif
KBDIR := ${KDIR}/build
PWD   := $(shell pwd)

BLK := $(shell \
   GF=`find -L $(KDIR) -name blkdev.h -o -name blk-mq.h | xargs grep blk_rq_is_passthrough | tail -1`; \
   echo $$GF; \
)

MORE_CFLAGS := $(shell \
   if [ -z "$(BLK)" ]; \
   then \
      echo "EXTRA_CFLAGS+=-DKFEATURE_NEEDS_BLK_RQ_IS_PASSTHROUGH"; \
   fi; \
)

FALLTHRU := $(shell \
   GF=`find -L $(KDIR)/*/include/linux -type f -print | xargs grep '__has_attribute(__fallthrough__)'`; \
   echo $$GF; \
)

MORE_CFLAGS += $(shell \
   if [ ! -z "$(FALLTHRU)" ]; \
   then \
      echo "EXTRA_CFLAGS+=-Wno-implicit-fallthrough"; \
   fi; \
)

MORE_CFLAGS += $(shell \
   GF=`find -L $(KDIR) -name scsi_host.h | xargs grep scsi_done | tail -1`; \
   if [ -z "$$GF" ]; \
   then \
      echo -n "EXTRA_CFLAGS+=-DKFEATURE_HAS_GLOBAL_SCSI_DONE"; \
   fi; \
)

MORE_CFLAGS += $(shell \
   GF=`find -L $(KDIR) -name scsi_cmnd.h | xargs grep scsi_cmd_to_rq | tail -1`; \
   if [ ! -z "$$GF" ]; \
   then \
      echo -n "EXTRA_CFLAGS+=-DKFEATURE_HAS_SCSI_CMD_TO_RQ"; \
   fi; \
)

MORE_CFLAGS += $(shell \
   GF=`find -L $(KDIR) -name scsi_cmnd.h | xargs grep scsi_cmd_priv | tail -1`; \
   if [ ! -z "$$GF" ]; \
   then \
      echo -n "EXTRA_CFLAGS+=-DKFEATURE_HAS_SCSI_CMD_PRIV"; \
   fi; \
)

MORE_CFLAGS += $(shell \
   GF=`find -L $(KDIR) -name scsi_host.h | xargs grep shost_groups | tail -1`; \
   if [ ! -z "$$GF" ]; \
   then \
      echo -n "EXTRA_CFLAGS+=-DKFEATURE_HAS_SDEV_GROUPS"; \
   fi; \
)

MORE_CFLAGS += $(shell \
   GF=`find -L $(KDIR) -name scsi_host.h | xargs grep host_tagset | tail -1`; \
   if [ ! -z "$$GF" ]; \
   then \
      echo -n "EXTRA_CFLAGS+=-DKFEATURE_HAS_HOST_TAGSET_SUPPORT"; \
   fi; \
)

MORE_CFLAGS += $(shell \
    GF=`find -L $(KDIR) -name scsi_host.h | xargs grep 'void scsi_rescan_device' | tail -1`; \
    if [ ! -z "$$GF" ]; \
    then \
       echo -n "EXTRA_CFLAGS+=-DKFEATURE_HAS_OLDER_SCSI_RESCAN_DEVICE"; \
    fi; \
)

MORE_CFLAGS += $(shell \
    GF=`find -L $(KDIR) -path *include* -name string.h | xargs grep 'size_t strlcpy' | tail -1`; \
    if [ ! -z "$$GF" ]; \
    then \
       echo -n "EXTRA_CFLAGS+=-DKFEATURE_HAS_OLD_STRLCPY"; \
    fi; \
)

default:
	$(MAKE) -C $(KBDIR) M=$(PWD) $(COMPAT_SWITCH) $(MORE_CFLAGS) modules

clean:
	$(MAKE) -C $(KBDIR) M=$(PWD) clean

install:
	$(MAKE) -C $(KBDIR) M=$(PWD) modules_install

