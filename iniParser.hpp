/***********************************************************************************************************************************************************
Original Web Link:
http://blog.pic-o.com/2013/05/another-ini-parser/

Personal request:
Drop me an email if you do use this / encounter problems =)

License Type:
BSD 3-Clause (This is both "GPL" and "commercial with acknowledgement" compatible)

Copyright (c) 2013, picoCreator (Eugene Cheah Hui Qin | pico.creator{at}gmail.com)
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

+ Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
+ Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
+ Neither the name of ANY ORGANIZATION(S) AFFLIATED WITH THE CREATOR (picoCreator) nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********************************************************************************************************************************************************/
#pragma once
#ifndef INCLUDED_INI_PARSER
#define INCLUDED_INI_PARSER

#include <stdint.h>
#include <map>
#include <string>
#include <string.h>
#include <stdio.h>
#include "charTools.hpp"

#define iniParser_defaultLineBufferSize 256

class iniParser {
  private:
	
  public:
	long _parseConfigSegment( const char * inputStr, std::map<char*,char*,charTools::cmpo_less> *& curSect );
	
	std::map<char*, std::map<char*,char*, charTools::cmpo_less> *, charTools::cmpo_less > storedConfig; //stores the configuration
	
	//iniParser();
	~iniParser();
	void clear();
	
	long parseIniStr( const char * inputStr, const char * defSection = NULL );
	long parseIniFile( const char * filePath, const char * defSection = NULL, bool fileLocks = true, int lineBufferSize = iniParser_defaultLineBufferSize );
	
	std::map<char*,char*,charTools::cmpo_less> * sectionMap( const char * sectionName, bool generates = false );
	int sectionCount();
	
	char * sectionValueGet( std::map<char*,char*,charTools::cmpo_less> * map, const char * valueName );
	void sectionValueSet( std::map<char*,char*,charTools::cmpo_less> * map, const char * valueName, const char * valueData );
	void _sectionValueSet( std::map<char*,char*,charTools::cmpo_less> * map, char * storeName, char * storeData );
	int sectionValueCount( std::map<char*,char*,charTools::cmpo_less> * map );
	
	char * sectionValueGet( const char * sectionName, const char * valueName );
	bool sectionValueSet( const char * sectionName, const char * valueName, const char * valueData, bool generates = true );
	int sectionValueCount( const char * sectionName );
	
	bool sectionMapSet( const char* sectionName, std::map<char*,char*,charTools::cmpo_less> * sourceMap, bool generates = true );
	bool sectionMapSet( std::map<char*,char*,charTools::cmpo_less> * targetMap, std::map<char*,char*,charTools::cmpo_less> * sourceMap );
	char * outputIniStr();
};

#endif //INCLUDED_INI_PARSER