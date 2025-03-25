/************************************************************************
** File:
**   $Id: generic_reaction_wheel_platform_cfg.h  $
**
** Purpose:
**  Define generic_reaction_wheel Platform Configuration Parameters
**
** Notes:
**
*************************************************************************/
#ifndef _GENERIC_REACTION_WHEEL_PLATFORM_CFG_H_
#define _GENERIC_REACTION_WHEEL_PLATFORM_CFG_H_

/*
** Default GENERIC_REACTION_WHEEL_1 Configuration
*/
#ifndef GENERIC_REACTION_WHEEL_1_CFG
/* Notes:
**   NOS3 uart requires matching handle and bus number
*/
#define GENERIC_REACTION_WHEEL_1_CFG_STRING      "/dev/tty2"
#define GENERIC_REACTION_WHEEL_1_CFG_HANDLE      2
#define GENERIC_REACTION_WHEEL_1_CFG_IS_OPEN     PORT_CLOSED
#define GENERIC_REACTION_WHEEL_1_CFG_BAUDRATE_HZ 115200
#define GENERIC_REACTION_WHEEL_1_CFG_MS_TIMEOUT  50 /* Max 255 */
/* Note: Debug flag disabled (commented out) by default */
//#define GENERIC_REACTION_WHEEL_1_CFG_DEBUG
#endif

/*
** Default GENERIC_REACTION_WHEEL_2 Configuration
*/
#ifndef GENERIC_REACTION_WHEEL_2_CFG
/* Notes:
**   NOS3 uart requires matching handle and bus number
*/
#define GENERIC_REACTION_WHEEL_2_CFG_STRING      "/dev/tty3"
#define GENERIC_REACTION_WHEEL_2_CFG_HANDLE      3
#define GENERIC_REACTION_WHEEL_2_CFG_IS_OPEN     PORT_CLOSED
#define GENERIC_REACTION_WHEEL_2_CFG_BAUDRATE_HZ 115200
#define GENERIC_REACTION_WHEEL_2_CFG_MS_TIMEOUT  50 /* Max 255 */
/* Note: Debug flag disabled (commented out) by default */
//#define GENERIC_REACTION_WHEEL_2_CFG_DEBUG
#endif

/*
** Default GENERIC_REACTION_WHEEL_3 Configuration
*/
#ifndef GENERIC_REACTION_WHEEL_3_CFG
/* Notes:
**   NOS3 uart requires matching handle and bus number
*/
#define GENERIC_REACTION_WHEEL_3_CFG_STRING      "/dev/tty4"
#define GENERIC_REACTION_WHEEL_3_CFG_HANDLE      4
#define GENERIC_REACTION_WHEEL_3_CFG_IS_OPEN     PORT_CLOSED
#define GENERIC_REACTION_WHEEL_3_CFG_BAUDRATE_HZ 115200
#define GENERIC_REACTION_WHEEL_3_CFG_MS_TIMEOUT  50 /* Max 255 */
/* Note: Debug flag disabled (commented out) by default */
//#define GENERIC_REACTION_WHEEL_3_CFG_DEBUG
#endif

#endif /* _GENERIC_REACTION_WHEEL_PLATFORM_CFG_H_ */
