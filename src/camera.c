#include "simple_logger.h"

#include "camera.h"

//static Camera _theCamera = { 0 };

GFC_Vector2D camera_get_position()
{
	return gfc_vector2d(_camera.camera.x, _camera.camera.y);
}

GFC_Vector2D camera_get_offset()
{
	return gfc_vector2d(-_camera.camera.x, -_camera.camera.y);//-_theCamera.x, -_theCamera.y);
}

void camera_set_position(GFC_Vector2D position)
{
	gfc_vector2d_copy(_camera.camera, position);
}

//void camera_bounds_check()
//{
//	if (_camera.camera.x < _camera.bounds.x) _camera.camera.x = _camera.bounds.x;
//	if (_camera.camera.y < _camera.bounds.y) _camera.camera.y = _camera.bounds.y;
//	if (_camera.camera.x + _camera.camera.w > _camera.bounds.x + _camera.bounds.w) _camera.camera.x = _camera.bounds.x + _camera.bounds.w;
//	if (_camera.camera.y + _camera.camera.h > _camera.bounds.y + _camera.bounds.h) _camera.camera.y = _camera.bounds.y + _camera.bounds.h;
//
//}

void camera_bounds_check()
{
	if (_camera.camera.x < _camera.bounds.x)
		_camera.camera.x = _camera.bounds.x;
	if (_camera.camera.y < _camera.bounds.y)
		_camera.camera.y = _camera.bounds.y;

	if (_camera.camera.x + _camera.camera.w > _camera.bounds.x + _camera.bounds.w)
		_camera.camera.x = (_camera.bounds.x + _camera.bounds.w) - _camera.camera.w;
	if (_camera.camera.y + _camera.camera.h > _camera.bounds.y + _camera.bounds.h)
		_camera.camera.y = (_camera.bounds.y + _camera.bounds.h) - _camera.camera.h;
}


void camera_set_size(GFC_Vector2D size)
{
	_camera.camera.w = size.x;
	_camera.camera.h = size.y;
}

void camera_set_bounds(GFC_Rect bounds)
{
	gfc_rect_copy(_camera.bounds, bounds);
}

void camera_center_on(GFC_Vector2D point)
{
	_camera.camera.x = point.x - (_camera.camera.w * 0.5);
	_camera.camera.y = point.y - (_camera.camera.h * 0.5);

}

void camera_center_on_mouse()
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	// The mouse position is screen coordinates.
	// To make camera follow it properly, center around it:
	GFC_Vector2D mouseWorldPos = gfc_vector2d(mx, my);

	// Now center camera on the mouse's world position
	camera_center_on(mouseWorldPos);
}