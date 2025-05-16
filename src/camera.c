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

//void camera_center_on_two_players(GFC_Vector2D point1, GFC_Vector2D point2)
//{
//    // Calculate midpoint between the two players
//    GFC_Vector2D midpoint;
//    midpoint.x = (point1.x + point2.x) * 0.5f;
//    midpoint.y = (point1.y + point2.y) * 0.5f;
//
//    // Calculate distance between players
//	
//    float distance = gfc_vector2d_magnitude(vector2d(point1.x - point2.x, point1.y - point2.y));
//
//    // Determine zoom level based on player distance (optional)
//    float minZoom = 1.0f;  // Minimum zoom level
//    float maxZoom = 0.5f;  // Maximum zoom level
//    float zoomDistanceThreshold = 500.0f; // Distance at which zoom starts changing
//
//    float zoom = minZoom;
//    if (distance > zoomDistanceThreshold) {
//        zoom = minZoom - ((distance - zoomDistanceThreshold) / 1000.0f);
//        if (zoom < maxZoom) zoom = maxZoom;
//    }
//
//    // Center camera on midpoint
//    _camera.camera.x = midpoint.x - (_camera.camera.w * 0.5f * zoom);
//    _camera.camera.y = midpoint.y - (_camera.camera.h * 0.5f * zoom);
//
//    // Apply zoom (if you want zoom functionality)
//    //_camera.zoom = zoom;
//}
void camera_center_on_two_players(GFC_Vector2D point1, GFC_Vector2D point2)
{
	// Calculate midpoint
	_camera.camera.x = ((point1.x + point2.x) * 0.5f) - (_camera.camera.w * 0.5f);
	_camera.camera.y = ((point1.y + point2.y) * 0.5f) - (_camera.camera.h * 0.5f);

	// Optional: Keep players within certain distance
	float max_player_distance = 600.0f;
	float actual_distance = gfc_vector2d_magnitude(gfc_vector2d(point1.x - point2.x, point1.y - point2.y));

	if (actual_distance > max_player_distance) {
		// Push players closer together (or implement your preferred behavior)
	}
}
