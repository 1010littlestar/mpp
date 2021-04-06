/*-------------------------------------------------------------------------

-------------------------------------------------------------------------*/
#include <string.h>
#include "stdlib.h"

#include "los_task.h"
#include "hi_type.h"
#include "shell.h"
#include "tools_shell_cmd.h"

extern HI_S32 vo_chn_dump(int argc, char* argv[]);
extern HI_S32 vo_screen_dump(int argc, char* argv[]);
extern HI_S32 avs_chn_dump(int argc, char* argv[]);
extern HI_S32 avs_src_dump(int argc, char* argv[]);
extern HI_S32 vpss_chn_dump(int argc, char* argv[]);
extern HI_S32 vpss_src_dump(int argc, char* argv[]);
extern HI_S32 vi_chn_dump(int argc, char* argv[]);
extern HI_S32 vi_bayerdump(int argc, char* argv[]);
extern HI_S32 fisheye_calibrate(int argc, char* argv[]);
extern HI_S32 vi_fisheye_attr(int argc, char* argv[]);
extern void tools_cmd_register(void);

#define  ARGS_SIZE_T         20
#define  ARG_BUF_LEN_T       256
static char *ptask_args[ARGS_SIZE_T];
static char *args_buf_t = NULL;


void tools_cmd_com(unsigned long p0, unsigned long p1, unsigned long p2, unsigned long p3)
{
    int i = 0, ret = 0;
    unsigned int argc = p0;
    char **argv = (char **)p1;

    //Set_Interupt(0);

    dprintf("\ntools input command:\n");
    for(i=0; i<argc; i++) {
        dprintf("%s ", argv[i]);
    }
    dprintf("\n");
    if(strcmp(argv[0],"vpss_chn_dump")==0) //vpss_chn_dump
    {
        ret = vpss_chn_dump(argc,argv);
        dprintf("\nvpss_chn_dump finshed\n");
    }
    else if(strcmp(argv[0],"vpss_src_dump")==0) //vpss_src_dump
    {
        ret = vpss_src_dump(argc,argv);
        dprintf("\nvpss_src_dump finshed\n");
    }
    else if(strcmp(argv[0],"vo_chn_dump")==0) //vou_chn_dump
    {
        ret = vo_chn_dump(argc,argv);
        dprintf("\nvo_chn_dump finshed\n");
    }
    else if(strcmp(argv[0],"vo_screen_dump")==0) //vou_screen_dump
    {
        ret = vo_screen_dump(argc,argv);
        dprintf("\nvo_screen_dump finshed\n");
    }
    else if(strcmp(argv[0],"avs_src_dump")==0) //avs_src_dump
    {
        ret = avs_src_dump(argc,argv);
        dprintf("\navs_src_dump finshed\n");
    }
    else if(strcmp(argv[0],"avs_chn_dump")==0) //avs_chn_dump
    {
        ret = avs_chn_dump(argc,argv);
        dprintf("\navs_chn_dump finshed\n");
    }
	else if(strcmp(argv[0],"vi_chn_dump")==0) //vi_chn_dump
    {
        ret = vi_chn_dump(argc,argv);
        dprintf("\nvi_chn_dump finshed\n");
    }
    else if(strcmp(argv[0],"vi_bayerdump")==0) //vi_bayerdump
    {
        ret = vi_bayerdump(argc,argv);
        dprintf("\nvi_bayerdump finshed\n");
    }
    else if(strcmp(argv[0],"fisheye_cal")==0) //fisheye_calibrate
    {
        ret = fisheye_calibrate(argc,argv);
        dprintf("\nfisheye_calibrate finshed\n");
    }
    else if(strcmp(argv[0],"vi_fisheye_attr")==0) //vi_fisheye_attr
    {
        ret = vi_fisheye_attr(argc,argv);
        dprintf("\nvi_fisheye_attr finshed\n");
    }

    #if 0
    else if(strcmp(argv[0],"isp_debug")==0) //vi_dump
    {
        ret = isp_debug(argc,argv);
        dprintf("\nisp_debug finshed\n");
    }
    #endif

    if(ret != 0)
    {
        dprintf("\nstart task fail.\n");
    }
    //taskid = -1;
}

unsigned int tools_cmd_app(int argc, char **argv )
{
    int i = 0, ret = 0;
    int len = 0;
    char *pch = NULL;
    TSK_INIT_PARAM_S stappTask;
    unsigned int taskid = -1;
//    printf("xxxx %d,%s,%s,%s,%s\n",argc,argv[0],argv[1],argv[2],argv[3]);

    if (taskid != -1)
    {
        dprintf("There's a app_main task existed.");
    }

    args_buf_t = zalloc(ARG_BUF_LEN_T);
    pch = args_buf_t;
    for(i=0; i<ARGS_SIZE_T; i++)
    {
        ptask_args[i] = NULL;
    }

    for(i = 0; i < argc; i++)
    {
        len =  strlen(argv[i]);
        memcpy(pch , argv[i], len);
        ptask_args[i] = pch;
        //keep a '\0' at the end of a string.
        pch = pch + len + 1;
        if (pch >= args_buf_t +ARG_BUF_LEN_T) {
            dprintf("args out of range!\n");
            break;
        }
    }

    memset(&stappTask, 0, sizeof(TSK_INIT_PARAM_S));
    stappTask.pfnTaskEntry = (TSK_ENTRY_FUNC)tools_cmd_com;
    stappTask.uwStackSize  = 0x80000;
    stappTask.pcName = "tools_cmd_com";
    stappTask.usTaskPrio = 10;
    stappTask.uwResved   = LOS_TASK_STATUS_DETACHED;
    stappTask.auwArgs[0] = argc;
    stappTask.auwArgs[1] = (unsigned long)ptask_args;
    ret = LOS_TaskCreate(&taskid, &stappTask);

    dprintf("Task %d\n", taskid);

    return ret;
}

void tools_cmd_register(void)
{
    osCmdReg(CMD_TYPE_STD,"vo_chn_dump",3,(CMD_CBK_FUNC)tools_cmd_app);
    osCmdReg(CMD_TYPE_STD,"vo_screen_dump",2,(CMD_CBK_FUNC)tools_cmd_app);

    osCmdReg(CMD_TYPE_STD,"vpss_chn_dump",3,(CMD_CBK_FUNC)tools_cmd_app);
    osCmdReg(CMD_TYPE_STD,"vpss_src_dump",2,(CMD_CBK_FUNC)tools_cmd_app);

    osCmdReg(CMD_TYPE_STD,"avs_chn_dump",3,(CMD_CBK_FUNC)tools_cmd_app);
    osCmdReg(CMD_TYPE_STD,"avs_src_dump",3,(CMD_CBK_FUNC)tools_cmd_app);

    osCmdReg(CMD_TYPE_STD,"vi_chn_dump",4,(CMD_CBK_FUNC)tools_cmd_app);
    osCmdReg(CMD_TYPE_STD,"vi_bayerdump",5,(CMD_CBK_FUNC)tools_cmd_app);

    osCmdReg(CMD_TYPE_STD,"fisheye_cal",3,(CMD_CBK_FUNC)tools_cmd_app);
    osCmdReg(CMD_TYPE_STD,"vi_fisheye_attr",4,(CMD_CBK_FUNC)tools_cmd_app);
}

