# smartpqi
Microchip PQI Linux Driver 

Version 2.1.12-055 (August 2021)
  - Fixed an issue where duplicate device nodes for Ultrium tape drive and
    medium changer are being created.
  - Fixed an issue where in some situations when the driver takes the
    controller offline, a kernel crash can occur.
  - Fixed an issue where OS boot may fail during logical volume rebuild.
  - Fixed an issue where using sysfs to temporarily remove a device does not
    work.
  - Fixed an issue where during system hibernation, driver frees all the irqs, 
    disables MSIx interrupts and requests legacy INTx interrupt. When driver 
    invokes request_irq(), OS returns?EINVAL.
  - Fixed an issue with request leakage, performance drop, and system crash.
    The issue happens in a max configuration where heavy I/O load is exercised
    with occasional LUN resets on the exposed devices.

Version 2.1.12-025 (July 2021)
  - Due to a change in the SCSI mid-layer, some Linux distributions may take a 
    long time to come up if the system is rebooted while a hard disk(s) is 
    being sanitized. Fixed.
  - Code clean up.
  - Updated kernel compatibility.

Version 2.1.10-020 (June 2021)
  - Enabled multi-queue support for Red Hat Enterprise Linux 8 and 
    SUSE Linux Enterprise Server 15.
  - Corrected an issue where the interrupt service routine could access
    uninitialized data.
  - Corrected an sg_reset issue that could oops the kernel.
  - Corrected a request leakage issue to avoid a potential server crash on reboot.
  - Changed method of detecting RAID bypass requests that should be retried.
  - Device ID updates.
  - Updated kernel compatibility.

Version 2.1.8-040 (February 2021)
  - Support for new Tri-mode controllers.
  - Fixed a firmware ASSERT issue when scsi-mid-layer sends requests that
    exceeded the exposed host queue depth.
  - Fixed an issue where I/O requests to the disk were blocked before
    Synchronize Cache requests are issued.
  - Fixed an issue where IOBypass read I/O requests were failing.
  - Updated kernel compatibility.

Version 1.2.16-045 (December 2020)
  - Kernel compatibility updates for later 5.X kernels.

Version 1.2.16-040 (October 2020)
  - Kernel compatibility updates.
  - Device ID updates
  - Added smartpqi man page.
  - Return busy indication for pass-through IOCTLs now only during "quiesce"
    stage of OFA.
  - Corrected an issue where the enclosure identifier in sysfs could show
    up empty.
  - PHY ID support added for the physical drives on multipath configurations.
  - Export valid initiator_port_protocols and target_port_protocols
    based on targets to sysfs.
  - Corrected timing of release of QRM memory during OFA
  - Driver is now aware of devices being deleted via sysfs.
  - Driver now handles invalid responses from the controller by taking the
    controller offline.
  - Improved driver synchronization code related to LUN reset, system shutdown,
    system suspend, system hibernate, OFA and controller offline.
  - Driver now identifies volume size expansion and notifies the OS to rescan
    logical volume.
  - Changed method of identifing physical devices.


Version 1.2.14-[015-017] (June 2020)
  - Kernel compatibility updates.

Version 1.2.14-010 (April 2020)
  - Code clean up and kernel compatibility updates.

Version 1.2.12-025 (March 2020)
  - New module parameter to limit LV xfer to 1MB for rotating media.
    Can improve performance for logical volumes with non-SDD disks.
    limit_xfer_size_to_1MB={0|1}
    Default is 0, no limit.  Set to 1 to enable 1MB max.
  - Correct SME issue during driver load by setting DMA mask and coherent
    DMA mask.
  - Code clean up and kernel compatibility updates.

Version 1.2.10-027 (January 2019)
  - No driver code change. Just new OS binary packages released. 

Version 1.2.10-025 (October 2019)
  - Corrected unique ID discovery method.
  - Tuned TMF timeout from 60 down to 30 seconds.
  - Added support for a timeout on LUN resets.

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
storagedev@microchip.com.

License: GPLv2

June 2020

