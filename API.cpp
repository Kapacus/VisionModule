/*****************************************************************************
*                                                                            *
*  OpenNI 1.x Alpha                                                          *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include "library.h"
#include "Communication.h"
using namespace std;
//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
string API_index;
libr stream; 
//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------

string make_checkSum(string s) {
    char temp[3] = {'0','0','\0'};
    for (size_t i = 0; i < s.size(); ++i)
    {   /*Using XOR*/
        temp[0] ^= s[i];
        temp[1] ^= temp[0];
    }
    string checksum(temp);
    return checksum;
}

bool compare_checkSum(string s1) {
	bool checksum= false;
	string s2 = s1.substr((s1.size()-2),2);
	string s3 = make_checkSum(s1.substr(0,s1.size()-2));

	if(s2 == s3){
		checksum = true;
	}


	return checksum;
}

string convert_float_to_string( float input, int size){
	bool negative = false;
	stringstream command;
	string finish;

	if(input < 0){
		negative = true;
		input = -input;
		command << setw(size-1) << setfill('0') << fixed << setprecision(2) << input;
	}else{
		command << setw(size) << setfill('0') << fixed << setprecision(2) << input;
	}
	finish = command.str();
	if(negative){
		finish = "-" + finish;
	}

	return finish;
}

string decoder(string data)
{
	/*Cut out first 2 chars(API index)*/
	API_index = data.substr(0,2);
	stringstream build_command;
	//Append to command		
	build_command << API_index;
	/*Check API Index*/
	switch(atoi(API_index.c_str()))
	{
	case 0:/*Ping*/
		
		break;
	case 1: /*New Depth stream*/
		stream.initDepth(); //Run init
		break;
	case 2: /*New Skeleton stream*/
		stream.initSkeleton(); //Run init
		break;
	case 3: /*SkeletonHeadposition*/		
		stream.SkeletonHeadposition();
		//Append to command
		int i; 
		for(i=0; i<stream.nUsers; i++)
        	{
            		if(stream.output_int[i]==FALSE){
                	continue;
			}
			build_command << stream.output_int[i];
           		build_command << convert_float_to_string(stream.output_float[i][0],7);
			build_command << convert_float_to_string(stream.output_float[i][1],7);
			build_command << convert_float_to_string(stream.output_float[i][2],7);
		}
		if(i == 0) //No users detected
		{
			build_command << "0";	
		}			
		break;
	case 4: /*Middle*/		
		stream.DepthMiddle();
		//Append to command		
		build_command << setw(4) << setfill('0') << stream.output_int[0]; //To keep value on 4 digits
		break;
	case 5: /*Resolution*/ 
		stream.DepthResolution();
		//Append to command			
		build_command << setw(4) << setfill('0') << stream.output_int[0];
		build_command << setw(4) << setfill('0') << stream.output_int[1];
		break;
	case 6: /*Point : x and y between 0-320 and 0-240*/		
		stream.DepthPoint(atoi((data.substr(2,3)).c_str()) , atoi((data.substr(5,3)).c_str()));
		//Append to command			
		build_command << setw(4) << setfill('0') << stream.output_int[0];
		break;	
	case 7: /*Close Depth stream*/
		stream.DepthClose(); 
		break;
	case 8: /*Close Skeleton stream*/
		stream.SkeletonClose(); 
		break;
	case 99: /*Checksum Error*/
		build_command << "Error in checksum";
		break;	
	default: /*Error*/
		
		break;
	}
	string command = build_command.str();
	return command;	
}

int main()
{	
	string data_in;
	string data_out;
	Communication USB;
	/*Init USB stream*/
	USB.openport();	//Opening port (Closing port:"USB.closeport();")	
	while(1<2){
		/*Wait for data and decode*/
		data_in = USB.readUSB();
		data_out = decoder(data_in);	 
		/*Write to */	
		USB.writeUSB(data_out);		
	}
	return 0;
}
