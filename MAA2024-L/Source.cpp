#include <ctime>
#include <iostream>

extern "C" {
	void showtime()
	{
		std::cout << clock();
	}

	void ruslang()
	{
		setlocale(LC_ALL, "rus");
	}

	void today()
	{
		const time_t tm = time(nullptr);
		struct tm con;
		std::cout << localtime_s(&con, &tm);
	}
	void showInt(int a)
	{
		std::cout << a;
	}

	void showString(std::string a)
	{
		std::cout << a;
	}

	int writeInt()
	{
		int a;
		std::cin >> a;
		return a;
	}	  
}