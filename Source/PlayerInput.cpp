#include "PlayerInput.h"

namespace
{
    const f32 DEADZONE = 30.0f;

    const u32 GAMEPAD_BUTTON_SOUTH = 0;
    const u32 GAMEPAD_BUTTON_EAST = 1;
    const u32 GAMEPAD_BUTTON_WEST = 2;
    const u32 GAMEPAD_BUTTON_NORTH = 3;

    const u32 GAMEPAD_LEFT_SHOULDER = 4;
    const u32 GAMEPAD_RIGHT_SHOULDER = 5;

    const u32 GAMEPAD_SELECT = 6;
    const u32 GAMEPAD_START = 7;

    const sf::Joystick::Axis GAMEPAD_DPAD_X = sf::Joystick::Axis::PovX;
    const sf::Joystick::Axis GAMEPAD_DPAD_Y = sf::Joystick::Axis::PovY;
}

PlayerInput* PlayerInput::m_instance = new PlayerInput();

PlayerInput::PlayerInput()
{
    // Initialize button states
    for (u8 i = 0; i < (u8)Button::BUTTON_COUNT; i++)
        m_buttonStates[(Button)i] = ButtonState::Up;
}

void PlayerInput::update()
{
    // Check directional input
    m_instance->updateDirectionButton(Button::Up, sf::Keyboard::Up, sf::Keyboard::W, GAMEPAD_DPAD_Y, false);
    m_instance->updateDirectionButton(Button::Down, sf::Keyboard::Down, sf::Keyboard::S, GAMEPAD_DPAD_Y, true);
    m_instance->updateDirectionButton(Button::Right, sf::Keyboard::Right, sf::Keyboard::A, GAMEPAD_DPAD_X, false);
    m_instance->updateDirectionButton(Button::Left, sf::Keyboard::Left, sf::Keyboard::D, GAMEPAD_DPAD_X, true);

    // Check action buttons
    m_instance->updateButton(Button::A, sf::Keyboard::H, GAMEPAD_BUTTON_SOUTH);
    m_instance->updateButton(Button::B, sf::Keyboard::J, GAMEPAD_BUTTON_EAST);
    m_instance->updateButton(Button::X, sf::Keyboard::K, GAMEPAD_BUTTON_WEST);
    m_instance->updateButton(Button::Y, sf::Keyboard::L, GAMEPAD_BUTTON_NORTH);

    // Check shoulder buttons
    m_instance->updateButton(Button::LeftShoulder, sf::Keyboard::U, GAMEPAD_LEFT_SHOULDER);
    m_instance->updateButton(Button::RighShoulder, sf::Keyboard::I, GAMEPAD_RIGHT_SHOULDER);

    // Check other buttons
    m_instance->updateButton(Button::Start, sf::Keyboard::O, GAMEPAD_START);
    m_instance->updateButton(Button::Select, sf::Keyboard::P, GAMEPAD_SELECT);
}

bool PlayerInput::isButtonPressed(Button button)
{
    return m_instance->m_buttonStates.at(button) == ButtonState::Pressed;
}

bool PlayerInput::isButtonDown(Button button)
{
    return m_instance->m_buttonStates.at(button) == ButtonState::Pressed
        || m_instance->m_buttonStates.at(button) == ButtonState::Down;
}

bool PlayerInput::isButtonReleased(Button button)
{
    return m_instance->m_buttonStates.at(button) == ButtonState::Released;
}

void PlayerInput::updateDirectionButton(Button button, sf::Keyboard::Key mainKey, sf::Keyboard::Key altKey, sf::Joystick::Axis joystickAxis, bool isNegative)
{
    const bool isPressed = sf::Keyboard::isKeyPressed(mainKey)
        || sf::Keyboard::isKeyPressed(altKey)
        || (sf::Joystick::isConnected(0)
            && sf::Joystick::getAxisPosition(0, joystickAxis) * (isNegative ? -1 : 1) > DEADZONE);
    updateButtonState(button, isPressed);
}

void PlayerInput::updateButton(Button button, sf::Keyboard::Key key, u32 joystickButton)
{
    const bool isPressed = sf::Keyboard::isKeyPressed(key)
        || (sf::Joystick::isConnected(0)
            && sf::Joystick::isButtonPressed(0, joystickButton));
    updateButtonState(button, isPressed);
}

void PlayerInput::updateButtonState(Button button, bool isPressed)
{
    const ButtonState currentState = m_buttonStates[button];
    if (isPressed)
    {
        m_buttonStates[button] = (currentState == ButtonState::Up || currentState == ButtonState::Released)
            ? ButtonState::Pressed
            : ButtonState::Down;
    }
    else
    {
        m_buttonStates[button] = (currentState == ButtonState::Down || currentState == ButtonState::Pressed)
            ? ButtonState::Released
            : ButtonState::Up;
    }
}
