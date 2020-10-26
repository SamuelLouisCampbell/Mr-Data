#pragma once
#include <queue>
class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			CPress,
			CRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		bool centerIsPressed;
		int x = 0;
		int y = 0;
	public:
		Event() noexcept
			:
			type(Type::Invalid),
			leftIsPressed(false),
			rightIsPressed(false),
			centerIsPressed(false),
			x(0),
			y(0)
		{}
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			centerIsPressed(parent.centerIsPressed),
			x(parent.x),
			y(parent.y)
		{}
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		Type GetType() const noexcept
		{
			return type;
		}
		std::pair<int, int> GetPos() const noexcept
		{
			return { x,y };
		}
		int GetPosX() const noexcept
		{
			return x;
		}
		int GetPosY() const noexcept
		{
			return y;
		}
		bool LeftIsPressed() const noexcept
		{
			return leftIsPressed;
		}
		bool RightIsPressed() const noexcept
		{
			return rightIsPressed;
		}
		bool CenterIsPressed() const noexcept
		{
			return centerIsPressed;
		}

	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	bool CenterIsPressed() const noexcept;
	bool IsInWindow() const noexcept;
	Mouse::Event Read();
	bool IsEmpty() const noexcept;
	void Flush() noexcept;


private:
	//windows api handles
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftRelease(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightRelease(int x, int y) noexcept;
	void OnCPressed(int x, int y) noexcept;
	void OnCRelease(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
	void TrimBuffer() noexcept;
private:
	static constexpr unsigned int bufferSize = 4u;
	int x = 0;
	int y = 0;
	int wheelDelta = 0;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool centerIsPressed = false;
	bool isInWindow = false;
	std::queue<Event> buffer;
};

