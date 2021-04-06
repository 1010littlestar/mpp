/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2008-2019. All rights reserved.
 * Description   : Picture queue functions declaration
 * Author        : Hisilicon multimedia software group
 * Created       : 2008/06/16
 */

#include <linux/list.h>
#include "hi_common_adapt.h"
#include "hi_comm_video_adapt.h"

#ifndef __PCIV_PIC_QUEUE_H__
#define __PCIV_PIC_QUEUE_H__

typedef struct {
    hi_video_frame_info video_frame;
    hi_mod_id           mod_id;
    hi_bool             block;
    hi_u32              index;
} pciv_pic_info;

/* the node in pci queue */
typedef struct {
    pciv_pic_info       pciv_pic;      /* info of image */
    struct list_head    list;

} pciv_pic_node;

/* PCIV queue info */
typedef struct {
    pciv_pic_node       *node_buf;    /* base address of node */

    struct list_head    free_list;
    struct list_head    busy_list;

    hi_u32              free_num;
    hi_u32              busy_num;
    hi_u32              max;
} pciv_pic_queue;

hi_s32   pciv_creat_pic_queue(pciv_pic_queue *pic_queue, hi_u32 max_num);
hi_void  pciv_destroy_pic_queue(pciv_pic_queue *node_queue);
hi_void  pciv_pic_queue_put_busy_head(pciv_pic_queue *node_queue, pciv_pic_node *pic_node);
hi_void  pciv_pic_queue_put_busy(pciv_pic_queue *node_queue, pciv_pic_node *pic_node);
pciv_pic_node *pciv_pic_queue_get_busy(pciv_pic_queue *node_queue);
pciv_pic_node *pciv_pic_queue_query_busy(pciv_pic_queue *node_queue);
pciv_pic_node *pciv_pic_queue_get_free(pciv_pic_queue *node_queue);
hi_void  pciv_pic_queue_put_free(pciv_pic_queue *node_queue, pciv_pic_node *pic_node);
hi_u32   pciv_pic_queue_get_free_num(pciv_pic_queue *node_queue);
hi_u32   pciv_pic_queue_get_busy_num(pciv_pic_queue *node_queue);

#endif
