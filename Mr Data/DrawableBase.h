#pragma once

#include"Drawable.h"
#include "IndexBuffer.h"
#include <assert.h>

template<class T>
class DrawableBase : public Drawable
{
public:
	bool IsStaticInitialized() const noexcept
	{
		return !staticBinds.empty();
	}
	void AddStaticBind(std::unique_ptr<Bindable> bind)
	{
		staticBinds.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> iBuffer)
	{
		assert(pIndexBuffer == nullptr);
		pIndexBuffer = iBuffer.get();
		staticBinds.push_back(std::move(iBuffer));
	}
	void SetIndexFromStatic() noexcept
	{
		assert(pIndexBuffer == nullptr);
		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p;
				return; 
			}
		}
		assert(pIndexBuffer == nullptr);
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept
	{
		return staticBinds;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;

