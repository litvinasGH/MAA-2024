#include <time.h>
#include <iostream>
#pragma warning(disable : 4996)

extern "C" {
	void showtime()
	{
		time_t mytime = time(NULL);
		struct tm* now = localtime(&mytime);
		std::cout << "����: " << now->tm_mday << "." << now->tm_mon + 1 << "." << now->tm_year + 1900 << std::endl;
	}

	void ruslang()
	{
		setlocale(LC_ALL, "rus");
	}

	void today()
	{
		time_t mytime = time(NULL);
		struct tm* now = localtime(&mytime);
		std::cout << "�����: " << now->tm_hour << ":" << now->tm_min << ":"<< now->tm_sec << std::endl;
	}
	void showInt(int a)
	{
		std::cout << "�� ����� �����: ";
		std::cout << a << std::endl;
	}

	

	int writeInt()
	{
		std::cout << "������� ����� �����: ";
		int a;
		std::cin >> a;
		return a;
	}	  
}