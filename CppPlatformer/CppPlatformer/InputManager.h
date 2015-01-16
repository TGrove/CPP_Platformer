#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>
#include <map>


//Holds Info on Registered Keys
struct KeyState{
	bool mChangedThisFrame;
	bool mIsDown;
	
	KeyState(): mChangedThisFrame(false), mIsDown(false) {/*Default constructor*/}
};


class InputManager
{
public:
	~InputManager(void);
	static InputManager * GetInstance();
	
	void CleanUp();
	void UpdateKeys( sf::Event keyEvent );
	void ResetForNewFrame();

	//Key is down; ignores this frame check
	bool KeyDown( sf::Keyboard::Key key );
	//Key pressed this frame
	bool KeyPressed( sf::Keyboard::Key key );
	//key released this frame
	bool KeyReleased( sf::Keyboard::Key key );
	
	void RegisterKey( sf::Keyboard::Key key );

	/*TODO*/
	bool MouseLeftDown();
	bool MouseLeftPressed();
	bool MouseLeftReleased();

	bool MouseRightDown();
	bool MouseRightPressed();
	bool MouseRightReleased();

private:
	InputManager(void);
	std::map<sf::Keyboard::Key, KeyState *> mKeyStates;

//	MouseState mMouseState;
};

#endif
