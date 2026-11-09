#pragma once
#include <Tusk/Components/ComponentBase.h>
#include <Tusk/Components/TransformComponent.h>

#include <vector>
#include <memory>

namespace Tusk
{
	class Scene;

	class GameObject
	{
	public:
		GameObject(Scene* parent) : parent(parent) { AddComponent<TransformComponent>(); };
		~GameObject() = default;

		template<class T>
		T* AddComponent() 
		{
			components.push_back(std::make_unique<T>(this));
			return dynamic_cast<T*>(components.back().get());
		}

		template<class T>
		T* GetComponent() noexcept
		{
			static_assert(std::is_base_of_v<ComponentBase, T>);
			for(const auto& component : components)
			{
				if (auto* match = dynamic_cast<T*>(component.get()))
					return match;
			}
			return nullptr;
		}

		void Start()
		{
			for (auto& component : components)
			{
				component->Start();
			}
		}

		void FixedUpdate(float fixedDeltaTime)
		{
			for(auto& component : components)
			{
				component->FixedUpdate(fixedDeltaTime);
			}
		}

		void Draw(Renderer& renderer)
		{
			for(auto& component : components)
			{
				component->Draw(renderer);
			}
		}

		Scene* GetParentScene() { return parent; }

	private:
		Scene* parent;
		std::vector<std::unique_ptr<ComponentBase>> components;
	};
}