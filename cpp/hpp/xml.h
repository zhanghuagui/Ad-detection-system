#ifndef _XML_H
#define _XML_H
#include <iostream>
#include <string>
#include "tinystr.h"
#include "tinyxml.h"
#include <vector>
typedef signed long long int64_t;

using namespace std;

struct TSInformation
{
	string ip_address;

	string port;

	char* channel_name;

	char* date;

};

struct DetectionResult
{
	int commercial_ID;

	int64_t start_dts;

	int64_t end_dts;

	char* start_time;

	char* end_time;
};


void Write2XML(TSInformation ts_information,vector<DetectionResult> detection_result);



#endif