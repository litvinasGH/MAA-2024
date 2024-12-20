#include <time.h>
#include <iostream>
#pragma warning(disable : 4996)

extern "C" {
	void showtime()
	{
		time_t mytime = time(NULL);
		struct tm* now = localtime(&mytime);
		std::cout << "Дата: " << now->tm_mday << "." << now->tm_mon + 1 << "." << now->tm_year + 1900 << std::endl;
	}

	void ruslang()
	{
		setlocale(LC_ALL, "rus");
	}

	void today()
	{
		time_t mytime = time(NULL);
		struct tm* now = localtime(&mytime);
		std::cout << "Время: " << now->tm_hour << ":" << now->tm_min << ":"<< now->tm_sec << std::endl;
	}
	void showInt(int a)
	{
		std::cout << "Вы ввели число: ";
		std::cout << a << std::endl;
	}

	

	int writeInt()
	{
		std::cout << "Введите число число: ";
		int a;
		std::cin >> a;
		return a;
	}	  
}