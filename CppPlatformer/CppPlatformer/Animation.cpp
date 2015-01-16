#include "Animation.h"
#include "Utility.h"
#include "AssetManager.h"

#include <SFML\Graphics\Sprite.hpp>

Animation::Animation( sf::Sprite *sprite, std::map<std::string,std::string> &props)
{
	mAssetManager = AssetManager::GetInstance();
	mSprite = sprite;
	mSprite->setTexture( (*mAssetManager->GetTexture( props["source"] )) );

	mCurrentFrameTime = 0.0f;
	mCurrentFrame = 0;
	mLoop = true;

	float f = Utility::Convert<float>( props["framerate"] );

	if( f != 0 ){
		mSecPerFrame = 1.0f / f;
	}else{
		mSecPerFrame = 0.f;
	}

	//Frames

	std::vector<std::string> rectStrings;
	Utility::Split(props ["frames"], rectStrings, ':');

	for( size_t i = 0; i < rectStrings.size(); i++){
		std::vector<std::string> rectVals;
		Utility::Split(rectStrings[i], rectVals, ',');
		mFrames.push_back(
			sf::IntRect(
				Utility::Convert<int>(rectVals[0]),
				Utility::Convert<int>(rectVals[1]),
				Utility::Convert<int>(rectVals[2]),
				Utility::Convert<int>(rectVals[3])
				)
			);
	}
		
	//FrameCount
}

Animation::Animation( sf::Sprite *sprite, int width, int height, int numframes, int framerate){
	mSprite = sprite;

	mCurrentFrameTime = 0.0f;
	mCurrentFrame = 0;

	//frames per second
	if( framerate != 0 ){
		mSecPerFrame = 1.0f / framerate;
	}else{
		mSecPerFrame = 0.0f;
	}

	//frames
	for( int i = 0; i < numframes; i++){
		mFrames.push_back(
			sf::IntRect( (i * width), 0, (width), (height) )
		);
	}
}

Animation::~Animation(void)
{
}

void Animation::Animate(float delta){
	//check timeset
	mCurrentFrameTime += delta;


	if( mCurrentFrameTime > mSecPerFrame){
		
		mCurrentFrameTime = 0;

		//Advance current animation
		if(mCurrentFrame +1  >= (int)mFrames.size()){
			//If looping
			if( mLoop ){
				//reset animation
				mCurrentFrame = 0;
			}
			//not looping
			else{
				//Clamp at end frame
			}
		}else{
			mCurrentFrame++;
		}
		
		//move rectangle
		mSprite->setTextureRect( mFrames[mCurrentFrame] );
	}

}

sf::Sprite * Animation::GetSprite(){
	return mSprite;
}

//Resets animation back to frame one
void Animation::Reset(){
	mCurrentFrame = 0;
	mCurrentFrameTime = 0;
	mSprite->setTextureRect( mFrames[mCurrentFrame] );
}

void Animation::SetLooping(bool b){
	mLoop = b;
}