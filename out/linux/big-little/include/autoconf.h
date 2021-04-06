/*
 * Automatically generated C config: don't edit
 * Busybox version: 
 */
#define AUTOCONF_TIMESTAMP "2019-09-16 16:09:46 CST"


/*
 * General Setup
 */
#define CONFIG_HI3559AV100 1
#define CONFIG_HI_CHIP_TYPE 0x3559A100
#define CONFIG_HI_ARCH "hi3559av100"
#define CONFIG_HI_SUBARCH ""
#define CONFIG_HI_SUBCHIP_TYPE 0x
#define CONFIG_HI_SUBARCH ""
#define CONFIG_HI_SUBCHIP_TYPE 0x
#define CONFIG_SMP 1
#define CONFIG_ARM_ARCH_TYPE "smp"
#define CONFIG_CPU_TYPE_BIG_LITTLE 1
#define CONFIG_CPU_TYPE "big-little"
#define CONFIG_VERSION_ASIC 1
#define CONFIG_HI_FPGA "n"
#define CONFIG_LINUX_OS 1
#define CONFIG_OS_TYPE "linux"
#define CONFIG_LINUX_4_9_y 1
#define CONFIG_KERNEL_VERSION "linux-4.9.y"
#define CONFIG_KERNEL_AARCH64_HIMIX100 1
#define CONFIG_HI_CROSS "aarch64-himix100-linux-"
#define CONFIG_LIBC_TYPE "glibc"
#define CONFIG_KERNEL_BIT "KERNEL_BIT_64"
#define CONFIG_USER_AARCH64_HIMIX100 1
#define CONFIG_HI_CROSS_LIB "aarch64-himix100-linux-"
#define CONFIG_USER_BIT "USER_BIT_64"
#define CONFIG_RELEASE_TYPE_RELEASE 1
#define CONFIG_HI_RLS_MODE "HI_RELEASE"
#define CONFIG_CUSTOMER_VERSION_COMMON 1
#define CONFIG_CUSTOMER_VERSION "COMMON"

/*
 * Media Modules Setup
 */

/*
 * media base config
 */
#define CONFIG_HI_VB_EXTPOOL_SUPPORT 1

/*
 * media sys config
 */
#define CONFIG_HI_SYS_SUPPORT 1
#define CONFIG_HI_SYS_SCALE_COEF_SUPPORT 1
#define CONFIG_HI_SYS_SCALE_COEF_ONLINE_SUPPORT 1

/*
 * media vi config
 */

/*
 * media isp config
 */

/*
 * media dis config
 */

/*
 * media vpss config
 */

/*
 * media avs config
 */

/*
 * media gdc config
 */

/*
 * media vgs config
 */

/*
 * media chnl config
 */

/*
 * media venc config
 */

/*
 * media vdec config
 */
#define CONFIG_VDEC_IP ""

/*
 * media vo config
 */
#define CONFIG_HI_VO_FB_SEPARATE 1

/*
 * media region config
 */

/*
 * media audio config
 */
#define CONFIG_HI_ACODEC_VERSION ""
#define CONFIG_HI_ACODEC_MAX_GAIN 0
#define CONFIG_HI_ACODEC_MIN_GAIN 0
#define CONFIG_HI_ACODEC_GAIN_STEP 0

/*
 * media hdr config
 */

/*
 * media mcf config
 */

/*
 * Device Driver Setup
 */

/*
 * drv config
 */
#define CONFIG_DRV 1
#define CONFIG_EXTDRV 1
#define CONFIG_INTERDRV 1
#define CONFIG_CIPHER 1
#define CONFIG_HIUSER 1
#define CONFIG_MIPI_TX 1
#define CONFIG_MIPI_RX 1
#define CONFIG_HI_IR 1
#define CONFIG_HI_WDG 1
#define CONFIG_HI_SYSCFG 1

/*
 * Component Setup
 */

/*
 * Component hdmi Config
 */

/*
 * Component hifb Config
 */
#define CONFIG_HI_HIFB_SUPPORT 1

/*
 * Component svp Config
 */
#define CONFIG_HI_SVP_SUPPORT 1
#define CONFIG_HI_SVP_DSP 1
#define CONFIG_HI_SVP_LITEOS 1
#define CONFIG_HI_SVP_CNN 1
#define CONFIG_HI_SVP_IVE 1
#define CONFIG_HI_SVP_IVE_CSC 1
#define CONFIG_HI_SVP_IVE_FILTER_AND_CSC 1
#define CONFIG_HI_SVP_IVE_NCC 1
#define CONFIG_HI_SVP_IVE_GMM 1
#define CONFIG_HI_SVP_IVE_LBP 1
#define CONFIG_HI_SVP_IVE_NORM_GRAD 1
#define CONFIG_HI_SVP_IVE_ST_CANDI_CORNER 1
#define CONFIG_HI_SVP_IVE_GRAD_FG 1
#define CONFIG_HI_SVP_IVE_ANN 1
#define CONFIG_HI_SVP_IVE_SVM 1
#define CONFIG_HI_SVP_IVE_RESIZE 1
#define CONFIG_HI_SVP_IVE_CNN 1
#define CONFIG_HI_SVP_MD 1

/*
 * Component photo Config
 */
#define CONFIG_HI_PHOTO_SUPPORT 1

/*
 * Component tde Config
 */
#define CONFIG_HI_TDE_SUPPORT 1

/*
 * Component pciv Config
 */

/*
 * Component avs lut Config
 */
#define CONFIG_HI_AVS_LUT_SUPPORT 1

/*
 * Component pos_query Config
 */
#define CONFIG_HI_POS_QUERY_SUPPORT 1

/*
 * Component tzasc Config
 */

/*
 * Component motionfusion config
 */
#define CONFIG_HI_MOTIONFUSION_SUPPORT 1

/*
 * Component pm Config
 */

/*
 * HISYSLINK Setup
 */

/*
 * hisyslink config
 */
#define CONFIG_HISYSLINK 1

/*
 * Debug Config
 */
#define CONFIG_HI_GDB_NO 1
#define CONFIG_HI_GDB "n"
#define CONFIG_HI_LOG_TRACE_SUPPORT 1
#define CONFIG_HI_LOG_TRACE_ALL 1
#define CONFIG_HI_LOG_TRACE_LEVEL 7

/*
 * Test Config
 */
