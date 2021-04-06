/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2008-2019. All rights reserved.
 * Description: pciv trans functions declaration
 * Author: Hisilicon multimedia software group
 * Create: 2011/06/28
 */

#include <linux/ioctl.h>

#ifndef __PCI_TRANS_H__
#define __PCI_TRANS_H__

#define HI_PCIT_MAX_BUS     2
#define HI_PCIT_MAX_SLOT    5

#define HI_PCIT_DMA_READ    0
#define HI_PCIT_DMA_WRITE   1

#define HI_PCIT_HOST_BUSID  0
#define HI_PCIT_HOST_SLOTID 0

#define HI_PCIT_NOSLOT    (-1)

#define HI_PCIT_INVAL_SUBSCRIBER_ID (-1)

#define HI_PCIT_DEV2BUS(dev)  ((dev) >> 16)
#define HI_PCIT_DEV2SLOT(dev) ((dev) & 0xff)
#define HI_PCIT_MKDEV(bus, slot) ((slot) | ((bus) << 16))

struct pcit_dma_req {
    hi_u32 bus;   /* bus and slot will be ignored on device */
    hi_u32 slot;
    hi_ulong dest;
    hi_ulong source;
    hi_u32 len;
};

struct pcit_dev_cfg {
    hi_u32 slot;
    hi_u16 vendor_id;
    hi_u16 device_id;

    hi_ulong np_phys_addr;
    hi_ulong np_size;

    hi_ulong pf_phys_addr;
    hi_ulong pf_size;

    hi_ulong cfg_phys_addr;
    hi_ulong cfg_size;
};

struct pcit_bus_dev {
    hi_u32 bus_nr; /* input argument */
    struct pcit_dev_cfg devs[HI_PCIT_MAX_SLOT];
};

#define HI_PCIT_EVENT_DOORBELL_0 0
#define HI_PCIT_EVENT_DOORBELL_1 1
#define HI_PCIT_EVENT_DOORBELL_2 2
#define HI_PCIT_EVENT_DOORBELL_3 3
#define HI_PCIT_EVENT_DOORBELL_4 4
#define HI_PCIT_EVENT_DOORBELL_5 5
#define HI_PCIT_EVENT_DOORBELL_6 6
#define HI_PCIT_EVENT_DOORBELL_7 7
#define HI_PCIT_EVENT_DOORBELL_8 8
#define HI_PCIT_EVENT_DOORBELL_9 9
#define HI_PCIT_EVENT_DOORBELL_10 10
#define HI_PCIT_EVENT_DOORBELL_11 11
#define HI_PCIT_EVENT_DOORBELL_12 12
#define HI_PCIT_EVENT_DOORBELL_13 13
#define HI_PCIT_EVENT_DOORBELL_14 14
#define HI_PCIT_EVENT_DOORBELL_15 15 // reserved by mmc


#define HI_PCIT_EVENT_DMARD_0 16
#define HI_PCIT_EVENT_DMAWR_0 17

#define HI_PCIT_PCI_NP  1
#define HI_PCIT_PCI_PF  2
#define HI_PCIT_PCI_CFG 3
#define HI_PCIT_AHB_PF  4


struct pcit_event {
    hi_u32      event_mask;
    hi_ulong    pts;
};

#define    HI_IOC_PCIT_BASE    'H'

/* Only used in host, you can get information of all devices on each bus. */
#define    HI_IOC_PCIT_INQUIRE _IOR(HI_IOC_PCIT_BASE, 1, struct pcit_bus_dev)

/* Only used in device, these tow command will block until DMA compeleted. */
#define    HI_IOC_PCIT_DMARD  _IOW(HI_IOC_PCIT_BASE, 2, struct pcit_dma_req)
#define    HI_IOC_PCIT_DMAWR  _IOW(HI_IOC_PCIT_BASE, 3, struct pcit_dma_req)

/* Only used in host, you can bind a fd returned by open() to a device,
 * then all operation by this fd is orient to this device.
 */
#define    HI_IOC_PCIT_BINDDEV  _IOW(HI_IOC_PCIT_BASE, 4, int)

/* Used in host and device.
 * on host, you should specify which device doorbell will be send to
 * by the parameter, but in device, the parameters will be ingored,and
 * a doorbell will be send to host.
 */
#define    HI_IOC_PCIT_DOORBELL  _IOW(HI_IOC_PCIT_BASE, 5, int)

/* Used in host and device.
 * you can subscribe more than one event using this command.
 * on host, fd passed to ioctl() indicates target device. so you should
 * use "HI_IOC_PCIT_BINDDEV" bind a fd first, otherwise an error will
 * be met. but on device, all events are triggered by host, so it NOT
 * needed to bind a fd.
 */
#define    HI_IOC_PCIT_SUBSCRIBE  _IOW(HI_IOC_PCIT_BASE, 6, int)

/* If nscribled all events, diriver will return NONE event to all listeners. */
#define    HI_IOC_PCIT_UNSUBSCRIBE  _IOW(HI_IOC_PCIT_BASE, 7, int)

/* On host, this command will listen the device specified by parameter. */
#define    HI_IOC_PCIT_LISTEN  _IOR(HI_IOC_PCIT_BASE, 8, struct pcit_event)


#ifdef __KERNEL__
#include <linux/list.h>

struct pcit_dma_task {
    struct list_head list;   /* internal data, don't touch */

    hi_u32      state;      /* internal data, don't touch. 0: todo, 1: doing, 2: finished */
    hi_u32      dir; /* read or write */
    hi_ulong    src;
    hi_ulong    dest;
    hi_u32      len;
    hi_void        *private_data;
    hi_void        (*finish)(struct pcit_dma_task *task);
};

/* only used in device */
hi_u32 pcit_create_task(struct pcit_dma_task *task);

/* only used in host */
struct pcit_subscriber {
    hi_char name[16];
    hi_void (*notify)(hi_u32 bus, hi_u32 slot, struct pcit_event *, hi_void *);
    hi_void *data;
};

hi_u32 pcit_subscriber_register(struct pcit_subscriber *user);
hi_u32 pcit_subscriber_deregister(hi_u32 id);
hi_u32 pcit_subscribe_event(hi_u32 id, hi_u32 bus, hi_u32 slot, struct pcit_event *pevent);
hi_u32 pcit_unsubscribe_event(hi_u32 id, hi_u32 bus, hi_u32 slot, struct pcit_event *pevent);
hi_void ss_doorbell_triggle(hi_u32 addr, hi_u32 value);
hi_ulong get_pf_window_base(hi_u32 slot);

#endif

#endif

