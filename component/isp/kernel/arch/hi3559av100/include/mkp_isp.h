/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#ifndef __MKP_ISP_H__
#define __MKP_ISP_H__

#include "hi_type.h"
#include "hi_comm_3a_adapt.h"
#include "hi_common.h"
#include "hi_defines.h"
#include "isp_reg_define.h"
#include "hi_comm_snap.h"
#include "hi_comm_snap_adapt.h"
#include "hi_comm_isp_adapt.h"
#include "isp_block.h"

#define IOC_TYPE_ISP                    'I'

#define MAX_ISP_STAT_BUF_NUM            2
#define MAX_ISP_BE_BUF_NUM              4
#define ISP_STITCH_MAX_NUM              6

#define ISP_VER_PRIX                    "_ISP_V"
#define ISP_VERSION                     CHIP_NAME ISP_VER_PRIX MK_VERSION(VER_X,VER_Y,VER_Z,VER_P,VER_B) VER_D

#define MAX_MMZ_NAMELEN                 16
#define ISP_INIT_HAND_SIGNAL            0x66778899
#define ISP_EXIT_HAND_SIGNAL            0x99887766


typedef enum hiIOC_NR_ISP_E {
    IOC_NR_ISP_SET_FD = 0,
    IOC_NR_ISP_GET_FRAME_EDGE,
    IOC_NR_ISP_GET_VD_TIMEOUT,
    IOC_NR_ISP_GET_VD_END_TIMEOUT,
    IOC_NR_ISP_GET_VD_BEEND_TIMEOUT,
    IOC_NR_ISP_SET_INT_ENABLE,
    IOC_NR_ISP_GET_VERSION,

    IOC_NR_ISP_STAT_BUF_INIT,
    IOC_NR_ISP_STAT_BUF_EXIT,
    IOC_NR_ISP_STAT_BUF_GET,
    IOC_NR_ISP_STAT_BUF_PUT,
    IOC_NR_ISP_STAT_ACT_GET,
    IOC_NR_ISP_REG_CFG_INIT,
    IOC_NR_ISP_REG_CFG_SET,
    IOC_NR_ISP_BE_CFG_BUF_INIT,
    IOC_NR_ISP_GET_BE_BUF_FIRST,
    IOC_NR_ISP_BE_FREE_BUF_GET,
    IOC_NR_ISP_BE_LAST_BUF_GET,
    IOC_NR_ISP_BE_CFG_BUF_EXIT,
    IOC_NR_ISP_BE_CFG_BUF_CTL,
    IOC_NR_ISP_BE_CFG_BUF_RUNNING,
    IOC_NR_ISP_BE_All_BUF_INIT,
    IOC_NR_ISP_PROC_INIT,
    IOC_NR_ISP_PROC_WRITE_ING,
    IOC_NR_ISP_PROC_WRITE_OK,
    IOC_NR_ISP_PROC_EXIT,
    IOC_NR_ISP_PROC_PARAM_GET,
    IOC_NR_ISP_TRANS_BUF_INIT,
    IOC_NR_ISP_TRANS_BUF_EXIT,
    IOC_NR_ISP_PRO_BUF_INIT,
    IOC_NR_ISP_PRO_BUF_EXIT,
    IOC_NR_ISP_SYNC_CFG_SET,
    IOC_NR_ISP_SYNC_STITCH_PARAM_INIT,
    IOC_NR_WDR_SYNC_CFG_SET,
    IOC_NR_ISP_MEM_INFO_SET,
    IOC_NR_ISP_MEM_INFO_GET,
    IOC_NR_ISP_P2EN_INFO_GET,
    IOC_NR_ISP_INIT_INFO_SET,
    IOC_NR_ISP_SYNC_INIT_SET,
    IOC_NR_ISP_RUN_STATE_SET,
    IOC_NR_ISP_RESET_CTX,
    IOC_NR_ISP_CONFIG_INFO_SET,
    IOC_NR_ISP_SNAP_INFO_GET,
    IOC_NR_ISP_PRO_EN_GET,
    IOC_NR_ISP_UPDATE_POS_GET,
    IOC_NR_ISP_FRAME_CNT_GET,
    IOC_NR_ISP_SNAP_ATTR_GET,
    IOC_NR_ISP_PWM_NUM_GET,
    IOC_NR_ISP_RES_SWITCH_SET,
    IOC_NR_ISP_CHN_SELECT_CFG,
    IOC_NR_ISP_BE_SWITCH_FINISH_STATE_SET,
    IOC_NR_ISP_MODE_SWITCH_SET,
    IOC_NR_ISP_FRAME_INFO_SET,
    IOC_NR_ISP_FRAME_INFO_GET,
    IOC_NR_ISP_DNG_INFO_SET,
    IOC_NR_ISP_DNG_INFO_GET,
    IOC_NR_ISP_SET_MOD_PARAM,
    IOC_NR_ISP_GET_MOD_PARAM,
    IOC_NR_ISP_SET_CTRL_PARAM,
    IOC_NR_ISP_GET_CTRL_PARAM,

    IOC_NR_ISP_MODE_INFO,
    IOC_NR_ISP_WORK_MODE_INIT,
    IOC_NR_ISP_WORK_MODE_GET,
    IOC_NR_ISP_WORK_MODE_EXIT,
    IOC_NR_ISP_PRE_BLK_NUM_UPDATE,
    IOC_NR_ISP_GET_HDR_ATTR,
    IOC_NR_ISP_GET_WDR_ATTR,
    IOC_NR_ISP_GET_STITCH_ATTR,
    IOC_NR_ISP_GET_FUSION_ATTR,
    IOC_NR_ISP_GET_PIPE_SIZE,
    IOC_NR_ISP_PUB_ATTR_INFO,
    IOC_NR_ISP_CLUT_BUF_INIT,
    IOC_NR_ISP_CLUT_BUF_EXIT,
    IOC_NR_ISP_CLUT_BUF_GET,
    IOC_NR_ISP_STT_BUF_INIT,
    IOC_NR_ISP_STT_BUF_EXIT,
    IOC_NR_ISP_STT_ADDR_INIT,
    IOC_NR_ISP_BE_LUT_STT_BUF_GET,
    IOC_NR_ISP_KERNEL_RUNONCE,
    IOC_NR_ISP_KERNEL_YUV_RUNONCE,
    IOC_NR_ISP_OPT_RUNONCE_INFO,
    IOC_NR_ISP_YUV_RUNONCE_INFO,
    IOC_NR_ISP_SET_PROCALCDONE,
    IOC_NR_ISP_SPECAWB_BUF_INIT,
    IOC_NR_ISP_SPECAWB_BUF_EXIT,
    IOC_NR_ISP_SPECAWB_BUF_GET,
    IOC_NR_ISP_SET_RGBIR_FORMAT,
    IOC_NR_ISP_UPDATE_INFO_SET,
    IOC_NR_ISP_BE_SYNC_PARAM_INIT,
    IOC_NR_ISP_LDCI_BUF_INIT,
    IOC_NR_ISP_LDCI_BUF_EXIT,
    IOC_NR_ISP_LDCI_READ_STT_BUF_GET,
    IOC_NR_ISP_AWB_EN_SET,
    IOC_NR_ISP_BUTT,
} IOC_NR_ISP_E;

typedef struct {
    hi_u32  milli_sec;     /* RW, Range: [0x0, 0xFFFFFFFF], the number of the */
    hi_u32  int_status;    /* RO, when there is ISP interrupt, then the s32IntStatus is 1 */
} isp_vd_timeout;

typedef struct hiISP_VERSION_S {
    HI_U8 au8MppVersion[4];
    HI_U32 u32Magic;
} ISP_VERSION_S;

/* the statistics of isp */
typedef struct {
    hi_u16  defect_pixel_count;
} isp_dp_stat;

#define ISP_FLICKER_G_MEAN_LENGTH       (512)
typedef struct {
    hi_u32 cur_diff_gr[ISP_STRIPING_MAX_NUM];                            // s24.0, current frame gr sum
    hi_u32 cur_diff_gb[ISP_STRIPING_MAX_NUM];                            // s24.0, current frame gb sum
    hi_u32 cur_abs_gr[ISP_STRIPING_MAX_NUM];                             // u23.0
    hi_u32 cur_abs_gb[ISP_STRIPING_MAX_NUM];                             // u23.0
    hi_u32 cur_cnt_over[ISP_STRIPING_MAX_NUM];                           // u26.0
    hi_u32 g_mean[ISP_STRIPING_MAX_NUM][ISP_FLICKER_G_MEAN_LENGTH];     // u22.0
} isp_flicker_stat;

typedef struct {
    hi_u32  max_stat_dout[ISP_STRIPING_MAX_NUM][DEFOG_ZONE_NUM];
    hi_u32  min_dout[ISP_STRIPING_MAX_NUM][DEFOG_ZONE_NUM / 2];
} isp_dehaze_stat;

typedef struct hiISP_MG_STAT_S {
    HI_U16  au16ZoneAvg[MG_ZONE_ROW][MG_ZONE_COLUMN][4];
} ISP_MG_STAT_S;

typedef struct {
    hi_u32  global_map[LDCI_STAT_WND_Y][LDCI_STAT_WND_X];
} isp_ldci_stat;

typedef struct hiISP_COMM_STAT_S {
    HI_U16  au16WhiteBalanceGain[4];
} ISP_COMM_STAT_S;

typedef struct hiISP_BE_STAT_S {
    ISP_BE_AE_STAT_1_S  stBEAeStat1;
    ISP_BE_AE_STAT_2_S  stBEAeStat2;
    ISP_BE_AE_STAT_3_S  stBEAeStat3;
    ISP_AWB_STAT_1_S    stAwbStat1;
    ISP_AWB_STAT_2_S    stAwbStat2;
    ISP_BE_AF_STAT_S    stAfStat;
    isp_dp_stat         dp_stat;
    ISP_COMM_STAT_S     stCommStat;
    isp_dehaze_stat   dehaze_stat;
    ISP_MG_STAT_S       stMgStat;
} ISP_BE_STAT_S;

typedef struct {
    ISP_FE_AE_STAT_1_S        stFEAeStat1;
    ISP_FE_AE_STAT_2_S        stFEAeStat2;
    ISP_FE_AE_STITCH_STAT_3_S stFEAeStat3;
    ISP_BE_AE_STAT_1_S        stBEAeStat1;
    ISP_BE_AE_STAT_2_S        stBEAeStat2;
    ISP_BE_AE_STITCH_STAT_3_S stBEAeStat3;
    ISP_AWB_STAT_STITCH_S     stAwbStat2;
} isp_stitch_stat;

typedef struct {
    ISP_FE_AE_STAT_1_S  stFEAeStat1;
    ISP_FE_AE_STAT_2_S  stFEAeStat2;
    ISP_FE_AE_STAT_3_S  stFEAeStat3;
    ISP_BE_AE_STAT_1_S  stBEAeStat1;
    ISP_BE_AE_STAT_2_S  stBEAeStat2;
    ISP_BE_AE_STAT_3_S  stBEAeStat3;
    ISP_AWB_STAT_1_S    stAwbStat1;
    ISP_AWB_STAT_2_S    stAwbStat2;
    ISP_FE_AF_STAT_S    stFEAfStat;
    ISP_BE_AF_STAT_S    stBEAfStat;
    isp_dp_stat         dp_stat;
    ISP_COMM_STAT_S     stCommStat;
    isp_dehaze_stat     dehaze_stat;
    ISP_MG_STAT_S       stMgStat;
    isp_flicker_stat    flicker_stat;
    isp_stitch_stat     stitch_stat;
    isp_ldci_stat       ldci_stat;
    hi_bool             be_update;
} isp_stat;
#define FLICKER_STAT_KEY_BIT            (18)

typedef union {
    hi_u64  key;
    struct {
        hi_u64  bit1FEAeGloStat     : 1 ;   /* [0] */
        hi_u64  bit1FEAeLocStat     : 1 ;   /* [1] */
        hi_u64  bit1FEAeStiGloStat  : 1 ;   /* [2] */
        hi_u64  bit1FEAeStiLocStat  : 1 ;   /* [3] */
        hi_u64  bit1BEAeGloStat     : 1 ;   /* [4] */
        hi_u64  bit1BEAeLocStat     : 1 ;   /* [5] */
        hi_u64  bit1BEAeStiGloStat  : 1 ;   /* [6] */
        hi_u64  bit1BEAeStiLocStat  : 1 ;   /* [7] */
        hi_u64  bit1AwbStat1        : 1 ;   /* [8] */
        hi_u64  bit1AwbStat2        : 1 ;   /* [9] */
        hi_u64  bit1AwbStat3        : 1 ;   /* [10] */
        hi_u64  bit1AwbStat4        : 1 ;   /* [11] */
        hi_u64  bit1FEAfStat        : 1 ;   /* [12] */
        hi_u64  bit1BEAfStat        : 1 ;   /* [13] */
        hi_u64  bit1DpStat          : 1 ;   /* [14] */
        hi_u64  bit1CommStat        : 1 ;   /* [15] */
        hi_u64  bit1Dehaze          : 1 ;   /* [16] */
        hi_u64  bit1MgStat          : 1 ;   /* [17] */
        hi_u64  bit1Flicker         : 1 ;   /* [18] */
        hi_u64  bit13Rsv            : 13;   /* [19:31] */
        hi_u64  bit32IsrAccess      : 32;   /* [32:63] */
    };
} isp_stat_key;

typedef struct {
    isp_stat_key  stat_key;
    hi_u64   phy_addr;
    hi_void *virt_addr;                 /* (isp_stat *) */
} isp_stat_info;

typedef struct hiISP_STAT_SHADOW_MEM_S {
    HI_U64  u64PhyAddr;
    HI_VOID *pVirtAddr;
    HI_U32 u32Size;
    HI_BOOL bUsrAccess;
} ISP_STAT_SHADOW_MEM_S;

/* the register config of isp */
typedef struct hiISP_AE_REG_CFG_1_S {
    HI_U8   au8MeteringHistThresh[4];
    HI_U8   au8WeightTable[15][17];
} ISP_AE_REG_CFG_1_S;

typedef struct hiISP_AE_REG_CFG_2_S {
    HI_U64  u64Exposure;
    HI_U64  u64ExposureSF;
    HI_U32  u32IntTime[4];  /* 0: Shortest shutter(SS), 1: Short shutter(S), 2: long shutter(L), 3: longest shutter(LL) */
    HI_U32  u32IspDgain;
    HI_U32  au32WDRGain[4];

    HI_BOOL bPirisValid;
    HI_S32  s32PirisPos;

    ISP_FSWDR_MODE_E enFSWDRMode;
} ISP_AE_REG_CFG_2_S;

typedef struct hiISP_AE_STATIC_CFG_S {
    HI_U8 u8FEEnable;
    HI_U8 u8BEEnable;

    // Overlap Configs
    HI_U16 u16FECropPosX;
    HI_U16 u16FECropPosY;
    HI_U16 u16FECropOutWidth;
    HI_U16 u16FECropOutHeight;

    HI_U16 u16BECropPosX;
    HI_U16 u16BECropPosY;
    HI_U16 u16BECropOutWidth;
    HI_U16 u16BECropOutHeight;
} ISP_AE_STATIC_CFG_S;

typedef struct hiISP_AE_DYNA_CFG_S {
    // ISP MPI Configs
    HI_U8 u8FEFourPlaneMode;
    HI_U8 u8FEHistOffsetX;
    HI_U8 u8FEHistOffsetY;
    HI_U8 u8FEHistSkipX;
    HI_U8 u8FEHistSkipY;

    HI_U8 u8BEAESel;
    HI_U8 u8BEFourPlaneMode;
    HI_U8 u8BEHistOffsetX;
    HI_U8 u8BEHistOffsetY;
    HI_U8 u8BEHistSkipX;
    HI_U8 u8BEHistSkipY;

    // WDR Configs
    HI_U8 u8FEGammaLimit;
    HI_U8 u8FEHistGammaMode;
    HI_U8 u8FEAverGammaMode;
    HI_U8 u8FEBitMove;

    HI_U8 u8BEGammaLimit;
    HI_U8 u8BEHistGammaMode;
    HI_U8 u8BEAverGammaMode;
    HI_U8 u8BEBitMove;

    // Weight Table Configs
    HI_U8 u8BEWightTableUpdate;
    HI_U8 u8BEWeightTableWidth;
    HI_U8 u8BEWeightTableHeight;
    HI_U8 au8BEWeightTable[15][17];

    HI_U8 u8FEWightTableUpdate;
    HI_U8 u8FEWeightTableWidth;
    HI_U8 u8FEWeightTableHeight;
    HI_U8 au8FEWeightTable[15][17];
} ISP_AE_DYNA_CFG_S;

typedef struct hiISP_AE_MODULE_REG_CFG_S {
    ISP_AE_STATIC_CFG_S stStaticRegCfg;
    ISP_AE_DYNA_CFG_S stDynaRegCfg;
} ISP_AE_REG_CFG_S;

typedef struct hiISP_HRS_STATIC_CFG_S {
    HI_BOOL bResh;
    HI_U8   u8Enable;
    HI_U8   u8RSEnable;
    HI_S16  as16HRSFilterLut0[6];
    HI_S16  as16HRSFilterLut1[6];
    HI_U16  u16Width;
    HI_U16  u16Height;
} ISP_HRS_STATIC_CFG_S;

typedef struct hiISP_HRS_DYNA_CFG_S {
    HI_U32 u32Reserved;
} ISP_HRS_DYNA_CFG_S;

typedef struct hiISP_HRS_MODULE_REG_CFG_S {
    ISP_HRS_STATIC_CFG_S stStaticRegCfg;
    ISP_HRS_DYNA_CFG_S stDynaRegCfg;
} ISP_HRS_REG_CFG_S;

typedef struct hiISP_MG_STATIC_CFG_S {
    HI_U8 u8Enable;

    // Overlap Configs
    HI_U16 u16CropPosX;
    HI_U16 u16CropPosY;
    HI_U16 u16CropOutWidth;
    HI_U16 u16CropOutHeight;
} ISP_MG_STATIC_CFG_S;

typedef struct hiISP_MG_DYNA_CFG_S {
    // WDR Configs
    HI_U8 u8GammaLimit;
    HI_U8 u8GammaMode;
    HI_U8 u8BitMove;

    // Zone Configs
    HI_U8 u8ZoneWidth;
    HI_U8 u8ZoneHeight;
} ISP_MG_DYNA_CFG_S;

typedef struct hiISP_MG_MODULE_REG_CFG_S {
    ISP_MG_STATIC_CFG_S stStaticRegCfg;
    ISP_MG_DYNA_CFG_S stDynaRegCfg;
} ISP_MG_REG_CFG_S;

typedef struct hiISP_DG_STATIC_CFG_S {
    HI_BOOL bResh;
} ISP_DG_STATIC_CFG_S;

typedef struct hiISP_DG_DYNA_CFG_S {
    HI_BOOL bResh;
    HI_U16  u16GainR;
    HI_U16  u16GainGR;
    HI_U16  u16GainGB;
    HI_U16  u16GainB;
    HI_U32  u32ClipValue;
} ISP_DG_DYNA_CFG_S;

typedef struct hiISP_FE_DG_DYNA_CFG_S {
    HI_BOOL bResh;
    HI_U16  au16GainR[4];
    HI_U16  au16GainGR[4];
    HI_U16  au16GainGB[4];
    HI_U16  au16GainB[4];
    HI_U32  u32ClipValue;
} ISP_FE_DG_DYNA_CFG_S;

typedef struct hiISP_DG_MODULE_REG_CFG_S {
    HI_BOOL bDgEn;
    ISP_DG_STATIC_CFG_S stStaticRegCfg;
    ISP_DG_DYNA_CFG_S   stDynaRegCfg;
} ISP_DG_REG_CFG_S;

typedef struct hiISP_FE_DG_MODULE_REG_CFG_S {
    HI_BOOL bDgEn;
    ISP_DG_STATIC_CFG_S stStaticRegCfg;
    ISP_FE_DG_DYNA_CFG_S   stDynaRegCfg;
} ISP_FE_DG_REG_CFG_S;

typedef struct hiISP_4DG_STATIC_CFG_S {
    HI_BOOL bResh;
    HI_U16 u16GainR0;
    HI_U16 u16GainGR0;
    HI_U16 u16GainGB0;
    HI_U16 u16GainB0;
    HI_U16 u16GainR1;
    HI_U16 u16GainGR1;
    HI_U16 u16GainGB1;
    HI_U16 u16GainB1;
    HI_U16 u16GainR2;
    HI_U16 u16GainGR2;
    HI_U16 u16GainGB2;
    HI_U16 u16GainB2;
    HI_U16 u16GainR3;
    HI_U16 u16GainGR3;
    HI_U16 u16GainGB3;
    HI_U16 u16GainB3;
} ISP_4DG_STATIC_CFG_S;

typedef struct hiISP_4DG_DYNA_CFG_S {
    HI_BOOL bResh;
    HI_U32 u32ClipValue0;
    HI_U32 u32ClipValue1;
    HI_U32 u32ClipValue2;
    HI_U32 u32ClipValue3;
} ISP_4DG_DYNA_CFG_S;

typedef struct hiISP_4DG_MODULE_REG_CFG_S {
    HI_BOOL bEnable;
    ISP_4DG_STATIC_CFG_S stStaticRegCfg;
    ISP_4DG_DYNA_CFG_S   stDynaRegCfg;
} ISP_4DG_REG_CFG_S;

typedef struct hiISP_AWB_REG_CFG_1_S {
    // BE DYNAMIC
    HI_U32  au32BEWhiteBalanceGain[ISP_BAYER_CHN_NUM];
    HI_U16  au16BEColorMatrix[9];
    HI_U16  u16BEMeteringWhiteLevelAwb;
    HI_U16  u16BEMeteringBlackLevelAwb;
    HI_U16  u16BEMeteringCrRefMaxAwb;
    HI_U16  u16BEMeteringCbRefMaxAwb;
    HI_U16  u16BEMeteringCrRefMinAwb;
    HI_U16  u16BEMeteringCbRefMinAwb;
    HI_U8   u8BEWbWorkEn;
    HI_U8   u8BECcEn;
    HI_BOOL u16BECcColortoneEn;
    HI_U16  u16BECcRGain;
    HI_U16  u16BECcGGain;
    HI_U16  u16BECcBGain;

    HI_U32 u32BECropPosX;
    HI_U32 u32BECropPosY;
    HI_U32 u32BEWidth;
    HI_U32 u32BEHeight;
    HI_U32 u32BECropOutHeight;
    HI_U32 u32BECropOutWidth;

    // FE DYNAMIC
    HI_U32 au32FEWhiteBalanceGain[4];
    HI_U8 u8FEWbWorkEn;
    HI_U32 au32WDRWBGain[ISP_BAYER_CHN_NUM];
    HI_U32 au32SensorWBGain[ISP_BAYER_CHN_NUM];
} ISP_AWB_REG_DYN_CFG_S;

typedef struct hiISP_AWB_REG_CFG_2_S {
    // BESTATIC
    HI_BOOL bBEAwbStaCfg;
    HI_U8   u8BEAwbBitmove;
    HI_U8   u8BEAwbWorkEn;
    HI_U8   u8BEWbWorkEn;

    HI_U32 u32BEAwbStatRaddr;
    HI_U8  u8BECcColortoneEn;
    HI_U8  u8BECcEn;

    HI_U32 u32BETopK;
    HI_U32 u32BETopB;
    HI_U32 u32BEBotK;
    HI_U32 u32BEBotB;

    HI_U32 u32BECcInDc0;
    HI_U32 u32BECcInDc1;
    HI_U32 u32BECcInDc2;
    HI_U32 u32BECcOutDc0;
    HI_U32 u32BECcOutDc1;
    HI_U32 u32BECcOutDc2;
    HI_U32 u32BEWbClipValue;

    HI_U16 u16BEAwbOffsetComp;

    // FESTATIC
    HI_BOOL bFEAwbStaCfg;
    HI_U8   u8FEWbWorkEn;

    HI_U32  u32FEClipValue;
    HI_BOOL bSensorGetWBGainEn;
    ISP_AWB_GAIN_SWITCH_E enAWBGainSwitch;
} ISP_AWB_REG_STA_CFG_S;

typedef struct hiISP_AWB_REG_CFG_3_S {
    // BE USER
    HI_BOOL bResh;
    HI_U32 u32UpdateIndex;
    ISP_AWB_SWITCH_E enBEAWBSwitch;
    HI_U16 u16BEZoneRow;
    HI_U16 u16BEZoneCol;
    HI_U16 u16BEZoneBin;
    HI_U16 u16BEMeteringBinHist0;
    HI_U16 u16BEMeteringBinHist1;
    HI_U16 u16BEMeteringBinHist2;
    HI_U16 u16BEMeteringBinHist3;
} ISP_AWB_REG_USR_CFG_S;

typedef struct hiISP_AWB_REG_CFG_S {
    ISP_AWB_REG_DYN_CFG_S        stAwbRegDynCfg;
    ISP_AWB_REG_STA_CFG_S        stAwbRegStaCfg;
    ISP_AWB_REG_USR_CFG_S        stAwbRegUsrCfg;
} ISP_AWB_REG_CFG_S;

typedef struct hiISP_AWB_SYNC_CFG_S {
    HI_U32  au32BEWhiteBalanceGain[ISP_BAYER_CHN_NUM];
    HI_U32  au32WDRWBGain[ISP_BAYER_CHN_NUM];
    HI_U16  au16ColorMatrix[CCM_MATRIX_SIZE];
    ISP_AWB_SWITCH_E enBEAWBSwitch;
    HI_U32  au32SensorWBGain[ISP_BAYER_CHN_NUM];
} ISP_AWB_SYNC_CFG_S;

typedef ISP_AWB_SYNC_CFG_S ISP_AWB_REG_CFG_2_S;

typedef struct {
    hi_u16  threshold_write;
    hi_u8   metrics_shift;
    hi_u8   np_offset;
    hi_bool af_enable;
    hi_bool iir0_enable0;
    hi_bool iir0_enable1;
    hi_bool iir0_enable2;
    hi_bool iir1_enable0;
    hi_bool iir1_enable1;
    hi_bool iir1_enable2;
    hi_isp_af_peak_mode peak_mode;
    hi_isp_af_squ_mode squ_mode;
    hi_bool offset_enable;
    hi_bool crop_enable;
    hi_bool lpf_enable;
    hi_bool mean_enable;
    hi_bool raw_mode;
    hi_bool iir0_ds_enable;
    hi_bool iir1_ds_enable;
    hi_bool fir0_lpf_enable;
    hi_bool fir1_lpf_enable;
    hi_bool iir0_ldg_enable;
    hi_bool iir1_ldg_enable;
    hi_bool fir0_ldg_enable;
    hi_bool fir1_ldg_enable;
    hi_bool update_mode;
    hi_bool update;

    hi_u8 bayer_mode;
    hi_u8 gamma;
    hi_u8 gain_limit;
    hi_u8 iir_plg_group0;
    hi_u8 iir_pls_group0;
    hi_u8 iir_plg_group1;
    hi_u8 iir_pls_group1;
    hi_u8 cnt_shift0_h;
    hi_u8 cnt_shift1_h;
    hi_u8 cnt_shift0_v;
    hi_u8 cnt_shift1_v;
    hi_u8 cnt_shift_y;
    hi_u8 iir_dilate0;
    hi_u8 iir_dilate1;
    hi_u8 hiligh_thre;
    hi_u8 af_pos_sel;
    hi_u8 iir0_shift;
    hi_u8 iir1_shift;

    hi_u16 mean_thres;
    hi_u16 window_vnum;
    hi_u16 window_hnum;
    hi_u16 iir_thre0_coring;
    hi_u16 iir_thre1_coring;
    hi_u16 iir_peak0_coring;
    hi_u16 iir_peak1_coring;
    hi_u16 fir_thre0_coring;
    hi_u16 fir_thre1_coring;
    hi_u16 fir_peak0_coring;
    hi_u16 fir_peak1_coring;
    hi_u16 iir_slope0_coring;
    hi_u16 iir_slope1_coring;
    hi_u16 fir_slope0_coring;
    hi_u16 fir_slope1_coring;
    hi_u16 offset_gr;
    hi_u16 offset_gb;
    hi_u16 input_v_size;
    hi_u16 input_h_size;
    hi_u16 iir0_shift_group0;
    hi_u16 iir1_shift_group0;
    hi_u16 iir2_shift_group0;
    hi_u16 iir3_shift_group0;
    hi_u16 iir0_shift_group1;
    hi_u16 iir1_shift_group1;
    hi_u16 iir2_shift_group1;
    hi_u16 iir3_shift_group1;
    hi_u16 iir_thre0_low;
    hi_u16 iir_thre0_high;
    hi_u16 iir_thre1_low;
    hi_u16 iir_thre1_high;
    hi_u16 iir_gain0_low;
    hi_u16 iir_gain0_high;
    hi_u16 iir_gain1_low;
    hi_u16 iir_gain1_high;
    hi_u16 iir_slope0_low;
    hi_u16 iir_slope0_high;
    hi_u16 iir_slope1_low;
    hi_u16 iir_slope1_high;
    hi_u16 fir_thre0_low;
    hi_u16 fir_thre0_high;
    hi_u16 fir_thre1_low;
    hi_u16 fir_thre1_high;
    hi_u16 fir_gain0_low;
    hi_u16 fir_gain0_high;
    hi_u16 fir_gain1_low;
    hi_u16 fir_gain1_high;
    hi_u16 fir_slope0_low;
    hi_u16 fir_slope0_high;
    hi_u16 fir_slope1_low;
    hi_u16 fir_slope1_high;
    hi_u16 acc_shift0_h;
    hi_u16 acc_shift1_h;
    hi_u16 acc_shift0_v;
    hi_u16 acc_shift1_v;
    hi_u16 acc_shift_y;
    hi_u16 crop_pos_y;
    hi_u16 crop_pos_x;
    hi_u16 shift_count_y;
    hi_u16 crop_v_size;
    hi_u16 crop_h_size;

    hi_s16 iir_gain0_group0;
    hi_s16 iir_gain0_group1;
    hi_s16 iir_gain1_group0;
    hi_s16 iir_gain1_group1;
    hi_s16 iir_gain2_group0;
    hi_s16 iir_gain2_group1;
    hi_s16 iir_gain3_group0;
    hi_s16 iir_gain3_group1;
    hi_s16 iir_gain4_group0;
    hi_s16 iir_gain4_group1;
    hi_s16 iir_gain5_group0;
    hi_s16 iir_gain5_group1;
    hi_s16 iir_gain6_group0;
    hi_s16 iir_gain6_group1;
    hi_s16 fir_h_gain0_group0;
    hi_s16 fir_h_gain0_group1;
    hi_s16 fir_h_gain1_group0;
    hi_s16 fir_h_gain1_group1;
    hi_s16 fir_h_gain2_group0;
    hi_s16 fir_h_gain2_group1;
    hi_s16 fir_h_gain3_group0;
    hi_s16 fir_h_gain3_group1;
    hi_s16 fir_h_gain4_group0;
    hi_s16 fir_h_gain4_group1;

    hi_u32 stat_read_addr;
    hi_u32 stat_read_data;
    hi_u32 update_index;
} isp_af_reg_cfg;

typedef struct hiISP_OFFSET_REG_CFG_S {
    HI_U16  au16BlackLevelOffset[4];
} ISP_OFFSET_REG_CFG_S;

typedef struct hiISP_DRC_STATIC_CFG_S {
    HI_BOOL bStaticResh;

    HI_BOOL bRchEn;
    HI_BOOL bWchEn;

    HI_BOOL bRdstatEn; // DRC read statistics enable, u1.0
    HI_BOOL bWrstatEn; // DRC write statistics enable, u1.0

    HI_BOOL bDetailBoostEnable;
    HI_BOOL bPdwSumEnable;
    HI_BOOL bWgtBoxTriSel;

    HI_U8   u8OffLineMode;  // Offline Mode, u1.0

    HI_U8   u8BinNumZ;       // DRC bin num

    HI_U8   u8BitWidthOut;  // Output bitwidth:12/20 select
    HI_U8   u8LocalEdgeLmt; // Limit of the difference between BLT filter's input & output, u8.0

    HI_U8   u8RWgt;         // u4.0
    HI_U8   u8GWgt;         // u5.0
    HI_U8   u8BWgt;         // u4.0

    HI_U8   u8ColorControlMode; // 0 - Linear mode, 1 - Powerlaw mode, u1.0
    HI_U8   u8ColorControlLUTCtrl;  // 0 - login LUT, 1 -logout LUT, u1.0
    HI_U16  u16GlobalColorCorr; // not in use now and should be fixed to 1024, u12.0

    HI_U8   u8PFRRGSlo;    // Purple Fringe Reduce, R/G slope control, u3.0
    HI_U8   u8PFRRGWid;    // Purple Fringe Reduce, R/G width, u7.0
    HI_U8   u8PFRRGCtr;    // Purple Fringe Reduce, R/G central point, u8.0

    HI_U8   u8PFRBGSlo;    // Purple Fringe Reduce, B/G slope control, u3.0
    HI_U8   u8PFRBGThr;    // Purple Fringe Reduce, B/G Threshold detection, u8.0

    HI_U8   u8UpdateMode;
    HI_U16  u16TimingStat;
    HI_U8   u8DrcUpdate;

    HI_BOOL bFirstFrame;
    HI_U8   u8ShpLog;
    HI_U8   u8ShpExp;
    HI_U32  u32DivDenomLog;
    HI_U32  u32DenomExp;
    HI_U32  au32PrevLuma[HI_ISP_DRC_EXP_COMP_SAMPLE_NUM];

    /* Regs for DRC STAT */
    HI_BOOL bDrcsEn; // DRCS enable (DRCS = DRC Statistics module in FE), u1.0
    HI_BOOL bDrcsVbiFltEn; // DRCS VBI filtering enable, u1.0
    HI_BOOL bDrcsWrtStatEn; // DRCS write statistics enable, u1.0
} ISP_DRC_STATIC_CFG_S;

typedef struct hiISP_DRC_DYNA_CFG_S {
    HI_BOOL bDynaResh;
    HI_BOOL bImgSizeChanged;
    HI_BOOL bVbifltEn; // DRC VBI filtering enable, u1.0
    HI_BOOL bColorCorrEnable;
    HI_U8 u8LutUpdateMode;

    HI_U16  u16StrengthBias;
    HI_U16  u16Strength;           // DRC strength
    HI_U16  u16LutMixCtrl;         // Mixing ctrl for LUT0 & LUT1
    HI_U16  u16AutoStrengthMax;     // Maximum strength in auto mode
    HI_U16  u16AutoStrengthMin;     // Minimum strength in auto mode

    HI_U16 au16ToneMappingValue0[200];
    HI_U16 au16ToneMappingDiff0[200];
    HI_U16 au16ToneMappingValue1[200];
    HI_U16 au16ToneMappingDiff1[200];

    HI_U8   u8PFRHighThr;   // Purple Fringe Reduce, High Threshold, u8.0

    HI_U16  u16VSize;  // Image Height
    HI_U16  u16HSize;  // Image Width
    HI_U8   u8OffLineMode;  // Offline Mode, u1.0

    HI_U8   u8BlockVNum;    // DRC block configure, Number, Vertical direction, u6.0
    HI_U8   u8BlockHNum;    // DRC block configure, Number, Horizontal direction, u6.0
    HI_U8   u8BlockChkY;    // DRC block configure, chk_y = vsize%num, u6.0
    HI_U8   u8BlockChkX;    // DRC block configure, chk_x = hsize%num, u6.0
    HI_U16  u16BlockVSize;  // DRC block configure, Size, Vertical direction, u9.0
    HI_U16  u16BlockHSize;  // DRC block configure, Size, Horizontal direction, u9.0

    HI_U16  u16DivY0;       // DRC block configure, div_y0 = 32768/vsize, u15.0
    HI_U16  u16DivX0;       // DRC block configure, div_x0 = 32768/hsize, u15.0
    HI_U16  u16DivY1;       // DRC block configure, div_y1 = 32768/vsize, u15.0
    HI_U16  u16DivX1;       // DRC block configure, div_x1 = 32768/hsize, u15.0
    HI_U8   u8BinScale;     // DRC bin scale (pixel weight), u3.0

    HI_U8   u8BigXInit; // DRC stripe configuration, initial is_big flag
    HI_U8   u8IdxXInit; // DRC stripe configuration, initial block index
    HI_U16  u16CntXInit; // DRC stripe configuration, initial pixel count
    HI_U16  u16AccXInit; // DRC stripe configuration, initial accumulation
    HI_U16  u16WgtXInit; // DRC stripe configuration, initial pixel position for calculating pixel weight
    HI_U16  u16TotalWidth; // DRC stripe configuration, total stripe width
    HI_U16  u16StatWidth; // DRC stripe configuration, active width for statistics

    HI_U8   u8DrcsBigXInit;
    HI_U8   u8DrcsIdxXInit;
    HI_U16  u16DrcsCntXInit;
    HI_U16  u16DrcsAccXInit;
    HI_U16  u16DrcsWgtXInit;
    HI_U16  u16DrcsTotalWidth;
    HI_U16  u16DrcsStatWidth;

    HI_U16  u16DrcsVSize;
    HI_U16  u16DrcsHSize;
} ISP_DRC_DYNA_CFG_S;

typedef struct hiISP_DRC_USR_CFG_S {
    HI_BOOL bUsrResh;
    HI_U16  u16Exposure;    // Exposure value, u16.0

    HI_U8   u8YSFT1;        // Used for dark region luminance gain limitation, u3.0
    HI_U8   u8YVAL1;        // Used for dark region luminance gain limitation, u7.0
    HI_U8   u8YSFT2;        // Used for dark region luminance gain limitation, u2.0
    HI_U8   u8YVAL2;        // Used for dark region luminance gain limitation, u4.0

    HI_U8   u8CSFT1;        // Used for dark region chroma gain limitation, u3.0
    HI_U8   u8CVAL1;        // Used for dark region chroma gain limitation, u7.0
    HI_U8   u8CSFT2;        // Used for dark region chroma gain limitation, u2.0
    HI_U8   u8CVAL2;        // Used for dark region chroma gain limitation, u4.0

    HI_U8   u8SFT;          // u3.0
    HI_U8   u8VAL;          // u7.0

    HI_U8   u8MixingCoring;    // u8.0
    HI_U8   u8MixingDarkMin;   // u8.0
    HI_U8   u8MixingDarkMax;   // u8.0
    HI_S8   s8MixingDarkSlo;   // u8.0
    HI_U8   u8MixingDarkThr;   // u8.0

    HI_U8   u8MixingBrightMin; // u8.0
    HI_U8   u8MixingBrightMax; // u8.0
    HI_S8   s8MixingBrightSlo; // u8.0
    HI_U8   u8MixingBrightThr; // u8.0

    HI_U8   u8GainClipKnee; // Startup for bright region gain limit, u4.0
    HI_U8   u8GainClipStep; // Region for bright region gain limit, u4.0

    HI_U8   u8PFRHighSlo;  // Purple Fringe Reduce, High Slope, u4.0
    HI_U8   u8PFRLowSlo;   // Purple Fringe Reduce, Low Slope, u4.0
    HI_U8   u8PFRLowThr;   // Purple Fringe Reduce, Low Slope, u8.0

    HI_U8   u8GradShift;   // Grad Reverse shift, u3.0
    HI_U8   u8GradSlope;   // Grad Reverse Slope, u4.0
    HI_U8   u8GradMax;     // Grad Reverse, max value, u7.0
    HI_U8   u8GradThr;     // Grad Reverse, Threshold Value, u8.0

    HI_U8   u8VarSpaCoarse;
    HI_U8   u8VarSpaMedium;
    HI_U8   u8VarSpaFine;

    HI_U8   u8VarRngCoarse;
    HI_U8   u8VarRngMedium;
    HI_U8   u8VarRngFine;

    HI_U8   au8BinMixCoarse[8];
    HI_U8   au8BinMixMedium[8];

    HI_U8   u8FltSpaCoarse;
    HI_U8   u8FltSpaMedium;
    HI_U8   u8FltSpaFine;
    HI_U8   u8FltRngCoarse;
    HI_U8   u8FltRngMedium;
    HI_U8   u8FltRngFine;

    HI_U8   u8DisThrCoefHigh;
    HI_U8   u8DisThrCoefLow;
    HI_U8   u8DisOffsetCoef;
    HI_U8   u8FltRngAdaMax;

    HI_S8   s8DetailSubFactor;

    HI_U16  u16PFRSum;         // May not be used in fw;
    HI_U16  u16CCLUT[HI_ISP_DRC_CC_NODE_NUM];     // Color Correction LUT
    HI_U8 u8ShpLog;
    HI_U8 u8ShpExp;
    HI_U32 u32DivDenomLog;
    HI_U32 u32DenomExp;

    /* New add for HDR mode */
    HI_U8   u8DetailCoring;
    HI_U8   u8DetailDarkStep;
    HI_U8   u8DetailBrightStep;

    HI_S8   s8DetailDarkSlo;
    HI_U8   u8DetailDarkThr;
    HI_U8   u8DetailDarkMax;
    HI_U8   u8DetailDarkMin;

    HI_S8   s8DetailBrightSlo;
    HI_U8   u8DetailBrightThr;
    HI_U8   u8DetailBrightMax;
    HI_U8   u8DetailBrightMin;

    HI_U8   u8DetailDarkCurve[16];
    HI_U8   u8DetailBrightCurve[16];

    /* Cubic TM Curve Related Parameters */
    HI_U16  u16CubicThres01;
    HI_U16  u16CubicThres10;
    HI_U16  u16CubicThres11;
    HI_BOOL bCubicCurveSel;

    HI_U8   u8CubicCoef00AExp;
    HI_U32  u32CubicCoef00A;
    HI_U8   u8CubicCoef01BExp;
    HI_U32  u32CubicCoef01B;
    HI_U8   u8CubicCoef02CExp;
    HI_U32  u32CubicCoef02C;
    HI_U32  u32CubicCoef03D;

    HI_U8   u8CubicCoef10AExp;
    HI_U32  u32CubicCoef10A;
    HI_U8   u8CubicCoef11BExp;
    HI_U32  u32CubicCoef11B;
    HI_U8   u8CubicCoef12CExp;
    HI_U32  u32CubicCoef12C;
    HI_U32  u32CubicCoef13D;

    HI_U8   u8CubicCoef20AExp;
    HI_U32  u32CubicCoef20A;
    HI_U8   u8CubicCoef21BExp;
    HI_U32  u32CubicCoef21B;
    HI_U8   u8CubicCoef22CExp;
    HI_U32  u32CubicCoef22C;
    HI_U32  u32CubicCoef23D;

    HI_U8   u8CubicCoef30AExp;
    HI_U32  u32CubicCoef30A;
    HI_U8   u8CubicCoef31BExp;
    HI_U32  u32CubicCoef31B;
    HI_U8   u8CubicCoef32CExp;
    HI_U32  u32CubicCoef32C;
    HI_U32  u32CubicCoef33D;
    HI_U32  u32UpdateIndex;
} ISP_DRC_USR_CFG_S;

typedef struct hiISP_DRC_SYNC_CFG_S {
    HI_U8  u8ShpLog;
    HI_U8  u8ShpExp;
    HI_BOOL bIsOfflineRepeatMode;
    HI_S32 as32PrevLumaDelta[HI_ISP_DRC_EXP_COMP_SAMPLE_NUM - 1];
} ISP_DRC_SYNC_CFG_S;

typedef ISP_DRC_SYNC_CFG_S ISP_DRC_REG_CFG_2_S;

typedef struct hiISP_DRC_REG_CFG_S {
    HI_BOOL bDrcEn;      // Enable
    ISP_DRC_STATIC_CFG_S stStaticRegCfg;
    ISP_DRC_DYNA_CFG_S   stDynaRegCfg;
    ISP_DRC_USR_CFG_S    stUsrRegCfg;
    ISP_DRC_SYNC_CFG_S   stSyncRegCfg;
} ISP_DRC_REG_CFG_S;

typedef struct hiISP_PREGAMMA_STATIC_CFG_S {
    HI_BOOL bStaticResh;
    HI_U8   u8BitDepthIn;
    HI_U8   u8BitDepthOut;
} ISP_PREGAMMA_STATIC_CFG_S;

typedef struct hiISP_PREGAMMA_DYNA_CFG_S {
    HI_BOOL bPreGammaLutUpdateEn;
    HI_U32  u32UpdateIndex;
    HI_U32  u32PreGammaLUT[PREGAMMA_NODE_NUM];  // u21.0
} ISP_PREGAMMA_DYNA_CFG_S;

typedef struct hiISP_PREGAMMA_REG_CFG_S {
    HI_BOOL bPreGammaEn;
    ISP_PREGAMMA_DYNA_CFG_S   stDynaRegCfg;
    ISP_PREGAMMA_STATIC_CFG_S stStaticRegCfg;
} ISP_PREGAMMA_REG_CFG_S;

typedef enum hiISP_DIS_DATA_TYPE_E {
    ISP_DIS_DATA_TYPE_YUV = 0,
    ISP_DIS_DATA_TYPE_RAW = 1,
    ISP_DIS_DATA_TYPE_BUTT
} ISP_DIS_DATA_TYPE_E;

#define DIS_STATS_NUM                   9
typedef struct hiISP_DIS_STATS_S {
    HI_S32 as32HDelta[DIS_STATS_NUM];
    HI_S32 as32HSad[DIS_STATS_NUM];
    HI_S32 as32HMv[DIS_STATS_NUM];
    HI_S32 as32VDelta[DIS_STATS_NUM];
    HI_S32 as32VSad[DIS_STATS_NUM];
    HI_S32 as32VMv[DIS_STATS_NUM];
    HI_U32 u32HMotion;
    HI_U32 u32VMotion;
    HI_U32 u32HOffset;
    HI_U32 u32VOffset;
} ISP_DIS_STATS_S;

typedef struct hiISP_DEHAZE_STATIC_CFG_S {
    HI_BOOL bResh;
    HI_U8  u8MaxMode;
    HI_U16 u16DehazeThld;

    HI_U16 u16DehazeBlthld;
    HI_U8  u8DehazeNegMode;

    HI_U16 u16BlockSum;
    HI_U8   u8Dchnum;
    HI_U8   u8Dcvnum;

    HI_U8   u8OfflineMode;
    HI_U8   u8UpdateMode;
    HI_U8   u8DehazeUpdate;
    HI_U16   u16fHblank;
    HI_U16   u16bHblank;
    HI_U16   u16fVblank;
    HI_U16   u16bVblank;
} ISP_DEHAZE_STATIC_CFG_S;

typedef struct hiISP_DEHAZE_DYNA_CFG_S {
    HI_BOOL bResh;
    HI_U8   u8Strength;
    HI_U16  u16AirR;
    HI_U16  u16AirG;
    HI_U16  u16AirB;
    HI_U8   u8Lut[256];
    HI_U32  prestat[DEFOG_ZONE_NUM / 2];
    HI_U32  u32Update;
    HI_U32  u32LutUpdate;

    HI_U32  u32Width;
    HI_U32  u32Height;
    HI_U16  u16Blockhsize;
    HI_U16  u16Blockvsize;
    HI_U32  u32phasex;
    HI_U32  u32phasey;
} ISP_DEHAZE_DYNA_CFG_S;

typedef struct hiISP_DEHAZE_REG_CFG_S {
    HI_BOOL   bDehazeEn;
    ISP_DEHAZE_DYNA_CFG_S   stDynaRegCfg;
    ISP_DEHAZE_STATIC_CFG_S stStaticRegCfg;
} ISP_DEHAZE_REG_CFG_S;

typedef struct hiISP_DPC_STATIC_CFG_S {
    HI_BOOL bStaticResh;
    HI_U8   u8DpccOutputMode;      // U4.0
    HI_U32  u32DpccBptCtrl;         // U12.0
} ISP_DPC_STATIC_CFG_S;

typedef struct hiISP_DPC_USR_STATIC_COR_CFG_S {
    HI_BOOL bResh;
    HI_U16  u16DpccBptNumber;
    HI_U32  u32UpdateIndex;
    HI_U32  au32DpccBpTable[STATIC_DP_COUNT_NORMAL];
} ISP_DPC_USR_STATIC_COR_CFG_S;

typedef struct hiISP_DPC_USR_DYNA_COR_CFG_S {
    HI_BOOL bResh;
    HI_BOOL bDpccHardThrEn;
    HI_S8   s8DpccSupTwinkleThrMax;
    HI_S8   s8DpccSupTwinkleThrMin;
    HI_U16  u16DpccRakeRatio;
} ISP_DPC_USR_DYNA_COR_CFG_S;

typedef struct hiISP_DPC_USR_CFG_S {
    ISP_DPC_USR_DYNA_COR_CFG_S   stUsrDynaCorRegCfg;
    ISP_DPC_USR_STATIC_COR_CFG_S stUsrStaCorRegCfg;
} ISP_DPC_USR_CFG_S;
typedef struct hiISP_DPC_DYNA_CFG_S {
    HI_BOOL bResh;
    HI_BOOL bDpcStatEn;
    HI_U8   u8DpccSetUse;           // U4.0
    HI_U8   u8DpccLineKerdiffFac;
    HI_U8   u8DpccBlendMode;
    HI_U8   u8DpccBitDepthSel;
    HI_U8   au8DpccLineStdThr[5];
    HI_U8   au8DpccLineDiffThr[5];
    HI_U8   au8DpccLineAverFac[5];
    HI_U16  u16DpccMode;            // U10.0
    HI_U16  u16DpccMethodsSet1;     // U13.0
    HI_U16  u16DpccMethodsSet2;     // U13.0
    HI_U16  u16DpccMethodsSet3;     // U13.0
    HI_U16  au16DpccLineThr[3];     // U16.0
    HI_U16  au16DpccLineMadFac[3];  // U14.0
    HI_U16  au16DpccPgFac[3];       // U14.0
    HI_U16  au16DpccRndThr[3];      // U16.0
    HI_U16  au16DpccRgFac[3];       // U14.0
    HI_U16  u16DpccRoLimits;        // U12.0
    HI_U16  u16DpccRndOffs;         // U12.0
    HI_U32  u32DpccBadThresh;       // U32.0
    HI_U32  u32DpccAlpha;           // U32.0
} ISP_DPC_DYNA_CFG_S;

typedef struct hiISP_DPC_MODULE_REG_CFG_S {
    HI_BOOL abDpcEn[4];
    HI_U8   u8ChnNum;
    ISP_DPC_STATIC_CFG_S    stStaticRegCfg;
    ISP_DPC_DYNA_CFG_S      stDynaRegCfg;
    ISP_DPC_USR_CFG_S       stUsrRegCfg;
} ISP_DPC_REG_CFG_S;

typedef struct hiISP_SHARPEN_STATIC_REG_CFG_S {
    HI_BOOL  bStaticResh;
    HI_U8    u8mfThdSelD;
    HI_U8    u8hfThdSelD;
    HI_U8    u8bEnChrSad;   //  1 bit
    HI_U8    u8dirVarScale;
    HI_U8    u8dirRly[2];
    HI_U16   u16oMaxChg;
    HI_U16   u16uMaxChg;
    HI_U8    u8oshtVarWgt1;
    HI_U8    u8ushtVarWgt1;
    HI_U8    u8oshtVarDiffWgt0;
    HI_U8    u8ushtVarDiffWgt0;
    HI_U8    u8oshtVarThd0;
    HI_U8    u8ushtVarThd0;
    HI_U8    u8shtVarSft;
    HI_U8    u8lmtMF[ISP_SHARPEN_FREQ_CORING_LENGTH];
    HI_U8    u8lmtHF[ISP_SHARPEN_FREQ_CORING_LENGTH];
    HI_U8    u8skinSrcSel;
    HI_U8    u8skinCntThd[2];
    HI_U8    u8skinEdgeSft;
    HI_U8    u8skinEdgeThd[2];
    HI_U16   u16skinAccumThd[2];
    HI_U8    u8skinAccumWgt[2];
    HI_U8    u8chrRVarSft;
    HI_U8    u8chrRVarScale;
    HI_U16   u16chrRScale;
    HI_U8    u8chrROri[2];
    HI_U8    u8chrRSft[4];
    HI_U8    u8chrRThd[2];
    HI_U8    u8chrRGain1;
    HI_U8    u8chrGVarSft;
    HI_U8    u8chrGVarScale;
    HI_U16   u16chrGScale;
    HI_U8    u8chrGOri[2];
    HI_U8    u8chrGSft[4];
    HI_U8    u8chrGThd[2];
    HI_U8    u8chrBVarSft;
    HI_U8    u8chrBVarScale;
    HI_U16   u16chrBScale;
    HI_U8    u8chrBOri[2];
    HI_U8    u8chrBSft[4];
    HI_U8    u8chrBThd[2];
    HI_U8    u8chrBGain1;
    HI_U16   u16chrGGain1;
    HI_U8    u8lumaSrcSel;
    //HI_U8    u8dirRt[2] ;
    HI_U8    u8shtNoiseMax;
    HI_U8    u8shtNoiseMin;
    HI_U8   bEnShp8Dir;
    HI_U8   bEnShpLowPow;
    HI_U8   u8hfGainSft;
    HI_U8   u8mfGainSft;
    HI_U8   u8lpfSel;
    HI_U8   u8hsfSel;
    HI_U8   u8shtVarSel;
    HI_U8   u8shtVar5x5Sft;
    HI_U8   u8detailThdSel;
    HI_U8   u8detailThdSft;
    HI_U8    u8skinCntMul;        // convert 31/(skin_cnt_th2-skin_cnt_th1) to multiplication.        // 5-bit unsigned integer    // U5.0
    HI_S16   s16skinEdgeMul;       // convert (skin_edge_wgt2-skin_edge_wgt1)/(skin_edge_th2-skin_edge_th1) to multiplication.      // 9-bit signed integer // S5.4
    HI_S16   s16skinAccumMul;      // convert (skin_accum_wgt2-skin_accum_wgt1)/(skin_accum_th2-skin_accum_th1) to multiplication.  // 8-bit signed integer // S5.3

} ISP_SHARPEN_STATIC_REG_CFG_S;

typedef struct hiISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S {

    HI_BOOL  bResh;
    HI_U8    u8mfThdSftD;
    HI_U8    u8mfThdSelUD;
    HI_U8    u8mfThdSftUD;
    HI_U8    u8hfThdSftD;
    HI_U8    u8hfThdSelUD;
    HI_U8    u8hfThdSftUD;
    HI_U8    u8dirVarSft;
    HI_U8    u8selPixWgt;
    HI_U8    u8oshtVarWgt0;
    HI_U8    u8ushtVarWgt0;
    HI_U8    u8oshtVarDiffThd[2];
    HI_U8    u8oshtVarDiffWgt1;
    HI_U8    u8ushtVarDiffThd[2];
    HI_U8    u8ushtVarDiffWgt1;
    HI_S16   s16oshtVarDiffMul;    // Convert division to multiplication.           // 12-bit signed integer   // S8.4
    HI_S16   s16ushtVarDiffMul;    // Convert division to multiplication.           // 12-bit signed integer // S8.4
} ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S;

typedef struct hiISP_SHARPEN_MPI_DYNA_REG_CFG_S {
    HI_BOOL  bResh;
    HI_U16   u16mfGainD[SHRP_GAIN_LUT_SIZE];
    HI_U16   u16mfGainUD[SHRP_GAIN_LUT_SIZE];
    HI_U16   u16hfGainD[SHRP_GAIN_LUT_SIZE];
    HI_U16   u16hfGainUD[SHRP_GAIN_LUT_SIZE];
    HI_U8    u8oshtAmt;
    HI_U8    u8ushtAmt;

    HI_U8    u8bEnChrCtrl;
    HI_U8    u8chrRGain0;
    HI_U16   u16chrGGain0;
    HI_U8    u8chrBGain0;
    HI_U8    u8bEnSkinCtrl;
    HI_U8    u8skinEdgeWgt[2];

    HI_U16   u16oMaxGain;   // 11bit
    HI_U16   u16uMaxGain;   // 11bit
    HI_U8    u8detailOshtThr[2];
    HI_U8    u8detailUshtThr[2];
    HI_U16   u16skinMaxU;
    HI_U16   u16skinMinU;
    HI_U16   u16skinMaxV;
    HI_U16   u16skinMinV;

    HI_U8    u8bEnShtCtrlByVar;
    HI_U8    u8shtBldRt;
    HI_U8    u8oshtVarThd1;
    HI_U8    u8ushtVarThd1;
    HI_U8    u8bEnLumaCtrl;
    HI_BOOL  bEnDetailCtrl;
    HI_U8    u8detailOshtAmt;
    HI_U8    u8detailUshtAmt;
    HI_U8    au8LumaWgt[ISP_SHARPEN_LUMA_NUM];
    HI_U8    u8dirDiffSft;
    HI_U16   u16oshtVarMul;        // Convert division to multiplication.           // 11-bit unsigned integer // U7.4
    HI_U16   u16ushtVarMul;        // Convert division to multiplication.           // 11-bit unsigned integer // U7.4
    HI_S16   s16detailOshtMul;
    HI_S16   s16detailUshtMul;
    HI_U8    u8dirRt[2] ;
    HI_S16   s16chrRMul;           //                                                         // 9-bit signed integer     // S5.4
    HI_S16   s16chrGMul;           //                                                         // 9-bit signed integer     // S5.4
    HI_S16   s16chrBMul;           //                                                         // 9-bit signed integer     // S5.4
    HI_S16   s16skinEdgeMul;       // convert (skin_edge_wgt2-skin_edge_wgt1)/(skin_edge_th2-skin_edge_th1) to multiplication.      // 9-bit signed integer // S5.4
    HI_U32   u32UpdateIndex;
} ISP_SHARPEN_MPI_DYNA_REG_CFG_S;

typedef struct hiISP_SHARPEN_DYNA_REG_CFG_S {
    ISP_SHARPEN_MPI_DYNA_REG_CFG_S          stMpiDynaRegCfg;
    ISP_SHARPEN_DEFAULT_DYNA_REG_CFG_S      stDefaultDynaRegCfg;
} ISP_SHARPEN_DYNA_REG_CFG_S;

typedef struct hiISP_SHARPEN_REG_CFG_S {
    HI_BOOL  bEnable;
    ISP_SHARPEN_DYNA_REG_CFG_S          stDynaRegCfg;
    ISP_SHARPEN_STATIC_REG_CFG_S        stStaticRegCfg;
} ISP_SHARPEN_REG_CFG_S;

typedef struct hiISP_EDGEMARK_REG_CFG_S {
    HI_U8   bEnable;
    HI_U8   u8markEdgeSft;
    HI_U8   u8MarkEdgeThd;
    HI_U16  u16uMarkValue;
    HI_U16  u16vMarkValue;
    HI_U32   u32UpdateIndex;
} ISP_EDGEMARK_REG_CFG_S;

typedef struct hiISP_GE_STATIC_CFG_S {
    HI_BOOL bStaticResh;
    HI_BOOL bGeGrGbEn;          // u1.0
    HI_BOOL bGeGrEn;            // u1.0
    HI_BOOL bGeGbEn;            // u1.0
    HI_U8   u8GeNumV;

} ISP_GE_STATIC_CFG_S;

typedef struct hiISP_GE_USR_CFG_S {
    HI_BOOL bResh;
    HI_U8   u8GeNumH;
    HI_U8   au8GeCtSlope1[4];   // u4.0    0x9
    HI_U8   au8GeCtSlope2[4];   // u4.0    0x9
    HI_U16  au16GeCtTh2[4];     // u14.0   0x200

    HI_U16  u16GeCropPosX;      // u16.0
    HI_U16  u16GeCropPosY;      // u16.0
    HI_U16  u16GeCropOutWidth;  // u16.0
    HI_U16  u16GeCropOutHeight; // u16.0
} ISP_GE_USR_CFG_S;

typedef struct hiISP_GE_DYNA_CFG_S {
    HI_BOOL bResh;
    HI_U16  au16GeCtTh1[4];     // u14.0   0x400
    HI_U16  au16GeCtTh3[4];     // u14.0   0x200
    HI_U16  u16GeStrength;      // u9.0  0x80
} ISP_GE_DYNA_CFG_S;

typedef struct hiISP_GE_MODULE_REG_CFG_S {
    HI_BOOL abGeEn[4];
    HI_U8   u8ChnNum;
    ISP_GE_STATIC_CFG_S     stStaticRegCfg;
    ISP_GE_DYNA_CFG_S       stDynaRegCfg;
    ISP_GE_USR_CFG_S        stUsrRegCfg;
} ISP_GE_REG_CFG_S;

typedef struct hiISP_MCDS_STATIC_REG_CFG_S {
    HI_BOOL bStaticResh;
    HI_BOOL bHcdsEn;        // UV Horizontal DS enable; 0: close; 1: open

    HI_S32  s32HCoef0;
    HI_S32  s32HCoef1;
    HI_S32  s32HCoef2;
    HI_S32  s32HCoef3;
    HI_S32  s32HCoef4;
    HI_S32  s32HCoef5;
    HI_S32  s32HCoef6;
    HI_S32  s32HCoef7;
    HI_S32  s32VCoef0;
    HI_S32  s32VCoef1;
    HI_U16  u16CoringLimit;
    HI_U8   u8MidfBldr;
} ISP_MCDS_STATIC_REG_CFG_S;

typedef struct hiISP_MCDS_DYNA_REG_CFG_S {
    HI_BOOL bDynaResh;
    HI_BOOL bMidfEn;        // 0: Med Filter close; 1: Med Filter open
    HI_BOOL bVcdsEn;        // UV vertical DS enable; 0: close; 1: open
} ISP_MCDS_DYNA_REG_CFG_S;

typedef struct hiISP_MCDS_REG_CFG_S {
    HI_BOOL bMCDSen;
    ISP_MCDS_STATIC_REG_CFG_S stStaticRegCfg;
    ISP_MCDS_DYNA_REG_CFG_S   stDynaRegCfg;

} ISP_MCDS_REG_CFG_S;
#define  NLUT_LENGTH       129
#define  WLUT_LENGTH       33
#define  WDR_LUT_LENGTH    128
#define  WDR_CHL_CHNLN     5

#define  WDR_FRAME0        0
#define  WDR_FRAME1        1
#define  WDR_FRAME2        2
#define  WDR_FRAME3        3

#define  WDR_CHL_Rg        0
#define  WDR_CHL_Gr        1
#define  WDR_CHL_Gb        2
#define  WDR_CHL_Bg        3
#define  WDR_CHL_MAX       4

typedef struct hiISP_FSWDR_STATIC_CFG_S {
    HI_BOOL bResh;
    HI_BOOL bGrayScaleMode;
    HI_BOOL bNrNosMode;                             // u1.0,  noise mode
    HI_BOOL bSaveBLC;
    HI_BOOL bFirstFrame;

    HI_U8   u8BitDepthInvalid;                      // u4.0, {12,14}

    HI_U8   u8bldrLHFIdx;                           // u5.0, [0,16]
    HI_U8   u8MaskSimilarThr;                       // u4.0, [0,15],
    HI_U8   u8MaskSimilarCnt;                       // u3.0, [0,7]
    HI_U16  au16ExpoValue[WDR_CHL_MAX];             // u8.6, [0,16383]
    HI_U16  u32MaxRatio;                            // u17.0, [0,131071]
    HI_U16  u16dftWgtFL;                            // u9.0, [0,256]
    HI_U16  u16FusionRLowThr;                       // u14.0,[0,16383]  u16FusionRLowThr <= u16FusionRHigThr
    HI_U16  u16FusionRHigThr;                       // u14.0,[0,16383]
    HI_U16  au16ExpoRRatio[WDR_CHL_MAX - 1];        // u10.0,[0,1023]
    HI_U32  au32BlcComp[WDR_CHL_MAX - 1];           // s27.0,[0,67108863]
    HI_U16  u16SaturateThr;             // u14.0,[0,16383]
} ISP_FSWDR_STATIC_CFG_S;

typedef struct hiISP_FSWDR_USR_CFG_S {
    HI_BOOL bResh;
    HI_BOOL bWDRBnr;                    // u1.0
    HI_BOOL bFusionMode;                // u1.0,  Fusion mode
    HI_BOOL bFusionBnr;                 // u1.0,  Fusion Bnr;
    HI_BOOL bShortExpoChk;              // u1.0, [0,1]
    HI_U8   u8MdtLBld;                  // u8.0, [0,254],the amount of long-expo is added in motion mask
    HI_U8   u8BnrFullMdtThr;            // u8.0, [0,254]
    HI_U8   u8BnrWgtG0;                 // u3.0, [0,7]
    HI_U8   u8BnrWgtG1;                 // u3.0, [0,7]
    HI_U8   u8BnrWgtG2;                 // u3.0, [0,7]
    HI_U8   u8BnrWgtC0;                 // u3.0, [0,7]
    HI_U8   u8BnrWgtC1;                 // u3.0, [0,7]
    HI_U8   u8BnrWgtC2;                 // u3.0, [0,7]
    HI_U8   u8MdtStillThr;              // u8.0, [0,254],less than this threshold, motion mask will be 0; u8MdtStillThr <= u8MdtFullThr
    HI_U8   u8MdtFullThr;               // u8.0, [0,254],more than this threshold, motion mask will look for the next one
    HI_U8   u8FullMotSigWgt;            // u5.0, [0,31]
    HI_U16  au16FusionThr[WDR_CHL_MAX]; // u14.0,[0,16383]
    HI_U32  u32UpdateIndex;
    HI_U8   u8Gsigma_gain2;             // u4.0, [0,255]
    HI_U8   u8Gsigma_gain3;             // u4.0, [0,255]
    HI_U8   u8Csigma_gain2;             // u4.0, [0,255]
    HI_U8   u8Csigma_gain3;             // u4.0, [0,255]
    HI_U8   u8Gsigma_gain1;             // u4.0, [0,255]
    HI_U8   u8Csigma_gain1;             // u4.0, [0,255]
} ISP_FSWDR_USR_CFG_S;

typedef struct hiISP_FSWDR_DYNA_CFG_S {
    HI_BOOL bResh;
    HI_BOOL bWDRMdtEn;                              // u1.0,  motion detection switch
    HI_BOOL bUpdateNosLut;
    HI_U8   u8MdThrLowGain;                         // u4.2, [0,63]
    HI_U8   u8MdThrHigGain;                         // u4.2, [0,63]
    HI_U8   u8SqrtAgainG;                           // u3.0, [0,6]
    HI_U8   u8SqrtDgainG;                           // u3.0, [0,6]
    HI_U8   u8MdtNosFloor;                          // u7.0, [0,127]
    HI_U16  u16ShortThr;                            // u14.0,[0,16383],the threshold of beginning to adopt short exposure
    HI_U16  u16LongThr;                             // u14.0,[0,16383],below the threshold will adopt long exposure  u16LongThr <= u16ShortThr
    HI_U16  au16StillThr[WDR_CHL_MAX - 1];          // u14.0,[0,16383]
    HI_U16  u16NosFloorG;                           // u12.0,[0,4095]
    HI_U16  u16NosFloorR;                           // u12.0,[0,4095]
    HI_U16  u16NosFloorB;                           // u12.0,[0,4095]
    HI_U16  u16ModelCoefGgain;                      // u12.0,[0,4095]
    HI_U16  u16ModelCoefRgain;                      // u13.0,[0,8191]
    HI_U16  u16ModelCoefBgain;                      // u13.0,[0,8191]
    HI_U16  u16NosNpThr;                            // u13.0,[0,8191]
    HI_U16  u16TNosFloor;                           // u12.0,[0,4095]
    HI_S32  as32BnrNosMDTLut[NLUT_LENGTH];         // u8.0, [0,128]  129 *8bit
    HI_BOOL bErosionEn;                             // u1.0, [0,1]

    HI_BOOL bBcomEn;
    HI_BOOL bBdecEn;
    HI_U8   u8bcom_alpha;
    HI_U8   u8bdec_alpha;
    HI_U8   u8FrmMerge;
} ISP_FSWDR_DYNA_CFG_S;

typedef struct hiISP_FSWDR_SYNC_CFG_S {
    HI_BOOL bFusionMode;
    HI_BOOL bWDRMdtEn;
    HI_U16  u16ShortThr;
    HI_U16  u16LongThr;
    HI_U16  u16Offset0;
    HI_U16  u16Offset1;
    HI_U16  u16Offset2;
} ISP_FSWDR_SYNC_CFG_S;

typedef struct hiISP_WDR_REG_CFG_S {
    HI_BOOL bWDREn;
    ISP_FSWDR_STATIC_CFG_S stStaticRegCfg;
    ISP_FSWDR_DYNA_CFG_S   stDynaRegCfg;
    ISP_FSWDR_USR_CFG_S    stUsrRegCfg;
    ISP_FSWDR_SYNC_CFG_S   stSyncRegCfg;
} ISP_WDR_REG_CFG_S;

typedef struct hiISP_SPLIT_STATIC_CFG_S {
    HI_BOOL bResh;
    HI_U8   u8BitDepthIn;
    HI_U8   u8BitDepthOut;
    HI_U8   u8ModeIn;
    HI_U8   u8ModeOut;
    HI_U16  au16WdrSplitLut[129];
} ISP_SPLIT_STATIC_CFG_S;

typedef struct hiISP_SPLIT_CFG_S {
    HI_BOOL bEnable;
    ISP_SPLIT_STATIC_CFG_S stStaticRegCfg;
} ISP_SPLIT_CFG_S;

typedef struct hiISP_FLICKER_STATIC_CFG_S {
    HI_BOOL bResh;
} ISP_FLICKER_STATIC_CFG_S;

typedef struct hiISP_FLICKER_DYNA_CFG_S {
    HI_BOOL bResh;
    HI_U8   u8MergeFrm;         // U3
    HI_U16  u16OverThr;         // U10
    HI_U32  u32OverCntThr;      // U26
    HI_U16  u16ExpRatio0;       // U8.6      isp.c
    HI_U16  u16ExpRatio1;       // U8.6      isp.c
    HI_U16  u16ExpRatio2;       // U8.6      isp.c
    HI_S16  s16GrAvgPre;        // S11       rw
    HI_S16  s16GbAvgPre;        // S11       rw
    HI_U32  u32CntOverPre;      // U26       rw
} ISP_FLICKER_DYNA_CFG_S;

typedef struct hiISP_FLICKER_REG_CFG_S {
    HI_BOOL bEnable;
    ISP_FLICKER_STATIC_CFG_S stStaticRegCfg;
    ISP_FLICKER_DYNA_CFG_S   stDynaRegCfg;
} ISP_FLICKER_REG_CFG_S;

typedef struct hiISP_FE_LSC_STATIC_CFG_S {
    HI_BOOL bResh;
    HI_U8   u8WinNumH;
    HI_U8   u8WinNumV;
    HI_U16  u16WidthOffset;
} ISP_FE_LSC_STATIC_CFG_S;

typedef struct hiISP_FE_LSC_USR_CFG_S {
    HI_BOOL bResh;
    HI_BOOL bLutUpdate;
    HI_U8   u8MeshScale;
    HI_U16  u16MeshStr;
    HI_U16  u16Weight;
    HI_U16  au16DeltaX[HI_ISP_LSC_GRID_COL - 1];
    HI_U16  au16DeltaY[(HI_ISP_LSC_GRID_ROW - 1) / 2];
    HI_U16  au16InvX[HI_ISP_LSC_GRID_COL - 1];
    HI_U16  au16InvY[(HI_ISP_LSC_GRID_ROW - 1) / 2];
    HI_U32  au32RGain[HI_ISP_LSC_GRID_POINTS];
    HI_U32  au32GrGain[HI_ISP_LSC_GRID_POINTS];
    HI_U32  au32GbGain[HI_ISP_LSC_GRID_POINTS];
    HI_U32  au32BGain[HI_ISP_LSC_GRID_POINTS];
} ISP_FE_LSC_USR_CFG_S;

typedef struct hiISP_FE_LSC_MODULE_REG_CFG_S {
    HI_BOOL bLscEn;
    ISP_FE_LSC_STATIC_CFG_S stStaticRegCfg;
    ISP_FE_LSC_USR_CFG_S    stUsrRegCfg;
} ISP_FE_LSC_REG_CFG_S;

typedef struct hiISP_FE_LOGLUT_STATIC_CFG_S {
    HI_BOOL bStaticResh;
    HI_U8   u8BitDepthOut;
    HI_U32  au32LogLUT[PRE_LOG_LUT_SIZE];
} ISP_FE_LOGLUT_STATIC_CFG_S;

typedef struct hiISP_FE_LOGLUT_DYNA_CFG_S {
    HI_BOOL bEnable;
} ISP_FE_LOGLUT_DYNA_CFG_S;

typedef struct hiISP_FE_LOGLUT_MODULE_REG_CFG_S {
    ISP_FE_LOGLUT_STATIC_CFG_S  stStaticRegCfg;
    ISP_FE_LOGLUT_DYNA_CFG_S    stDynaRegCfg;
} ISP_FE_LOGLUT_REG_CFG_S;

typedef struct hiISP_LOGLUT_STATIC_CFG_S {
    HI_BOOL bStaticResh;
    HI_U8   u8BitDepthOut;
    HI_U32  au32LogLUT[LOG_LUT_SIZE];
} ISP_LOGLUT_STATIC_CFG_S;

typedef struct hiISP_LOGLUT_DYNA_CFG_S {
    HI_BOOL bEnable;
} ISP_LOGLUT_DYNA_CFG_S;

typedef struct hiISP_LOGLUT_MODULE_REG_CFG_S {
    ISP_LOGLUT_STATIC_CFG_S  stStaticRegCfg;
    ISP_LOGLUT_DYNA_CFG_S    stDynaRegCfg;
} ISP_LOGLUT_REG_CFG_S;

typedef struct {
    HI_BOOL bStaticResh;
} ISP_RC_STATIC_CFG_S;

typedef struct {
    HI_BOOL bResh;
    HI_U16  u16CenterVerCoor;
    HI_U16  u16CenterHorCoor;
    HI_U32  u32SquareRadius;
} ISP_RC_USR_CFG_S;

typedef struct hiISP_RADIAL_CROP_REG_CFG_S {
    HI_BOOL bRcEn;
    ISP_RC_STATIC_CFG_S stStaticRegCfg;
    ISP_RC_USR_CFG_S    stUsrRegCfg;
} ISP_RADIAL_CROP_REG_CFG_S;

typedef struct hiISP_LSC_STATIC_CFG_S {
    HI_BOOL bStaticResh;
    HI_U8   u8WinNumH;
    HI_U8   u8WinNumV;
} ISP_LSC_STATIC_CFG_S;

typedef struct hiISP_LSC_USR_CFG_S {
    HI_BOOL bCoefUpdate;
    HI_BOOL bLutUpdate;
    HI_U8   u8MeshScale;
    HI_U16  u16Weight;
    HI_U16  u16MeshStr;
    HI_U16  u16WidthOffset;
    HI_U16  au16DeltaX[HI_ISP_LSC_GRID_COL - 1];
    HI_U16  au16DeltaY[(HI_ISP_LSC_GRID_ROW - 1) / 2];
    HI_U16  au16InvX[HI_ISP_LSC_GRID_COL - 1];
    HI_U16  au16InvY[(HI_ISP_LSC_GRID_ROW - 1) / 2];
    HI_U32  au32RGain[HI_ISP_LSC_GRID_POINTS];
    HI_U32  au32GrGain[HI_ISP_LSC_GRID_POINTS];
    HI_U32  au32GbGain[HI_ISP_LSC_GRID_POINTS];
    HI_U32  au32BGain[HI_ISP_LSC_GRID_POINTS];
    HI_U32  u32UpdateIndex;
} ISP_LSC_USR_CFG_S;

typedef struct hiISP_LSC_MODULE_REG_CFG_S {
    HI_BOOL bLscEn;
    HI_BOOL bLutRegFin;
    ISP_LSC_USR_CFG_S       stUsrRegCfg;
    ISP_LSC_STATIC_CFG_S    stStaticRegCfg;
} ISP_LSC_REG_CFG_S;

typedef struct hiISP_RLSC_STATIC_CFG_S {
    HI_BOOL bStaticResh;

    HI_U16  u16NodeNum;

} ISP_RLSC_STATIC_CFG_S;

typedef struct hiISP_RLSC_USR_CFG_S {
    HI_BOOL bUsrResh;
    HI_BOOL bRadialCropEn;
    HI_BOOL bRLscFuncEn;
    HI_BOOL bCoefUpdate;
    HI_BOOL bLutUpdate;

    HI_U16  u16WidthOffset;

    HI_U16  u16CenterRX;
    HI_U16  u16CenterRY;
    HI_U16  u16CenterGrX;
    HI_U16  u16CenterGrY;
    HI_U16  u16CenterGbX;
    HI_U16  u16CenterGbY;
    HI_U16  u16CenterBX;
    HI_U16  u16CenterBY;

    HI_U16  u16OffCenterR;
    HI_U16  u16OffCenterGr;
    HI_U16  u16OffCenterGb;
    HI_U16  u16OffCenterB;

    HI_U32  u32ValidRadius;

    HI_U16  u16GainStr;
    HI_U8   u8GainScale;

    HI_U32  au32Lut0Chn0[HI_ISP_RLSC_POINTS + 1];
    HI_U32  au32Lut0Chn1[HI_ISP_RLSC_POINTS + 1];
    HI_U32  au32Lut1Chn2[HI_ISP_RLSC_POINTS + 1];
    HI_U32  au32Lut1Chn3[HI_ISP_RLSC_POINTS + 1];
    HI_U32  u32UpdateIndex;

} ISP_RLSC_USR_CFG_S;

typedef struct hiISP_RLSC_MODULE_REG_CFG_S {
    HI_BOOL bRLscEn;
    ISP_RLSC_USR_CFG_S      stUsrRegCfg;
    ISP_RLSC_STATIC_CFG_S   stStaticRegCfg;
} ISP_RLSC_REG_CFG_S;

typedef struct hiISP_LOCAL_CAC_STATIC_CFG_S {
    HI_BOOL bStaticResh;
    HI_BOOL bNddmCacBlendEn;
    HI_U8   u8RCounterThr;
    HI_U8   u8GCounterThr;
    HI_U8   u8BCounterThr;
    HI_U16  u16SatuThr;
    HI_U16  u16NddmCacBlendRate;
} ISP_LOCAL_CAC_STATIC_CFG_S;

typedef struct hiISP_LOCAL_CAC_USR_CFG_S {
    HI_BOOL bResh;
    HI_U8   u8LumaHighCntThr;    // u8.0, [0,153]
    HI_U8   u8CbCntHighThr;      // u7.0, [0,68]
    HI_U8   u8CbCntLowThr;       // u7.0, [0,68]
    HI_U8   u8BldAvgCur;         // u4.0, [0, 8]
    HI_U16  u16CbThr;            // u12.0, [0,4095]
    HI_U16  u16VarThr;
    HI_S16  s16CbCrRatioLmtHigh;
    HI_S16  s16CbCrRatioLmtLow;
} ISP_LOCAL_CAC_USR_CFG_S;

typedef struct hiISP_LOCAL_CAC_DYNA_CFG_S {
    HI_BOOL bResh;
    HI_U8   u8DePurpleCtrCr;
    HI_U8   u8DePurpleCtrCb;
    HI_U16  u16LumaThr;          // u12.0, [0,4095]
    HI_U16  u16RLumaThr;
    HI_U16  u16GLumaThr;
    HI_U16  u16BLumaThr;
} ISP_LOCAL_CAC_DYNA_CFG_S;

typedef struct hiISP_LOCAL_CAC_MODULE_REG_CFG_S {
    HI_BOOL bLocalCacEn;
    ISP_LOCAL_CAC_USR_CFG_S    stUsrRegCfg;
    ISP_LOCAL_CAC_DYNA_CFG_S   stDynaRegCfg;
    ISP_LOCAL_CAC_STATIC_CFG_S stStaticRegCfg;
} ISP_LCAC_REG_CFG_S;

typedef struct hiISP_GLOBAL_CAC_STATIC_CFG_S {
    HI_BOOL bStaticResh;
    HI_BOOL bGCacVerFilEn;
    HI_U8   u8GCacBlendSel;
    HI_U8   u8GcacChrVerMode;
    HI_U8   u8GcacClipModeHor;
    HI_U8   u8GcacClipModeVer;
} ISP_GLOBAL_CAC_STATIC_CFG_S;

typedef struct hiISP_GLOBAL_CAC_USR_CFG_S {
    HI_BOOL bResh;
    HI_U8   u8VerNormShift;
    HI_U8   u8VerNormFactor;
    HI_U8   u8HorNormShift;
    HI_U8   u8HorNormFactor;
    HI_U16  u16StartCoorHor;
    HI_U16  u16StartCoorVer;
    HI_S16  s16ParamRedA;
    HI_S16  s16ParamRedB;
    HI_S16  s16ParamRedC;
    HI_S16  s16ParamBlueA;
    HI_S16  s16ParamBlueB;
    HI_S16  s16ParamBlueC;
    HI_U16  u16CenterCoorHor;
    HI_U16  u16CenterCoorVer;
    HI_U16  u16CorVarThr;
    HI_U32  u32UpdateIndex;
} ISP_GLOBAL_CAC_USR_CFG_S;

typedef struct hiISP_GLOBAL_CAC_DYNA_CFG_S {
    HI_U32 u32Reserved;
} ISP_GLOBAL_CAC_DYNA_CFG_S;

typedef struct hiISP_GLOBAL_CAC_MODULE_REG_CFG_S {
    HI_BOOL bGlobalCacEn;
    ISP_GLOBAL_CAC_USR_CFG_S    stUsrRegCfg;
    ISP_GLOBAL_CAC_DYNA_CFG_S   stDynaRegCfg;
    ISP_GLOBAL_CAC_STATIC_CFG_S stStaticRegCfg;
} ISP_GCAC_REG_CFG_S;

typedef struct hiISP_DEMOSAIC_DARKREGION_PROT_S {
    HI_BOOL bCcmProtEn;
    HI_U32 u32DataCCThd0;
    HI_U32 u32DataCCThd1;
    HI_U16 u16CCProtRatio;
    HI_U32 u32DataRRThd0;
    HI_U32 u32DataRRThd1;
    HI_U32 u32DataGGThd0;
    HI_U32 u32DataGGThd1;
    HI_U32 u32DataBBThd0;
    HI_U32 u32DataBBThd1;
    HI_U32 u32MaxRGBThd;
    HI_U16 u16RGBProtRatio;
} ISP_DEMOSAIC_DARKREGION_PROT_S;

typedef struct hiISP_DEMOSAIC_STATIC_CFG {
    HI_BOOL bResh;
    HI_BOOL bFcsEnable;         // u1.0
    HI_BOOL bAHDEnable;         // u1.0
    HI_BOOL bDeFakeEnable;      // u1.0
    HI_BOOL bAlphaFilter;       // u1.0
    HI_BOOL bAptFltEn;          // u1.0
    HI_U8   u8GClipBitSft;      // u4.0,
    HI_U8   u8hvBlendLimit1;    // u8.0,
    HI_U8   u8hvBlendLimit2;    // u8.0,
    HI_U8   u8hvColorRatio;     // u8.0,
    HI_U8   u8hvSelection;      // u2.0
    HI_U8   u8BldrGrGb;         // u4.0,   apt gb-gr blend ratio
    HI_U8   u8MultiMFRed;       // u5.0,   usm mf-Red gain
    HI_U8   u8MultiMFBlue;      // u5.0,   usm mf-Blue gain,
    HI_U8   u8DitherMask;       // u8.0,
    HI_U8   u8DitherRatio;      // u8.0,
    HI_U8   u8ShtCtrlGain;      // u4.0,
    HI_U8   u8ClipDeltaGain;    // u8.0,
    HI_U8   u8ClipAdjustMax;    // u8.0,
    HI_U8   u8FcrScale;         // u4.0,   fcr demoire scale
    HI_U8   u8BldrGFStr;        // u5.0,  gf r-g-b strength
    HI_U8   u8CxVarMaxRate;     // u4.0,
    HI_U8   u8CxVarMinRate;     // u4.0,
    HI_U16  u16hvBlendRatio1;   // u9.0,
    HI_U16  u16hvBlendRatio2;   // u9.0,

    HI_U16  u16AhdPart1;        // u10.0,
    HI_U16  u16AhdPart2;        // u8.0,
    HI_U16  u16GFThLow;         // u9.0,   gf low-fix strength
    HI_U16  u16GFThHig;         // u9.0,   gf high-fix strength
    HI_U16  u16ClipUSM;         // u12.0,  usm clip
    HI_U16  u16SatuThFix;       // u9.0,   usm fix-high gain for saturation
    HI_U16  u16SatuThLow;       // u9.0,   usm low thresh for saturation
    HI_U16  u16SatuThHig;       // u9.0,   usm high thresh for saturation
    HI_U16  u16GrayThLow;       // u9.0,   usm low thresh for gray
    HI_U16  u16GrayThHig;       // u9.0,   usm high thresh for gray
    HI_U16  u16GrayThFixLow;    // u9.0,   usm fix-low gain for gray
    HI_U16  u16GrayThFixHig;    // u9.0,   usm fix-high gain for gray
    HI_U16  u16FcrLimitLow;     // u12.0,  fcr demoire low clip
    HI_U16  u16FcrLimitHigh;    // u12.0,  fcr demoire high clip
    HI_U16  u16ShtCtrlTh;       // u12.0,
    HI_U16  u16ClipRUdSht;      // u12.0,
    HI_U16  u16ClipROvSht;      // u12.0,
    HI_U16  u16ClipBUdSht;      // u12.0,
    HI_U16  u16ClipBOvSht;      // u12.0,
    HI_U16  u16CbCrAvgThr;      // u12.0,
} ISP_DEMOSAIC_STATIC_CFG_S;

typedef struct hiISP_DEMOSAIC_DYNA_CFG {
    HI_BOOL bResh;
    HI_BOOL bUpdateGF;
    HI_BOOL bUpdateUsm;

    HI_U8   u8Lpff0;           // s4.0,
    HI_U8   u8Lpff1;           // u5.0,
    HI_U8   u8Lpff2;           // u5.0,
    HI_U8   u8Lpff3;           // u8.0,
    HI_U8   u8CcHFMaxRatio;    // u5.0,
    HI_U8   u8CcHFMinRatio;    // u5.0,
    HI_U8   u8RJumpLow;        // u8.0
    HI_U8   u8RJumpHigh;       // u8.0
    HI_U8   u8GJumpMaxThres;   // u8.0
    HI_U8   u8GRLevel;         // u8.0
    HI_U8   u8GRSlope;         // u4.0
    HI_U8   u8BldrCbCr;        // u5.0,  gf cr-cb strength
    HI_U8   u8AwbGFGainLow;    // u3.4,
    HI_U8   u8AwbGFGainHig;    // u3.4,
    HI_U8   u8AwbGFGainMax;    // u4.0,
    HI_U8   u8DitherMax;       // u8.0,
    HI_U8   u8MultiMF;         // u3.2,
    HI_U8   u8FcrGFGain;       // s3.2,   fcr control
    HI_U8   u8BldrGray;        // u5.0, 0~16
    HI_U8   u8FilterStrIntp;   // u6.0,  [0 16]
    HI_U8   u8FilterStrFilt;   // u6.0,  [0 16]

    HI_U16  u16FakeCrVarThrLow;
    HI_U16  u16FakeCrVarThrHigh;
    HI_U16  u16FcrDetLow;          // u12.0,  fcr det thresh
    HI_U16  u16ClipDeltaIntpLow;   // u12.0,
    HI_U16  u16ClipDeltaIntpHigh;  // u12.0,
    HI_U16  u16ClipDeltaFiltLow;   // u12.0,
    HI_U16  u16ClipDeltaFiltHigh;  // u12.0,
    HI_U16  u16SatuRThFix;         // u9.0,
    HI_U16  u16SatuRThLow;         // u9.0,
    HI_U16  u16SatuRThHig;         // u9.0,
    HI_U16  u16SatuBThFix;         // u9.0,
    HI_U16  u16SatuBThLow;         // u9.0,
    HI_U16  u16SatuBThHig;         // u9.0,
    HI_S16  s16SatuFixEhcY;        // s10.0,
    HI_U16  u16hfIntpBldLow;       // u9.0,
    HI_U16  u16hfIntpBldHig;       // u9.0,
    HI_U16  u16hfIntpThLow;        // u10.0,
    HI_U16  u16hfIntpThHig;        // u10.0,
    HI_U16  u16hfIntpThLow1;       // u10.0,
    HI_U16  u16hfIntpThHig1;       // u10.0,
    HI_U32  u32hfIntpRatio;        // u10.0,
    HI_U32  u32hfIntpRatio1;       // s9.8,

    HI_U8   au8EhcGainLut[HI_ISP_DEMOSAIC_LUT_LENGTH]; // u8.0,
    HI_U16  au16GFBlurLut[HI_ISP_DEMOSAIC_LUT_LENGTH]; // u12.0,
    HI_U8   u8ColorNoiseThdY;    // u8.0
    HI_U8   u8ColorNoiseStrY;    // u8.0
    ISP_DEMOSAIC_DARKREGION_PROT_S stProDarkRegion;
} ISP_DEMOSAIC_DYNA_CFG_S;

typedef struct hiISP_DEMOSAIC_MODULE_REG_CFG {
    HI_BOOL bVhdmEnable;       // u1.0,
    HI_BOOL bNddmEnable;       // u1.0,
    ISP_DEMOSAIC_STATIC_CFG_S stStaticRegCfg;
    ISP_DEMOSAIC_DYNA_CFG_S   stDynaRegCfg;
} ISP_DEMOSAIC_REG_CFG_S;

typedef struct hiISP_ANTIFALSECOLOR_STATIC_CFG {
    HI_BOOL bResh;
    HI_U16  u16FcrLimit1;      // u12.0,
    HI_U16  u16FcrLimit2;      // u12.0,
    HI_U16  u16FcrThr;         // u12.0,
} ISP_ANTIFALSECOLOR_STATIC_CFG_S;

typedef struct hiISP_ANTIFALSECOLOR_DYNA_CFG {
    HI_BOOL bResh;
    HI_U8   u8FcrGain;         // u5.0,
    HI_U8   u8FcrRatio;        // u6.0,
    HI_U8   u8FcrGrayRatio;    // u5.0,
    HI_U8   u8FcrCmaxSel;      // u4.0,
    HI_U8   u8FcrDetgSel;      // u4.0,
    HI_U16  u16FcrHfThreshLow; // u10.0,  fcr hf thresh low
    HI_U16  u16FcrHfThreshHig; // u10.0,  fcr hf thresh high
} ISP_ANTIFALSECOLOR_DYNA_CFG_S;

typedef struct hiISP_ANTIFALSECOLOR_MODULE_REG_CFG {
    HI_BOOL bFcrEnable;        // u1.0,
    ISP_ANTIFALSECOLOR_STATIC_CFG_S stStaticRegCfg;
    ISP_ANTIFALSECOLOR_DYNA_CFG_S   stDynaRegCfg;
} ISP_ANTIFALSECOLOR_REG_CFG_S;

typedef struct hiISP_CA_USR_CFG {
    HI_BOOL bResh;
    HI_BOOL bCaCpEn;    // u1.0
    HI_BOOL bCaLutUpdateEn; // u1.0
    HI_U16  u16CaLumaThrHigh; // u10.0
    HI_U16  u16CaLumaRatioHigh; // U1.10   High Lightness Thresh' Ratio in Y-Alpha
    HI_U32  au32YRatioLUT[HI_ISP_CA_YRATIO_LUT_LENGTH];
    HI_U32  u32UpdateIndex;
} ISP_CA_USR_CFG_S;

typedef struct hiISP_CA_DYNA_CFG {
    HI_BOOL bResh;
    HI_U16  u16CaISORatio; // 1.10bit  ISO Ratio  For UV,Max = 2047 FW Limi
} ISP_CA_DYNA_CFG_S;


typedef struct hiISP_CA_STATIC_CFG {
    HI_BOOL bStaticResh;
    HI_BOOL bCaLlhcProcEn;  // u1.0
    HI_BOOL bCaSkinProcEn;  // u1.0
    HI_BOOL bCaSatuAdjEn;   // u1.0
    HI_U16  u16CaLumaThrLow;        // u10.0
    HI_U16  u16CaDarkChromaThrLow;  // u10.0
    HI_U16  u16CaDarkChromaThrHigh; // u10.0
    HI_U16  u16CaSDarkChromaThrLow; // u10.0
    HI_U16  u16CaSDarkChromaThrHigh; // u10.0
    HI_U16  u16CaLumaRatioLow;      // U1.10
    HI_S16  s16CaYuv2RgbCoef00;     // s5.10
    HI_S16  s16CaYuv2RgbCoef01;     // s5.10
    HI_S16  s16CaYuv2RgbCoef02;     // s5.10
    HI_S16  s16CaYuv2RgbCoef10;     // s5.10
    HI_S16  s16CaYuv2RgbCoef11;     // s5.10
    HI_S16  s16CaYuv2RgbCoef12;     // s5.10
    HI_S16  s16CaYuv2RgbCoef20;     // s5.10
    HI_S16  s16CaYuv2RgbCoef21;     // s5.10
    HI_S16  s16CaYuv2RgbCoef22;     // s5.10
    HI_S16  s16CaYuv2RgbInDc0;      // S11.0
    HI_S16  s16CaYuv2RgbInDc1;      // S11.0
    HI_S16  s16CaYuv2RgbInDc2;      // S11.0
    HI_S16  s16CaYuv2RgbOutDc0;     // S11.0
    HI_S16  s16CaYuv2RgbOutDc1;     // S11.0
    HI_S16  s16CaYuv2RgbOutDc2;     // S11.0
    HI_U16  u16CaSkinLowLumaMinU;   // u10
    HI_U16  u16CaSkinLowLumaMaxU;   // u10
    HI_U16  u16CaSkinLowLumaMinUy;  // u10
    HI_U16  u16CaSkinLowLumaMaxUy;  // u10
    HI_U16  u16CaSkinHighLumaMinU;  // u10
    HI_U16  u16CaSkinHighLumaMaxU;  // u10
    HI_U16  u16CaSkinHighLumaMinUy; // u10
    HI_U16  u16CaSkinHighLumaMaxUy; // u10
    HI_U16  u16CaSkinLowLumaMinV;   // u10
    HI_U16  u16CaSkinLowLumaMaxV;   // u10
    HI_U16  u16CaSkinLowLumaMinVy;  // u10
    HI_U16  u16CaSkinLowLumaMaxVy;  // u10
    HI_U16  u16CaSkinHighLumaMinV;  // u10
    HI_U16  u16CaSkinHighLumaMaxV;  // u10
    HI_U16  u16CaSkinHighLumaMinVy; // u10
    HI_U16  u16CaSkinHighLumaMaxVy; // u10
    HI_S16  s16CaSkinUvDiff;        // S11
    HI_U16  u16CaSkinRatioThrLow;   // u12.0
    HI_U16  u16CaSkinRatioThrMid;   // u12.0
    HI_U16  u16CaSkinRatioThrHigh;  // u12.0
} ISP_CA_STATIC_CFG_S;

typedef struct hiISP_CA_MODULE_REG_CFG {
    HI_BOOL bCaEn;
    ISP_CA_STATIC_CFG_S stStaticRegCfg;
    ISP_CA_DYNA_CFG_S   stDynaRegCfg;
    ISP_CA_USR_CFG_S    stUsrRegCfg;
} ISP_CA_MODULE_REG_CFG_S;

typedef struct hiISP_BAS_REG_CFG {
    HI_U32  u32BasMode;                              /* RW,Range: [ 0x0, 0x1FFFFF]   */
    HI_U32  u32BasOutWidth;                          /* RW,Range: [ 0x0, 0x1FFFFFFF] */
    HI_U32  u32BasOutHeight;                         /* RW,Range: [ 0x0, 0x1FFFFFFF] */
    HI_U32  u32BasIncH;                              /* RW,Range: [ 0x0, 0x1FFFFFFF] */
    HI_U32  u32BasIncV;                              /* RW,Range: [ 0x0, 0x1FFFFFFF] */
    HI_U32  u32RphWeiH;                              /* RW,Range: [ 0x0, 0x3FFFFF]   */
    HI_U32  u32RphWeiV;                              /* RW,Range: [ 0x0, 0x3FFFFF]   */
    HI_U32  au32BasTapsH6LutL[ISP_BAS_TAPS_ROW_NUM]; /* RW,Range: [ 0x0, 0xFFFFF]    */
    HI_U32  au32BasTapsH6LutH[ISP_BAS_TAPS_ROW_NUM]; /* RW,Range: [ 0x0, 0xFFFFF]    */
    HI_U32  au32BasTapsV6LutL[ISP_BAS_TAPS_ROW_NUM]; /* RW,Range: [ 0x0, 0xFFFFF]    */
    HI_U32  au32BasTapsV6LutH[ISP_BAS_TAPS_ROW_NUM]; /* RW,Range: [ 0x0, 0xFFFFF]    */
    HI_U32  au32BasTapsV4LutL[ISP_BAS_TAPS_ROW_NUM]; /* RW,Range: [ 0x0, 0x7FFF]     */
    HI_U32  au32BasTapsV4LutH[ISP_BAS_TAPS_ROW_NUM]; /* RW,Range: [ 0x0, 0x7FFF]     */
} ISP_BAS_REG_CFG_S;

typedef struct hiISP_BAYERNR_STATIC_CFG_S {
    HI_BOOL  bResh;
    HI_BOOL  bBnrDetailEnhanceEn;               // u1.0
    HI_BOOL  bSkipEnable;                       // u1.0
    HI_BOOL  bSkipLevel1Enable;                 // u1.0
    HI_BOOL  bSkipLevel3Enable;                 // u1.0
    HI_U8    u8JnlmSel;                         // u2.0
    HI_U8    u8BnrDePosClip;                    // u7.0
    HI_U8    u8BnrDeNegClip;                    // u7.0
    HI_U8    u8WtiSvalThr;                      // u8.0,
    HI_U8    u8WtiDvalThr;                      // u8.0,
    HI_U8    u8WtiCoefMid;                      // u1.7,
    HI_S16   s16WtiDenomOffset;                 // s8.0,
    HI_U16   u16WtiCoefMax;                     // u2.7,
    HI_U16   u16RLmtBlc;                        // u5.0
    HI_U16   u16JnlmMaxWtCoef;                  // u10.0
    HI_U16   u16BnrDeBlcValue;                  // u12.0
} ISP_BAYERNR_STATIC_CFG_S;

typedef struct hiISP_BAYERNR_DYNA_CFG_S {
    HI_BOOL bResh;
    HI_BOOL bMedcEnable;                        // u1.0
    HI_BOOL bTriSadEn;                          // u1.0
    HI_BOOL bSkipLevel2Enable;                  // u1.0
    HI_BOOL bSkipLevel4Enable;                  // u1.0
    HI_BOOL bBnrLutUpdateEn;                    // u1.0
    HI_U8   au8BnrCRatio[ISP_BAYER_CHN_NUM];    // u6.0
    HI_U8   au8AmedMode[ISP_BAYER_CHN_NUM];     // u1.0
    HI_U8   au8AmedLevel[ISP_BAYER_CHN_NUM];    // u2.0
    HI_U8   u8JnlmSymCoef;                      // u8.0
    HI_U8   u8JnlmGain;                         // u8.0
    HI_U16  u16LmtNpThresh;                     // u15.0
    HI_U16  u16JnlmCoringHig;                   // u14.0
    HI_U16  u16RLmtRgain;                       // u10.0
    HI_U16  u16RLmtBgain;                       // u10.0
    HI_U16  au16JnlmCoringLowLUT[HI_ISP_BAYERNR_LUT_LENGTH]; // u14.0
    HI_U8   au8JnlmLimitLut[HI_ISP_BAYERNR_LMTLUTNUM];      // u8.0
    HI_U32  au32JnlmLimitMultGain[2][ISP_BAYER_CHN_NUM];    // u25.0
} ISP_BAYERNR_DYNA_CFG_S;

typedef struct hiISP_BAYERNR_USR_CFG_S {
    HI_BOOL bResh;
    HI_BOOL bBnrLscEn;                          // u1.0
    HI_BOOL bBnrMonoSensorEn;                   // u1.0
    HI_U8   u8BnrLscRatio;                      // u8.0
} ISP_BAYERNR_USR_CFG_S;

typedef struct hiISP_BAYERNR_MODULE_REG_CFG_S {
    HI_BOOL bBnrEnable;                             // u1.0
    ISP_BAYERNR_STATIC_CFG_S    stStaticRegCfg;
    ISP_BAYERNR_DYNA_CFG_S      stDynaRegCfg;
    ISP_BAYERNR_USR_CFG_S       stUsrRegCfg;
} ISP_BAYERNR_REG_CFG_S;

typedef struct hiISP_LDCI_STATIC_CFG_S {
    HI_BOOL bEnable;
    HI_BOOL bCalcDrcEnable;
    HI_BOOL bStatDrcEnable;
    HI_BOOL bDeEnable;
    HI_BOOL bCalcDeRef;
    HI_BOOL bCalcPfLmtEnable;
    HI_BOOL bDrcLutUpdate;
    HI_BOOL bStaticResh;

    HI_U32  u32CalcLumaSel;
    HI_U32  u32DehLpfSft;
    HI_S16  as16DehLpfCoefH[3];
    HI_S16  as16DehLpfCoefV[2];
    HI_U32  u32CalcPfmul;
    HI_U32  u32CalcPfOri[2];
    HI_S16  as16CalcPfSft[4];
    HI_U32  u32CalcPfThd[2];
    HI_U32  u32CalcPfRly[2];
    HI_U32  u32GlobalHeWgt;
    HI_U32  u32LpfSft;
    HI_U32  u32ChrPosDamp;
    HI_U32  u32ChrNegDamp;
    HI_S16  as16StatPolyP2[65];
    HI_S16  as16StatPolyP3[65];
    HI_S16  as16PolyP1[65];
    HI_S16  as16PolyQ0[65];
    HI_S16  as16PolyQ1[65];
    HI_S16  as16PolyQ2[65];
    HI_S16  as16PolyQ3[65];
    HI_S16  as16StatDynRngCmpLut[65];
    HI_S16  as16CalcDynRngCmpLut[65];
} ISP_LDCI_STATIC_CFG_S;

typedef struct hiISP_LDCI_DYNA_CFG_S {
    HI_BOOL bEnable;
    HI_BOOL bWrstatEn;
    HI_BOOL bRdstatEn;
    HI_BOOL bCalcEnable;
    HI_BOOL bCalcLutUpdate;

    HI_U32  u32CalcBlcCtrl;
    HI_U32  u32HePosLut[LDCI_HE_LUT_SIZE];
    HI_U32  u32HeNegLut[LDCI_HE_LUT_SIZE];
    HI_U32  u32UsmPosLut[LDCI_DE_USM_LUT_SIZE];
    HI_U32  u32UsmNegLut[LDCI_DE_USM_LUT_SIZE];
    HI_U32  u32ColorGainLut[LDCI_COLOR_GAIN_LUT_SIZE];
    HI_U32  u32DeLut[LDCI_DE_USM_LUT_SIZE];
    HI_U32  u32LpfCoef[LDCI_LPF_LUT_SIZE];

    HI_U32  u32CalcMapOffsetX;
    HI_U32  u32CalcSmlMapStride;
    HI_U32  u32CalcSmlMapHeight;
    HI_U32  u32CalcTotalZone;
    HI_U32  u32CalcScaleX;
    HI_U32  u32CalcScaleY;
    HI_U32  u32BlkSmlMapWidth[3];
    HI_U32  u32StatVStart;
    HI_U32  u32StatVEnd;
    HI_U32  u32StatHStart;
    HI_U32  u32StatHEnd;
    HI_U32  u32StatSmlMapWidth;
    HI_U32  u32StatSmlMapHeight;
    HI_U32  u32StatTotalZone;
    isp_ldci_stat stTprStat;
} ISP_LDCI_DYNA_CFG_S;

typedef struct hiISP_LDCI_REG_CFG {
    ISP_LDCI_STATIC_CFG_S stStaticRegCfg;
    ISP_LDCI_DYNA_CFG_S   stDynaRegCfg;
} ISP_LDCI_REG_CFG_S;

typedef struct hiISP_FPN_STATIC_CFG {

} ISP_FPN_STATIC_CFG_S;

typedef struct hiISP_FPN_DYNA_CFG {
    HI_U32  u32IspFpnCalibCorr;
    HI_U32  u32IspFpnOverflowThr;
    HI_U32  u32IspFpnStrength[4];
} ISP_FPN_DYNA_CFG_S;

typedef struct hiISP_FPN_REG_CFG {
    ISP_FPN_STATIC_CFG_S  stStaticRegCfg;
    ISP_FPN_DYNA_CFG_S   stDynaRegCfg;
} ISP_FPN_REG_CFG_S;

#define HI_ISP_CLUT_LUT0                (3087)
#define HI_ISP_CLUT_LUT1                (2871)
#define HI_ISP_CLUT_LUT2                (2871)
#define HI_ISP_CLUT_LUT3                (2664)
#define HI_ISP_CLUT_LUT4                (2871)
#define HI_ISP_CLUT_LUT5                (2664)
#define HI_ISP_CLUT_LUT6                (2664)
#define HI_ISP_CLUT_LUT7                (2475)

typedef struct hiISP_CLUT_STATIC_CFG_S {
    HI_BOOL  bResh;
    HI_U32   u32Size;
} ISP_CLUT_STATIC_CFG_S;

typedef struct hiISP_CLUT_USR_COEF_CFG_S {
    HI_BOOL bResh;
    HI_U32  u32UpdateIndex;
    HI_U32 au32lut0[HI_ISP_CLUT_LUT0];   /* RW; Range:[0,4294967295];Format:32.0; */
    HI_U32 au32lut1[HI_ISP_CLUT_LUT1];   /* RW; Range:[0,4294967295];Format:32.0; */
    HI_U32 au32lut2[HI_ISP_CLUT_LUT2];   /* RW; Range:[0,4294967295];Format:32.0; */
    HI_U32 au32lut3[HI_ISP_CLUT_LUT3];   /* RW; Range:[0,4294967295];Format:32.0; */
    HI_U32 au32lut4[HI_ISP_CLUT_LUT4];   /* RW; Range:[0,4294967295];Format:32.0; */
    HI_U32 au32lut5[HI_ISP_CLUT_LUT5];   /* RW; Range:[0,4294967295];Format:32.0; */
    HI_U32 au32lut6[HI_ISP_CLUT_LUT6];   /* RW; Range:[0,4294967295];Format:32.0; */
    HI_U32 au32lut7[HI_ISP_CLUT_LUT7];   /* RW; Range:[0,4294967295];Format:32.0; */
} ISP_CLUT_USR_COEF_CFG_S;

typedef struct hiISP_CLUT_USR_CTRL_CFG_S {
    HI_BOOL         bResh;
    HI_BOOL         bEnable;
    HI_BOOL         bDemoEnable; /* 0-debug closed; 1-debug open, the left screen is original video and the right screen is CLUT-processed video */
    HI_BOOL         bDemoMode;
    HI_U32          u32DemoPos;
    HI_U32          u32GainR; /* [0,512] */
    HI_U32          u32GainG; /* [0,512] */
    HI_U32          u32GainB; /* [0,512] */
} ISP_CLUT_USR_CTRL_CFG_S;

typedef struct hiISP_CLUT_USR_CFG_S {
    ISP_CLUT_USR_COEF_CFG_S     stClutUsrCoefCfg;
    ISP_CLUT_USR_CTRL_CFG_S     stClutUsrCtrlCfg;
} ISP_CLUT_USR_CFG_S;

typedef struct hiISP_CLUT_REG_CFG_S {
    HI_BOOL bEnable;
    ISP_CLUT_STATIC_CFG_S    stStaticRegCfg;
    ISP_CLUT_USR_CFG_S       stUsrRegCfg;
} ISP_CLUT_REG_CFG_S;

typedef struct hiISP_GAMMA_STATIC_CFG_S {
    HI_BOOL  bStaticResh;
} ISP_GAMMA_STATIC_CFG_S;

typedef struct hiISP_GAMMA_USR_CFG_S {
    HI_BOOL bResh;
    HI_BOOL bGammaLutUpdateEn;
    HI_U16  au16GammaLUT[GAMMA_NODE_NUM];  // u12.0
    HI_U32  u32UpdateIndex;
} ISP_GAMMA_USR_CFG_S;

typedef struct hiISP_GAMMA_REG_CFG_S {
    HI_BOOL bGammaEn;
    ISP_GAMMA_STATIC_CFG_S stStaticRegCfg;
    ISP_GAMMA_USR_CFG_S    stUsrRegCfg;
} ISP_GAMMA_REG_CFG_S;

typedef struct hiISP_CSC_DYNAMIC_CFG_S {
    // Multiplier coefficients
    HI_BOOL bResh;

    HI_S16  s16CscCoef00;    // s5.10
    HI_S16  s16CscCoef01;    // s5.10
    HI_S16  s16CscCoef02;    // s5.10
    HI_S16  s16CscCoef10;    // s5.10
    HI_S16  s16CscCoef11;    // s5.10
    HI_S16  s16CscCoef12;    // s5.10
    HI_S16  s16CscCoef20;    // s5.10
    HI_S16  s16CscCoef21;    // s5.10
    HI_S16  s16CscCoef22;    // s5.10

    // DC components
    HI_S16  s16CscInDC0;     // s11.0
    HI_S16  s16CscInDC1;     // s11.0
    HI_S16  s16CscInDC2;     // s11.0

    HI_S16  s16CscOutDC0;    // s11.0
    HI_S16  s16CscOutDC1;    // s11.0
    HI_S16  s16CscOutDC2;    // s11.0
} ISP_CSC_DYNA_CFG_S;

typedef struct hiISP_CSC_REG_CFG_S {
    HI_BOOL bEnable;       // u1.0
    ISP_CSC_DYNA_CFG_S    stDynaRegCfg;
} ISP_CSC_REG_CFG_S;

typedef struct {
    hi_bool blc_in;
    hi_bool blc_out;
} isp_blc_static_cfg;

typedef struct {
    hi_u16 blc[4];
    hi_u16 out_blc;
} isp_blc_usr_cfg;

typedef struct {
    isp_blc_static_cfg  static_reg_cfg;
    isp_blc_usr_cfg     usr_reg_cfg;
} isp_blc_reg_cfg;

typedef struct {
    hi_bool resh_static;
    hi_bool resh_dyna;
    hi_u16 dg_blc_cfg_save[ISP_BAYER_CHN_NUM];
    hi_u16 wdr_blc_cfg_save[ISP_WDR_CHN_MAX][ISP_BAYER_CHN_NUM];
    hi_u16 wdr_blc_out_cfg_save;

    /* BE module */
    isp_blc_reg_cfg lsc_blc;
    isp_blc_reg_cfg rlsc_blc;
    isp_blc_reg_cfg dg_blc;
    isp_blc_reg_cfg wdr_blc[ISP_WDR_CHN_MAX];
    isp_blc_reg_cfg wb_blc;
    isp_blc_reg_cfg wdr_dg_blc[ISP_WDR_CHN_MAX];
    isp_blc_reg_cfg ae_blc;
    isp_blc_reg_cfg mg_blc;
    isp_blc_reg_cfg flicker_blc[ISP_WDR_CHN_MAX];
    isp_blc_reg_cfg pre_gamma_blc;
    isp_blc_reg_cfg split_blc;
    isp_blc_reg_cfg log_lut_blc;
} isp_be_blc_cfg;

typedef struct {
    hi_bool resh_static;
    hi_bool resh_dyna;
    /* FE module */
    hi_bool fe_blc_cfg_changed;
    hi_u16 fe_blc_cfg_save[ISP_BAYER_CHN_NUM];

    isp_blc_reg_cfg fe_dg_blc;
    isp_blc_reg_cfg fe_wb_blc;
    isp_blc_reg_cfg fe_ae_blc;
    isp_blc_reg_cfg fe_lsc_blc;
    isp_blc_reg_cfg rc_blc;
    isp_blc_reg_cfg fe_log_lut_blc;
    isp_blc_reg_cfg fe_blc;
} isp_fe_blc_cfg;

typedef struct hiISP_FE_LUT_UPDATE_REG_CFG_S {
    HI_BOOL bAe1LutUpdate;
    HI_BOOL bLsc1LutUpdate;
} ISP_FE_LUT_UPDATE_REG_CFG_S;

typedef struct hiISP_BE_LUT_UPDATE_REG_CFG_S {
    HI_BOOL bAeLutUpdate;
    HI_BOOL bSharpenLutUpdate;
    HI_BOOL bNddmGfLutUpdate;
    HI_BOOL bNddmUsmLutUpdate;
    HI_BOOL bLdciDrcLutUpdate;
    HI_BOOL bLdciCalcLutUpdate;
    HI_BOOL bDpcLutUpdate;
    HI_BOOL bLscLutUpdate;
    HI_BOOL bBnrLscLutUpdate;
    HI_BOOL bRlscLutUpdate;
    HI_BOOL bGammaLutUpdate;
    HI_BOOL bCaLutUpdate;
    HI_BOOL bWdrLutUpdate;
    HI_BOOL bDehazeLutUpdate;
    HI_BOOL bDehazePreStatLutUpdate;
    HI_BOOL bBnrLutUpdate;
    HI_BOOL bClutLutUpdate;
} ISP_BE_LUT_UPDATE_REG_CFG_S;

typedef union {
    hi_u32  key;
    struct {
        HI_U32  bit1AwbCfg     : 1 ;   /* [0] */
        hi_u32  bit1_be_af_cfg    : 1 ;   /* [1] */
        hi_u32  bit30_rsv       : 30;   /* [2:31] */
    };
} isp_reg_kernel_cfg_key;

typedef  struct hiISP_AWB_ZONE_CFG_S {
    HI_U32 u32Colum;
    HI_U32 u32Row;
    HI_U32 u32ZoneBin;
} AWB_ZONE_CFG_S;

typedef struct {
    hi_u32 colum;
    hi_u32 row;
} af_zone_cfg;

typedef struct {
    AWB_ZONE_CFG_S              stAWBZoneCfg;
    af_zone_cfg               be_af_zone_cfg;
} isp_reg_kernel_cfg;

typedef struct {
    isp_reg_kernel_cfg_key    cfg_key;
    isp_reg_kernel_cfg        alg_kernel_cfg[ISP_STRIPING_MAX_NUM];
} isp_kernel_reg_cfg;

typedef union {
    hi_u64  key;
    struct {
        hi_u64  bit1AeCfg1      : 1 ;   /* [0] */
        hi_u64  bit1AeCfg2      : 1 ;   /* [1] */
        hi_u64  bit1AwbDynCfg   : 1 ;   /* [2] */
        hi_u64  bit1AwbCfg      : 1 ;   /* [3] */
        hi_u64  bit1_af_fe_cfg     : 1 ;   /* [4] */
        hi_u64  bit1DrcCfg      : 1 ;   /* [5] */
        hi_u64  bit1DpCfg       : 1 ;   /* [6] */
        hi_u64  bit1SharpenCfg  : 1 ;   /* [7] */
        hi_u64  bit1GeCfg       : 1 ;   /* [8] */
        hi_u64  bit1WdrCfg      : 1 ;   /* [9] */
        hi_u64  bit1DehazeCfg   : 1 ;   /* [10] */
        hi_u64  bit1AwbCfg3     : 1 ;   /* [11] */
        hi_u64  bit1LscCfg      : 1 ;   /* [12] */
        hi_u64  bit1DemCfg      : 1 ;   /* [13] */
        hi_u64  bit1FcrCfg      : 1 ;   /* [14] */
        hi_u64  bit1McdsCfg     : 1 ;   /* [15] */
        hi_u64  bit1RcCfg       : 1 ;   /* [16] */
        hi_u64  bit1FsWdrCfg    : 1 ;   /* [17] */
        hi_u64  bit1BayernrCfg  : 1 ;   /* [18] */
        hi_u64  bit1FlickerCfg  : 1 ;   /* [19] */
        hi_u64  bit1CaCfg       : 1 ;   /* [20] */
        hi_u64  bit1NddmCfg     : 1 ;   /* [21] */
        hi_u64  bit1LocalCacCfg : 1 ;   /* [22] */
        hi_u64  bit1GlobalCacCfg: 1 ;   /* [23] */
        hi_u64  bit1DciCfg      : 1 ;   /* [24] */
        hi_u64  bit1LdciCfg     : 1 ;   /* [25] */
        hi_u64  bit1CscCfg      : 1 ;   /* [26] */
        hi_u64  bit1GammaCfg    : 1 ;   /* [27] */
        hi_u64  bit1FeLscCfg    : 1 ;   /* [28] */
        hi_u64  bit1PreGammaCfg : 1 ;   /* [29] */
        hi_u64  bit1SplitCfg    : 1 ;   /* [30] */
        hi_u64  bit1EdgeMarkCfg : 1;    /* [31] */
        hi_u64  bit1HrsCfg      : 1 ;   /* [32] */
        hi_u64  bit1DgCfg       : 1 ;   /* [33] */
        hi_u64  bit1WDRDgCfg    : 1 ;   /* [34] */
        hi_u64  bit1FeDgCfg     : 1 ;   /* [35] */
        hi_u64  bit1_af_be_cfg     : 1 ;   /* [36] */
        hi_u64  bit1_fe_blc_cfg : 1 ;   /* [37] */
        hi_u64  bit1_be_blc_cfg : 1 ;   /* [38] */
        hi_u64  bit1ClutCfg     : 1 ;   /* [39] */
        hi_u64  bit1FeLogLUTCfg : 1 ;   /* [40] */
        hi_u64  bit1LogLUTCfg   : 1 ;   /* [41] */
        hi_u64  bit1RLscCfg     : 1 ;   /* [42] */
        hi_u64  bit1FpnCfg      : 1 ;   /* [43] */
        hi_u64  bit20Rsv        : 20;   /* [44:63] */
    };
} isp_reg_cfg_key;

typedef struct {
    ISP_AE_REG_CFG_1_S           stAeRegCfg1;
    ISP_AE_REG_CFG_2_S           stAeRegCfg2;
    ISP_AE_REG_CFG_S             stAeRegCfg;
    ISP_MG_REG_CFG_S             stMgRegCfg;
    ISP_DG_REG_CFG_S             stDgRegCfg;
    ISP_4DG_REG_CFG_S            st4DgRegCfg;
    ISP_FE_DG_REG_CFG_S          stFeDgRegCfg;
    ISP_HRS_REG_CFG_S            stHrsRegCfg;
    ISP_AWB_REG_CFG_S            stAwbRegCfg;

    isp_af_reg_cfg             fe_af_reg_cfg;
    isp_af_reg_cfg             be_af_reg_cfg;
    ISP_DPC_REG_CFG_S            stDpRegCfg;
    ISP_DRC_REG_CFG_S            stDrcRegCfg;
    ISP_PREGAMMA_REG_CFG_S       stPreGammaCfg;
    ISP_GE_REG_CFG_S             stGeRegCfg;
    ISP_SHARPEN_REG_CFG_S        stSharpenRegCfg;
    ISP_EDGEMARK_REG_CFG_S       stEdgeMarkRegCfg;
    ISP_OFFSET_REG_CFG_S         stOffsetRegCfg;
    ISP_WDR_REG_CFG_S            stWdrRegCfg;
    ISP_DEHAZE_REG_CFG_S         stDehazeRegCfg;
    ISP_MCDS_REG_CFG_S           stMcdsRegCfg;
    ISP_RADIAL_CROP_REG_CFG_S    stRcRegCfg;
    ISP_FE_LSC_REG_CFG_S         stFeLscRegCfg;
    ISP_LSC_REG_CFG_S            stLscRegCfg;
    ISP_RLSC_REG_CFG_S           stRLscRegCfg;
    ISP_DEMOSAIC_REG_CFG_S       stDemRegCfg;
    ISP_ANTIFALSECOLOR_REG_CFG_S stAntiFalseColorRegCfg;
    ISP_BAYERNR_REG_CFG_S        stBnrRegCfg;
    ISP_FLICKER_REG_CFG_S        stFlickerRegCfg;
    ISP_CA_MODULE_REG_CFG_S      stCaRegCfg;
    ISP_LCAC_REG_CFG_S           stLCacRegCfg;
    ISP_GCAC_REG_CFG_S           stGCacRegCfg;
    ISP_LDCI_REG_CFG_S           stLdciRegCfg;
    ISP_FPN_REG_CFG_S            stFpnRegCfg;
    isp_be_blc_cfg               be_blc_cfg;
    isp_be_blc_cfg               be_last_blc_cfg;
    isp_fe_blc_cfg               fe_blc_cfg;
    ISP_SPLIT_CFG_S              stSplitCfg;
    ISP_FE_LOGLUT_REG_CFG_S      stFeLogLUTRegCfg;
    ISP_LOGLUT_REG_CFG_S         stLogLUTRegCfg;
    ISP_CSC_REG_CFG_S            stCscCfg;
    ISP_GAMMA_REG_CFG_S          stGammaCfg;
    ISP_CLUT_REG_CFG_S           stClutCfg;
    ISP_FE_LUT_UPDATE_REG_CFG_S  stFeLutUpdateCfg;
    ISP_BE_LUT_UPDATE_REG_CFG_S  stBeLutUpdateCfg;
} isp_alg_reg_cfg;

typedef struct {
    hi_u8 cfg_num;
    isp_reg_cfg_key     cfg_key;
    isp_alg_reg_cfg     alg_reg_cfg[ISP_STRIPING_MAX_NUM];
    isp_kernel_reg_cfg  kernel_reg_cfg;
} isp_reg_cfg;

typedef struct {
    S_ISPBE_REGS_TYPE       be_reg_cfg[ISP_STRIPING_MAX_NUM];
} isp_be_wo_reg_cfg;

typedef struct {
} isp_be_rw_online_stt_reg;

typedef struct {
    ISP_DPC_REG_CFG_S       dp_reg_cfg;
} isp_ro_reg_cfg;

typedef struct {
    hi_u64  phy_addr;
    hi_u64  size;
    hi_void ATTRIBUTE *vir_addr;
} isp_be_wo_cfg_buf;

typedef struct {
    hi_bool             init;
    hi_s32              use_cnt;
    isp_be_wo_cfg_buf   be_buf_haddr;
} isp_be_cfg_buf_info;

/* offline mode isp sync para */
#define SYNC_DRC_PRELUMA_NUM            8
#define SYNC_WDR_EXP_VAL_NUM            4
#define SYNC_EXP_RATIO_NUM              3
#define SYNC_WDR_BLC_COMP_NUM           3
typedef struct {
    hi_u32  ldci_comp;
    hi_u8   drc_shp_log;
    hi_u32  drc_div_denom_log;
    hi_u32  drc_denom_exp;
    hi_u32  drc_prev_luma[SYNC_DRC_PRELUMA_NUM];
    hi_u32  isp_dgain[ISP_BAYER_CHN_NUM];
    hi_u32  wdr_gain[SYNC_WDR_EXP_VAL_NUM][ISP_BAYER_CHN_NUM];
    hi_u32  wdr_exp_ratio[SYNC_EXP_RATIO_NUM];
    hi_u32  flick_exp_ratio[SYNC_EXP_RATIO_NUM];
    hi_u32  wdr_exp_val[SYNC_WDR_EXP_VAL_NUM];
    hi_u32  wdr_blc_comp[SYNC_WDR_BLC_COMP_NUM];
    hi_bool wdr_mdt_en;
    hi_u16  short_thr;
    hi_u16  long_thr;
    hi_u32  wdr_max_ratio;
    hi_bool fusion_mode;
    hi_u32  wdr_wb_gain[ISP_BAYER_CHN_NUM];
    hi_u16  ccm[CCM_MATRIX_SIZE];
    hi_u64  ldci_write_stt_addr[ISP_STRIPING_MAX_NUM];
    hi_u64  ldci_read_stt_addr[ISP_STRIPING_MAX_NUM];
} isp_be_sync_para;

typedef struct {
    hi_u8   block_num;
    hi_u8   block_dev;
} isp_be_regs_attr;

typedef struct {
    hi_u64  phy_addr;
    hi_u64  size;
    hi_void ATTRIBUTE *vir_addr;
} isp_mmz_buf_ex;

typedef struct {
    isp_mmz_buf_ex lut_stt_buf[2];   /* ping pong be lut stt info */
} isp_be_lut2stt_buf;

typedef struct {
    hi_bool              init;
    isp_be_lut2stt_buf be_lut_stt_buf[ISP_MAX_BE_NUM];
} isp_be_lut2stt_attr;

typedef struct {
    hi_bool init;
    hi_u32  cur_write_flag;
    isp_mmz_buf_ex be_stt_buf[2];   /* ping pong be online stt info */
} isp_be_online_stt_info;

typedef struct {
    hi_bool first_frame;
    hi_u32  cur_write_flag;
    isp_mmz_buf_ex fe_stt_buf[2];   /* ping pong fe stt info */
} isp_fe_stt_info;

typedef struct {
    hi_bool         init;
    isp_fe_stt_info fe_stt[ISP_WDR_CHN_MAX];
} isp_fe_stt_attr;

typedef struct {
    hi_bool           init;
} isp_fe_stit_stt_attr;

typedef struct {
    hi_bool           init;
    isp_mmz_buf_ex  be_stt_buf[ISP_STRIPING_MAX_NUM];
} isp_be_off_stt_attr;

typedef struct {
    hi_bool           init;
    isp_mmz_buf_ex  be_stitch_buf[ISP_MAX_PIPE_NUM];
} isp_be_stit_stt_attr;

typedef struct {
    hi_bool           init;
    hi_u8             buf_idx;
    hi_u8             buf_num;
    isp_mmz_buf_ex  ldci_buf[MAX_ISP_BE_BUF_NUM];
} isp_ldci_buf;

typedef struct {
    hi_u64 cur_write_stt_addr[ISP_STRIPING_MAX_NUM];
    hi_u64 read_stt_addr[ISP_STRIPING_MAX_NUM];
    isp_ldci_stat ldci_stat;
} isp_ldci_stt_addr;

typedef struct {
    hi_u8  buf_num;
    hi_u8  buf_idx;
    hi_u64 head_phy_addr;
    isp_mmz_buf_ex read_buf[MAX_ISP_BE_BUF_NUM];
} isp_ldci_read_stt_buf;

typedef struct {
    hi_bool           init;
    isp_mmz_buf_ex  clut_buf;
} isp_clut_buf;

typedef struct {
    hi_bool           init;
    isp_mmz_buf_ex  spec_awb_buf;
} isp_spec_awb_buf;


#define ISP_SYNC_BUF_NODE_NUM           5
#define CFG2VLD_DLY_LIMIT               5
#define WDR_EXP_RATIO_SHIFT             6
#define DRC_COMP_SHIFT                  12

#define ISP_MAX_UPDATEINFO_BUF_NUM      (CFG2VLD_DLY_LIMIT)
#define ISP_MAX_FRAMEINFO_BUF_NUM       (CFG2VLD_DLY_LIMIT)
#define ISP_SAVEINFO_MAX                (CFG2VLD_DLY_LIMIT)
#define ISP_INTERRUPTS_SAVEINFO_MAX     (ISP_MAX_PHY_PIPE_NUM * 4)

typedef struct {
    hi_bool valid;

    ISP_SNS_REGS_INFO_S   stSnsRegsInfo;
    ISP_AE_REG_CFG_2_S    stAERegCfg;
    ISP_FSWDR_SYNC_CFG_S  stWDRRegCfg;
    ISP_DRC_REG_CFG_2_S   stDRCRegCfg;
    ISP_AWB_REG_CFG_2_S stAWBRegCfg;
} isp_sync_cfg_buf_node;

typedef struct {
    hi_bool bInit;
    isp_reg_cfg reg_cfg;
    isp_sync_cfg_buf_node sync_cfg_node;
} isp_reg_cfg_attr;

typedef struct {
    hi_u32  channel_sel;
    hi_u32  wdr_chn_sel[5];
} isp_chn_switch;

typedef struct {
    hi_u8 wdr_mode;
    hi_u32 exp_ratio[EXP_RATIO_NUM];
    ISP_FSWDR_SYNC_CFG_S wdr_reg_cfg;
} isp_wdr_cfg;

typedef struct {
    hi_u16   width;
    hi_u16   height;
    hi_float fps;
    hi_u8 sns_mode;
} isp_res_switch_mode;

typedef struct {
    hi_bool init;
    hi_u32  size;
    hi_u64  phy_addr;
    hi_void *virt_addr;
} isp_proc_mem;

typedef struct {
    hi_u64  phy_addr;
    hi_void *vir_addr;
} isp_mmz_buf;

typedef struct {
    hi_bool     init;
    isp_mmz_buf dng_info;
    isp_mmz_buf atta_info;
    isp_mmz_buf color_gammut_info;
    isp_mmz_buf frame_info;
    isp_mmz_buf update_info;
} isp_trans_info_buf;

typedef struct {
    hi_bool     init;
    isp_mmz_buf pro_nr_param_info;
    isp_mmz_buf pro_shp_param_info;
} isp_pro_info_buf;

typedef struct {
    hi_operation_mode operation_mode;
    hi_u8 pro_frame_num;
    hi_snap_pro_auto_param   auto_param;
    hi_snap_pro_manual_param manual_param;
} isp_pro_param;

/*
0 = default mode:no snap
1 = only preview
2 = only picture
3 = preview and picture is the same pipe
*/
typedef enum {
    ISP_SNAP_NONE            = 0,
    ISP_SNAP_PREVIEW,
    ISP_SNAP_PICTURE,
    ISP_SNAP_PREVIEW_PICTURE,
    ISP_SNAP_BUTT
} isp_snap_pipe_mode;

typedef struct {
    hi_snap_type snap_type;
    hi_s32 preview_pipe_id; /* -1,none snap */
    hi_s32 picture_pipe_id; /* -1,none snap */
    isp_pro_param pro_param;
    hi_bool load_ccm;    /* HI_TRUE:use CCM of snap_isp_info, HI_FALSE: algorithm calculate */
} isp_snap_attr;

typedef struct {
    hi_bool enable;
    hi_u32  param_num;
    hi_isp_nr_auto_attr nr_attr[PRO_MAX_FRAME_NUM];
} isp_pro_nr_param;

typedef struct {
    hi_bool enable;
    hi_u32  param_num;
    hi_isp_sharpen_auto_attr shp_attr[PRO_MAX_FRAME_NUM];
} isp_pro_shp_param;
#define ISP_1ST_INT                     0x1
#define ISP_2ND_INT                     (1<<4)
#define ISP_UNDEF_INT                   0xF4
#define ISP_BE_FSTART                   (1<<16)

#define ISP_DEV_SET_FD                  _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_FD, hi_s32)
#define ISP_GET_FRAME_EDGE              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_FRAME_EDGE, hi_u32)
#define ISP_GET_VD_TIMEOUT              _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_TIMEOUT, isp_vd_timeout)
#define ISP_GET_VD_END_TIMEOUT          _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_END_TIMEOUT, isp_vd_timeout)
#define ISP_GET_VD_BEEND_TIMEOUT        _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_BEEND_TIMEOUT, isp_vd_timeout)
#define ISP_SET_INT_ENABLE              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_INT_ENABLE, hi_bool)
#define ISP_GET_VERSION                 _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VERSION, ISP_VERSION_S)
#define ISP_STAT_BUF_INIT               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_INIT, hi_u64)
#define ISP_STAT_BUF_EXIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_EXIT)
#define ISP_STAT_BUF_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_GET, isp_stat_info)
#define ISP_STAT_BUF_PUT                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_PUT, isp_stat_info)
#define ISP_STAT_ACT_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_ACT_GET, isp_stat_info)
#define ISP_REG_CFG_INIT                _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_INIT, isp_reg_cfg)
#define ISP_REG_CFG_SET                 _IOW(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_SET, isp_kernel_reg_cfg)

#define ISP_BE_CFG_BUF_INIT             _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_INIT, hi_u64)
#define ISP_GET_BE_BUF_FIRST            _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_BE_BUF_FIRST, hi_u64)
#define ISP_BE_FREE_BUF_GET             _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_BE_FREE_BUF_GET, isp_be_wo_cfg_buf)
#define ISP_BE_LAST_BUF_GET             _IOR(IOC_TYPE_ISP, IOC_NR_ISP_BE_LAST_BUF_GET, hi_u64)
#define ISP_BE_CFG_BUF_EXIT             _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_EXIT)
#define ISP_BE_CFG_BUF_CTL              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_CTL, isp_be_wo_cfg_buf)
#define ISP_BE_CFG_BUF_RUNNING          _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_CFG_BUF_RUNNING)
#define ISP_BE_All_BUF_INIT             _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_All_BUF_INIT)

#define ISP_MEM_INFO_SET                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_SET, hi_bool)
#define ISP_MEM_INFO_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_GET, hi_bool)
#define ISP_P2EN_INFO_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_P2EN_INFO_GET, hi_bool)
#define ISP_INIT_INFO_SET               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_INIT_INFO_SET, hi_bool)
#define ISP_SYNC_INIT_SET               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_INIT_SET, hi_bool)
#define ISP_RUN_STATE_SET               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_RUN_STATE_SET, hi_u64)
#define ISP_RESET_CTX                   _IO(IOC_TYPE_ISP, IOC_NR_ISP_RESET_CTX)
#define ISP_CONFIG_INFO_SET             _IOW(IOC_TYPE_ISP, IOC_NR_ISP_CONFIG_INFO_SET, hi_isp_config_info)
#define ISP_SNAP_INFO_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_SNAP_INFO_GET, isp_snap_info)
#define ISP_PRO_TRIGGER_GET             _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PRO_EN_GET, hi_bool)
#define ISP_UPDATE_POS_GET              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_UPDATE_POS_GET, hi_u32)
#define ISP_FRAME_CNT_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_CNT_GET, hi_u32)
#define ISP_SNAP_ATTR_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_SNAP_ATTR_GET, isp_snap_attr)

#define ISP_PROC_INIT                   _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_INIT, isp_proc_mem)
#define ISP_PROC_WRITE_ING              _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_ING)
#define ISP_PROC_WRITE_OK               _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_OK)
#define ISP_PROC_EXIT                   _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_EXIT)
#define ISP_PROC_PARAM_GET              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_PARAM_GET, hi_u32)
#define ISP_TRANS_BUF_INIT              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_TRANS_BUF_INIT, isp_trans_info_buf)
#define ISP_TRANS_BUF_EXIT              _IO(IOC_TYPE_ISP, IOC_NR_ISP_TRANS_BUF_EXIT)
#define ISP_PRO_BUF_INIT                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PRO_BUF_INIT, isp_pro_info_buf)
#define ISP_PRO_BUF_EXIT                _IO(IOC_TYPE_ISP,  IOC_NR_ISP_PRO_BUF_EXIT)

#define ISP_UPDATE_INFO_SET             _IOW(IOC_TYPE_ISP, IOC_NR_ISP_UPDATE_INFO_SET, hi_isp_dcf_update_info)
#define ISP_FRAME_INFO_SET              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_SET, hi_isp_frame_info)
#define ISP_FRAME_INFO_GET              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_FRAME_INFO_GET, hi_isp_frame_info)
#define ISP_DNG_INFO_SET                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_SET, hi_dng_image_dynamic_info)
#define ISP_DNG_INFO_GET                _IOR(IOC_TYPE_ISP, IOC_NR_ISP_DNG_INFO_GET, hi_isp_dng_image_static_info)
#define ISP_SYNC_CFG_SET                _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_CFG_SET, isp_sync_cfg_buf_node)
#define ISP_WDR_CFG_SET                 _IOW(IOC_TYPE_ISP, IOC_NR_WDR_SYNC_CFG_SET, isp_wdr_cfg)
#define ISP_BE_SYNC_PARAM_INIT          _IOW(IOC_TYPE_ISP, IOC_NR_ISP_BE_SYNC_PARAM_INIT, isp_be_sync_para)
#define ISP_RES_SWITCH_SET              _IO(IOC_TYPE_ISP, IOC_NR_ISP_RES_SWITCH_SET)
#define ISP_CHN_SELECT_CFG              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_CHN_SELECT_CFG, hi_u32)
#define ISP_BE_SWITCH_FINISH_STATE_SET  _IO(IOC_TYPE_ISP, IOC_NR_ISP_BE_SWITCH_FINISH_STATE_SET)
#define ISP_MODE_SWITCH_SET             _IO(IOC_TYPE_ISP, IOC_NR_ISP_MODE_SWITCH_SET)
#define ISP_AWB_EN_SET                  _IOW(IOC_TYPE_ISP, IOC_NR_ISP_AWB_EN_SET, hi_bool)

#define ISP_PWM_NUM_GET                 _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PWM_NUM_GET, hi_u32)
#define ISP_WORK_MODE_INIT              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_INIT, isp_block_attr)
#define ISP_WORK_MODE_GET               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_GET, isp_working_mode)
#define ISP_WORK_MODE_EXIT              _IO(IOC_TYPE_ISP, IOC_NR_ISP_WORK_MODE_EXIT)
#define ISP_SET_MOD_PARAM               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_MOD_PARAM, hi_isp_mod_param)
#define ISP_GET_MOD_PARAM               _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_MOD_PARAM, hi_isp_mod_param)
#define ISP_SET_CTRL_PARAM              _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_CTRL_PARAM, hi_isp_ctrl_param)
#define ISP_GET_CTRL_PARAM              _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_CTRL_PARAM, hi_isp_ctrl_param)
#define ISP_PRE_BLK_NUM_UPDATE          _IOW(IOC_TYPE_ISP, IOC_NR_ISP_PRE_BLK_NUM_UPDATE, hi_u8)

#define ISP_GET_HDR_ATTR                _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_HDR_ATTR, vi_pipe_hdr_attr)
#define ISP_GET_WDR_ATTR                _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_WDR_ATTR, vi_pipe_wdr_attr)
#define ISP_GET_STITCH_ATTR             _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_STITCH_ATTR, vi_stitch_attr)
#define ISP_GET_PIPE_SIZE               _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_PIPE_SIZE, hi_size)
#define ISP_PUB_ATTR_INFO               _IOW(IOC_TYPE_ISP, IOC_NR_ISP_PUB_ATTR_INFO, hi_isp_pub_attr)
#define ISP_KERNEL_RUNONCE              _IO(IOC_TYPE_ISP, IOC_NR_ISP_KERNEL_RUNONCE)
#define ISP_KERNEL_YUV_RUNONCE          _IO(IOC_TYPE_ISP, IOC_NR_ISP_KERNEL_YUV_RUNONCE)
#define ISP_OPT_RUNONCE_INFO            _IOW(IOC_TYPE_ISP, IOC_NR_ISP_OPT_RUNONCE_INFO, hi_bool)
#define ISP_YUV_RUNONCE_INFO            _IOW(IOC_TYPE_ISP, IOC_NR_ISP_YUV_RUNONCE_INFO, hi_bool)

#define ISP_SET_PROCALCDONE             _IO(IOC_TYPE_ISP, IOC_NR_ISP_SET_PROCALCDONE)

#define ISP_SYNC_STITCH_PARAM_INIT      _IO(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_STITCH_PARAM_INIT)

#define ISP_CLUT_BUF_INIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_CLUT_BUF_INIT)
#define ISP_CLUT_BUF_EXIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_CLUT_BUF_EXIT)
#define ISP_CLUT_BUF_GET                _IOR(IOC_TYPE_ISP,IOC_NR_ISP_CLUT_BUF_GET, hi_u64)

#define ISP_LDCI_BUF_INIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_BUF_INIT)
#define ISP_LDCI_BUF_EXIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_BUF_EXIT)
#define ISP_LDCI_READ_STT_BUF_GET       _IOR(IOC_TYPE_ISP, IOC_NR_ISP_LDCI_READ_STT_BUF_GET, isp_ldci_read_stt_buf)
#define ISP_STT_BUF_INIT                _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_BUF_INIT)
#define ISP_STT_BUF_EXIT                _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_BUF_EXIT)
#define ISP_STT_ADDR_INIT               _IO(IOC_TYPE_ISP, IOC_NR_ISP_STT_ADDR_INIT)
#define ISP_BE_LUT_STT_BUF_GET          _IOR(IOC_TYPE_ISP,IOC_NR_ISP_BE_LUT_STT_BUF_GET, hi_u64)

#define ISP_SPECAWB_BUF_INIT            _IO(IOC_TYPE_ISP, IOC_NR_ISP_SPECAWB_BUF_INIT)
#define ISP_SPECAWB_BUF_EXIT            _IO(IOC_TYPE_ISP, IOC_NR_ISP_SPECAWB_BUF_EXIT)
#define ISP_SPECAWB_BUF_GET             _IOR(IOC_TYPE_ISP,IOC_NR_ISP_SPECAWB_BUF_GET, hi_u64)
#define ISP_SET_RGBIR_FORMAT            _IOW(IOC_TYPE_ISP,IOC_NR_ISP_SET_RGBIR_FORMAT, hi_isp_ir_bayer_format)

#define ISP_GET_DEV(f)             (*((hi_u32*)(f)))

#define ISP_CHECK_PIPE(pipe)\
    do {\
        if (((pipe) < 0) || ((pipe) >= ISP_MAX_PIPE_NUM))\
        {\
            ISP_ERR_TRACE("Err isp pipe %d!\n", pipe);\
            return HI_ERR_ISP_ILLEGAL_PARAM;\
        }\
    }while(0)

#define ISP_CHECK_VIR_PIPE(pipe)\
    do {\
        if (((pipe) < ISP_MAX_PIPE_NUM) && ((pipe) >= ISP_MAX_PHY_PIPE_NUM))\
        {\
            return HI_SUCCESS;\
        }\
    }while(0)

#define ISP_CHECK_POINTER(ptr)\
    do {\
        if (ptr == HI_NULL)\
        {\
            ISP_ERR_TRACE("Null Pointer!\n");\
            return HI_ERR_ISP_NULL_PTR;\
        }\
    }while(0)

#define ISP_CHECK_POINTER_VOID(ptr)\
    do {\
        if (ptr == HI_NULL)\
        {\
            ISP_ERR_TRACE("Null Pointer!\n");\
            return;\
        }\
    }while(0)

#define ISP_CHECK_BOOL(bool)\
    do {\
        if (( HI_TRUE != bool)&&( HI_FALSE != bool))\
        {\
            ISP_ERR_TRACE("Invalid ISP Bool Type %d!\n", bool);\
            return HI_ERR_ISP_ILLEGAL_PARAM;\
        }\
    }while(0)

#define ISP_CHECK_BUF_INIT(pipe, init)\
    do{\
        if ((init) != HI_FALSE)\
        {\
            ISP_ERR_TRACE("ISP[%d] buffer is initialized, Please exit ISP!!!\n", pipe);\
            return HI_FAILURE;\
        }\
    }while(0)

#define ISP_CHECK_BUF_EXIT(pipe, init)\
    do{\
        if ((init) != HI_TRUE)\
        {\
            return HI_SUCCESS;\
        }\
    }while(0)

#define ISP_CHECK_BEBUF_INIT(pipe)\
    do{\
        if (g_astIspDrvCtx[pipe].stBeBufInfo.init != HI_TRUE)\
        {\
            ISP_ERR_TRACE("ISP[%d] BeBuf doesn't initialized!\n", pipe);\
            return HI_ERR_ISP_NOT_INIT;\
        }\
    }while(0)

#define ISP_CHECK_STABUF_INIT(pipe)\
    do{\
        if (g_astIspDrvCtx[pipe].stStatisticsBuf.bInit != HI_TRUE)\
        {\
            ISP_ERR_TRACE("ISP[%d] StaBuf doesn't initialized!\n", pipe);\
            return HI_ERR_ISP_NOT_INIT;\
        }\
    }while(0)

#define ISP_CHECK_TRANBUF_INIT(pipe)\
    do{\
        if (g_astIspDrvCtx[pipe].stTransInfo.init != HI_TRUE)\
        {\
            ISP_WARN_TRACE("ISP[%d] TransBuf doesn't initialized!\n", pipe);\
            return HI_ERR_ISP_NOT_INIT;\
        }\
    }while(0)

#define ISP_CHECK_EXIT_STATE(pipe)\
    do{\
        if (g_astIspDrvCtx[pipe].bIspRunFlag == HI_TRUE)\
        {\
            ISP_ERR_TRACE("Isp[%d] cann't exit!\n", pipe);\
            return HI_FAILURE;\
        }\
    }while(0)

#define ISP_CHECK_ONLINE_MODE(pipe)\
    do{\
        if ((IS_ONLINE_MODE(g_astIspDrvCtx[pipe].stWorkMode.running_mode)) ||  \
            (IS_SIDEBYSIDE_MODE(g_astIspDrvCtx[pipe].stWorkMode.running_mode)))\
        {\
            return HI_SUCCESS;\
        }\
    }while(0)

#define ISP_DBG(bEn, fmt...)\
    do{\
        if (bEn)\
            printf(fmt);\
    }while(0)

#define SLAVE_CHECK_DEV(dev)\
    do {\
        if (((dev) < 0) || ((dev) >= CAP_SLAVE_MAX_NUM))\
        {\
            ISP_ERR_TRACE("Err slave dev %d!\n", dev);\
            return HI_ERR_ISP_ILLEGAL_PARAM;\
        }\
    }while(0)

#define ISP_STATISTICS_KEY              (0xFFFBBF77)
#define ISP_PIPE_FEAF_MODULE_ENABLE     1,1,0,0,1,1,0,0
#define ISP_PIPE_FELOGLUT_MODULE_ENABLE 0,1,0,0,0,1,0,0

#define ISP_CHECK_FEAF_MODULE(dev)\
    do{\
        HI_U8 bFeHaveAfMod[] = {ISP_PIPE_FEAF_MODULE_ENABLE};\
        if (!bFeHaveAfMod[dev])\
        {\
            ISP_ERR_TRACE("Invalid Af Fe Pipe %d!\n", dev);\
            return HI_ERR_ISP_ILLEGAL_PARAM;\
        }\
    }while(0)

#endif
