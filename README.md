# smartpqi
Microsemi PQI Linux Driver 

Version 1.2.8-026 (August 2019)
  - More controller IDs
  - Added timestamp compatibility
  - More shutdown prep by the driver before PQI reset.
  - Kernel compat for SuSE Enterprise Linux 12 sp5.
 
Version 1.2.8-015 (July 2019)
  - Corrected CCISS_REGNEWD to return good completion once all devices
    are added to the OS.
  - Now updating box_index, phys_box_on_bus and phy_connected_dev_type
    in the pqi_scsi_dev structure.
  - PCI_VENDOR_ID update.
 
Version 1.2.6-035 (June 2019)
  - Added support for Red Hat Enterprise Linux 8 kernel compat.
  - Added support for SuSE Enterprise Linux 15 sp1 kernel compat.
  - Kernel compat updates for Kernel 5.x.
  - New controller IDs.
  - Add a module parameter “expose_ld_first” switch to the smartpqi driver
    to choose between exposing physical disks or logical volumes to the
    OS first. 
  - Add a module parameter which will hide the virtual SEP from being
    presented to the host.
  - Driver update to persistently clear IO before PQI reset.
  - Update to fully convert to the generic DMA APIProblem: Christoph Hellwig
  - Increased LUN reset timeout.
  - Increased timeout for firmware status register check.  Accounts for
    boot delays when bad drives are connected to the controller.

Version 1.2.4-065 (January 2019)
  - Added support for OFA events.
  - Added smp_utils support.
  - Better handling of LUN resets.
  - Update to spin up drives when OS resumes.
  - Enhanced numa node detection.
  - General code refactoring.

Version 1.1.4-138 (December 2018)
  - Minor update to build on kernel released in SuSE Enterprise Linux 12 SP 4

Version 1.1.4-132 (July 2018)
  - A repackaged version of this driver was release as 1.1.4-133, no driver
    code was changed from the 1.1.4-132.
  - Change made to the reading of PQI index registers for ARM64.

Version 1.1.4-125 (June 2018)
  - Improved "driver busy" handling and error checking for synchronous requests.
  - Some kernel compatibility updates.
  - PCI ID updates

Version 1.1.2-125 (November 2017)
  - PCI ID updates
  - Error message update

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
  - Move Makefile.alt to Makefile
  - dkms add -m smartpqi -v 1.0.4.101
  - dkms build -m smartpqi -v 1.0.4.101
  - dkms install -m smartpqi -v 1.0.4.101

To provide kernel/driver development feedback, send email to 
esc.storagedev@microsemi.com.

License: GPLv2

August 2018

