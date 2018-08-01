/*
 *    driver for Microsemi PQI-based storage controllers
 *    Copyright (c) 2016-2017 Microsemi Corporation
 *    Copyright (c) 2016 PMC-Sierra, Inc.
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; version 2 of the License.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *    NON INFRINGEMENT.  See the GNU General Public License for more details.
 *
 *    Questions/Comments/Bugfixes to esc.storagedev@microsemi.com
 *
 */

#include <linux/pci.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_device.h>
#include "smartpqi.h"
#include "smartpqi_kernel_compat.h"

#if !KFEATURE_HAS_2011_03_QUEUECOMMAND

int pqi_scsi_queue_command_compat(struct scsi_cmnd *scmd,
	void (*done)(struct scsi_cmnd *))
{
	scmd->SCp.ptr = (char *)done;

	return pqi_scsi_queue_command(scmd->device->host, scmd);
}

#endif	/* !KFEATURE_HAS_2011_03_QUEUECOMMAND */

#if !KFEATURE_HAS_PCI_ENABLE_MSIX_RANGE

int pci_enable_msix_range(struct pci_dev *pci_dev, struct msix_entry *entries,
	int minvec, int maxvec)
{
	int nvec = maxvec;
	int rc;

	if (maxvec < minvec)
		return -ERANGE;

	do {
		rc = pci_enable_msix(pci_dev, entries, nvec);
		if (rc < 0)
			return rc;
		if (rc > 0) {
			if (rc < minvec)
				return -ENOSPC;
			nvec = rc;
		}
	} while (rc);

	return nvec;
}

#endif	/* !KFEATURE_HAS_PCI_ENABLE_MSIX_RANGE */

#if !KFEATURE_HAS_SCSI_CHANGE_QUEUE_DEPTH

int scsi_change_queue_depth(struct scsi_device *sdev, int queue_depth)
{
	scsi_adjust_queue_depth(sdev, scsi_get_tag_type(sdev), queue_depth);

	return queue_depth;
}

static int pqi_change_queue_depth(struct scsi_device *sdev, int qdepth,
	int reason)
{
	if (reason == SCSI_QDEPTH_DEFAULT || reason == SCSI_QDEPTH_RAMP_UP) {
		struct pqi_scsi_dev *device = sdev->hostdata;

		if (!device)
			return -ENODEV;

		if (qdepth < 1)
			qdepth = 1;
		else if (qdepth > device->queue_depth)
			qdepth = device->queue_depth;

		scsi_adjust_queue_depth(sdev, scsi_get_tag_type(sdev), qdepth);

	} else if (reason == SCSI_QDEPTH_QFULL)
		scsi_track_queue_full(sdev, qdepth);
	else
		return -ENOTSUPP;

	return sdev->queue_depth;
}

static int pqi_change_queue_type(struct scsi_device *sdev, int tag_type)
{
	if (sdev->tagged_supported) {
		scsi_set_tag_type(sdev, tag_type);
		if (tag_type)
			scsi_activate_tcq(sdev, sdev->queue_depth);
		else
			scsi_deactivate_tcq(sdev, sdev->queue_depth);
	} else {
		tag_type = 0;
	}

	return tag_type;
}

#endif	/* !KFEATURE_HAS_SCSI_CHANGE_QUEUE_DEPTH */

void pqi_compat_init_scsi_host_template(struct scsi_host_template *hostt)
{
#if !KFEATURE_HAS_SCSI_CHANGE_QUEUE_DEPTH
	hostt->change_queue_depth = pqi_change_queue_depth;
	hostt->change_queue_type = pqi_change_queue_type;
#endif	/* !KFEATURE_HAS_SCSI_CHANGE_QUEUE_DEPTH */
}

void pqi_compat_init_scsi_host(struct Scsi_Host *shost,
	struct pqi_ctrl_info *ctrl_info)
{
#if KFEATURE_HAS_MQ_SUPPORT
	shost->nr_hw_queues = ctrl_info->num_queue_groups;
#endif	/* KFEATURE_HAS_MQ_SUPPORT */
}

#if !KFEATURE_HAS_SCSI_SANITIZE_INQUIRY_STRING

void scsi_sanitize_inquiry_string(unsigned char *s, int len)
{
	bool terminated = false;

	for (; len > 0; (--len, ++s)) {
		if (*s == 0)
			terminated = true;
		if (terminated || *s < 0x20 || *s > 0x7e)
			*s = ' ';
	}
}

#endif	/* !KFEATURE_HAS_SCSI_SANITIZE_INQUIRY_STRING */

#if !KFEATURE_HAS_PCIE_CAPABILITY_SUPPORT

#if defined(RHEL6U3)
/*
 * Note that these accessor functions are only for the "PCI Express
 * Capability" (see PCIe spec r3.0, sec 7.8).  They do not apply to the
 * other "PCI Express Extended Capabilities" (AER, VC, ACS, MFVC, etc.)
 */
int pcie_capability_read_word(struct pci_dev *dev, int pos, u16 *val)
{
	int ret;

	*val = 0;
	if (pos & 1)
		return -EINVAL;

	ret = pci_read_config_word(dev, pci_pcie_cap(dev) + pos, val);
	/*
	 * Reset *val to 0 if pci_read_config_word() fails, it may
	 * have been written as 0xFFFF if hardware error happens
	 * during pci_read_config_word().
	 */
	if (ret)
		*val = 0;
	return ret;
}

int pcie_capability_write_word(struct pci_dev *dev, int pos, u16 val)
{
	if (pos & 1)
		return -EINVAL;

	return pci_write_config_word(dev, pci_pcie_cap(dev) + pos, val);
}

#endif /* RHEL6U3 */

int pcie_capability_clear_and_set_word(struct pci_dev *dev, int pos,
	u16 clear, u16 set)
{
	int ret;
	u16 val;

	ret = pcie_capability_read_word(dev, pos, &val);
	if (!ret) {
		val &= ~clear;
		val |= set;
		ret = pcie_capability_write_word(dev, pos, val);
	}

	return ret;
}

#endif
