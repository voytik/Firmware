 /**
 * @file skydog_autopilot_params.c
 * Parameters for skydog_autopilot
 *
 * Author: Vojtech Kuchar
 * vojtech.kuchar@seznam.cz
 */

#include "Skydog_autopilot_params.h"
#include <skydog_autopilot/Skydog_autopilot_ert_rtw/Skydog_autopilot.h>

/*
PARAM_DEFINE_FLOAT(INAV_Alti_I, 0.05F);
PARAM_DEFINE_FLOAT(INAV_Alti_P, 0.2f);
PARAM_DEFINE_FLOAT(INAV_Pitch_I, 0.5F);
PARAM_DEFINE_FLOAT(INAV_Pitch_P, 1.0F);
PARAM_DEFINE_FLOAT(INAV_Pitch_r_P, 0.2F);
PARAM_DEFINE_FLOAT(INAV_Roll_I, 1.0F);
PARAM_DEFINE_FLOAT(INAV_Roll_P, 3.0F);
PARAM_DEFINE_FLOAT(INAV_Roll_r, 0.1F);
PARAM_DEFINE_FLOAT(INAV_Rollyaw_FF, 0.0F);
PARAM_DEFINE_FLOAT(INAV_Speed_I, 0.3F);
PARAM_DEFINE_FLOAT(INAV_Speed_P, 0.5F);
PARAM_DEFINE_FLOAT(INAV_Yaw_r_I, 0.0F);
PARAM_DEFINE_FLOAT(INAV_Yaw_r_P, 0.01F);
*/

PARAM_DEFINE_FLOAT(INAV_W_SKDG1, 0.5f);


int parameters_init(struct skydog_autopilot_param_handles *h)
{
	h->Alti_control_I = param_find("INAV_Alti__I");
	h->Alti_control_P = param_find("INAV_Alti_P");
	h->Pitch_control_I = param_find("INAV_Pitch_I");
	h->Pitch_control_P = param_find("INAV_Pitch_P");
	h->Pitch_rate_control_P = param_find("INAV_Pitch_r_P");
	h->Roll_control_I = param_find("INAV_Roll_I");
	h->Roll_control_P = param_find("INAV_Roll_P");
	h->Roll_rate_control = param_find("INAV_Roll_r");
	h->Roll_yaw_FF = param_find("INAV_Rollyaw_FF");
	h->Speed_control_I = param_find("INAV_Speed_I");
	h->Speed_control_P = param_find("INAV_Speed_P");
	h->Yaw_rate_control_I = param_find("INAV_Yaw_r_I");
	h->Yaw_rate_control_P = param_find("INAV_Yaw_r_P");

	return OK;
}

int parameters_update(const struct skydog_autopilot_param_handles *h, struct skydog_autopilot_params *p)
{
	param_get(h->Alti_control_I, &(p->Alti_control_I));
	param_get(h->Alti_control_P, &(p->Alti_control_P));
	param_get(h->Pitch_control_I, &(p->Pitch_control_I));
	param_get(h->Pitch_control_P, &(p->Pitch_control_P));
	param_get(h->Pitch_rate_control_P, &(p->Pitch_rate_control_P));
	param_get(h->Roll_control_I, &(p->Roll_control_I));
	param_get(h->Roll_control_P, &(p->Roll_control_P));
	param_get(h->Roll_rate_control, &(p->Roll_rate_control));
	param_get(h->Speed_control_I, &(p->Speed_control_I));
	param_get(h->Speed_control_P, &(p->Speed_control_P));
	param_get(h->Yaw_rate_control_I, &(p->Yaw_rate_control_I));
	param_get(h->Yaw_rate_control_P, &(p->Yaw_rate_control_P));

	return OK;
}
