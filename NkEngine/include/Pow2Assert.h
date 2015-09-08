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


#pragma once

#ifndef pow2_core_Assert_h
#define pow2_core_Assert_h

#define POW2_ASSERTS_ENABLED

namespace pow2 { namespace Assert
{
	typedef bool (*Handler)(const char* condition, const char* msg, const char* file, int line);

	Handler GetHandler();
	void SetHandler(Handler newHandler);
	
	bool Report(const char* condition, const char* msg, const char* file, int line);
	bool ReportFormatted(const char* condition, const char* file, int line, const char* msg, ...);
}}

#define POW2_HALT() do { __debugbreak(); } while(0)
#define POW2_UNUSED(x) do { (void)sizeof(x); } while(0)

#ifdef POW2_ASSERTS_ENABLED
	#define POW2_ASSERT(cond) \
		do { if ((!(cond)) && pow2::Assert::Report(#cond, 0, __FILE__, __LINE__)) \
			POW2_HALT(); } while(0)

	#define POW2_ASSERT_MSG(cond, msg, ...) \
		do { if ((!(cond)) && pow2::Assert::ReportFormatted(#cond, __FILE__, __LINE__, (msg), __VA_ARGS__)) \
			POW2_HALT(); } while(0)

	#define POW2_ASSERT_FAIL(msg, ...) \
		do { if (pow2::Assert::ReportFormatted(NULL, __FILE__, __LINE__, (msg), __VA_ARGS__)) \
			POW2_HALT(); } while(0)
#else
	#define POW2_ASSERT(condition) \
		do { POW2_UNUSED(condition); } while(0)
	#define POW2_ASSERT_MSG(condition, msg, ...) \
		do { POW2_UNUSED(condition); POW2_UNUSED(msg); } while(0)
	#define POW2_ASSERT_FAIL(msg, ...) \
		do { POW2_UNUSED(msg); } while(0)
#endif

#define POW2_STATIC_ASSERT(x) \
	typedef char pow2StaticAssert[(x) ? 1 : -1];

//lint -esym(751,pow2StaticAssert)
//lint -esym(751,pow2::pow2StaticAssert)

#endif
