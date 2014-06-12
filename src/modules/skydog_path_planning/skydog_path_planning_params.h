/*
 * @file skydog_path_planning_params.h
 *
 * Parameters for skydog_path_planning
 */

#include <systemlib/param/param.h>

struct skydog_path_planning_params {
	float L;
	float R;
	float Trash;
	float Alt_Trash;
	float Home_Alt_Offset;
	float WP_Speed;
};

struct skydog_path_planning_param_handles {
	param_t L;
	param_t R;
	param_t Trash;
	param_t Alt_Trash;
	param_t Home_Alt_Offset;
	param_t WP_Speed;

};

/**
 * Initialize all parameter handles and values
 *
 */
int parameters_init(struct skydog_path_planning_param_handles *h);

/**
 * Update all parameters
 *
 */
int parameters_update(const struct skydog_path_planning_param_handles *h, struct skydog_path_planning_params *p);
