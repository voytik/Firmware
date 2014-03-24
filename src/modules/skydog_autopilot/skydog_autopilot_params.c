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



int parameters_init(struct skydog_autopilot_param_handles *h)
{
	//get handles
	h->Alt_P = param_find("SKDG_ALT_P");
	h->Alt_I = param_find("SKDG_ALT_I");
	h->Pitch_P = param_find("SKDG_PITCH_P");
	h->Pitch_I = param_find("SKDG_PITCH_I");

	//initialize with simulink values

	return OK;
}

int parameters_update(const struct skydog_autopilot_param_handles *h, struct skydog_autopilot_params *p)
{
	param_get(h->Alt_P, &(p->Alt_P));
	param_get(h->Alt_I, &(p->Alt_I));
	param_get(h->Pitch_P, &(p->Pitch_P));
	param_get(h->Pitch_I, &(p->Pitch_I));

	return OK;
}
