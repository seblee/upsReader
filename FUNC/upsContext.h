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
    UPS_QPI,
    UPS_M,
    UPS_QS,
    UPS_QMD,
    UPS_QGS,
    UPS_QFS,
    UPS_QWS,
    UPS_QWF,
    UPS_QMOD,
    UPS_QRI,
    UPS_F,
    UPS_Q3PV,
    UPS_Q3PC,
    UPS_Q3OC,
    UPS_Q3YV,
    UPS_QYF,
    UPS_Q3YC,
    UPS_Q3LD,
    UPS_QRPV,
    UPS_QBYV,
    UPS_QBYF,
    UPS_QPAR,
    UPS_QFLAG,
    UPS_QVFW,
    UPS_QVFW2,
    UPS_QVFW3,
    UPS_QVER,
    UPS_QID,
    UPS_QBV,
    UPS_QNBV,
    UPS_QLDL,
    UPS_QHE,
    UPS_QFRE,
    UPS_QSK,
    UPS_QSKT,
    UPS_QBDR,
    UPS_QTPR,
    UPS_Q5,
    UPS_QCHGC,
    UPS_QBUS,
    UPS_QBUSP,
    UPS_QBUSN,
    UPS_QVB,
    UPS_QV,
    UPS_QVC,
    UPS_QVLINE,
    UPS_QVBYP,
    UPS_QMAXW,
    UPS_QMXVA,
    UPS_QPD,
    UPS_QBCO,
    UPS_QIPT,
    UPS_QNL,
    UPS_QBAT,
    UPS_QTV,
    UPS_QVSOC,
    UPS_QBTAH,
    UPS_QPST,
    UPS_QMF,
    UPS_QTIME,
    UPS_QSTZ,
    UPS_QDWH,
} _upsInquiryCmd_t;

typedef enum
{
    UPS_PE,
    UPS_PD,
    UPS_PSK,
    UPS_PSF,
    UPS_PGF,
    UPS_PLV,
    UPS_PHV,
    UPS_PF,
    UPS_PPD,
    UPS_BATN,
    UPS_BATGN,
    UPS_HEH,
    UPS_HEL,
    UPS_FREH,
    UPS_HEFH,
    UPS_HEFL,
    UPS_FREL,
    UPS_BDR,
    UPS_ID,
    UPS_RESET,
    UPS_REEP,
    UPS_V,
    UPS_BUS,
    UPS_F50,
    UPS_F60,
    UPS_CHGC,
    UPS_MAXW,
    UPS_MXVA,
    //UPS_V,
    UPS_VLINE,
    UPS_VBYP,
    UPS_BUSP,
    UPS_BUSN,
    UPS_VB,
    UPS_VC,
    UPS_VOP,
    UPS_VOPS,
    UPS_VOPT,
    UPS_VBATCO,
    UPS_IPT,
    UPS_NL,
    UPS_BATCAP,
    UPS_BATCOEF,
    UPS_VDCR,
    UPS_VDCS,
    UPS_VDCT,
    UPS_VSOC,
    UPS_BATTAH,
    UPS_PST,
    UPS_TSET,
    UPS_DWHRST,
    UPS_MM,
    UPS_MU,
    UPS_MD,
    UPS_PP1E,
    UPS_PP1D,
    UPS_I,
    UPS_O,
} _upsSettingCmd_t;
typedef struct
{
    char *cmd;
    char *bkHead;
    char *bkKey;
    void (*process)(char *);
} cmd_map_t;

typedef struct
{
    uint16_t inputVoltage;
    uint16_t inputFaultValtage;
    uint16_t outputVoltage;
    uint16_t outputLoad;
    uint16_t outputFrequency;
    uint16_t batteryVoltage;
    uint16_t internalTemperature;
    union
    {
        uint8_t upsStatus;
        struct
        {
            uint8_t b0 : 1;
            uint8_t b1 : 1;
            uint8_t b2 : 1;
            uint8_t b3 : 1;
            uint8_t b4 : 1;
            uint8_t b5 : 1;
            uint8_t b6 : 1;
            uint8_t b7 : 1;
        };
    };
} upsQs_t;

typedef struct
{
    char model[15];
    uint8_t powerFactor;
    union
    {
        uint8_t phaseNUM;
        struct
        {
            uint8_t inputPhase : 4;
            uint8_t outputPhase : 4;
        };
    };
    uint8_t batteryPieceNum;
    uint16_t nominalVoltageI;
    uint16_t nominalVoltageO;
    uint16_t batteryVoltagePerUnit;
    uint32_t rateVA;
} upaQmd_t;

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
osEvent upsCommand(_upsInquiryCmd_t cmd, uint32_t timeout);
void contextVariablesInit(void);
/* Private user code ---------------------------------------------------------*/

#endif /*__UPSCONTEXT_H_*/
