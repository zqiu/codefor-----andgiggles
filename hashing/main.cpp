#include ".\cryptopp562\rsa.h"
#include ".\cryptopp562\cryptlib.h"
#include <iostream>
#include <cstring>

int main(int argc, char ** argv){
	std::string map = "wW5%BbNnUu)0Pp/?";

}

std::string SHA256(const std::string data){
	byte const* pbData = (byte*) data.data();
    unsigned int nDataLen = data.size();
    byte abDigest[CryptoPP::SHA256::DIGESTSIZE];
    CryptoPP::SHA256().CalculateDigest(abDigest, pbData, nDataLen);
    return std::string((char*)abDigest);
}