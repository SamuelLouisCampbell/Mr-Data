#include "MyException.h"
#include <sstream>

MyException::MyException(int line, const wchar_t* file) noexcept
	:
	line(line),
	file(file)
{
}

const wchar_t* MyException::wideWhat() const noexcept
{
	std::wostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const wchar_t* MyException::GetType() const noexcept
{
	return L"An Exception Mr Data!";
}

int MyException::GetLine() const noexcept
{
	return line;
}

const std::wstring& MyException::GetFile() const noexcept
{
	return file;
}

std::wstring MyException::GetOriginString() const noexcept
{
	std::wostringstream oss;
	oss << "[File] " << file << std::endl;
	oss << "[Line] " << line;
	return oss.str();
}
