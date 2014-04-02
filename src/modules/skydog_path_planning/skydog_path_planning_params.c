/**
 * @file skydog_autopilot_params.c
 * Parameters for skzdog autopilot
 *
 * Author: Vojtech Kuchar
 * vojtech.kuchar@seznam.cz
 */

#include "skydog_path_planning_params.h"


PARAM_DEFINE_FLOAT(INAV_W_SKDG14, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_PP_L, 20.0f);
PARAM_DEFINE_FLOAT(INAV_W_SKDG15, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_PP_R, 25.0f);
PARAM_DEFINE_FLOAT(INAV_W_SKDG16, 50.0f);
PARAM_DEFINE_FLOAT(SKDG_PP_TRASH, 20.0f);



int parameters_init(struct skydog_path_planniong_param_handles *h)
{
	//get handles
	h->L = param_find("SKDG_PP_L");
	h->R = param_find("SKDG_PP_R");
	h->Trash = param_find("SKDG_PP_TRASH");

	return OK;
}

int parameters_update(const struct skydog_path_planniong_param_handles *h, struct skydog_path_planning_params *p)
{
	param_get(h->L, &(p->L));
	param_get(h->R, &(p->R));
	param_get(h->Trash, &(p->Trash));


	return OK;
}
