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
#include "src/auth_handler.hpp"
#include <iostream>
#include <time.h>
#include "src/utils.hpp"
#include <curl/curl.h>
#include <sstream>

using namespace std;
namespace auth{
	void Authorization::auth_modify(utils::auth_var &data)
	{	

		// test for this part:TODO
		if (!strcmp(data.path,""))
			strcpy(data.path,"/");

		std::string protocol = utils::get_protocol(data.url);	//utils
		std::string host = utils::get_host(data.url);			//utils
		//todo: see how to handle
		if( protocol.compare("https") && protocol.compare("http"))
		{
			std::cout<<"Unsupported protocol present in given url";	
			//exit here
		}

		strcpy(data.protocol,protocol.c_str());
		strcpy(data.host, host.c_str());
		strcpy(data.port,"None"); // default to http port
		
		size_t pos = host.find(":"); // if port specified in url

		if(pos != std::string::npos)
		{
			strcpy(data.port,host.substr(pos+1).c_str());
			strcpy(data.host, host.substr(0,pos).c_str());	
		}

	}

	void Authorization::add_params(std::map <std::string,std::string > &params, utils::auth_var &data)
	{	
		//Add generic key-value pairs in the param map
		params[JCS_ACCESS_KEY_ID] = data.access_key;
		params[SIGNATURE_VERSION] = "2";
		params[SIGNATURE_METHOD] = HMAC_SHA256_ALGORITHM;
		//Time Stamp
		time_t now = time(NULL);
		tm *gmtm = gmtime(&now);
		char stamp[64];
		//utf-8 encoding
		strftime(stamp,64,"%Y-%m-%dT%H:%M:%SZ",gmtm);
		params[TIMESTAMP] = stamp;
	}

	std::string Authorization::_get_utf8_value(std::string value)
	{
		//todo(devender): To look at string to utf-8.
		return value;
	}

	std::string Authorization::sort_params(std::map<std::string ,std::string> &params)
	{
		
		stringstream qs;
		CURL *curl = curl_easy_init();
		char *value; 
		for (std::map<std::string,std::string>::iterator it=params.begin(); it!=params.end(); ++it)
		{
			//Some parse and safety check left  refer auth_handler.py
			// utf encoding on values left
			//url encoding
			value = curl_easy_escape(curl,it->second.c_str(),0);
			qs<<it->first<<"="<<value<<"&";
		}
		string qs_ = qs.str();
		qs_[qs_.length()-1]='\0'; //removing last &
		curl_free(value);
		return qs_;


	}
	std::string Authorization::string_to_sign(std::map <std::string , std::string> &params, utils::auth_var &data)
	{
		//Calculate the canonical string for the request
		std::string verb_ = data.verb;
		stringstream ss;
		ss<<verb_<<"\n"<<data.host;
		
		if(strcmp(data.port,"None"))
		{
			ss<<":";
			ss<<data.port;
		}

		ss<<"\n"<<data.path<<"\n";
		add_params(params, data);
		ss<<sort_params(params);
		return ss.str();

	}

	void Authorization::add_authorization(std::map<std::string, std::string> &params, utils::auth_var &data)
	{

		//data
		auth_modify(data);
		std::string canonical_string = string_to_sign(params, data);
		//HMAC SHA 256
		std::string hmac_256 = utils::hmac_sha256(canonical_string,data.secret_key);
		//base64 and urlencode
		CURL *curl = curl_easy_init();
		char *hmac_Signature = curl_easy_escape(curl,utils::base64encode(&hmac_256[0], hmac_256.length()).c_str(),0);
		params[SIGNATURE]=hmac_Signature;
	#ifdef CLI_DEBUG
		std::cout<<"HMAC SIGNATURE 0: " << utils::base64encode(&hmac_256[0], hmac_256.length())<<"\n";
		std::cout<<"HMAC SIGNATURE:  " << hmac_Signature<<"\n";
	#endif
		free(hmac_Signature);

	}
}