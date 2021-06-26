#include "GingerLog.hpp"

#include <iostream>
#include <csignal>
using namespace std;

int run_flag = 1;
void func_sig(int sig)
{
	run_flag = 0;
}

int main()
{
	bool will_log = true;
	GingerLog gloger(will_log);

	while(run_flag == 1)
	{
		string str;
		cout << "Say something here: ";
		cin >> str;
		gloger.log_info( INFO, str);
		this_thread::sleep_for(chrono::milliseconds(10));
		gloger.log_info( WARN, str);
		this_thread::sleep_for(chrono::milliseconds(10));
		gloger.log_info(ERROR, str);
		this_thread::sleep_for(chrono::milliseconds(10));
		gloger.log_info( DATA, str);
		this_thread::sleep_for(chrono::milliseconds(10));
		gloger.log_info(DEBUG, str);
		this_thread::sleep_for(chrono::milliseconds(50));
		cout << "----------------------------------" << endl;
	}
}