/**
 * @file skydog_autopilot_params.c
 * Parameters for skydog autopilot
 *
 * Author: Vojtech Kuchar
 * vojtech.kuchar@seznam.cz
 */

#include "skydog_path_planning_params.h"

// define all necessary parameters
// used INAV  parameters between due to bug in current firmware
PARAM_DEFINE_FLOAT(INAV_W_SKDG14, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_PP_L, 20.0f);
PARAM_DEFINE_FLOAT(INAV_W_SKDG15, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_PP_R, 25.0f);
PARAM_DEFINE_FLOAT(INAV_W_SKDG16, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_PP_TRASH, 20.0f);

PARAM_DEFINE_FLOAT(INAV_W_SKDG17, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_ALT_TRESH, 15.0f);

PARAM_DEFINE_FLOAT(INAV_W_SKDG18, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_HOME_ALT, 50.0f);

PARAM_DEFINE_FLOAT(INAV_W_SKDG19, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_SPEED, 15.0f);



int parameters_init(struct skydog_path_planning_param_handles *h)
{
	//get handles
	h->L = param_find("SKDG_PP_L");
	h->R = param_find("SKDG_PP_R");
	h->Trash = param_find("SKDG_PP_TRASH");
	h->Alt_Trash = param_find("SKDG_ALT_TRESH");
	h->Home_Alt_Offset = param_find("SKDG_HOME_ALT");
	h->WP_Speed = param_find("SKDG_SPEED");

	return OK;
}

int parameters_update(const struct skydog_path_planning_param_handles *h, struct skydog_path_planning_params *p)
{
	param_get(h->L, &(p->L));
	param_get(h->R, &(p->R));
	param_get(h->Trash, &(p->Trash));
	param_get(h->Alt_Trash, &(p->Alt_Trash));
	param_get(h->Home_Alt_Offset, &(p->Home_Alt_Offset));
	param_get(h->WP_Speed, &(p->WP_Speed));


	return OK;
}
