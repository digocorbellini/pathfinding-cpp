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

	Button(Shape* buttonShape, Color fillColor);

	void setPosition(Vector2f pos);

	void setFillColor(Color color);

	bool isButtonPressedDown(RenderWindow *window);

	bool isButtonPressedUP(RenderWindow* window);

	bool isButtonPressed(RenderWindow* window);

	void drawButton(RenderWindow* window);
};

/// <summary>
/// Construct a new button object
/// </summary>
/// <param name="buttonShape">the shape of the button</param>
Button::Button(Shape* buttonShape)
{
	btnShape = buttonShape;
	isPressed = false;
}

/// <summary>
/// Construct a new button object and give it the given fill color
/// </summary>
/// <param name="buttonShape">the shape of the button</param>
/// <param name="fillColor">the fill color of the button</param>
Button::Button(Shape* buttonShape, Color fillColor)
{
	btnShape = buttonShape;
	btnShape->setFillColor(fillColor);
}

/// <summary>
/// Set the position of the button
/// </summary>
/// <param name="pos">The new position of this button</param>
void Button::setPosition(Vector2f pos)
{
	btnShape->setPosition(pos);
}

/// <summary>
/// Set the fill fillColor of this button
/// </summary>
/// <param name="fillColor">The new fill fillColor of the button</param>
void Button::setFillColor(Color color)
{
	btnShape->setFillColor(color);
}

/// <summary>
/// See if this button has been pressed down
/// </summary>
/// <param name="window">the window in which this button is in</param>
/// <returns>true if this button has been pressed down
/// and false otherwise</returns>
bool Button::isButtonPressedDown(RenderWindow* window)
{
	Vector2i mousePos = Mouse::getPosition(*window);
	Vector2f mouseWorldPos(mousePos);
	if (Mouse::isButtonPressed(Mouse::Button::Left) 
			&& btnShape->getGlobalBounds().contains(mouseWorldPos) 
			&& !isPressed)
	{
		isPressed = true;
		return true;
	}
	isButtonPressed(window);
	return false;
}

/// <summary>
/// See if this button has been released
/// </summary>
/// <param name="window">the window in which this button is in</param>
/// <returns>true if this button has being released
/// and false otherwise</returns>
bool Button::isButtonPressedUP(RenderWindow* window)
{
	Vector2i mousePos = Mouse::getPosition(*window);
	Vector2f mouseWorldPos(mousePos);
	if (Mouse::isButtonPressed(Mouse::Button::Left) 
			&& btnShape->getGlobalBounds().contains(mouseWorldPos) 
			&& isPressed)
	{
		isPressed = false;
		return true;
	}
	isButtonPressed(window);
	return false;
}

/// <summary>
/// See if this button is being held down
/// </summary>
/// <param name="window">the window in which this button is in</param>
/// <returns>true if this button is being held down
/// and false otherwise</returns>
bool Button::isButtonPressed(RenderWindow* window)
{
	Vector2i mousePos = Mouse::getPosition(*window);
	Vector2f mouseWorldPos(mousePos);
	if (Mouse::isButtonPressed(Mouse::Button::Left) 
			&& btnShape->getGlobalBounds().contains(mouseWorldPos))
	{
		isPressed = true;
		return true;
	}
	isPressed = false;
	return false;
}

/// <summary>
/// Draw this button
/// </summary>
/// <param name="window">the window that this button should be 
/// drawn to</param>
void Button::drawButton(RenderWindow* window)
{
	window->draw(*btnShape);
}

#endif
