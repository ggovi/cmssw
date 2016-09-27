#include "CondCore/CondDB/interface/Exception.h"
#include <openssl/sha.h>

namespace cond {

  namespace {

    inline Hash makeHash( const std::string& objectType, const cond::Binary& data ){
      SHA_CTX ctx;
      if( !SHA1_Init( &ctx ) ){
	throwException( "SHA1 initialization error.","IOVSchema::makeHash");
      }
      if( !SHA1_Update( &ctx, objectType.c_str(), objectType.size() ) ){
	throwException( "SHA1 processing error (1).","IOVSchema::makeHash");
      }
      if( !SHA1_Update( &ctx, data.data(), data.size() ) ){
	throwException( "SHA1 processing error (2).","IOVSchema::makeHash");
      }
      unsigned char hash[SHA_DIGEST_LENGTH];
      if( !SHA1_Final(hash, &ctx) ){
	throwException( "SHA1 finalization error.","IOVSchema::makeHash");
      }
  
      char tmp[SHA_DIGEST_LENGTH*2+1];
      // re-write bytes in hex
      for (unsigned int i = 0; i < 20; i++) {
	::sprintf(&tmp[i * 2], "%02x", hash[i]);
      }
      tmp[20*2] = 0;
      return tmp;
    }

  }

}
 
