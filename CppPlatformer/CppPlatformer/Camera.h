#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\View.hpp>

namespace sf{
	class View;
}

class Camera
{
public:
	Camera(void);
	virtual ~Camera(void);

	void DeInit();
	void Init(const sf::FloatRect &size, const sf::FloatRect &viewPort = sf::FloatRect(0.0f,0.0f,1.0f,1.0f) );
	void MoveCamera(sf::Vector2f offset);
	const sf::View & GetView();

private:
	sf::View mView;

};

#endif
