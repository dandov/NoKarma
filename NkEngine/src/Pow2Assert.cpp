/* 
Copyright 2015 Alvaro Herrasti and Daniel Dovali

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Pow2Assert.h"

#include <cstdio>
#include <cstdarg>

namespace pow2 { namespace Assert
{

struct AssertHandler
{
	AssertHandler()
		: handler(&AssertHandler::DefaultHandler)
	{
	}

	static bool DefaultHandler(const char* condition, const char* msg, const char* file, const int line)
	{
		std::printf("%s(%d): Assert Failure: ", file, line);
		
		if (condition != NULL)
			std::printf("'%s' ", condition);

		if (msg != NULL)
			std::printf("%s", msg);

		std::printf("\n");

		return true;
	}

	Handler handler;
};

AssertHandler& GetAssertHandlerInstance()
{
	static AssertHandler s_handler;
	return s_handler;
}


Handler GetHandler()
{
	return GetAssertHandlerInstance().handler;
}

void SetHandler(Handler newHandler)
{
	GetAssertHandlerInstance().handler = newHandler;
}

bool Report(const char* condition, const char* msg, const char* file, const int line)
{
	return GetAssertHandlerInstance().handler(condition, msg, file, line);
}

bool ReportFormatted(const char* condition, const char* file, const int line, const char* msg, ...)
{
	if (msg != NULL)
	{
		char messageBuffer[1024];
		{
			va_list args;
			va_start(args, msg);
			//vsnprintf(messageBuffer, 1024, msg, args);
			vsnprintf_s(messageBuffer, 1024, msg, args);
			va_end(args);
		}

		return Report(condition, messageBuffer, file, line);
	}

	return Report(condition, NULL, file, line);
}

}}
