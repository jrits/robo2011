/* helloworld.c for TOPPERS/ATK(OSEK) */
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void){ /* do nothing */ }

TASK(OSEK_Task_Background)
{
	SINT vars[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 0};
	while(1)
	{
		ecrobot_set_light_sensor_active(NXT_PORT_S3);
		vars[0] = (SINT)ecrobot_get_light_sensor(NXT_PORT_S3);
		vars[1] = (SINT)ecrobot_get_gyro_sensor(NXT_PORT_S1);
  		ecrobot_sint_var_monitor(vars);
		systick_wait_ms(50); /* 50msec wait */
	}
}
