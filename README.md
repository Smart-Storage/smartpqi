# smartpqi
Microchip PQI Linux Driver 

Version 2.1.22-040 (March 2023)
 - Added support for NCQ priority in the RAID path. Enable the NCQ priority
   feature for the physical device in the RAID path when IOBypass is disabled.

 - Fixed an issue where drivers are not sending priority bit to SATA SMR drives.
   - Root Cause: The driver did not support this functionality on this type of
     device.
   - Fix: Enable NCQ priority feature for a physical SATA SMR device in the
     RAID path.
   - Risk: Low

 - Fixed an issue of sending a command to physical devices during device
   discovery.
   - Root Cause: The smartPQI driver was sending a SCSI TEST UNIT READY command
     to physical devices during device discovery. The driver's device discovery
     thread hung if a physical device failed to complete this command.
   - Fix: The driver no longer sends a SCSI TEST UNIT READY command to physical
     devices during device discovery. It uses a different method to detect
     sanitize/erase in progress.
   - Risk: Low

 - Fixed an issue where the OS crashes on aarch64 servers using the out-of-box
   driver during driver initialization.
   - Root Cause: The driver attempts to communicate with the controller
     firmware using a writew() kernel call to a byte aligned address. This 
     works on x86_64 servers but fails on aarch64 systems.
   - Fix: Change the writew() to two writeb() calls.
   - Risk: Medium

 - Fixed an issue where the OS crashes when a drive is hot removed during I/O
   stress test.
   - Root Cause: An I/O request pointer can be invalid if the block layer
     provides incorrect multi-queue host tag. This can lead to invalid I/O
     request pointer deference.
   - Fix: Validate the block layer provided host tag and handle the I/O request
     pointer properly if an invalid host tag is received.
   - Risk: Low

 - Fixed an issue with the driver not mapping full length of PCI BAR 0.
   - Root Cause: The driver is only mapping the controller registers up to and
     including the PQI standard registers.
   - Fix: Update the length to include the full size of PCI BAR.
   - Risk: Low

Version 2.1.20-035 (November 2022)
 - Switched to using "blk-mq" tags instead of linear searching.

 - Fixed an issue where the maximum LUN number supported by SmartPQI is not
   set correctly.
   - Root Cause: When multi-actuator support was added to SmartPQI, the
     maximum number of LUNs supported by SmartPQI was supposed to be changed
     from unlimited to 256, but the setting was inadvertently left at
     unlimited.
   - Fix: The maximum LUN number supported by SmartPQI is now set correctly
     to 256.
   - Risk: Low

 - Fixed an issue where Linux performance drops when large CPU affinity is
   used.
   - Root Cause: The driver was using a single hint variable in the function
     that gets a free I/O request element from the I/O request pool that was
     causing contention when it was utilized by a large number of threads.

 - Fix: Eliminate the initial contention by removing the hint and instead
   assign each CPU its own starting point within the request element array
   based on its CPU number.
   - Risk: Low

 - Fixed an issue to update hardware queue mapping when ?block-mq? is not
   enabled or supported.
   - Root Cause: No mapping for CPUs exceeding the maximum queue group count.
   - Fix: Updated the mapping algorithm to provide a valid mapping for all
     CPUs.
   - Risk: Low

 - Fixed an issue where "blk-mq" and managed interrupts support are not
   enabled by default for 5.x Linux kernels.
   - Root Cause: The appropriate definitions are not enabled in the build
     files.
   - Fix: Enable the appropriate flags for 5.x Linux kernels.
   - Risk: Low

 - Fixed a problem where the driver does not issue flush cache to physical
   drives during PCIe hot remove.
   - Root Cause: During controller PCIe graceful hot remove, the driver does
     not send commands to the drives to flush the cache.
   - Fix: Add Graceful Removal state check in remove path to allow flush
     cache to be issued to the physical drives.
   - Risk: Low

 - In some situations, the presence of a multi-actuator drive could cause no
   drives to be listed for a controller, during OS installation. The driver
   can also hit an unrecoverable Call trace during rmmod.
   - Root Cause: The pqi_slave_destroy routine is called multiple times for
     a multi-LUN device that causes a Call trace.
   - Fix: Remove device only upon the last pqi_slave_destroy call.
   - Risk: Low

Version 2.1.18-045 (August 2022)
 - Added support for Multi-Actuator disk drives.

 - Added support for displaying controller firmware version in the OS message
   log. The controller firmware version is printed to OS message log during
   driver initialization.

 - Added support for a controller ready timeout module parameter 
   (ctrl_ready_timeout). The valid range is 0 or 30-1800 seconds. The default
   value is 0, which causes the driver to use a timeout of 180 seconds
   (3 minutes).

 - Added support for deleting a LUN via sysfs using the following syntax:
   echo 1 > /sys/block/sdX/device/delete

 - Added module parameter to disable managed interrupts
   (disable_managed_interrupts=1).

 - Fixed a race condition where the driver can access the RAID map when using
   IOBypass during a RAID configuration change.
   - Root cause: A race condition in the driver might cause it to access a
     stale RAID map when a logical drive is reconfigured.
   - Fix: Modified the driver logic to
     - Invalidate a RAID map at an early stage when a RAID configuration
       change is detected.
     - Switch to a new RAID map only after the driver detects that the RAID
       map has changed.

 - Fixed an issue where the sg_map tool issues SCSI READ BLOCK LIMITS (0x5)
   command and the firmware never completes it, causing a system call trace
   and sg_map hang.
   - Root cause: Driver is sending an incorrect data direction flag for the
     RAID path request.
   - Fix: Corrected the data direction flag for the RAID path request.

 - Fixed an issue where PQI Reset might fail with an error "- 6" if
   firmware takes more than 100 ms to complete Reset.
   - Root cause: Method used by the driver to detect controller firmware
     crash during PQI Reset was incorrect in some cases.
   - Fix: Changed method used by the driver to detect controller firmware
     crash during PQI Reset.

 - Kernel compatibility updates.

Version 2.1.16-030 (February 2022)
 - Fixed an issue where the removal of a drive from the OS could be delayed up
   to 30 seconds after being physically pulled.
   - Root cause: The driver was retrying a LUN reset three times even though
     the return code indicated the LUN was no longer valid. There was a 10
     seconds delay between each retry. Additionally, the rescan worker was
     scheduled to run 10 seconds after the driver received the event.
   - Fix: Check the response code returned from the LUN Reset Task Management
     function and if it indicates the LUN is not valid, do not retry. Also,
     reduced the delay of the rescan worker to 5 seconds for the event
     handler only.
   - Risk: Low
 -  Fixed an issue where the outstanding requests and accessing the SCSI
    attributes for a device post linkdown leads to a hang.
    - Root cause: Post linkdown, driver does not fail the outstanding
      requests leading to long wait time before all the I/Os eventually
      fail. Also, access to the SCSI attributes by the host applications
      lead to a system hang.
 - Fix: Sanity check added in various functions to block the host applications
   from accessing the SCSI attributes of a device when the controller goes
   offline.
   - Risk: Low
 - Fixed an issue of disabling a controller by writing to this node in
   sysfs: /sys/bus/pci/devices/<Domain>:<Bus>:<Device>.<Function>/remove
   can cause processes with outstanding I/O to the controller or that attempt
   to access device attributes via sysfs for devices exposed by the SmartPQI
   driver to appear hung.
   - Root cause: Disabling the controller using this technique can cause
     outstanding I/O requests to take a long time to fail, which can cause
     processes accessing the controller to appear hung.
 - Fix: The driver now promptly fails outstanding I/O requests and the device
   attribute accesses for a controller is removed using this technique.
   - Risk: Low
 - Fixed an issue where the Linux kernel was not creating symbolic links in
   sysfs between SATA drives and their enclosure.
   - Root cause: The driver was enabling the UNIQUE_WWID_IN_REPORT_PHYS_LUN
     PQI feature, which causes the firmware to return a WWID for SATA drives
     that is derived from a unique ID read from the SATA drive itself. The
     driver was exposing this WWID as the drive's SAS address. However, because
     this SAS address does not match the SAS address returned by an enclosure's
     SES Page 0xA data, the Linux kernel was unable to match a SATA drive with
     its enclosure.
 - Fix: The driver no longer enables the UNIQUE_WWID_IN_REPORT_PHYS_LUN PQI
   feature as it is not required.
   - Risk: Low
 - Fixed an issue where the controller boot timeout error message displays
   wrong number of seconds.
   - Root cause: A recent change to the driver inadvertently changed the units
     displayed from seconds to Linux kernel "jiffies".

Version 2.1.14-035 (November 2021)
 - Fixed an issue of driver spin down when system transitions to the Suspend
   (S3) state in certain systems.
   - Root Cause: In certain system (based on PCI IDs), when the OS transitions
     the system into the Suspend
     (S3) state, the flush cache command indicates a system RESTART instead of
     SUSPEND. This avoids drive spin-down.
   - Fix: Avoid drive spin-down when system transitions to the Suspend state.
   - Risk: Medium
 - Added enable SATA NCQ priority support to sysfs. The driver needed device
   attribute sas_ncq_prio_enable for I/O utility to enable SATA NCQ priority
   support and to recognize I/O priority in SCSI command and pass priority
   information to controller firmware. This device attribute works only when
   device has NCQ priority support and the controller firmware can handle I/O
   with NCQ priority attribute.
 - Fixed an issue where logical drive size is not reflecting after expansion.
   After modifying the logical drive size, lsblk command still shows previous
   size of the logical volume.
   - Root Cause: When the driver gets any event from firmware, the driver
     schedules a rescan worker with a delay of 10 seconds. If the array
     expansion completes too quickly (in a second), the driver does not catch
     the logical drive expansion due to worker delay. Since the driver doesn't
     detect logical drive expansion, it does not call rescan device to update
     the new size of the logical drive to the OS. This causes lsblk to
     report the original size.
   - Fix: For every logical device event notification, driver rescans the
     logical drive.
   - Risk: Low
 - Fixed an issue where during kdump OS is dropping into a shell if the
     controller is in Locked-up state.
   - Root Cause: Driver issues SIS soft reset to restore the controller to SIS
     mode when OS boots into kdump mode. If the controller is in Locked-up
     state, the SIS soft reset does not work. Since the controller lockup code
     has not been cleared, the driver considers firmware is no longer up and
     running. In this case, the driver returns an error code to OS and kdump
     fails. After kdump failure, some OS distributions do not reboot cleanly
     which leads to the OS dropping into a recovery shell.
   - Fix: During kdump, driver will reboot the system if the controller is in
     Locked-up state.
   - Risk: Low
 - Fixed an issue where the logical drive creation takes longer time to expose
   logical drive.
   - Root Cause: HZ is defined as the number of times jiffies is to be
     incremented per second. If HZ=100, then it would take 0.01s to increment a
     jiffy by one. If HZ=1000, then it would take 0.001 s (1ms) to increment a
     jiffy by one. Delay of n seconds can be achieved by simply multiplying n
     with HZ.
     PQI_HZ macro is set to 1000 when HZ value is less than 1000. By default,
     PQI_HZ will result into a delay of 10 s (for kernel, which has HZ=100).
     In this case, when firmware raises an event, rescan worker will be
     scheduled after a delay of (10 x PQI_HZ) = 100 s instead of 10 s.
     Additionally, driver uses PQI_HZ at many instances, which might result in
     some other issues with respect to delay.
   - Fix: Use macro HZ for delay calculation and remove PQI_HZ.
   - Risk: Low
 - Fixed an issue where when one of the path fails during I/O and IOBypass path
   gets disabled for a multipath device, the I/O is again retried in the RAID
   path. These requests were submitted to non-existent devices in the RAID path
   and firmware responded to those requests with Illegal request and 'Invalid
   field in parameter list' sense data.
   - Root Cause: Even when the device path has gone, the driver continued
     submitting requests in RAID path and they are returned from firmware as
     Illegal requests.
   - Fix: When one of the paths is removed in dual domain, return DID_NO_CONNECT
     to SCSI mid-layer of the OS. The DID_NO_CONNECT return helps multipath to
     stop issuing Test Unit Ready and other media access commands before failing
     the path. Failing the path quickly helps routing I/O to the opposite path
     faster.
   - Risk: Low
 - Fixed an issue where the controller spins down drives during a warm boot on
   Linux.
   - Root Cause: The Linux SmartPQI driver has a callback function that the OS
     calls when the system is being shut down or being rebooted. This callback
     function calls the Flush Cache command. The command has a parameter that
     allows the driver to indicate to the firmware the reason for the flush
     cache (shutdown, hibernate, suspend, or restart). The OS callback function
     does not indicate to the driver whether it is being called for shutdown or
     warm boot, so the driver indicates to the firmware that the reason for the
     flush cache is a system shutdown. The firmware always spins down drives in
     this case.
   - Fix: The SmartPQI driver uses a Linux kernel global variable to distinguish
     between a system shutdown and a warm boot and sets the Flush Cache command
     parameter accordingly.
   - Risk: Low
 - Fixed an issue where duplicate device nodes for Ultrium tape drive and medium
   changer are being created.
   - Root Cause: The Ultrium tape drive is a multi-LUN SCSI target. It presents
     a LUN for the tape drive and a second LUN for the medium changer. The
     controller firmware lists both LUNs in the report logical LUNS results, so
     the SmartPQI driver exposes both devices to the OS. Then the OS does its
     normal device discovery through the SCSI REPORT LUNS command, which causes
     it to re-discover both devices a second time, resulting in duplicate device
     nodes.
   - Fix: When the OS re-discovers the two LUNs for the tape drive and medium
     changer, the driver recognizes that they have already been reported and
     blocks the OS from adding them a second time.
   - Risk: Low

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

August 2022

