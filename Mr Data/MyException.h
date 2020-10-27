#pragma once
#include <exception>
#include <string>

class MyException : public std::exception
{
public:
	MyException(int line, const wchar_t* file) noexcept;
	const wchar_t* wideWhat() const noexcept;
	virtual const wchar_t* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::wstring& GetFile() const noexcept;
	std::wstring GetOriginString() const noexcept;
private:
	int line;
	std::wstring file;
protected:
	mutable std::wstring whatBuffer;
};