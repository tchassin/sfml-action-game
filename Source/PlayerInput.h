#pragma once

#include <unordered_map>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

#include "Types.h"

// Singleton class with static method that manages virtual buttons reprensenting player input
class PlayerInput
{
public:
    // Represent virtual buttons based on both keyboard and gamepad input
    enum class Button : u8
    {
        Down,
        Up,
        Left,
        Right,

        A,
        B,
        X,
        Y,

        LeftShoulder,
        RighShoulder,

        Start,
        Select,

        BUTTON_COUNT
    };

private:
    // Represent the state of virtual buttons
    enum class ButtonState : u8
    {
        // Not pressed
        Up,
        // Pressed for more than one frame
        Down,
        // Pressed since last frame
        Pressed,
        // Released since last frame
        Released,
    };

private:
    PlayerInput();
    ~PlayerInput() {};

public:
    // Updated the state of the player's input
    static void update();

    // Return true if the button is pressed down
    static bool isButtonDown(Button button);
    // Return true if the button has been pressed this frame
    static bool isButtonPressed(Button button);
    // Return true if the button has been released this frame
    static bool isButtonReleased(Button button);

private:
    // Check for actual key and axis states then update virtual button state
    void updateDirectionButton(Button button, sf::Keyboard::Key mainKey, sf::Keyboard::Key altKey, sf::Joystick::Axis joystickAxis, bool isNegative);
    // Check for actual key and button states then update virtual button state
    void updateButton(Button button, sf::Keyboard::Key key, u32 joystickButton);
    // Set the virtual button state based on previous state and currnt input
    void updateButtonState(Button button, bool isPressed);

private:
    static PlayerInput* m_instance;

    std::unordered_map<Button, ButtonState> m_buttonStates;
};

