/*     here is piris arch .
*
*
*  This file defines piris micro-definitions for user.
*
* History:
*     03-Mar-2016 Start of Hi351xx Digital Camera,H6
*
*/

#ifndef __PIRIS_HAL_H__
#define __PIRIS_HAL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


/***************base address***********************/
/* Piris use GPIO1_5, GPIO1_4, GPIO1_3, GPIO1_2 */
/* GPIO1's base address is 0x180D1000 */

#define PIRISI_ADRESS_BASE 0x180D1000


/***************A base address***********************/
/* Piris use GPIO1_6, GPIO1_4 */
/* GPIO1's base address is 0x180D1000 */
/***************PIRIS_DRV_Write A REG value***********************/
#define PIRIS_A_CASE0_REG0 0x40

#define PIRIS_A_CASE1_REG0 0x10

#define PIRIS_A_CASE2_REG0 0x10

#define PIRIS_A_CASE3_REG0 0x40

/***************B base address***********************/
/* Piris use GPIO2_7, GPIO2_5  */
/* GPIO2's base address is 0x180D2000 */
/***************PIRIS_DRV_Write REG value***********************/
#define PIRIS_B_CASE0_REG0 0x80

#define PIRIS_B_CASE1_REG0 0x80

#define PIRIS_B_CASE2_REG0 0x20

#define PIRIS_B_CASE3_REG0 0x20



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __PIRIS_HAL_H__ */
