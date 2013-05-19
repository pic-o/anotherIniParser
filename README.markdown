anotherIniParser
=======
The label says it all, "another" (there are many many out there) simple ini parser for config files. This was made to be cross platform / license compatible

Usage
-----
Ensure the compiller source libary is setup to include the source codes. And....

	#include "iniParser.hpp"
	
And that you compile, and include the needed object files at the same time (iniParser, charTools)
(note that BSD 3-Clause NOT a viral GPL/LGPL, it mainly require acknowledgment. So you can include this in your work directly as source code)

Example?
--------
	//The following is a quick rough guide on how to start things working. For more details see the various individual function notes
	
	iniParser configParser;
	configParser.parseIniFile( "./configFile.ini", "default" ); //fileLocks = true, lineBuffer = 256
	...
	//Getting a value in section "default" (if no section stated in config file, this is assumed, as indicated above)
	std::cout << configParser.sectionValueGet( "default", "optionA" );
	...
	//Setting a value in section "default"
	std::string setValue;
	std::cin >> setValue;
	configParser.sectionValueSet( "default", "optionB", setValue.c_str() ); //generates = true
	...
	//And when things go wrong : a "useful debug"
	std::cout << configParser.outputIniStr();

Short-Story
-----------
Creating your "own ini parser" is seriously a good "exercise" for ANY programmer. And would also give subtle exposure to the programmer "mindset" on peer review.

For example: This uses c-style const char* strings. Instead of C++ strings, due to my belief (debatable) on the usesage of char*, and the self-decipline involved in its memory managment.
While taking into account possible memory leaks (hence rquiring const char* inputs). In-event that performance (seriously?) is a consideration, there is always a leaky _sectionN() varient.
But seriously, use that at your own risk. Etc, Etc.

Also generally speaking, after several years (coming back and going) of refining this iniParser. I found it relatively "good" to use in many of the projects. Especially while avoiding the huge 
overhead libraries (its not performance, its the ability to easily setup and compile anywhere : ie client machines). And henced served me well.

Note: This is the most recent cleanup version. And has yet to seen full wild deployment. But so far it passed most of my test. I will remove this line in due time, when i have enough "code reviews"

Documentation Notes
-------------------
The source code commenting was based on NaturalDocs (http://www.naturaldocs.org/). And hence requires that seperately (has its AGPL license) to output the documents.

Cross Platform Notes
--------------------
File locking is based on linux style file lock. For windows which features a file lock on a "byte level", the first byte of the file is defined as the "byte range" to lock/unlock. See the source code, for the specific platform defines.

License Notes?
--------
### author: 
picoCreator AKA Eugene Cheah
### email:
pico.creator{at}gmail.com
### website:		
www.pic-o.com/blog
### license-short:	
BSD 3-Clause License
[http://opensource.org/licenses/BSD-3-Clause]

### license-full:
Copyright (c) 2013, picoCreator (Eugene Cheah Hui Qin | pico.creator{at}gmail.com)
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

+ Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
+ Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
+ Neither the name of ANY ORGANIZATION(S) AFFLIATED WITH THE CREATOR nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.