/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include "mpi_sys.h"
#include "hi_comm_vi.h"
#include "hi_comm_isp.h"
#include "hi_comm_3a.h"
#include "hi_ae_comm.h"
#include "hi_awb_comm.h"
#include "isp_inner.h"
#include "isp_main.h"
#include "isp_vreg.h"
#include "isp_ext_config.h"
#include "hi_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

hi_void isp_calc_grid_info(hi_u16 wdith, hi_u16 start_pox, hi_u16 block_num, hi_u16 *grid_info)
{
    hi_u16 i;
    hi_u16 integer;
    hi_u16 remainder;

    integer   = wdith / DIV_0_TO_1(block_num);
    remainder = wdith % DIV_0_TO_1(block_num);
    grid_info[0] = start_pox;
    for (i = 1; i < block_num; i++) {
        if (remainder > 0) {
            grid_info[i] =  grid_info[i - 1] + integer + 1;
            remainder = remainder - 1;
        } else {
            grid_info[i] =  grid_info[i - 1] + integer;
        }
    }

    return;
}

hi_u32 isp_get_striping_active_img_start(hi_u8 block_index, isp_working_mode *isp_work_mode)
{
    hi_u32 over_lap;
    hi_u32 block_start;

    over_lap = isp_work_mode->over_lap;
    if (block_index == 0) {
        block_start = isp_work_mode->block_rect[block_index].x;
    } else {
        block_start = isp_work_mode->block_rect[block_index].x + over_lap;
    }
    return block_start;
}

hi_u32 isp_get_striping_active_img_width(hi_u8 block_index, isp_working_mode *isp_work_mode)
{
    hi_u32 block_width;
    hi_u32 over_lap;
    hi_u8  u8BlockNum;

    over_lap  = isp_work_mode->over_lap;
    block_width = isp_work_mode->block_rect[block_index].width;
    u8BlockNum  = isp_work_mode->block_num;

    if ((block_index == 0) || (block_index == (u8BlockNum - 1))) { // first block and last block
        block_width = block_width - over_lap;
    } else {
        block_width = block_width - over_lap * 2;
    }
    return block_width;
}

hi_u32 isp_get_striping_grid_x_info(hi_u16 *grid_pos, hi_u16 grid_num, isp_working_mode *isp_work_mode)
{
    hi_u8  i;
    hi_u16 start;
    hi_u16 width;
    hi_u16 div_num;
    hi_u16 index = 0;

    for (i = 0; i < isp_work_mode->block_num; i++) {
        start = isp_get_striping_active_img_start(i, isp_work_mode);
        width = isp_get_striping_active_img_width(i, isp_work_mode);

        if (i < grid_num % DIV_0_TO_1(isp_work_mode->block_num)) {
            div_num = grid_num / DIV_0_TO_1(isp_work_mode->block_num) + 1;
        } else {
            div_num = grid_num / DIV_0_TO_1(isp_work_mode->block_num);
        }
        isp_calc_grid_info(width, start, div_num, &(grid_pos[index]));
        index = index + div_num;
    }
    return HI_SUCCESS;
}

hi_s32 isp_get_ae_grid_info(VI_PIPE vi_pipe, hi_isp_ae_grid_info *fe_grid_info, hi_isp_ae_grid_info *be_grid_info)
{
    hi_bool crop_en = HI_FALSE;
    hi_u16  img_total_width, img_total_height;
    hi_u16  img_start_x, img_start_y;
    hi_u16  be_width, be_height;
    hi_u16  be_start_x  = 0;
    hi_u16  be_start_y  = 0;
    isp_working_mode isp_work_mode;

    memset(fe_grid_info, 0, sizeof(hi_isp_ae_grid_info));
    memset(be_grid_info, 0, sizeof(hi_isp_ae_grid_info));

    if (ioctl(g_as32IspFd[vi_pipe], ISP_WORK_MODE_GET, &isp_work_mode) != HI_SUCCESS) {
        ISP_ERR_TRACE("get work mode error!\n");
        return HI_FAILURE;
    }

    crop_en = hi_ext_system_ae_crop_en_read(vi_pipe);

    if (crop_en == HI_TRUE) {
        img_start_x      = hi_ext_system_ae_crop_x_read(vi_pipe);
        img_start_y      = hi_ext_system_ae_crop_y_read(vi_pipe);
        img_total_width  = hi_ext_system_ae_crop_width_read(vi_pipe);
        img_total_height = hi_ext_system_ae_crop_height_read(vi_pipe);
    } else {
        img_start_x      = 0;
        img_start_y      = 0;
        img_total_width  = hi_ext_sync_total_width_read(vi_pipe);
        img_total_height = hi_ext_sync_total_height_read(vi_pipe);
    }
    isp_calc_grid_info(img_total_width,  img_start_x, AE_ZONE_COLUMN, fe_grid_info->grid_x_pos);
    isp_calc_grid_info(img_total_height, img_start_y, AE_ZONE_ROW, fe_grid_info->grid_y_pos);

    fe_grid_info->grid_x_pos[AE_ZONE_COLUMN] = img_start_x + img_total_width - 1;
    fe_grid_info->grid_y_pos[AE_ZONE_ROW]    = img_start_y + img_total_height - 1;
    fe_grid_info->status = 1;

    if ((IS_STRIPING_MODE(isp_work_mode.running_mode)) ||
        (IS_SIDEBYSIDE_MODE(isp_work_mode.running_mode))) {
        isp_get_striping_grid_x_info(be_grid_info->grid_x_pos, AE_ZONE_COLUMN, &isp_work_mode);
        be_start_y  = isp_work_mode.block_rect[0].y;
        be_height   = isp_work_mode.frame_rect.height;
        isp_calc_grid_info(be_height, be_start_y, AE_ZONE_ROW, be_grid_info->grid_y_pos);
        be_width    = isp_work_mode.frame_rect.width;
    } else {
        if (crop_en == HI_TRUE) {
            be_start_x = hi_ext_system_ae_crop_x_read(vi_pipe);
            be_start_y = hi_ext_system_ae_crop_y_read(vi_pipe);
            be_width  = hi_ext_system_ae_crop_width_read(vi_pipe);
            be_height = hi_ext_system_ae_crop_height_read(vi_pipe);
        } else {
            be_start_x = 0;
            be_start_y = 0;
            be_width  = isp_work_mode.frame_rect.width;
            be_height = isp_work_mode.frame_rect.height;
        }

        isp_calc_grid_info(be_width,  be_start_x, AE_ZONE_COLUMN, be_grid_info->grid_x_pos);
        isp_calc_grid_info(be_height, be_start_y, AE_ZONE_ROW, be_grid_info->grid_y_pos);
    }
    be_grid_info->grid_x_pos[AE_ZONE_COLUMN] = be_start_x + be_width  - 1; // last position
    be_grid_info->grid_y_pos[AE_ZONE_ROW]    = be_start_y + be_height - 1; // last position

    be_grid_info->status = 1;

    return HI_SUCCESS;
}

hi_s32 isp_get_mg_grid_info(VI_PIPE vi_pipe, hi_isp_mg_grid_info *grid_info)
{
    hi_bool crop_en = HI_FALSE;
    hi_u16  be_width, be_height;
    hi_u16  be_start_x = 0;
    hi_u16  be_start_y = 0;
    isp_working_mode isp_work_mode;
    memset(grid_info, 0, sizeof(hi_isp_mg_grid_info));

    if (ioctl(g_as32IspFd[vi_pipe], ISP_WORK_MODE_GET, &isp_work_mode) != HI_SUCCESS) {
        ISP_ERR_TRACE("get work mode error!\n");
        return HI_FAILURE;
    }

    crop_en = hi_ext_system_ae_crop_en_read(vi_pipe);

    if ((IS_STRIPING_MODE(isp_work_mode.running_mode)) ||
        (IS_SIDEBYSIDE_MODE(isp_work_mode.running_mode))) {
        isp_get_striping_grid_x_info(grid_info->grid_x_pos, MG_ZONE_COLUMN, &isp_work_mode);
        be_start_y   = isp_work_mode.block_rect[0].y;
        be_height   = isp_work_mode.frame_rect.height;
        isp_calc_grid_info(be_height, be_start_y, MG_ZONE_ROW, grid_info->grid_y_pos);
        be_width    = isp_work_mode.frame_rect.width;
    } else {
        if (crop_en == HI_TRUE) {
            be_start_x = hi_ext_system_ae_crop_x_read(vi_pipe);
            be_start_y = hi_ext_system_ae_crop_y_read(vi_pipe);
            be_width  = hi_ext_system_ae_crop_width_read(vi_pipe);
            be_height = hi_ext_system_ae_crop_height_read(vi_pipe);
        } else {
            be_start_x = 0;
            be_start_y = 0;
            be_width  = isp_work_mode.frame_rect.width;
            be_height = isp_work_mode.frame_rect.height;
        }

        isp_calc_grid_info(be_width,  be_start_x, MG_ZONE_COLUMN, grid_info->grid_x_pos);
        isp_calc_grid_info(be_height, be_start_y, MG_ZONE_ROW,    grid_info->grid_y_pos);

    }

    grid_info->grid_x_pos[MG_ZONE_COLUMN] = be_start_x + be_width  - 1; // last position
    grid_info->grid_y_pos[MG_ZONE_ROW]    = be_start_y + be_height - 1; // last position
    grid_info->status                     = 1;

    return HI_SUCCESS;
}

hi_s32 isp_get_af_grid_info(VI_PIPE vi_pipe, hi_isp_focus_grid_info *fe_grid_info, hi_isp_focus_grid_info *be_grid_info)
{
    hi_bool crop_en = HI_FALSE;
    hi_u16  img_total_height;
    hi_u16  img_total_width;
    hi_u16  img_start_y;
    hi_u16  img_start_x;
    hi_u16  be_width, be_height;
    hi_u16  be_start_x = 0;
    hi_u16  be_start_y = 0;
    hi_u16  af_x_grid_num, af_y_grid_num;
    isp_working_mode isp_work_mode;

    memset(fe_grid_info, 0, sizeof(hi_isp_focus_grid_info));
    memset(be_grid_info, 0, sizeof(hi_isp_focus_grid_info));

    // 1.get block info
    if (ioctl(g_as32IspFd[vi_pipe], ISP_WORK_MODE_GET, &isp_work_mode) != HI_SUCCESS) {
        ISP_ERR_TRACE("get work mode error!\n");
        return HI_FAILURE;
    }

    crop_en = hi_ext_af_crop_enable_read(vi_pipe);
    af_y_grid_num = hi_ext_af_window_vnum_read(vi_pipe);
    af_x_grid_num = hi_ext_af_window_hnum_read(vi_pipe);
    if (crop_en == HI_TRUE) {
        img_start_x      = hi_ext_af_crop_pos_x_read(vi_pipe);
        img_start_y      = hi_ext_af_crop_pos_y_read(vi_pipe);
        img_total_width  = hi_ext_af_crop_hsize_read(vi_pipe);
        img_total_height = hi_ext_af_crop_vsize_read(vi_pipe);
    } else {
        img_start_x      = 0;
        img_start_y      = 0;
        img_total_width  = hi_ext_sync_total_width_read(vi_pipe);
        img_total_height = hi_ext_sync_total_height_read(vi_pipe);
    }

    isp_calc_grid_info(img_total_width,  img_start_x, af_x_grid_num, fe_grid_info->grid_x_pos);
    isp_calc_grid_info(img_total_height, img_start_y, af_y_grid_num, fe_grid_info->grid_y_pos);

    fe_grid_info->grid_x_pos[af_x_grid_num] = img_start_x + img_total_width  - 1;
    fe_grid_info->grid_y_pos[af_y_grid_num] = img_start_y + img_total_height - 1;
    fe_grid_info->status                    = 1;

    if ((IS_STRIPING_MODE(isp_work_mode.running_mode)) ||
        (IS_SIDEBYSIDE_MODE(isp_work_mode.running_mode))) {
        isp_get_striping_grid_x_info(be_grid_info->grid_x_pos, af_x_grid_num, &isp_work_mode);
        be_start_y = isp_work_mode.block_rect[0].y;
        be_height = isp_work_mode.frame_rect.height;
        isp_calc_grid_info(be_height, be_start_y, af_y_grid_num, be_grid_info->grid_y_pos);
        be_width  = isp_work_mode.frame_rect.width;
    } else {
        if (crop_en == HI_TRUE) {
            be_start_x = hi_ext_af_crop_pos_x_read(vi_pipe);
            be_start_y = hi_ext_af_crop_pos_y_read(vi_pipe);
            be_width  = hi_ext_af_crop_hsize_read(vi_pipe);
            be_height = hi_ext_af_crop_vsize_read(vi_pipe);
        } else {
            be_start_x = 0;
            be_start_y = 0;
            be_width  = isp_work_mode.frame_rect.width;
            be_height = isp_work_mode.frame_rect.height;
        }

        isp_calc_grid_info(be_width,  be_start_x, af_x_grid_num, be_grid_info->grid_x_pos);
        isp_calc_grid_info(be_height, be_start_y, af_y_grid_num, be_grid_info->grid_y_pos);
    }
    be_grid_info->grid_x_pos[af_x_grid_num] = be_start_x + be_width  - 1; // last position
    be_grid_info->grid_y_pos[af_y_grid_num] = be_start_y + be_height - 1; // last position
    be_grid_info->status = 1;
    return HI_SUCCESS;
}

hi_s32 isp_get_wb_grid_info(VI_PIPE vi_pipe, hi_isp_awb_grid_info *grid_info)
{
    hi_bool crop_en = HI_FALSE;
    hi_u16  be_width, be_height;
    hi_u16  be_start_x = 0;
    hi_u16  be_start_y = 0;
    hi_u16  u16awb_x_grid_num, u16awb_y_grid_num;
    isp_working_mode isp_work_mode;

    memset(grid_info, 0, sizeof(hi_isp_awb_grid_info));
    if (ioctl(g_as32IspFd[vi_pipe], ISP_WORK_MODE_GET, &isp_work_mode) != HI_SUCCESS) {
        ISP_ERR_TRACE("get work mode error!\n");
        return HI_FAILURE;
    }

    u16awb_y_grid_num = hi_ext_system_awb_vnum_read(vi_pipe);
    u16awb_x_grid_num = hi_ext_system_awb_hnum_read(vi_pipe);
    crop_en        = hi_ext_system_awb_crop_en_read(vi_pipe);

    if ((IS_STRIPING_MODE(isp_work_mode.running_mode)) ||
        (IS_SIDEBYSIDE_MODE(isp_work_mode.running_mode))) {
        isp_get_striping_grid_x_info(grid_info->grid_x_pos, u16awb_x_grid_num, &isp_work_mode);
        be_start_y = isp_work_mode.block_rect[0].y;
        be_height = isp_work_mode.frame_rect.height;
        isp_calc_grid_info(be_height, be_start_y, u16awb_y_grid_num, grid_info->grid_y_pos);
        be_width = isp_work_mode.frame_rect.width;
    } else {
        if (crop_en == HI_TRUE) {
            be_start_x = hi_ext_system_awb_crop_x_read(vi_pipe);
            be_start_y = hi_ext_system_awb_crop_y_read(vi_pipe);
            be_width  = hi_ext_system_awb_crop_width_read(vi_pipe);
            be_height = hi_ext_system_awb_crop_height_read(vi_pipe);
        } else {
            be_start_x = 0;
            be_start_y = 0;
            be_width = isp_work_mode.frame_rect.width;
            be_height = isp_work_mode.frame_rect.height;
        }

        isp_calc_grid_info(be_width, be_start_x, u16awb_x_grid_num, grid_info->grid_x_pos);
        isp_calc_grid_info(be_height, be_start_y, u16awb_y_grid_num, grid_info->grid_y_pos);
    }

    grid_info->grid_x_pos[u16awb_x_grid_num] = be_start_x + be_width  - 1; // last position
    grid_info->grid_y_pos[u16awb_y_grid_num] = be_start_y + be_height - 1; // last position
    grid_info->status                     = 1;

    return HI_SUCCESS;
}

hi_s32 isp_get_ae_stitch_statistics(VI_PIPE vi_pipe, hi_isp_ae_stitch_statistics *ae_stitch_stat)
{
    hi_s32 i, j, k, l;
    hi_u32 pipe_num;
    hi_s32 ret;
    hi_u32 key_lowbit, key_highbit;

    vi_pipe_wdr_attr wdr_attr;
    vi_stitch_attr stitch_attr;
    VI_PIPE vi_main_stitch_pipe;
    hi_isp_statistics_ctrl stat_key;
    isp_stat_info stat_info;
    isp_stat *isp_act_stat;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(ae_stitch_stat);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ret = ioctl(g_as32IspFd[vi_pipe], ISP_GET_STITCH_ATTR, &stitch_attr);

    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get stitch attr failed\n", vi_pipe);
        return ret;
    }

    vi_main_stitch_pipe = stitch_attr.stitch_bind_id[0];

    ret = ioctl(g_as32IspFd[vi_main_stitch_pipe], ISP_STAT_ACT_GET, &stat_info);

    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("get active stat buffer err\n");
        return HI_ERR_ISP_NOMEM;
    }

    stat_info.virt_addr = HI_MPI_SYS_MmapCache(stat_info.phy_addr, sizeof(isp_stat));

    if (stat_info.virt_addr == HI_NULL) {
        return HI_ERR_ISP_NULL_PTR;
    }

    isp_act_stat = (isp_stat *)stat_info.virt_addr;

    ret = ioctl(g_as32IspFd[vi_pipe], ISP_GET_WDR_ATTR, &wdr_attr);

    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("ISP[%d] get WDR attr failed\n", vi_pipe);
        HI_MPI_SYS_Munmap(stat_info.virt_addr, sizeof(isp_stat));
        return ret;
    }

    key_lowbit  = hi_ext_system_statistics_ctrl_lowbit_read(vi_pipe);
    key_highbit = hi_ext_system_statistics_ctrl_highbit_read(vi_pipe);
    stat_key.key = ((hi_u64)key_highbit << 32) + key_lowbit;

    /* AE FE stat */
    k = 0;
    pipe_num  = MIN2(wdr_attr.dev_bind_pipe.num, ISP_CHN_MAX_NUM);
    if (stat_key.bit1_fe_ae_sti_glo_stat && wdr_attr.mast_pipe) {
        for (; k < pipe_num; k++) {
            for (i = 0; i < HIST_NUM; i++) {
                ae_stitch_stat->fe_hist1024_value[k][i] = isp_act_stat->stitch_stat.stFEAeStat1.au32HistogramMemArray[k][i];
            }

            ae_stitch_stat->fe_global_avg[k][0] = isp_act_stat->stitch_stat.stFEAeStat2.u16GlobalAvgR[k];
            ae_stitch_stat->fe_global_avg[k][1] = isp_act_stat->stitch_stat.stFEAeStat2.u16GlobalAvgGr[k];
            ae_stitch_stat->fe_global_avg[k][2] = isp_act_stat->stitch_stat.stFEAeStat2.u16GlobalAvgGb[k];
            ae_stitch_stat->fe_global_avg[k][3] = isp_act_stat->stitch_stat.stFEAeStat2.u16GlobalAvgB[k];
        }

        for (; k < ISP_CHN_MAX_NUM; k++) {
            for (i = 0; i < HIST_NUM; i++) {
                ae_stitch_stat->fe_hist1024_value[k][i] = 0;
            }

            ae_stitch_stat->fe_global_avg[k][0] = 0;
            ae_stitch_stat->fe_global_avg[k][1] = 0;
            ae_stitch_stat->fe_global_avg[k][2] = 0;
            ae_stitch_stat->fe_global_avg[k][3] = 0;
        }
    }

    k = 0;
    if (stat_key.bit1_fe_ae_sti_loc_stat && wdr_attr.mast_pipe) {
        for (; k < pipe_num; k++) {
            for (i = 0; i < AE_ZONE_ROW; i++) {
                for (j = 0; j < AE_ZONE_COLUMN; j++) {
                    for (l = 0; l < VI_MAX_PIPE_NUM; l++) {
                        ae_stitch_stat->fe_zone_avg[l][k][i][j][0] = isp_act_stat->stitch_stat.stFEAeStat3.au16ZoneAvg[l][k][i][j][0];  /* R */
                        ae_stitch_stat->fe_zone_avg[l][k][i][j][1] = isp_act_stat->stitch_stat.stFEAeStat3.au16ZoneAvg[l][k][i][j][1];  /* Gr */
                        ae_stitch_stat->fe_zone_avg[l][k][i][j][2] = isp_act_stat->stitch_stat.stFEAeStat3.au16ZoneAvg[l][k][i][j][2];  /* Gb */
                        ae_stitch_stat->fe_zone_avg[l][k][i][j][3] = isp_act_stat->stitch_stat.stFEAeStat3.au16ZoneAvg[l][k][i][j][3];  /* B */
                    }
                }
            }
        }

        for (; k < ISP_CHN_MAX_NUM; k++) {
            for (i = 0; i < AE_ZONE_ROW; i++) {
                for (j = 0; j < AE_ZONE_COLUMN; j++) {
                    for (l = 0; l < VI_MAX_PIPE_NUM; l++) {
                        ae_stitch_stat->fe_zone_avg[l][k][i][j][0] = 0;  /* R */
                        ae_stitch_stat->fe_zone_avg[l][k][i][j][1] = 0;  /* Gr */
                        ae_stitch_stat->fe_zone_avg[l][k][i][j][2] = 0;  /* Gb */
                        ae_stitch_stat->fe_zone_avg[l][k][i][j][3] = 0;  /* B */
                    }
                }
            }
        }
    }

    /* AE BE stat */
    if (stat_key.bit1_be_ae_sti_glo_stat) {
        for (i = 0; i < HIST_NUM; i++) {
            ae_stitch_stat->be_hist1024_value[i] = isp_act_stat->stitch_stat.stBEAeStat1.au32HistogramMemArray[i];
        }

        ae_stitch_stat->be_global_avg[0] = isp_act_stat->stitch_stat.stBEAeStat2.u16GlobalAvgR;
        ae_stitch_stat->be_global_avg[1] = isp_act_stat->stitch_stat.stBEAeStat2.u16GlobalAvgGr;
        ae_stitch_stat->be_global_avg[2] = isp_act_stat->stitch_stat.stBEAeStat2.u16GlobalAvgGb;
        ae_stitch_stat->be_global_avg[3] = isp_act_stat->stitch_stat.stBEAeStat2.u16GlobalAvgB;
    }

    if (stat_key.bit1_be_ae_sti_loc_stat) {
        for (i = 0; i < AE_ZONE_ROW; i++) {
            for (j = 0; j < AE_ZONE_COLUMN; j++) {
                for (l = 0; l < VI_MAX_PIPE_NUM; l++) {
                    ae_stitch_stat->be_zone_avg[l][i][j][0] = isp_act_stat->stitch_stat.stBEAeStat3.au16ZoneAvg[l][i][j][0];  /* R */
                    ae_stitch_stat->be_zone_avg[l][i][j][1] = isp_act_stat->stitch_stat.stBEAeStat3.au16ZoneAvg[l][i][j][1];  /* Gr */
                    ae_stitch_stat->be_zone_avg[l][i][j][2] = isp_act_stat->stitch_stat.stBEAeStat3.au16ZoneAvg[l][i][j][2];  /* Gb */
                    ae_stitch_stat->be_zone_avg[l][i][j][3] = isp_act_stat->stitch_stat.stBEAeStat3.au16ZoneAvg[l][i][j][3];  /* B */
                }
            }
        }
    }

    HI_MPI_SYS_Munmap((hi_void *)isp_act_stat, sizeof(isp_stat));

    return HI_SUCCESS;
}

hi_s32 isp_get_wb_stitch_statistics(VI_PIPE vi_pipe, hi_isp_wb_stitch_statistics *stitch_wb_stat)
{
    hi_s32 i;
    hi_isp_statistics_ctrl stat_key;
    isp_stat_info act_stat_info;
    isp_stat *isp_act_stat;
    hi_s32 ret;
    hi_u32 key_lowbit, key_highbit;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(stitch_wb_stat);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    key_lowbit  = hi_ext_system_statistics_ctrl_lowbit_read(vi_pipe);
    key_highbit = hi_ext_system_statistics_ctrl_highbit_read(vi_pipe);
    stat_key.key = ((hi_u64)key_highbit << 32) + key_lowbit;

    ret = ioctl(g_as32IspFd[vi_pipe], ISP_STAT_ACT_GET, &act_stat_info);

    if (ret != HI_SUCCESS) {
        ISP_ERR_TRACE("get active stat buffer err\n");
        return HI_ERR_ISP_NOMEM;
    }

    act_stat_info.virt_addr = HI_MPI_SYS_MmapCache(act_stat_info.phy_addr, sizeof(isp_stat));

    if (act_stat_info.virt_addr == HI_NULL) {
        return HI_ERR_ISP_NULL_PTR;
    }

    isp_act_stat = (isp_stat *)act_stat_info.virt_addr;

    if (stat_key.bit1_awb_stat2) {
        for (i = 0; i < AWB_ZONE_STITCH_MAX; i++) {
            stitch_wb_stat->zone_avg_r[i] = isp_act_stat->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgR[i];
            stitch_wb_stat->zone_avg_g[i] = isp_act_stat->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgG[i];
            stitch_wb_stat->zone_avg_b[i] = isp_act_stat->stitch_stat.stAwbStat2.au16MeteringMemArrayAvgB[i];
            stitch_wb_stat->zone_count_all[i] = isp_act_stat->stitch_stat.stAwbStat2.au16MeteringMemArrayCountAll[i];
        }

        stitch_wb_stat->zone_row = isp_act_stat->stitch_stat.stAwbStat2.u16ZoneRow;
        stitch_wb_stat->zone_col = isp_act_stat->stitch_stat.stAwbStat2.u16ZoneCol;
    }

    HI_MPI_SYS_Munmap(act_stat_info.virt_addr, sizeof(isp_stat));

    return HI_SUCCESS;
}

hi_s32 isp_get_fe_focus_statistics(VI_PIPE vi_pipe, hi_isp_fe_focus_statistics *fe_af_stat, isp_stat *isp_act_stat, hi_u8 wdr_chn)
{
    hi_u8 i, j, k = 0;
    hi_u8 col, row;

    col = MIN2(hi_ext_af_window_hnum_read(vi_pipe), AF_ZONE_COLUMN);
    row = MIN2(hi_ext_af_window_vnum_read(vi_pipe), AF_ZONE_ROW);

    for (; k < wdr_chn; k++) {
        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                fe_af_stat->zone_metrics[k][i][j].v1 = isp_act_stat->stFEAfStat.stZoneMetrics[k][i][j].u16v1;
                fe_af_stat->zone_metrics[k][i][j].h1 = isp_act_stat->stFEAfStat.stZoneMetrics[k][i][j].u16h1;
                fe_af_stat->zone_metrics[k][i][j].v2 = isp_act_stat->stFEAfStat.stZoneMetrics[k][i][j].u16v2;
                fe_af_stat->zone_metrics[k][i][j].h2 = isp_act_stat->stFEAfStat.stZoneMetrics[k][i][j].u16h2;
                fe_af_stat->zone_metrics[k][i][j].y = isp_act_stat->stFEAfStat.stZoneMetrics[k][i][j].u16y;
                fe_af_stat->zone_metrics[k][i][j].hl_cnt = isp_act_stat->stFEAfStat.stZoneMetrics[k][i][j].u16HlCnt;
            }
        }
    }

    for (; k < ISP_CHN_MAX_NUM; k++) {
        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                fe_af_stat->zone_metrics[k][i][j].v1 = 0;
                fe_af_stat->zone_metrics[k][i][j].h1 = 0;
                fe_af_stat->zone_metrics[k][i][j].v2 = 0;
                fe_af_stat->zone_metrics[k][i][j].h2 = 0;
                fe_af_stat->zone_metrics[k][i][j].y  = 0;
                fe_af_stat->zone_metrics[k][i][j].hl_cnt = 0;
            }
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_set_radial_shading_attr(VI_PIPE vi_pipe, const hi_isp_radial_shading_attr *radial_shading_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(radial_shading_attr);
    ISP_CHECK_BOOL(radial_shading_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    hi_ext_system_isp_radial_shading_enable_write(vi_pipe, radial_shading_attr->enable);
    hi_ext_system_isp_radial_shading_strength_write(vi_pipe, radial_shading_attr->radial_str);

    hi_ext_system_isp_radial_shading_coefupdate_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}

hi_s32 isp_get_radial_shading_attr(VI_PIPE vi_pipe, hi_isp_radial_shading_attr *radial_shading_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(radial_shading_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    radial_shading_attr->enable        = hi_ext_system_isp_radial_shading_enable_read(vi_pipe);
    radial_shading_attr->radial_str   = hi_ext_system_isp_radial_shading_strength_read(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_set_radial_shading_lut(VI_PIPE vi_pipe, const hi_isp_radial_shading_lut_attr *radial_shading_lut_attr)
{
    hi_u16 i;
    hi_u16 width, height;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(radial_shading_lut_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (radial_shading_lut_attr->radial_scale > 13) {
        ISP_ERR_TRACE("Invalid radial_scale %d!\n", radial_shading_lut_attr->radial_scale);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((radial_shading_lut_attr->light_type1 > 2) || (radial_shading_lut_attr->light_type2 > 2)) {
        ISP_ERR_TRACE("Invalid light_info (%d, %d)!\n", radial_shading_lut_attr->light_type1, radial_shading_lut_attr->light_type2);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (radial_shading_lut_attr->blend_ratio > 256) {
        ISP_ERR_TRACE("Invalid blend_ratio %d!\n", radial_shading_lut_attr->blend_ratio);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (radial_shading_lut_attr->light_mode >= OPERATION_MODE_BUTT) {
        ISP_ERR_TRACE("Invalid light_mode %d!\n", radial_shading_lut_attr->light_mode);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    width  = hi_ext_system_be_total_width_read(vi_pipe);
    height = hi_ext_system_be_total_height_read(vi_pipe);
    if ((radial_shading_lut_attr->center_r_x >= width) || (radial_shading_lut_attr->center_gr_x >= width) ||
        (radial_shading_lut_attr->center_gb_x >= width) || (radial_shading_lut_attr->center_b_x >= width)) {
        ISP_ERR_TRACE("Invalid center_x!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((radial_shading_lut_attr->center_r_y >= height) || (radial_shading_lut_attr->center_gr_y >= height) ||
        (radial_shading_lut_attr->center_gb_y >= height) || (radial_shading_lut_attr->center_b_y >= height)) {
        ISP_ERR_TRACE("Invalid center_y!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_ext_system_isp_radial_shading_lightmode_write(vi_pipe, radial_shading_lut_attr->light_mode);
    hi_ext_system_isp_radial_shading_scale_write(vi_pipe, radial_shading_lut_attr->radial_scale);
    hi_ext_system_isp_radial_shading_lightinfo_write(vi_pipe, 0, radial_shading_lut_attr->light_type1);
    hi_ext_system_isp_radial_shading_lightinfo_write(vi_pipe, 1, radial_shading_lut_attr->light_type2);
    hi_ext_system_isp_radial_shading_blendratio_write(vi_pipe, radial_shading_lut_attr->blend_ratio);
    hi_ext_system_isp_radial_shading_centerrx_write(vi_pipe, radial_shading_lut_attr->center_r_x);
    hi_ext_system_isp_radial_shading_centerry_write(vi_pipe, radial_shading_lut_attr->center_r_y);
    hi_ext_system_isp_radial_shading_centergrx_write(vi_pipe, radial_shading_lut_attr->center_gr_x);
    hi_ext_system_isp_radial_shading_centergry_write(vi_pipe, radial_shading_lut_attr->center_gr_y);
    hi_ext_system_isp_radial_shading_centergbx_write(vi_pipe, radial_shading_lut_attr->center_gb_x);
    hi_ext_system_isp_radial_shading_centergby_write(vi_pipe, radial_shading_lut_attr->center_gb_y);
    hi_ext_system_isp_radial_shading_centerbx_write(vi_pipe, radial_shading_lut_attr->center_b_x);
    hi_ext_system_isp_radial_shading_centerby_write(vi_pipe, radial_shading_lut_attr->center_b_y);
    hi_ext_system_isp_radial_shading_offcenterr_write(vi_pipe, radial_shading_lut_attr->off_center_r);
    hi_ext_system_isp_radial_shading_offcentergr_write(vi_pipe, radial_shading_lut_attr->off_center_gr);
    hi_ext_system_isp_radial_shading_offcentergb_write(vi_pipe, radial_shading_lut_attr->off_center_gb);
    hi_ext_system_isp_radial_shading_offcenterb_write(vi_pipe, radial_shading_lut_attr->off_center_b);

    for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
        // gain strength maximum can be up to 65535, thus no need to make assert before config to ext. regs
        hi_ext_system_isp_radial_shading_r_gain0_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[0].r_gain[i]);
        hi_ext_system_isp_radial_shading_gr_gain0_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[0].gr_gain[i]);
        hi_ext_system_isp_radial_shading_gb_gain0_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[0].gb_gain[i]);
        hi_ext_system_isp_radial_shading_b_gain0_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[0].b_gain[i]);

        hi_ext_system_isp_radial_shading_r_gain1_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[1].r_gain[i]);
        hi_ext_system_isp_radial_shading_gr_gain1_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[1].gr_gain[i]);
        hi_ext_system_isp_radial_shading_gb_gain1_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[1].gb_gain[i]);
        hi_ext_system_isp_radial_shading_b_gain1_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[1].b_gain[i]);

        hi_ext_system_isp_radial_shading_r_gain2_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[2].r_gain[i]);
        hi_ext_system_isp_radial_shading_gr_gain2_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[2].gr_gain[i]);
        hi_ext_system_isp_radial_shading_gb_gain2_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[2].gb_gain[i]);
        hi_ext_system_isp_radial_shading_b_gain2_write(vi_pipe, i, radial_shading_lut_attr->rlsc_gain_lut[2].b_gain[i]);
    }

    hi_ext_system_isp_radial_shading_lutupdate_write(vi_pipe, HI_TRUE);
    return HI_SUCCESS;
}

hi_s32 isp_get_radial_shading_lut(VI_PIPE vi_pipe, hi_isp_radial_shading_lut_attr *radial_shading_lut_attr)
{
    hi_u16 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(radial_shading_lut_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    radial_shading_lut_attr->light_mode   = hi_ext_system_isp_radial_shading_lightmode_read(vi_pipe);
    radial_shading_lut_attr->blend_ratio = hi_ext_system_isp_radial_shading_blendratio_read(vi_pipe);
    radial_shading_lut_attr->light_type1  = hi_ext_system_isp_radial_shading_lightinfo_read(vi_pipe, 0);
    radial_shading_lut_attr->light_type2  = hi_ext_system_isp_radial_shading_lightinfo_read(vi_pipe, 1);

    radial_shading_lut_attr->radial_scale  = hi_ext_system_isp_radial_shading_scale_read(vi_pipe);
    radial_shading_lut_attr->center_r_x    = hi_ext_system_isp_radial_shading_centerrx_read(vi_pipe);
    radial_shading_lut_attr->center_r_y    = hi_ext_system_isp_radial_shading_centerry_read(vi_pipe);
    radial_shading_lut_attr->center_gr_x   = hi_ext_system_isp_radial_shading_centergrx_read(vi_pipe);
    radial_shading_lut_attr->center_gr_y   = hi_ext_system_isp_radial_shading_centergry_read(vi_pipe);
    radial_shading_lut_attr->center_gb_x   = hi_ext_system_isp_radial_shading_centergbx_read(vi_pipe);
    radial_shading_lut_attr->center_gb_y   = hi_ext_system_isp_radial_shading_centergby_read(vi_pipe);
    radial_shading_lut_attr->center_b_x    = hi_ext_system_isp_radial_shading_centerbx_read(vi_pipe);
    radial_shading_lut_attr->center_b_y    = hi_ext_system_isp_radial_shading_centerby_read(vi_pipe);
    radial_shading_lut_attr->off_center_r  = hi_ext_system_isp_radial_shading_offcenterr_read(vi_pipe);
    radial_shading_lut_attr->off_center_gr = hi_ext_system_isp_radial_shading_offcentergr_read(vi_pipe);
    radial_shading_lut_attr->off_center_gb = hi_ext_system_isp_radial_shading_offcentergb_read(vi_pipe);
    radial_shading_lut_attr->off_center_b  = hi_ext_system_isp_radial_shading_offcenterb_read(vi_pipe);

    for (i = 0; i < HI_ISP_RLSC_POINTS; i++) {
        radial_shading_lut_attr->rlsc_gain_lut[0].r_gain[i]  = hi_ext_system_isp_radial_shading_r_gain0_read(vi_pipe, i);
        radial_shading_lut_attr->rlsc_gain_lut[0].gr_gain[i] = hi_ext_system_isp_radial_shading_gr_gain0_read(vi_pipe, i);
        radial_shading_lut_attr->rlsc_gain_lut[0].gb_gain[i] = hi_ext_system_isp_radial_shading_gb_gain0_read(vi_pipe, i);
        radial_shading_lut_attr->rlsc_gain_lut[0].b_gain[i]  = hi_ext_system_isp_radial_shading_b_gain0_read(vi_pipe, i);

        radial_shading_lut_attr->rlsc_gain_lut[1].r_gain[i]  = hi_ext_system_isp_radial_shading_r_gain1_read(vi_pipe, i);
        radial_shading_lut_attr->rlsc_gain_lut[1].gr_gain[i] = hi_ext_system_isp_radial_shading_gr_gain1_read(vi_pipe, i);
        radial_shading_lut_attr->rlsc_gain_lut[1].gb_gain[i] = hi_ext_system_isp_radial_shading_gb_gain1_read(vi_pipe, i);
        radial_shading_lut_attr->rlsc_gain_lut[1].b_gain[i]  = hi_ext_system_isp_radial_shading_b_gain1_read(vi_pipe, i);

        radial_shading_lut_attr->rlsc_gain_lut[2].r_gain[i]  = hi_ext_system_isp_radial_shading_r_gain2_read(vi_pipe, i);
        radial_shading_lut_attr->rlsc_gain_lut[2].gr_gain[i] = hi_ext_system_isp_radial_shading_gr_gain2_read(vi_pipe, i);
        radial_shading_lut_attr->rlsc_gain_lut[2].gb_gain[i] = hi_ext_system_isp_radial_shading_gb_gain2_read(vi_pipe, i);
        radial_shading_lut_attr->rlsc_gain_lut[2].b_gain[i]  = hi_ext_system_isp_radial_shading_b_gain2_read(vi_pipe, i);
    }

    return HI_SUCCESS;
}

hi_s32 isp_set_pipe_differ_attr(VI_PIPE vi_pipe, const hi_isp_pipe_diff_attr *pipe_differ)
{
    hi_u32 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(pipe_differ);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        if ((pipe_differ->offset[i] > 0xFFF) || (pipe_differ->offset[i] < -0xFFF)) {
            ISP_ERR_TRACE("Invalid offset :%x!\n", pipe_differ->offset[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((pipe_differ->gain[i] < 0x80) || (pipe_differ->gain[i] > 0x400)) {
            ISP_ERR_TRACE("Invalid gain :%x!\n", pipe_differ->gain[i]);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        hi_ext_system_isp_pipe_diff_offset_write(vi_pipe, i, pipe_differ->offset[i]);
        hi_ext_system_isp_pipe_diff_gain_write(vi_pipe, i, pipe_differ->gain[i]);
    }

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        hi_ext_system_isp_pipe_diff_ccm_write(vi_pipe, i, pipe_differ->color_matrix[i]);
    }

    hi_ext_system_black_level_change_write(vi_pipe, (hi_u8)HI_TRUE);

    return HI_SUCCESS;
}

hi_s32 isp_get_pipe_differ_attr(VI_PIPE vi_pipe, hi_isp_pipe_diff_attr *pipe_differ)
{
    hi_u32 i;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(pipe_differ);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
        pipe_differ->offset[i] = hi_ext_system_isp_pipe_diff_offset_read(vi_pipe, i);
        pipe_differ->gain[i]   = hi_ext_system_isp_pipe_diff_gain_read(vi_pipe, i);
    }

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        pipe_differ->color_matrix[i] = hi_ext_system_isp_pipe_diff_ccm_read(vi_pipe, i);
    }

    return HI_SUCCESS;
}

hi_s32 isp_set_rc_attr(VI_PIPE vi_pipe, const hi_isp_rc_attr *rc_attr)
{
    hi_u16 width, height, max;
    hi_u32 sq_length;
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(rc_attr);
    ISP_CHECK_BOOL(rc_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    width  = hi_ext_sync_total_width_read(vi_pipe);
    height = hi_ext_sync_total_height_read(vi_pipe);
    sq_length = (hi_u32)(width * width + height * height);
    max      = Sqrt32(sq_length);

    if (rc_attr->center_coor.x >= width || rc_attr->center_coor.x < 0) {
        ISP_ERR_TRACE("Invalid center_coor.x!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (rc_attr->center_coor.y >= height || rc_attr->center_coor.y < 0) {
        ISP_ERR_TRACE("Invalid center_coor.y!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (rc_attr->radius >= (hi_u32)max) {
        ISP_ERR_TRACE("Invalid radius!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_rc_en_write(vi_pipe, rc_attr->enable);
    hi_ext_system_rc_center_hor_coor_write(vi_pipe, rc_attr->center_coor.x);
    hi_ext_system_rc_center_ver_coor_write(vi_pipe, rc_attr->center_coor.y);
    hi_ext_system_rc_radius_write(vi_pipe, rc_attr->radius);
    hi_ext_system_rc_coef_update_en_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}

hi_s32 isp_get_rc_attr(VI_PIPE vi_pipe, hi_isp_rc_attr *rc_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(rc_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    rc_attr->enable           = hi_ext_system_rc_en_read(vi_pipe);
    rc_attr->center_coor.x = hi_ext_system_rc_center_hor_coor_read(vi_pipe);
    rc_attr->center_coor.y = hi_ext_system_rc_center_ver_coor_read(vi_pipe);
    rc_attr->radius         = hi_ext_system_rc_radius_read(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_set_rgbir_attr(VI_PIPE vi_pipe, const hi_isp_rgbir_attr *rgbir_attr)
{
    ISP_ERR_TRACE("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_get_rgbir_attr(VI_PIPE vi_pipe, hi_isp_rgbir_attr *rgbir_attr)
{
    ISP_ERR_TRACE("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_set_pre_log_lut_attr(VI_PIPE vi_pipe, const hi_isp_preloglut_attr *pre_log_lut_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(pre_log_lut_attr);
    ISP_CHECK_BOOL(pre_log_lut_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    hi_ext_system_feloglut_enable_write(vi_pipe, pre_log_lut_attr->enable);

    return HI_SUCCESS;
}

hi_s32 isp_get_pre_log_lut_attr(VI_PIPE vi_pipe, hi_isp_preloglut_attr *pre_log_lut_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(pre_log_lut_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    pre_log_lut_attr->enable = hi_ext_system_feloglut_enable_read(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_set_log_lut_attr(VI_PIPE vi_pipe, const hi_isp_loglut_attr *log_lut_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(log_lut_attr);
    ISP_CHECK_BOOL(log_lut_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    hi_ext_system_loglut_enable_write(vi_pipe, log_lut_attr->enable);

    return HI_SUCCESS;
}

hi_s32 isp_get_log_lut_attr(VI_PIPE vi_pipe, hi_isp_loglut_attr *log_lut_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(log_lut_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    log_lut_attr->enable = hi_ext_system_loglut_enable_read(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_set_clut_coeff(VI_PIPE vi_pipe, const hi_isp_clut_lut *clut_lut)
{
    hi_u32         *vir_addr = HI_NULL;
    isp_mmz_buf_ex clut_buf;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(clut_lut);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (ioctl(g_as32IspFd[vi_pipe], ISP_CLUT_BUF_GET, &clut_buf.phy_addr) != HI_SUCCESS) {
        ISP_ERR_TRACE("get clut buffer err\n");
        return HI_ERR_ISP_NOMEM;
    }

    clut_buf.vir_addr = HI_MPI_SYS_Mmap(clut_buf.phy_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));

    if (clut_buf.vir_addr == HI_NULL) {
        return HI_ERR_ISP_NULL_PTR;
    }

    vir_addr = (hi_u32 *)clut_buf.vir_addr;

    memcpy(vir_addr, clut_lut->lut, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));

    hi_ext_system_clut_lut_update_en_write(vi_pipe, HI_TRUE);

    HI_MPI_SYS_Munmap(clut_buf.vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));

    return HI_SUCCESS;
}

hi_s32 isp_get_clut_coeff(VI_PIPE vi_pipe, hi_isp_clut_lut *clut_lut)
{
    hi_u32         *vir_addr = HI_NULL;
    isp_mmz_buf_ex clut_buf;

    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(clut_lut);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (ioctl(g_as32IspFd[vi_pipe], ISP_CLUT_BUF_GET, &clut_buf.phy_addr) != HI_SUCCESS) {
        ISP_ERR_TRACE("get clut buffer err\n");
        return HI_ERR_ISP_NOMEM;
    }

    clut_buf.vir_addr = HI_MPI_SYS_Mmap(clut_buf.phy_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));

    if (clut_buf.vir_addr == HI_NULL) {
        return HI_ERR_ISP_NULL_PTR;
    }

    vir_addr = (hi_u32 *)clut_buf.vir_addr;

    memcpy(clut_lut->lut, vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));

    HI_MPI_SYS_Munmap(clut_buf.vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));

    return HI_SUCCESS;
}

hi_s32 isp_set_clut_attr(VI_PIPE vi_pipe, const hi_isp_clut_attr *clut_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(clut_attr);
    ISP_CHECK_BOOL(clut_attr->enable);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    if (clut_attr->gain_r > 4095) {
        ISP_ERR_TRACE("Invalid gain_r!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (clut_attr->gain_g > 4095) {
        ISP_ERR_TRACE("Invalid gain_g!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (clut_attr->gain_b > 4095) {
        ISP_ERR_TRACE("Invalid gain_b!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_ext_system_clut_en_write(vi_pipe, clut_attr->enable);

    hi_ext_system_clut_gainR_write(vi_pipe, clut_attr->gain_r);
    hi_ext_system_clut_gainG_write(vi_pipe, clut_attr->gain_g);
    hi_ext_system_clut_gainB_write(vi_pipe, clut_attr->gain_b);
    hi_ext_system_clut_ctrl_update_en_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}

hi_s32 isp_get_clut_attr(VI_PIPE vi_pipe, hi_isp_clut_attr *clut_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(clut_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    clut_attr->enable = hi_ext_system_clut_en_read(vi_pipe);
    clut_attr->gain_r = hi_ext_system_clut_gainR_read(vi_pipe);
    clut_attr->gain_g = hi_ext_system_clut_gainG_read(vi_pipe);
    clut_attr->gain_b = hi_ext_system_clut_gainB_read(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_set_raw_pos(VI_PIPE vi_pipe, const hi_isp_raw_pos_attr *raw_pos_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(raw_pos_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    ISP_CHECK_BOOL(raw_pos_attr->raw_enable);

    if (raw_pos_attr->raw_pos >= RAW_POS_BUTT) {
        ISP_ERR_TRACE("Invalid firmware raw postion %d!\n", raw_pos_attr->raw_pos);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_isp_raw_position_enable_write(vi_pipe, raw_pos_attr->raw_enable);
    hi_ext_system_isp_raw_position_write(vi_pipe, (hi_u32)raw_pos_attr->raw_pos);

    return HI_SUCCESS;
}

hi_s32 isp_get_raw_pos(VI_PIPE vi_pipe, hi_isp_raw_pos_attr *raw_pos_attr)
{
    ISP_CHECK_PIPE(vi_pipe);
    ISP_CHECK_POINTER(raw_pos_attr);
    ISP_CHECK_OPEN(vi_pipe);
    ISP_CHECK_MEM_INIT(vi_pipe);

    raw_pos_attr->raw_enable = hi_ext_system_isp_raw_position_enable_read(vi_pipe);
    raw_pos_attr->raw_pos    = hi_ext_system_isp_raw_position_read(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_calc_flicker_type(VI_PIPE ViPipe, ISP_CALCFLICKER_INPUT_S *pstInputParam,ISP_CALCFLICKER_OUTPUT_S *pstOutputParam, VIDEO_FRAME_INFO_S stFrame[], HI_U32 u32ArraySize)
{
    return HI_ERR_ISP_NOT_SUPPORT;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
