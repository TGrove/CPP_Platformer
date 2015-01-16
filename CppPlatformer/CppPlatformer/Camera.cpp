#include "Camera.h"


Camera::Camera(void)
{
}


Camera::~Camera(void)
{
}

void Camera::DeInit()
{
}

void Camera::Init(const sf::FloatRect &size, const sf::FloatRect &viewPort )
{
	mView.reset(size);
	mView.setViewport(viewPort);
}

void Camera::MoveCamera(sf::Vector2f offset)
{
	mView.move(offset);
	//mView.setCenter(offset);
}

const sf::View & Camera::GetView()
{
	return mView;
}
