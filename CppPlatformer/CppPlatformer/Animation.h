#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <vector>
#include <map>
#include <string>

#include <SFML\Graphics\Rect.hpp>

class AssetManager;
namespace sf{
	class Sprite;
	class RenderWindow;
}


class Animation
{

public:
	Animation( sf::Sprite *sprite, std::map<std::string,std::string> &props);
	//sprite, framewidth, framehieght, number of frames in texture, frames per second 
	Animation( sf::Sprite *sprite, int width, int height, int numframes, int framerate);
	~Animation(void);
	//loops through animation
	void Animate(float delta);
	//sets animation back to frame 1
	void Reset();
	void Animation::SetLooping(bool b);
	sf::Sprite * GetSprite();

private:
	AssetManager *mAssetManager;
	sf::Sprite *mSprite;
	
	float mCurrentFrameTime;
	float mSecPerFrame;
	int mCurrentFrame;

	bool mLoop;

	std::vector<sf::IntRect> mFrames;
};

#endif