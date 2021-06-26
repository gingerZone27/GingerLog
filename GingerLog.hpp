/*
	Author : Ginger Young
	Email  : chinayangjj@gmail.com 
*/

#ifndef _GINGER_LOG_HPP_
#define _GINGER_LOG_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <ctime>
#include <chrono>
#include <mutex>
using namespace std;

enum LOG_LEVEL
{
	INFO,
	WARN,
	ERROR,
	DATA,
	DEBUG
};

class GingerLog
{
	public:
		GingerLog();
		GingerLog(bool);
		~GingerLog();

		void run_loop();

		void log_info(LOG_LEVEL in_level, string in_str);

	private:
		string get_time() const;

		bool is_log_enabled;
		string log_name;
		ofstream out_file;

		mutex lock_content;
		bool new_log = false;
		string log_content = "";
};

GingerLog::GingerLog()
{
	is_log_enabled = false;
	log_name = "";
}

GingerLog::GingerLog(bool is_logged)
{
	is_log_enabled = is_logged;

	if(is_log_enabled)
	{
		log_name = get_time() + ".log";
		cout << "|***** log stored in \'" << log_name << "\' *****|"<< endl;

		/* Must detach this thread or the program will terminate immediately with error. */
		thread th_func(&GingerLog::run_loop, this);
		th_func.detach();
	}
	else
	{
		log_name = "";
	}
}

GingerLog::~GingerLog()
{
	/* No resource will be released. */	
}

void GingerLog::log_info(LOG_LEVEL in_level, string in_str)
{
	string out_str = "";
	switch(in_level)
	{
		case INFO:
		{
			/* Color for INFO mode: Green */
			out_str = "[INFO  " + get_time() + "] " + in_str;
			cout << "\033[32m" << out_str << "\033[0m" << endl;
			break;
		}
		case WARN:
		{
			/* Color for WARN mode: Yellow */
			out_str = "[WARN  " + get_time() + "] " + in_str;
			cout << "\033[33m" << out_str << "\033[0m" << endl;
			break;
		}
		case ERROR:
		{
			/* Color for ERROR mode: Red */
			out_str = "[ERROR " + get_time() + "] " + in_str;
			cout << "\033[31m" << out_str << "\033[0m" << endl;
			break;
		}
		case DATA:
		{
			/* Color for DATA mode: White */
			out_str = "[DATA  " + get_time() + "] " + in_str;
			cout << "\033[37m" << out_str << "\033[0m" << endl;
			break;
		}
		case DEBUG:
		{
			/* Color for DEBUG mode: Pink */
			out_str = "[DEBUG " + get_time() + "] " + in_str;
			cout << "\033[35m" << out_str << "\033[0m" << endl;
			break;
		}
		default:
		{
			break;
		}
	}

	if(is_log_enabled)
	{
		const lock_guard<mutex> lock(lock_content);
		GingerLog::new_log = true;
		GingerLog::log_content = out_str;
	}
}

void GingerLog::run_loop()
{
	while(true)
	{
		/* Consider that the sleep duration may have to be adjusted according to the period of application. */
		this_thread::sleep_for(chrono::milliseconds(5));

		/* Sleeping must be done before the mutex is locked. Or the mutex will be locked all the time. */
		const lock_guard<mutex> lock(lock_content);
		if(new_log)
		{
			try
			{
				out_file.open(log_name, ios::out|ios::app);
				out_file << log_content << "\n";
				out_file.close();
			}
			catch(exception e)
			{
				cout << "[ERROR] " << e.what() << endl;
				exit(-1);
			}
			new_log = false;
			log_content = "";
		}
	}
}

string GingerLog::get_time() const
{
	auto this_time = chrono::system_clock::now();
	time_t end_time = chrono::system_clock::to_time_t(this_time);
	string time_str = ctime(&end_time);

	string tmp = "";
	stringstream ss;
	ss << time_str;
	ss >> tmp >> tmp >> tmp >> tmp;

	return tmp;
}



#endif // _GINGER_LOG_HPP_