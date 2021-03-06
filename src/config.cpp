/*****************************************************************************
* Copyright (c) 2016 Jiocloud.com, Inc. or its affiliates.  All Rights Reserved
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish, dis-
* tribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the fol-
* lowing conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABIL-
* ITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
* SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
******************************************************************************/
#include "src/config.hpp"
#include <iostream>
#include <cstdlib>
#include <map>
#include <fstream>
using namespace std;
namespace config
{
	// Setting up the various endpoints of the API.
	std::map<std::string,std::string> endpoints;
		
	std::string get_service_url(std::string service_name)
	{	
		endpoints["vpc"]="https://vpc.ind-west-1.jiocloudservices.com"; // Virtual Private Cloud Endpoint
		endpoints["iam"] = "https://iam.ind-west-1.jiocloudservices.com"; // Authentication Endpoint
		endpoints["rds"] = "https://rds.ind-west-1.jiocloudservices.com"; // Data Storage Enpoint
		endpoints["dss"]= "https://dss.ind-west-1.jiocloudservices.com";// Durable Data Storage Endpoint
		endpoints["compute"] = "https://compute.ind-west-1.staging.jiocloudservices.com"; // Compute Endpoint
		return endpoints[service_name]; // return the respective service's endpoint url 
	}
	ConfigHandler::ConfigHandler()
	{	
		//read this from configuration file
		secure = false;
		debug = false;
		int pos1,pos2;

		// reading keys from file
		// TODO : for windows "/" shuld be substituted with "\" 
		string config_path(getenv("CPLUS_INCLUDE_PATH"));

		if(config_path[config_path.length()-1] == '/')
		{
			config_path = config_path.substr(0,config_path.length()-1);
		}

		config_path = config_path + "/config.txt";  
		std::ifstream infile(config_path.c_str());
		std::string line;

		getline(infile,line); // First Line is the access Key
		pos1 = line.find("\"");
		pos2 = line.find("\"",pos1+1);
		line = line.substr(pos1+1,pos2-pos1-1);
		access_key = line;
		getline(infile,line); // Second Line is the secret Key
		pos1 = line.find("\"");
		pos2 = line.find("\"",pos1+1);
		line = line.substr(pos1+1,pos2-pos1-1);
		secret_key = line;

		getline(infile,line); // Third Line is the secure
		pos1 = line.find("\"");
		pos2 = line.find("\"",pos1+1);
		line = line.substr(pos1+1,pos2-pos1-1);
		if(line == "1")
		{
			secure = true;
		}
		
		getline(infile,line); // Forth Line is the debug
		pos1 = line.find("\"");
		pos2 = line.find("\"",pos1+1);
		line = line.substr(pos1+1,pos2-pos1-1);
		if(line == "1")
		{
			debug = true;
		}

		infile.close();

		//set up endpoints

		if (access_key.empty() || secret_key.empty())
		{
		std::cout<<"Unknown Credentials:Access key or secret key not set";
		}
	}
	// Getter for access Key
	std::string ConfigHandler::get_access_key() const
	{
		return access_key;
	}
	//Getter for secret Key
	std::string ConfigHandler::get_secret_key() const
	{
		return secret_key;
	}
	bool ConfigHandler::check_secure() const
	{
		return secure;
	}
	bool ConfigHandler::check_debug() const
	{
		return debug;
	}
}