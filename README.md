# smartpqi
Microsemi PQI Linux Driver 

Version 1.1.2-120 (September 2017)
  - Updates made to PQI resets.
  - PCI ID updates.
  - Enhanced BMIC cache flush support.

Version 1.0.4-101
  - Initial public driver release.


To build the driver outside of a kernel tree.
  - make -f Makefile.alt

A dkms.conf file is included in the source for use with DKMS.

DKMS helps insure a driver (like smartpqi) rebuilds for each kernel update
that happens on a system.
  - http://linux.dell.com/dkms
  - http://help.ubuntu.com/community/DKMS

Steps for using DKMS and the smartpqi driver source with Ubuntu:
  - Insure dkms and compiler tools are installed.
    - apt-get install dkms build-essential
  - Unpack smartpqi source tarball
  - copy the directory to /usr/src/smartpqi-<driver_version>
    - Note DKMS does not support the "-" in the version number. 
      Substitute with ".".
    - EX. cp -a smartpqi-1.0.4 /usr/src/smartpqi-1.0.4.101
  - dkms add -m smartpqi -v 1.0.4.101
  - dkms build -m smartpqi -v 1.0.4.101
  - dkms install -m smartpqi -v 1.0.4.101

To provide kernel/driver development feedback, send email to 
esc.storagedev@microsemi.com.

License: GPLv2

August 2017

