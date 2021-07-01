/**
 * @file upsContext.h
 * @author  xiaowine (xiaowine@sina.cn)
 * @brief
 * @version 01.00
 * @date    2021-06-24
 *
 * @copyright Copyright (c) {2020}  xiaowine
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-06-24 <td>1.0     <td>wangh     <td>内容
 * </table>
 * ******************************************************************
 * *                   .::::
 * *                 .::::::::
 * *                ::::::::::
 * *             ..:::::::::::
 * *          '::::::::::::
 * *            .:::::::::
 * *       '::::::::::::::..        女神助攻,流量冲天
 * *            ..::::::::::::.     永不宕机,代码无bug
 * *          ``:::::::::::::::
 * *           ::::``:::::::::'        .:::
 * *          ::::'   ':::::'       .::::::::
 * *        .::::'      ::::     .:::::::'::::
 * *       .:::'       :::::  .:::::::::' ':::::
 * *      .::'        :::::.:::::::::'      ':::::
 * *     .::'         ::::::::::::::'         ``::::
 * * ...:::           ::::::::::::'              ``::
 * *```` ':.          ':::::::::'                  ::::.
 * *                   '.:::::'                    ':'````.
 * ******************************************************************
 */

#ifndef __UPSCONTEXT_H_
#define __UPSCONTEXT_H_

/* Private includes ----------------------------------------------------------*/
#include "cmsis_os.h"  // CMSIS RTOS header file

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Public define ------------------------------------------------------------*/
typedef enum
{
    UPS_Q3GS,
    UPS_QFS,
    UPS_Q3WS,
    UPS_Q3YV,
    UPS_QYF,
    UPS_Q3YC,
    UPS_Q3LD,
    UPS_QBYV,
    UPS_QBYF,
} _upsCmd_t;
typedef struct
{
    char *cmd;
    char *bkHead;
    char *bkKey;
    void (*process)(char *);
} cmd_map_t;

typedef struct
{
    uint16_t R_inputVoltage;
    uint16_t S_inputVoltage;
    uint16_t T_inputVoltage;
    uint16_t inputFrequency;
    uint16_t R_outputVoltage;
    uint16_t S_outputVoltage;
    uint16_t T_outputVoltage;
    uint16_t outputFrequency;
    uint16_t R_outputCurrent;
    uint16_t S_outputCurrent;
    uint16_t T_outputCurrent;
    uint16_t R_outputLoadPercent;
    uint16_t S_outputLoadPercent;
    uint16_t T_outputLoadPercent;
    uint16_t P_batteryVoltage;
    uint16_t N_batteryVoltage;
    uint16_t detectingPointersMaxTemp;
    union
    {
        uint16_t upsStatus;
        struct
        {
            uint8_t a1 : 1;
            uint8_t a0 : 1;
            uint8_t b0 : 1;
            uint8_t b1 : 1;
            uint8_t b2 : 1;
            uint8_t b3 : 1;
            uint8_t b4 : 1;
            uint8_t b5 : 1;
            uint8_t b6 : 1;
            uint8_t b7 : 1;
            uint8_t b8 : 1;
            uint8_t b9 : 1;
        };
    };

} q3gs_t;

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern const cmd_map_t commandMap[];
/* Public variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
osEvent upsCommand(_upsCmd_t cmd, uint32_t timeout);
void contextVariablesInit(void);
/* Private user code ---------------------------------------------------------*/

#endif /*__UPSCONTEXT_H_*/
