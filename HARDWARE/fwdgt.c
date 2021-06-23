#include "fwdgt.h"

/*!
    \brief      IRC40K configuration function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void irc40k_config(void)
{
    /* enable IRC40K */
    rcu_osci_on(RCU_IRC40K);
    /* wait till IRC40K is ready */
    rcu_osci_stab_wait(RCU_IRC40K);
}

void IWDG_Configuration(void)
{
    /* enable write access to FWDGT_PSC and FWDGT_RLD registers.
   FWDGT counter clock: 40KHz(IRC40K) / 64 = 0.625 KHz */

    if (rcu_flag_get(RCU_FLAG_FWDGTRST) != RESET)
    {
        rcu_all_reset_flag_clear();
    }

    fwdgt_config(313, FWDGT_PSC_DIV256);
    fwdgt_enable();
    return;
}
