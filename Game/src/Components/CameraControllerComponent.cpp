#include <TuskGame/Components/CameraControllerComponent.h>
#include <Tusk/Input.h>
#include <Tusk/GameObject.h>

namespace TuskGame
{
	void CameraControllerComponent::HandleInput(Tusk::Input& input)
	{
        //Demo Input. Basic input for purpose of demo.

        moveForward = moveBackward = moveLeft = moveRight = moveUp = moveDown = rotate = false;

        if (input.GetKeyDown(Tusk::W))
            moveForward = true;
        else if (input.GetKeyDown(Tusk::S))
            moveBackward = true;

        if (input.GetKeyDown(Tusk::D))
            moveRight = true;
        else if (input.GetKeyDown(Tusk::A))
            moveLeft = true;

        if (input.GetKeyDown(Tusk::SPACE))
            moveUp = true;
        else if (input.GetKeyDown(Tusk::LSHIFT))
            moveDown = true;
        
        if (input.GetMouseBtnDown(Tusk::LEFT))
            rotate = true;

        input.GetMouseDelta(&x, &y);
	}

    void CameraControllerComponent::FixedUpdate(float fixedDeltaTime)
    {
        auto* cameraTranasform = GetParentObject()->GetComponent<Tusk::TransformComponent>();
        auto newCamTranslation = cameraTranasform->GetTranslation();
        auto cameraForward = cameraTranasform->GetForward();
        auto cameraRight = cameraTranasform->GetRight();
        auto cameraUp = cameraTranasform->GetUp();

        if(moveForward)
            newCamTranslation += cameraForward * 8.f * fixedDeltaTime;
        else if(moveBackward)
            newCamTranslation -= cameraForward * 8.f * fixedDeltaTime;

        if(moveRight)
            newCamTranslation += cameraRight * 8.f * fixedDeltaTime;
        else if(moveLeft)
            newCamTranslation -= cameraRight * 8.f * fixedDeltaTime;

        if(moveUp)
            newCamTranslation += cameraUp * 8.f * fixedDeltaTime;
        else if(moveDown)
            newCamTranslation -= cameraUp * 8.f * fixedDeltaTime;

        if(rotate)
        {
            const float sensitivity = 0.4f;
            yaw += x * sensitivity;
            pitch += y * sensitivity;
            pitch = Tusk::Math::clamp(pitch, -89.f, 89.f);

            auto yawRotation = Tusk::Math::angleAxis(Tusk::Math::radians(yaw), Tusk::Math::vec3(0.f, 1.f, 0.f));
            auto pitchRotation = Tusk::Math::angleAxis(Tusk::Math::radians(pitch), Tusk::Math::vec3(-1.f, 0.f, 0.f));

            auto newRotation = pitchRotation * yawRotation;
            cameraTranasform->SetRotation(newRotation);
        }

        cameraTranasform->SetTranslation(newCamTranslation);
    }
}