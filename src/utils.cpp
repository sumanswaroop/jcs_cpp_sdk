#ifndef UTILS
#define UTILS

#include <string>
#include <map>
#include <string.h>
#include <iostream>
#include <openssl/hmac.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

namespace utils{
	
	struct auth_var{
		char url[512];
		char verb[8];
		char access_key[128];
		char secret_key[128];
		char headers[128];
		char path[128];
		char protocol[8];
		char host[128];
		char port[8];
		//copy constructor


	};

	struct http_var{
		char url[512];
		char verb[8];
		char version[16];
		char headers[128];
		
	};

	struct block_device_mapping{
		std::string device_name;
		bool delete_on_termination;
		int volume_size;
	};

	std::string get_protocol(char url[512])
	{	
		//EXTRA CHECK REQUIRED Regexp
		std::string url_ = url;
		return url_.substr(0,5);   //https
	}

	std::string get_host(char url[128])
	{
		std::string host_ = url;
		return host_.substr(8);
	}
	std::string hmac_sha256(std::string canonical_string ,const char *secret_key)
	{
		unsigned len = 32;
		unsigned char hmac_256[len+1];
	    
	    HMAC_CTX ctx;
	    HMAC_CTX_init(&ctx);
		HMAC_Init_ex(&ctx, secret_key, strlen(secret_key), EVP_sha256(), NULL);
		HMAC_Update(&ctx,(unsigned char*)(canonical_string.c_str()), strlen(canonical_string.c_str()));
		HMAC_Final(&ctx, hmac_256, &len);
	   	HMAC_CTX_cleanup(&ctx);
	   	//hmac_256[len]='\0';
	   	return std::string((const char*)hmac_256, len);
	}

	std::string base64encode(const char * instring, size_t len)
	{	
		//Length of hmac signature 256 bits(32bytes) : 64 base encoding length 4*32/3 Therfore introduced \0 at 44		
		BIO *bio, *b64;
		
		const size_t mlen = len*8/6 + len%6;	
		char b64message[mlen+1];
		b64 = BIO_new(BIO_f_base64());
		bio = BIO_new(BIO_s_mem());
		BIO_push(b64, bio);
		BIO_set_flags(b64,BIO_FLAGS_BASE64_NO_NL);
		BIO_write(b64, instring, len);
		BIO_flush(b64);
		int length = BIO_read(bio, b64message, mlen);
		BIO_free_all(b64);
		//use len to remove \n
		b64message[length]='\0';
		return b64message;

	}
}

#endif


