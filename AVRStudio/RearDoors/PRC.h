#ifndef PRC_H
#define PRC_H

#define PRC_U16_FILTER1 0x120
#define PRC_U16_FILTER2 0x120

extern void PRC_v_init_f(void);
extern void PRC_v_refresh_control_f(void);
extern void PRC_v_process_f(void);
extern void PRC_v_refresh_status_f(void);

extern CAN_st_message_t PRC_stm_tx_message;

#endif
