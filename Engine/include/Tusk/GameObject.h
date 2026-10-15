#pragma once
#include <vector>

class ComponentBase;

class GameObject
{
public:
	GameObject() = default;
	~GameObject() = default;

	template<class T>
	T* AddComponent() 
	{
		components.push_back(new T());
	}

	template<class T>
	T* GetComponent() noexcept
	{
		static_assert(std::is_base_of_v<ComponentBase, T>);
		for(const auto& component : components)
		{
			if (auto* match = dynamic_cast<T*>(component))
				return match;
			else
				return nullptr;
		}
	}

private:
	std::vector<ComponentBase> components;
};