/**
 * @file skydog_autopilot_params.c
 * Parameters for skzdog autopilot
 *
 * Author: Vojtech Kuchar
 * vojtech.kuchar@seznam.cz
 */

#include "Skydog_autopilot_params.h"
//#include <skydog_autopilot/Skydog_autopilot_ert_rtw/Skydog_autopilot.h>

PARAM_DEFINE_FLOAT(INAV_W_SKDG, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_ALT_P, 0.2f);
PARAM_DEFINE_FLOAT(INAV_W_SKDG2, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_ALT_I, 0.05f);
PARAM_DEFINE_FLOAT(INAV_W_SKDG3, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_PITCH_P, 1.0f);
PARAM_DEFINE_FLOAT(INAV_W_SKDG4, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_PITCH_I, 0.5f);
PARAM_DEFINE_FLOAT(INAV_W_SKDG5, 50.0f);

PARAM_DEFINE_FLOAT(SKDG_PITCH_RATE_P, -0.2F);

PARAM_DEFINE_FLOAT(INAV_W_SKDG6, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_ROLL_I, 1.0F);
PARAM_DEFINE_FLOAT(INAV_W_SKDG7, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_ROLL_P, 3.0f);

PARAM_DEFINE_FLOAT(INAV_W_SKDG8, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_ROLL_RATE_P, 0.1f);

PARAM_DEFINE_FLOAT(INAV_W_SKDG9, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_ROLLYAW_FF, 0.0f);

PARAM_DEFINE_FLOAT(INAV_W_SKDG10, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_SPEED_I, 0.3f);

PARAM_DEFINE_FLOAT(INAV_W_SKDG11, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_SPEED_P, 0.5f);

PARAM_DEFINE_FLOAT(INAV_W_SKDG12, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_YAW_RATE_I, 0.0f);

PARAM_DEFINE_FLOAT(INAV_W_SKDG13, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_YAW_RATE_P, 0.01f);



int parameters_init(struct skydog_autopilot_param_handles *h)
{
	//get handles
	h->Alt_P = param_find("SKDG_ALT_P");
	h->Alt_I = param_find("SKDG_ALT_I");
	h->Pitch_P = param_find("SKDG_PITCH_P");
	h->Pitch_I = param_find("SKDG_PITCH_I");
	h->Pitch_rate_control_P = param_find("SKDG_PITCH_RATE_P");
	h->Roll_control_I = param_find("SKDG_ROLL_I");
	h->Roll_control_P = param_find("SKDG_ROLL_P");
	h->Roll_rate_control_P = param_find("SKDG_ROLL_RATE_P");
	h->Roll_yaw_FF = param_find("SKDG_ROLLYAW_FF");
	h->Speed_control_I= param_find("SKDG_SPEED_I");
	h->Speed_control_P = param_find("SKDG_SPEED_P");
	h->Yaw_rate_control_I = param_find("SKDG_YAW_RATE_I");
	h->Yaw_rate_control_P = param_find("SKDG_YAW_RATE_P");

	return OK;
}

int parameters_update(const struct skydog_autopilot_param_handles *h, struct skydog_autopilot_params *p)
{
	param_get(h->Alt_P, &(p->Alt_P));
	param_get(h->Alt_I, &(p->Alt_I));
	param_get(h->Pitch_P, &(p->Pitch_P));
	param_get(h->Pitch_I, &(p->Pitch_I));
	param_get(h->Pitch_rate_control_P, &(p->Pitch_rate_control_P));
	param_get(h->Roll_control_I, &(p->Roll_control_I));
	param_get(h->Roll_control_P, &(p->Roll_control_P));
	param_get(h->Roll_rate_control_P, &(p->Roll_rate_control_P));

	param_get(h->Roll_yaw_FF, &(p->Roll_yaw_FF));
	param_get(h->Speed_control_I, &(p->Speed_control_I));
	param_get(h->Speed_control_P, &(p->Speed_control_P));
	param_get(h->Yaw_rate_control_I, &(p->Yaw_rate_control_I));
	param_get(h->Yaw_rate_control_P, &(p->Yaw_rate_control_P));

	return OK;
}
