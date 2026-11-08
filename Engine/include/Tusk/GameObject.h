#pragma once
#include <vector>
#include <memory>

#include <Tusk/Components/ComponentBase.h>
#include <Tusk/Components/TransformComponent.h>

namespace Tusk
{
	class GameObject
	{
	public:
		GameObject() { AddComponent<TransformComponent>(); };
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

		void Draw(Renderer& renderer)
		{
			for(auto& component : components)
			{
				component->Draw(renderer);
			}
		}

	private:
		std::vector<std::unique_ptr<ComponentBase>> components;
	};
}