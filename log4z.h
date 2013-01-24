
/*
 * Log4z License
 * -----------
 * 
 * Log4z is licensed under the terms of the MIT license reproduced below.
 * This means that Log4z is free software and can be used for both academic
 * and commercial purposes at absolutely no cost.
 * 
 * 
 * ===============================================================================
 * 
 * Copyright (C) 2012 YaweiZhang <yawei_zhang@foxmail.com>.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * ===============================================================================
 * 
 * (end of COPYRIGHT)
 * /


/*
 * AUTHORS:  YaweiZhang <yawei_zhang@foxmail.com>
 * VERSION:  1.1.0
 * PURPOSE:  A lightweight library for error reporting and logging to file and screen .
 * CREATION: 2010.10.4
 * LCHANGE:  2013.01.24
 * LICENSE:  Expat/MIT License, See Copyright Notice at the begin of this file.
 */


/*
 *
 * QQ Group: 19811947
 * Web Site: www.zsummer.net
 * mail: yawei_zhang@foxmail.com
 */


/*
 * UPDATES
 *
 * VERSION 0.1.0 <DATE: 2010.10.4>
 * 	create the first project.  
 * 	It support put log to screen and files, 
 * 	support log level, support one day one log file.
 * 	support multiple thread, multiple operating system.
 * 
 * VERSION .... <DATE: ...>
 * 	...
 * 
 * VERSION 0.9.0 <DATE: 2012.12.24>
 * 	support config files.
 * 	support color text in screen.
 * 	support multiple logger.
 * 
 * VERSION 1.0.0 <DATE: 2012.12.29>
 * 	support comments in the config file.
 * 	add a advanced demo in the ./project
 * 	fix some details.
 *
 * VERSION 1.0.1 <DATE: 2013.01.01>
 * 	the source code haven't any change.
 *	fix some Comments in the log4z
 *	add some comments in the test projects.
 *	delete some needless code in the 'fast_test' demo projects, it's so simple.
 *
 * VERSION 1.1.0 <DATE: 2013.01.24>
 * 	the method Start will wait for the logger thread started.
 *	config and add method change. 
 *	namespace change.
 *	append some macro
 *
 */

#pragma once
#ifndef _ZSUMMER_LOG4Z_H_
#define _ZSUMMER_LOG4Z_H_

#include <string>
#include <sstream>


//the max logger count.
const static unsigned int LOGGER_MAX = 20;

//the max log content length.
const static unsigned int LOG_BUF_SIZE = 2048;

typedef int LoggerId;

#define INVALID_LOGGERID -1


enum ENUM_LOG_LEVEL
{
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_ALARM,
	LOG_LEVEL_FATAL,
};


#ifndef _ZSUMMER_BEGIN
#define _ZSUMMER_BEGIN namespace zsummer {
#endif  
#ifndef _ZSUMMER_LOG4Z_BEGIN
#define _ZSUMMER_LOG4Z_BEGIN namespace log4z {
#endif
_ZSUMMER_BEGIN
_ZSUMMER_LOG4Z_BEGIN







//log4z class
class ILog4zManager
{
public:
	ILog4zManager(){};
	virtual ~ILog4zManager(){};
	//log4z Singleton
	static ILog4zManager * GetInstance();

	///Config and Get  logger.
	virtual std::string GetExampleConfig() = 0;
	virtual bool ConfigFromFile(std::string cfg) = 0;
	virtual LoggerId GetLoggerFromName(std::string name) =0;

	//Get and Config the Main Logger. can ignore ConfigMainLogger.
	virtual LoggerId GetMainLogger() = 0;
	virtual bool	ConfigMainLogger(	std::string path="",
						std::string name ="",
						int nLevel = LOG_LEVEL_DEBUG,
						bool display = true) = 0;
	//
	virtual LoggerId DynamicCreateLogger(	std::string path="",
							std::string name ="",
							int nLevel = LOG_LEVEL_DEBUG,
							bool display = true) = 0;

	// any time and any where can change the logger's filter level.
	virtual bool ChangeLoggerLevel(LoggerId nLoggerID, int nLevel) = 0;

	// any time and any where can change.
	// if enable that the logger only log to the file
	virtual bool ChangeLoggerDisplay(LoggerId nLoggerID, bool enable) = 0;

	//start log4z.
	virtual bool Start() = 0;

	//stop log4z, It can  automatically stop at the process dea.
	virtual bool Stop() = 0;

	//push a base log msg to log4z
	virtual bool PushLog(LoggerId id, int level, const char * log) = 0;

};
#ifndef _ZSUMMER_END
#define _ZSUMMER_END }
#endif  
#ifndef _ZSUMMER_LOG4Z_END
#define _ZSUMMER_LOG4Z_END }
#endif

_ZSUMMER_LOG4Z_END
_ZSUMMER_END


//generally log micro. It must specify the logger ID and the log level.
#define LOG_STREAM(id, level, log)\
{\
	std::ostringstream ss;\
	ss << log;\
	ss << "(" << __FILE__ << " )" <<  "(" << __FUNCTION__ << "):" << __LINE__;\
	zsummer::log4z::ILog4zManager::GetInstance()->PushLog(id, level, ss.str().c_str());\
}

#define LOG_DEBUG(id, log) LOG_STREAM(id, LOG_LEVEL_DEBUG, log)
#define LOG_INFO(id, log)  LOG_STREAM(id, LOG_LEVEL_INFO, log)
#define LOG_WARN(id, log)  LOG_STREAM(id, LOG_LEVEL_WARN, log)
#define LOG_ERROR(id, log) LOG_STREAM(id, LOG_LEVEL_ERROR, log)
#define LOG_ALARM(id, log) LOG_STREAM(id, LOG_LEVEL_ALARM, log)
#define LOG_FATAL(id, log) LOG_STREAM(id, LOG_LEVEL_FATAL, log)

///fast log micro. It record at the main logger.
#define LOGD( log ) LOG_DEBUG( zsummer::log4z::ILog4zManager::GetInstance()->GetMainLogger(), log )
#define LOGI( log ) LOG_INFO( zsummer::log4z::ILog4zManager::GetInstance()->GetMainLogger(), log )
#define LOGW( log ) LOG_WARN( zsummer::log4z::ILog4zManager::GetInstance()->GetMainLogger(), log )
#define LOGE( log ) LOG_ERROR( zsummer::log4z::ILog4zManager::GetInstance()->GetMainLogger(), log )
#define LOGA( log ) LOG_ALARM( zsummer::log4z::ILog4zManager::GetInstance()->GetMainLogger(), log )
#define LOGF( log ) LOG_FATAL( zsummer::log4z::ILog4zManager::GetInstance()->GetMainLogger(), log )





#endif





