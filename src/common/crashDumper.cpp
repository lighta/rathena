#include "crashDumper.hpp"

#include <execinfo.h> // for backtrace
#include <dlfcn.h>    // for dladdr
#include <cxxabi.h>   // for __cxa_demangle

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

std::string mGetCurDateForFile() {
	// current date/time based on current system (nb tbd see if not available in time.hpp)
    time_t rawtime;
    struct tm * timeinfo;
    char datestr [10+1+9];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (datestr,10+1+9,"%F_%T",timeinfo);
    return std::string(datestr);
}

#ifdef __linux__
void CrashDumper::Backtrace(int skip) {
    void *callstack[128];
    const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
    char buf[1024];
    int nFrames = backtrace(callstack, nMaxFrames);
    char **symbols = backtrace_symbols(callstack, nFrames);

    std::ofstream trace_buf;
    trace_buf.open("dumps/backtrace_"+mGetCurDateForFile()+".txt");
    for (int i = skip; i < nFrames; i++) {
        printf("%s\n", symbols[i]);

        Dl_info info;
        if (dladdr(callstack[i], &info) && info.dli_sname) {
            char *demangled = NULL;
            int status = -1;
            if (info.dli_sname[0] == '_')
                demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
            snprintf(buf, sizeof(buf), "%-3d %*p %s + %zd\n",
                     i, int(2 + sizeof(void*) * 2), callstack[i],
                     status == 0 ? demangled :
                     info.dli_sname == 0 ? symbols[i] : info.dli_sname,
                     (char *)callstack[i] - (char *)info.dli_saddr);
            free(demangled);
        } else {
            snprintf(buf, sizeof(buf), "%-3d %*p %s\n",
                     i, int(2 + sizeof(void*) * 2), callstack[i], symbols[i]);
        }
        trace_buf << buf;
    }
    free(symbols);
    if (nFrames == nMaxFrames)
        trace_buf << "[truncated]\n";
    //return trace_buf.str();
}
#elif _WIN32
void CrashDumper::Backtrace(int skip)
{   
	HMODULE mhLib = ::LoadLibrary(_T("dbghelp.dll"));
	MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress(mhLib, "MiniDumpWriteDump");

	HANDLE  hFile = ::CreateFile(_T("dumps/backtrace_"+mGetCurDateForFile()), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL, NULL);


	_MINIDUMP_EXCEPTION_INFORMATION ExInfo;
	ExInfo.ThreadId = ::GetCurrentThreadId();
	ExInfo.ExceptionPointers = apExceptionInfo;
	ExInfo.ClientPointers = FALSE;

	pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
	::CloseHandle(hFile);
}

#else
void CrashDumper::Backtrace(int skip){}
	//no implementation yet
#endif
