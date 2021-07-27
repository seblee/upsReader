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
void QPIPro(char* buff);
void MPro(char* buff);
void QSPro(char* buff);
void QMDPro(char* buff);
void QGSPro(char* buff);
void QFSPro(char* buff);
void QWSPro(char* buff);
void QWFPro(char* buff);
void QMODPro(char* buff);
void QRIPro(char* buff);
void FPro(char* buff);
void Q3PVPro(char* buff);
void Q3PCPro(char* buff);
void Q3OCPro(char* buff);
void Q3YVPro(char* buff);
void QYFPro(char* buff);
void Q3YCPro(char* buff);
void Q3LDPro(char* buff);
void QRPVPro(char* buff);
void QBYVPro(char* buff);
void QBYFPro(char* buff);
void QPARPro(char* buff);
void QFLAGPro(char* buff);
void QVFWPro(char* buff);
void QVFW2Pro(char* buff);
void QVFW3Pro(char* buff);
void QVERPro(char* buff);
void QIDPro(char* buff);
void QBVPro(char* buff);
void QNBVPro(char* buff);
void QLDLPro(char* buff);
void QHEPro(char* buff);
void QFREPro(char* buff);
void QSKPro(char* buff);
void QSKTPro(char* buff);
void QBDRPro(char* buff);
void QTPRPro(char* buff);
void Q5Pro(char* buff);
void QCHGCPro(char* buff);
void QBUSPro(char* buff);
void QBUSPPro(char* buff);
void QBUSNPro(char* buff);
void QVBPro(char* buff);
void QVPro(char* buff);
void QVCPro(char* buff);
void QVLINEPro(char* buff);
void QVBYPPro(char* buff);
void QMAXWPro(char* buff);
void QMXVAPro(char* buff);
void QPDPro(char* buff);
void QBCOPro(char* buff);
void QIPTPro(char* buff);
void QNLPro(char* buff);
void QBATPro(char* buff);
void QTVPro(char* buff);
void QVSOCPro(char* buff);
void QBTAHPro(char* buff);
void QPSTPro(char* buff);
void QMFPro(char* buff);
void QTIMEPro(char* buff);
void QSTZPro(char* buff);
void QDWHPro(char* buff);

/* Private variables ---------------------------------------------------------*/
const cmd_map_t commandMap[] = {
    {"QPI", "(", "\r", QPIPro},        // UPS_QPI
    {"M", "(", "\r", MPro},            // UPS_M
    {"QS", "(", "\r", QSPro},          // UPS_QS
    {"QMD", "(", "\r", QMDPro},        // UPS_QMD
    {"QGS", "(", "\r", QGSPro},        // UPS_QGS
    {"QFS", "(", "\r", QFSPro},        // UPS_QFS
    {"QWS", "(", "\r", QWSPro},        // UPS_QWS
    {"QWF", "(", "\r", QWFPro},        // UPS_QWF
    {"QMOD", "(", "\r", QMODPro},      // UPS_QMOD
    {"QRI", "(", "\r", QRIPro},        // UPS_QRI
    {"F", "(", "\r", FPro},            // UPS_F
    {"Q3PV", "(", "\r", Q3PVPro},      // UPS_Q3PV
    {"Q3PC", "(", "\r", Q3PCPro},      // UPS_Q3PC
    {"Q3OC", "(", "\r", Q3OCPro},      // UPS_Q3OC
    {"Q3YV", "(", "\r", Q3YVPro},      // UPS_Q3YV
    {"QYF", "(", "\r", QYFPro},        // UPS_QYF
    {"Q3YC", "(", "\r", Q3YCPro},      // UPS_Q3YC
    {"Q3LD", "(", "\r", Q3LDPro},      // UPS_Q3LD
    {"QRPV", "(", "\r", QRPVPro},      // UPS_QRPV
    {"QBYV", "(", "\r", QBYVPro},      // UPS_QBYV
    {"QBYF", "(", "\r", QBYFPro},      // UPS_QBYF
    {"QPAR", "(", "\r", QPARPro},      // UPS_QPAR
    {"QFLAG", "(", "\r", QFLAGPro},    // UPS_QFLAG
    {"QVFW", "(", "\r", QVFWPro},      // UPS_QVFW
    {"QVFW2", "(", "\r", QVFW2Pro},    // UPS_QVFW2
    {"QVFW3", "(", "\r", QVFW3Pro},    // UPS_QVFW3
    {"QVER", "(", "\r", QVERPro},      // UPS_QVER
    {"QID", "(", "\r", QIDPro},        // UPS_QID
    {"QBV", "(", "\r", QBVPro},        // UPS_QBV
    {"QNBV", "(", "\r", QNBVPro},      // UPS_QNBV
    {"QLDL", "(", "\r", QLDLPro},      // UPS_QLDL
    {"QHE", "(", "\r", QHEPro},        // UPS_QHE
    {"QFRE", "(", "\r", QFREPro},      // UPS_QFRE
    {"QSK", "(", "\r", QSKPro},        // UPS_QSK
    {"QSKT", "(", "\r", QSKTPro},      // UPS_QSKT
    {"QBDR", "(", "\r", QBDRPro},      // UPS_QBDR
    {"QTPR", "(", "\r", QTPRPro},      // UPS_QTPR
    {"Q5", "(", "\r", Q5Pro},          // UPS_Q5
    {"QCHGC", "(", "\r", QCHGCPro},    // UPS_QCHGC
    {"QBUS", "(", "\r", QBUSPro},      // UPS_QBUS
    {"QBUSP", "(", "\r", QBUSPPro},    // UPS_QBUSP
    {"QBUSN", "(", "\r", QBUSNPro},    // UPS_QBUSN
    {"QVB", "(", "\r", QVBPro},        // UPS_QVB
    {"QV", "(", "\r", QVPro},          // UPS_QV
    {"QVC", "(", "\r", QVCPro},        // UPS_QVC
    {"QVLINE", "(", "\r", QVLINEPro},  // UPS_QVLINE
    {"QVBYP", "(", "\r", QVBYPPro},    // UPS_QVBYP
    {"QMAXW", "(", "\r", QMAXWPro},    // UPS_QMAXW
    {"QMXVA", "(", "\r", QMXVAPro},    // UPS_QMXVA
    {"QPD", "(", "\r", QPDPro},        // UPS_QPD
    {"QBCO", "(", "\r", QBCOPro},      // UPS_QBCO
    {"QIPT", "(", "\r", QIPTPro},      // UPS_QIPT
    {"QNL", "(", "\r", QNLPro},        // UPS_QNL
    {"QBAT", "(", "\r", QBATPro},      // UPS_QBAT
    {"QTV", "(", "\r", QTVPro},        // UPS_QTV
    {"QVSOC", "(", "\r", QVSOCPro},    // UPS_QVSOC
    {"QBTAH", "(", "\r", QBTAHPro},    // UPS_QBTAH
    {"QPST", "(", "\r", QPSTPro},      // UPS_QPST
    {"QMF", "(", "\r", QMFPro},        // UPS_QMF
    {"QTIME", "(", "\r", QTIMEPro},    // UPS_QTIME
    {"QSTZ", "(", "\r", QSTZPro},      // UPS_QSTZ
    {"QDWH", "(", "\r", QDWHPro},      // UPS_QDWH

};

uint8_t contextArea[128] = {0};
uint8_t* qpiValuePoint   = contextArea;
upsQs_t* qsValuePoint    = (upsQs_t*)(contextArea + 1);
upaQmd_t* qmdPoint       = (upaQmd_t*)(contextArea + 1 + sizeof(upsQs_t));

q3gs_t* contextQ3GS   = (q3gs_t*)contextArea;
uint8_t* contextQFS   = (contextArea + sizeof(q3gs_t));
uint32_t* contextQ3WS = (uint32_t*)(contextArea + sizeof(q3gs_t) + 1);
/* Public variables ---------------------------------------------------------*/
osMessageQId ups0ResponseMsgId;
osMessageQDef(ups0ResponseMsgId, 5, unsigned char);
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

void contextVariablesInit(void)
{
    ups0ResponseMsgId = osMessageCreate(osMessageQ(ups0ResponseMsgId), NULL);
}

osEvent upsCommand(_upsInquiryCmd_t cmd, uint32_t timeout)
{
    osEvent ThreadEvent;
    memset(rx1buffer, 0, rx1Count);
    rx1Count = 0;
    uart1_dma_send((uint8_t*)commandMap[cmd].cmd, 5);  // strlen(commandMap[cmd].cmd));
    ThreadEvent = osMessageGet(ups0ResponseMsgId, timeout);
    if (ThreadEvent.status == osEventTimeout)
    {
        // ThreadEvent.status = osOK;
    }
    else if (ThreadEvent.status == osEventMessage)
    {
        if (ThreadEvent.value.v > 0)
        {
            char* p;
            p = strstr((void*)rx1buffer, commandMap[cmd].bkHead);
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

void Q3WSPro(char* buff)
{
    char cache[35] = {0};
    uint8_t i;
    sscanf(buff, "(%[^\r]\r", cache);
    *contextQ3WS = 0;
    for (i = 0; i < strlen(cache); i++)
    {
        *contextQ3WS |= (((cache[i] == '1') ? 1 : 0) << i);
    }
}

void QPIPro(char* buff)
{
    int qpi_v;
    sscanf((void*)buff, "(PI%02d\r", &qpi_v);
    *qpiValuePoint = qpi_v;
}
void MPro(char* buff)
{
    *buff = *buff;
}
void QSPro(char* buff)
{
    float mmm, nnn, ppp, rrr, sss, ttt;
    int qqq;
    char upsStatus[20];
    //"(MMM.M NNN.N PPP.P QQQ RR.R SS.S TT.T 76543210\r"
    sscanf(buff, "(%f %f %f %d %f %f %f %s\r", &mmm, &nnn, &ppp, &qqq, &rrr, &sss, &ttt, upsStatus);

    qsValuePoint->inputVoltage        = mmm * 10;
    qsValuePoint->inputFaultValtage   = nnn * 10;
    qsValuePoint->outputVoltage       = ppp * 10;
    qsValuePoint->outputLoad          = qqq;
    qsValuePoint->outputFrequency     = rrr * 10;
    qsValuePoint->batteryVoltage      = sss * 10;
    qsValuePoint->internalTemperature = ttt * 10;
    qsValuePoint->upsStatus           = 0;
    for (qqq = 0; qqq < 8; qqq++)
    {
        qsValuePoint->upsStatus |= ((upsStatus[qqq] == '1') ? (1 << qqq) : 0);
    }

    // sscanf(buff, "%[ ]%[^ ]%[ ]%[^ ] %[^ ] %[^ ] %[^ ] %[^ ] %[^ ] %[^ ]", TTTTTTTTTTTTTTT, TTTTTTTTTTTTTTT, WWWWWWW,
    //        WWWWWWW, KKK, P_P, MMM, NNN, RR, BB_B);
}

void QMDPro(char* buff)
{
    char ttt[15];
    char www[7];
    int kk, p1, p2, mmm, nnn, rr;
    float bbb;

    //(TTTTTTTTTTTTTTT WWWWWWW KK P/P MMM NNN RR BB.B\r
    sscanf(buff, "(%[^ ] %[^ ] %d %d/%d %d %d %d %f\r", ttt, www, &kk, &p1, &p2, &mmm, &nnn, &rr, &bbb);
}
void QGSPro(char* buff)
{
    *buff = *buff;
}
void QFSPro(char* buff)
{
    int qfsValue = 0;
    sscanf(buff, "(%02d\r", &qfsValue);
    *contextQFS = qfsValue;
    // sscanf(buff, "(%02d\r", contextQFS);

    // sscanf(buff, "%[ ]%[^ ]%[ ]%[^ ] %[^ ] %[^ ] %[^ ] %[^ ] %[^ ] %[^ ]", TTTTTTTTTTTTTTT, TTTTTTTTTTTTTTT, WWWWWWW,
    //        WWWWWWW, KKK, P_P, MMM, NNN, RR, BB_B);
}
void QWSPro(char* buff)
{
    *buff = *buff;
}
void QWFPro(char* buff)
{
    *buff = *buff;
}
void QMODPro(char* buff)
{
    *buff = *buff;
}
void QRIPro(char* buff)
{
    *buff = *buff;
}
void FPro(char* buff)
{
    *buff = *buff;
}
void Q3PVPro(char* buff)
{
    *buff = *buff;
}
void Q3PCPro(char* buff)
{
    *buff = *buff;
}
void Q3OCPro(char* buff)
{
    *buff = *buff;
}
void Q3YVPro(char* buff)
{
    *buff = *buff;
}
void QYFPro(char* buff)
{
    *buff = *buff;
}
void Q3YCPro(char* buff)
{
    *buff = *buff;
}
void Q3LDPro(char* buff)
{
    *buff = *buff;
}
void QRPVPro(char* buff)
{
    *buff = *buff;
}
void QBYVPro(char* buff)
{
    *buff = *buff;
}
void QBYFPro(char* buff)
{
    *buff = *buff;
}
void QPARPro(char* buff)
{
    *buff = *buff;
}
void QFLAGPro(char* buff)
{
    *buff = *buff;
}
void QVFWPro(char* buff)
{
    *buff = *buff;
}
void QVFW2Pro(char* buff)
{
    *buff = *buff;
}
void QVFW3Pro(char* buff)
{
    *buff = *buff;
}
void QVERPro(char* buff)
{
    *buff = *buff;
}
void QIDPro(char* buff)
{
    *buff = *buff;
}
void QBVPro(char* buff)
{
    *buff = *buff;
}
void QNBVPro(char* buff)
{
    *buff = *buff;
}
void QLDLPro(char* buff)
{
    *buff = *buff;
}
void QHEPro(char* buff)
{
    *buff = *buff;
}
void QFREPro(char* buff)
{
    *buff = *buff;
}
void QSKPro(char* buff)
{
    *buff = *buff;
}
void QSKTPro(char* buff)
{
    *buff = *buff;
}
void QBDRPro(char* buff)
{
    *buff = *buff;
}
void QTPRPro(char* buff)
{
    *buff = *buff;
}
void Q5Pro(char* buff)
{
    *buff = *buff;
}
void QCHGCPro(char* buff)
{
    *buff = *buff;
}
void QBUSPro(char* buff)
{
    *buff = *buff;
}
void QBUSPPro(char* buff)
{
    *buff = *buff;
}
void QBUSNPro(char* buff)
{
    *buff = *buff;
}
void QVBPro(char* buff)
{
    *buff = *buff;
}
void QVPro(char* buff)
{
    *buff = *buff;
}
void QVCPro(char* buff)
{
    *buff = *buff;
}
void QVLINEPro(char* buff)
{
    *buff = *buff;
}
void QVBYPPro(char* buff)
{
    *buff = *buff;
}
void QMAXWPro(char* buff)
{
    *buff = *buff;
}
void QMXVAPro(char* buff)
{
    *buff = *buff;
}
void QPDPro(char* buff)
{
    *buff = *buff;
}
void QBCOPro(char* buff)
{
    *buff = *buff;
}
void QIPTPro(char* buff)
{
    *buff = *buff;
}
void QNLPro(char* buff)
{
    *buff = *buff;
}
void QBATPro(char* buff)
{
    *buff = *buff;
}
void QTVPro(char* buff)
{
    *buff = *buff;
}
void QVSOCPro(char* buff)
{
    *buff = *buff;
}
void QBTAHPro(char* buff)
{
    *buff = *buff;
}
void QPSTPro(char* buff)
{
    *buff = *buff;
}
void QMFPro(char* buff)
{
    *buff = *buff;
}
void QTIMEPro(char* buff)
{
    *buff = *buff;
}
void QSTZPro(char* buff)
{
    *buff = *buff;
}
void QDWHPro(char* buff)
{
    *buff = *buff;
}
