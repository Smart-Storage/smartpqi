/*
 *    driver for PMC-Sierra PQI-based storage controllers
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

#if !defined(_SMARTPQI_KERNEL_COMPAT_H)
#define _SMARTPQI_KERNEL_COMPAT_H

/* #define RHEL6 */
/* #define RHEL7 */
/* default is kernel.org */

/* ----- RHEL6 variants --------- */
#if \
	defined(RHEL6U0) || \
	defined(RHEL6U1) || \
	defined(RHEL6U2) || \
	defined(RHEL6U3) || \
	defined(RHEL6U4) || \
	defined(RHEL6U5) || \
	defined(RHEL6U6) || \
	defined(RHEL6U7) || \
	defined(RHEL6U8) || \
	defined(RHEL6U9)
#define RHEL6
#endif

/* ----- RHEL7 variants --------- */
#if \
	defined(RHEL7U0)    || \
	defined(RHEL7U1)    || \
	defined(RHEL7U2)    || \
	defined(RHEL7U4)    || \
	defined(RHEL7U4ARM) || \
	defined(RHEL7U5)    || \
	defined(RHEL7U5ARM)
#define RHEL7
#endif

/* ----- SLES11 variants --------- */
#if \
	defined(SLES11SP0) || \
	defined(SLES11SP1) || \
	defined(SLES11SP2) || \
	defined(SLES11SP3) || \
	defined(SLES11SP4)
#define SLES11
#endif

/* ----- SLES12 variants --------- */
#if \
	defined(SLES12SP0) || \
	defined(SLES12SP1) || \
	defined(SLES12SP2)
#define SLES12
#endif

/* ----- SLES15 variants --------- */
#if \
	defined(SLES15SP0) || \
	defined(SLES15SP1) || \
	defined(SLES15SP2) || \
	defined(SLES15SP3) || \
	defined(SLES15SP4)
#define SLES15
#endif

#include <scsi/scsi_tcq.h>
#if defined(MSG_SIMPLE_TAG)
#define KFEATURE_HAS_SCSI_CHANGE_QUEUE_DEPTH		0
#if !defined(RHEL7U3)
#define KFEATURE_HAS_MQ_SUPPORT				0
#endif
#endif

#if !defined(PCI_EXP_DEVCTL2_COMP_TIMEOUT)
#define PCI_EXP_DEVCTL2_COMP_TIMEOUT	0x000f
#if TORTUGA
#define KFEATURE_HAS_PCIE_CAPABILITY_SUPPORT		1
#else
#define KFEATURE_HAS_PCIE_CAPABILITY_SUPPORT		0
#endif
#endif

#if defined(RHEL6)
#define KFEATURE_HAS_WAIT_FOR_COMPLETION_IO		0
#define KFEATURE_HAS_2011_03_QUEUECOMMAND		0
#if defined(RHEL6U3) || defined(RHEL6U4) || defined(RHEL6U5)
#if defined(RHEL6U3)
#define KFEATURE_HAS_DMA_ZALLOC_COHERENT		0
#endif
#define KFEATURE_HAS_PCI_ENABLE_MSIX_RANGE		0
#endif
#elif defined(RHEL7)
#if defined(RHEL7U0)
#define KFEATURE_HAS_PCI_ENABLE_MSIX_RANGE		0
#elif defined(RHEL7U4ARM) || defined(RHEL7U5ARM)
#define KFEATURE_HAS_BLK_RQ_IS_PASSTHROUGH		1
#endif
#elif defined(SLES11)
#if defined(SLES11SP0) || defined(SLES11SP1)
#define KFEATURE_HAS_WAIT_FOR_COMPLETION_IO		0
#define KFEATURE_HAS_2011_03_QUEUECOMMAND		0
#endif
#if defined(SLES11SP3)
#define KFEATURE_HAS_WAIT_FOR_COMPLETION_IO		0
#define KFEATURE_HAS_DMA_ZALLOC_COHERENT		0
#define KFEATURE_HAS_PCI_ENABLE_MSIX_RANGE		0
#endif
#if defined(SLES11SP4)
#define KFEATURE_HAS_WAIT_FOR_COMPLETION_IO		0
#endif
#elif defined(SLES12)
#if defined(SLES12SP0)
#define KFEATURE_HAS_PCI_ENABLE_MSIX_RANGE		0
#endif
#elif defined(SLES15)
#define KFEATURE_HAS_BLK_RQ_IS_PASSTHROUGH		1
#elif defined(UBUNTU1404) || TORTUGA || defined(KCLASS3C)
#define KFEATURE_HAS_PCI_ENABLE_MSIX_RANGE		0
#elif defined(OL7U2) || defined(KCLASS3B)
#define KFEATURE_HAS_WAIT_FOR_COMPLETION_IO		0
#endif
#if defined(KCLASS4B)
#define KFEATURE_HAS_BLK_RQ_IS_PASSTHROUGH		1
#endif

#define KFEATURE_HAS_SCSI_SANITIZE_INQUIRY_STRING	0

/* default values */
#if !defined(KFEATURE_HAS_WAIT_FOR_COMPLETION_IO)
#define KFEATURE_HAS_WAIT_FOR_COMPLETION_IO		1
#endif
#if !defined(KFEATURE_HAS_2011_03_QUEUECOMMAND)
#define KFEATURE_HAS_2011_03_QUEUECOMMAND		1
#endif
#if !defined(KFEATURE_HAS_DMA_ZALLOC_COHERENT)
#define KFEATURE_HAS_DMA_ZALLOC_COHERENT		1
#endif
#if !defined(KFEATURE_HAS_PCI_ENABLE_MSIX_RANGE)
#define KFEATURE_HAS_PCI_ENABLE_MSIX_RANGE		1
#endif
#if !defined(KFEATURE_HAS_SCSI_CHANGE_QUEUE_DEPTH)
#define KFEATURE_HAS_SCSI_CHANGE_QUEUE_DEPTH		1
#endif
#if !defined(KFEATURE_HAS_MQ_SUPPORT)
#define KFEATURE_HAS_MQ_SUPPORT				1
#endif
#if !defined(KFEATURE_HAS_SCSI_SANITIZE_INQUIRY_STRING)
#define KFEATURE_HAS_SCSI_SANITIZE_INQUIRY_STRING	1
#endif
#if !defined(KFEATURE_HAS_PCIE_CAPABILITY_SUPPORT)
#define KFEATURE_HAS_PCIE_CAPABILITY_SUPPORT		1
#endif
#if !defined(KFEATURE_HAS_BLK_RQ_IS_PASSTHROUGH)
#define KFEATURE_HAS_BLK_RQ_IS_PASSTHROUGH		0
#endif

#if !defined(list_next_entry)
#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, typeof(*(pos)), member)
#endif

#if !defined(list_first_entry_or_null)
#define list_first_entry_or_null(ptr, type, member) \
	(!list_empty(ptr) ? list_first_entry(ptr, type, member) : NULL)
#endif

#if !defined(TYPE_ZBC)
#define TYPE_ZBC	0x14
#endif

#if !defined(readq)
#define readq readq
static inline u64 readq(const volatile void __iomem *addr)
{
	u32 lower32;
	u32 upper32;

	lower32 = readl(addr);
	upper32 = readl(addr + 4);

	return ((u64)upper32 << 32) | lower32;
}
#endif

#if !defined(writeq)
#define writeq writeq
static inline void writeq(u64 value, volatile void __iomem *addr)
{
	u32 lower32;
	u32 upper32;

	lower32 = lower_32_bits(value);
	upper32 = upper_32_bits(value);

	writel(lower32, addr);
	writel(upper32, addr + 4);
}
#endif

#if !defined(PCI_DEVICE_SUB)
#define PCI_DEVICE_SUB(vend, dev, subvend, subdev) \
	.vendor = (vend), .device = (dev), \
	.subvendor = (subvend), .subdevice = (subdev)
#endif

#if !defined(PCI_VENDOR_ID_ADVANTECH)
#define PCI_VENDOR_ID_ADVANTECH		0x13fe
#endif

void pqi_compat_init_scsi_host_template(struct scsi_host_template *template);
void pqi_compat_init_scsi_host(struct Scsi_Host *shost,
	struct pqi_ctrl_info *ctrl_info);

#if !KFEATURE_HAS_WAIT_FOR_COMPLETION_IO

static inline unsigned long wait_for_completion_io_timeout(struct completion *x,
	unsigned long timeout)
{
	return wait_for_completion_timeout(x, timeout);
}

static inline unsigned long wait_for_completion_io(struct completion *x)
{
	wait_for_completion(x);
	return 0;
}

#endif	/* !KFEATURE_HAS_WAIT_FOR_COMPLETION_IO */

#if KFEATURE_HAS_2011_03_QUEUECOMMAND

#define PQI_SCSI_QUEUE_COMMAND		pqi_scsi_queue_command

static inline void pqi_scsi_done(struct scsi_cmnd *scmd)
{
	pqi_prep_for_scsi_done(scmd);
	scmd->scsi_done(scmd);
}

#else

int pqi_scsi_queue_command_compat(struct scsi_cmnd *scmd,
	void (*done)(struct scsi_cmnd *));

#define PQI_SCSI_QUEUE_COMMAND		pqi_scsi_queue_command_compat

static inline void pqi_scsi_done(struct scsi_cmnd *scmd)
{
	void (*scsi_done)(struct scsi_cmnd *);

	pqi_prep_for_scsi_done(scmd);

	scsi_done = (void(*)(struct scsi_cmnd *))scmd->SCp.ptr;
	scsi_done(scmd);
}

#endif	/* KFEATURE_HAS_2011_03_QUEUECOMMAND */

#if !KFEATURE_HAS_DMA_ZALLOC_COHERENT

static inline void *dma_zalloc_coherent(struct device *dev, size_t size,
	dma_addr_t *dma_handle, gfp_t flag)
{
	void *ret = dma_alloc_coherent(dev, size, dma_handle,
		flag | __GFP_ZERO);
	return ret;
}

#endif	/* !KFEATURE_HAS_DMA_ZALLOC_COHERENT */

#if !KFEATURE_HAS_PCI_ENABLE_MSIX_RANGE

int pci_enable_msix_range(struct pci_dev *pci_dev, struct msix_entry *entries,
	int minvec, int maxvec);

#endif	/* !KFEATURE_HAS_PCI_ENABLE_MSIX_RANGE */

#if !KFEATURE_HAS_SCSI_CHANGE_QUEUE_DEPTH

int scsi_change_queue_depth(struct scsi_device *sdev, int queue_depth);

#endif	/* !KFEATURE_HAS_SCSI_CHANGE_QUEUE_DEPTH */

#if !KFEATURE_HAS_SCSI_SANITIZE_INQUIRY_STRING

void scsi_sanitize_inquiry_string(unsigned char *s, int len);

#endif	/* !KFEATURE_HAS_SCSI_SANITIZE_INQUIRY_STRING */

#if !KFEATURE_HAS_PCIE_CAPABILITY_SUPPORT

#define PCI_EXP_DEVCTL2			40	/* Device Control 2 */

int pcie_capability_clear_and_set_word(struct pci_dev *dev, int pos,
	u16 clear, u16 set);

#endif	/* !KFEATURE_HAS_PCIE_CAPABILITY_SUPPORT */

static inline u16 pqi_get_hw_queue(struct pqi_ctrl_info *ctrl_info,
	struct scsi_cmnd *scmd)
{
	u16 hw_queue;

#if KFEATURE_HAS_MQ_SUPPORT
	hw_queue = blk_mq_unique_tag_to_hwq(blk_mq_unique_tag(scmd->request));
#else
	hw_queue = smp_processor_id();
#endif
	if (hw_queue > ctrl_info->max_hw_queue_index)
		hw_queue = 0;

	return hw_queue;
}

#if !KFEATURE_HAS_BLK_RQ_IS_PASSTHROUGH

static inline bool blk_rq_is_passthrough(struct request *rq)
{
	return rq->cmd_type != REQ_TYPE_FS;
}

#endif	/* !KFEATURE_HAS_BLK_RQ_IS_PASSTHROUGH */

#endif	/* _SMARTPQI_KERNEL_COMPAT_H */
