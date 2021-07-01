/**
 * @file upsContext.c
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

/* Private includes ----------------------------------------------------------*/
#include "upsContext.h"
#include "usart.h"
#include <string.h>
#include <stdint.h>
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
void q3gsPro(char *buff);
void qfsPro(char *buff);
void q3wsPro(char *buff);
/* Private variables ---------------------------------------------------------*/
const cmd_map_t commandMap[] = {
    {"Q3GS\r", "(", "\r", q3gsPro},
    {"QFS\r", "(", "\r", qfsPro},
    {"Q3WS\r", "(", "\r", q3wsPro},
};

uint8_t contextArea[128] = {0};
q3gs_t *contextQ3GS      = (q3gs_t *)contextArea;
uint8_t *contextQFS      = (contextArea + sizeof(q3gs_t));
uint32_t *contextQ3WS    = (uint16_t *)(contextArea + sizeof(q3gs_t) + 1);
/* Public variables ---------------------------------------------------------*/
osMessageQId ups0ResponseMsgId;
osMessageQDef(ups0ResponseMsgId, 5, unsigned char);
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

void contextVariablesInit(void)
{
    ups0ResponseMsgId = osMessageCreate(osMessageQ(ups0ResponseMsgId), NULL);
}

osEvent upsCommand(_upsCmd_t cmd, uint32_t timeout)
{
    osEvent ThreadEvent;
    memset(rx1buffer, 0, rx1Count);
    rx1Count = 0;
    uart1_dma_send((uint8_t *)commandMap[cmd].cmd, 5);  // strlen(commandMap[cmd].cmd));
    ThreadEvent = osMessageGet(ups0ResponseMsgId, timeout);
    if (ThreadEvent.status == osEventTimeout)
    {
        // ThreadEvent.status = osOK;
    }
    else if (ThreadEvent.status == osEventMessage)
    {
        if (ThreadEvent.value.v > 0)
        {
            char *p;
            p = strstr((void *)rx1buffer, commandMap[cmd].bkHead);
            if ((p != NULL) && (commandMap[cmd].process != NULL))
            {
                commandMap[cmd].process(p);
            }
            memset(rx1buffer, 0, rx1Count);
            rx1Count = 0;
        }
    }
    return ThreadEvent;
}

void q3gsPro(char *buff)
{
    float r_in, s_in, t_in, in_freq, r_out, s_out, t_out, out_freq, r_outC, s_outC, t_outC;
    int r_out_load, s_out_load, t_out_load;
    float p_v, n_v, dpmt;
    char upsSataus[12];

    sscanf((void *)buff, "(%f %f %f %f %f %f %f %f %f %f %f %03d %03d %03d %f %f %f %s\r", &r_in, &s_in, &t_in,
           &in_freq, &r_out, &s_out, &t_out, &out_freq, &r_outC, &s_outC, &t_outC, &r_out_load, &s_out_load,
           &t_out_load, &p_v, &n_v, &dpmt, upsSataus);
    contextQ3GS->R_inputVoltage           = (uint16_t)(r_in * 10);
    contextQ3GS->S_inputVoltage           = (uint16_t)(s_in * 10);
    contextQ3GS->T_inputVoltage           = (uint16_t)(t_in * 10);
    contextQ3GS->inputFrequency           = (uint16_t)(in_freq);
    contextQ3GS->R_outputVoltage          = (uint16_t)(r_out * 10);
    contextQ3GS->S_outputVoltage          = (uint16_t)(s_out * 10);
    contextQ3GS->T_outputVoltage          = (uint16_t)(t_out * 10);
    contextQ3GS->outputFrequency          = (uint16_t)(out_freq * 10);
    contextQ3GS->R_outputCurrent          = (uint16_t)(r_outC * 10);
    contextQ3GS->S_outputCurrent          = (uint16_t)(s_outC * 10);
    contextQ3GS->T_outputCurrent          = (uint16_t)(t_outC * 10);
    contextQ3GS->R_outputLoadPercent      = (uint16_t)(r_out_load);
    contextQ3GS->S_outputLoadPercent      = (uint16_t)(s_out_load);
    contextQ3GS->T_outputLoadPercent      = (uint16_t)(t_out_load);
    contextQ3GS->P_batteryVoltage         = (uint16_t)(p_v * 10);
    contextQ3GS->N_batteryVoltage         = (uint16_t)(n_v * 10);
    contextQ3GS->detectingPointersMaxTemp = (uint16_t)(dpmt * 10);
    contextQ3GS->upsStatus                = 0;
    for (r_out_load = 0; r_out_load < 12; r_out_load++)
    {
        contextQ3GS->upsStatus |= (((upsSataus[r_out_load] == '0') ? 0 : 1) << r_out_load);
    }
}
void qfsPro(char *buff)
{
    // int qfsValue = 0;
    // sscanf(buff, "(%02d\r", &qfsValue);
    // *contextQFS = qfsValue;
    sscanf(buff, "(%02d\r", contextQFS);

    // sscanf(buff, "%[ ]%[^ ]%[ ]%[^ ] %[^ ] %[^ ] %[^ ] %[^ ] %[^ ] %[^ ]", TTTTTTTTTTTTTTT, TTTTTTTTTTTTTTT, WWWWWWW,
    //        WWWWWWW, KKK, P_P, MMM, NNN, RR, BB_B);
}
void q3wsPro(char *buff)
{
    char cache[35] = {0};
    uint8_t i;
    sscanf(buff, "(%[^\r]\r", cache);
    *contextQ3WS = 0;
    for (i = 0; i < strlen(cache); i++)
    {
        *contextQ3WS |= (((cache == '1') ? 1 : 0) << i);
    }
}
