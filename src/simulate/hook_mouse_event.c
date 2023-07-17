/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_mouse_event.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonshin <yonshin@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 02:10:36 by minjungk          #+#    #+#             */
/*   Updated: 2023/07/17 14:53:07 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hook.h"
#include "simulate.h"

int	button_press(int button, int x, int y, void *param)
{
	t_world *const	world = param;

	world->gui.mouse.action[button] = MOUSE_PRESS;
	world->gui.mouse.prev[button] = vector3(x, y, 0);
	return (EXIT_SUCCESS);
}

int	button_release(int button, int x, int y, void *param)
{
	t_world *const	world = param;

	world->gui.mouse.action[button] = MOUSE_RELEASE;
	world->gui.mouse.prev[button] = vector3(x, y, 0);
	return (EXIT_SUCCESS);
}

void	rotate_camera(void *param)
{
	t_world *const			world = param;
	t_camera *const			camera = world->camera;
	t_gui_setting *const	gui = &world->gui;
	t_vector3				v[4];
	float					angle;

	enum e_type {FRONT, RIGHT, NEW, MOVE};
	if (gui->mouse.action[MOUSE_BUTTON_RIGHT] == MOUSE_IDLE)
		return ;
	if (gui->mouse.action[MOUSE_BUTTON_RIGHT] == MOUSE_RELEASE)
		gui->mouse.action[MOUSE_BUTTON_RIGHT] = MOUSE_IDLE;
	v[MOVE] = v3_sub(gui->mouse.prev[MOUSE_BUTTON_RIGHT], gui->mouse.curr);
	v[FRONT] = v3_normalize(v3_hadamard(camera->base.axis, vector3(1, 0, 1)));
	v[RIGHT] = v3_normalize(v3_cross(v[FRONT], world->axis));
	camera->base.axis = v3_rotate_axis(camera->base.axis, world->axis,
			v[MOVE].x / gui->screen.x);
	v[NEW] = v3_rotate_axis(camera->base.axis, v[RIGHT],
			v[MOVE].y / gui->screen.y);
	angle = v3_dot(v3_normalize(v3_cross(v[NEW], world->axis)), v[RIGHT]);
	if (angle > 0)
		camera->base.axis = v[NEW];
	gui->mouse.prev[MOUSE_BUTTON_RIGHT] = gui->mouse.curr;
}
