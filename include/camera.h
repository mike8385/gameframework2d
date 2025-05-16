#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "gfc_shape.h"
#include "gfc_vector.h"


typedef struct
{
	GFC_Rect		camera;
	GFC_Rect		bounds;
	GFC_Vector2D	position;
	GFC_Vector2D	size;
}Camera;

static Camera _camera = { 0 };

#include "simple_logger.h"

#include "camera.h"

//static Camera _theCamera = { 0 };

GFC_Vector2D camera_get_position();

GFC_Vector2D camera_get_offset();

void camera_set_position(GFC_Vector2D position);

void camera_bounds_check();

void camera_set_size(GFC_Vector2D size);

void camera_set_bounds(GFC_Rect bounds);

void camera_center_on(GFC_Vector2D point);

void camera_center_on_mouse();

//void camera_center_on_two_players(GFC_Vector2D point1, GFC_Vector2D point2);


void camera_center_on_two_players(GFC_Vector2D point1, GFC_Vector2D point2);

#endif

#pragma once
