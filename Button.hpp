#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Button
{
private:
	Shape *btnShape;
	bool isPressed;

public:
	Button(Shape *buttonShape);

};

Button::Button(Shape* buttonShape)
{
	btnShape = buttonShape;
	isPressed = false;
}

#endif
