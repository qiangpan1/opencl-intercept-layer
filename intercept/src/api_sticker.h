
//api_stiker.h
#pragma once
#ifndef _ETW_TRACE_H
#define _ETW_TRACE_H
// ETW dependecies: ================================================================================================================
#include "windows.h"
#include "stdio.h"
#include "stdint.h"
#include <map>
#include <vector>
#include <evntrace.h>
#include <evntprov.h>
#include "CL/cl.h"
#include <string>
#include <set>
#include <sstream>
#include <cstdlib> 

typedef struct _ETW_TRACE_CONTEXT {
    REGHANDLE Handle;
    uint64_t Keyword;
    uint8_t Level;
    uint32_t IsEnabled;
    uint32_t IsPlatSent;
    struct {
        uint32_t DriverVersion;
        uint32_t ProductFamily;
        uint32_t RenderFamily;
        uint32_t DeviceID;
    } PlatInfo;
} ETW_TRACE_CONTEXT, * PETW_TRACE_CONTEXT;

typedef enum _OCL_TASK_ID {
    API_STICKER_OCL = 130,
    API_OCL_EnqueueNDRangeKernel,
    API_OCL_EnqueueCopyBuffer,
    API_OCL_EnqueueMapBuffer,
    API_OCL_EnqueueUnmapMemObject,
    API_OCL_CreateBuffer,
    API_OCL_EnqueueReadBuffer,
    API_OCL_EnqueueWriteBuffer,
    API_OCL_CreateImage,
    API_OCL_EnqueueWriteImage,
    API_OCL_EnqueueReadImage,
    API_OCL_ReleaseMemObject,
} OCL_TASK_ID;

typedef enum _OCL_EVENT_TYPE{
    EVENT_TYPE_INFO = 0,           //! function information event
    EVENT_TYPE_START = 1,           //! function entry event
    EVENT_TYPE_END = 2,           //! function exit event
    EVENT_TYPE_INFO2 = 3,           //! function extra information event
} OCL_EVENT_TYPE;

void NTAPI EtwControlCallback(LPCGUID SourceId,
    ULONG ControlCode,
    UCHAR Level,
    ULONGLONG MatchAnyKeyword,
    ULONGLONG MatchAllKeyword,
    PEVENT_FILTER_DESCRIPTOR FilterData,
    PVOID CallbackContext);

void MosTraceEventInit();

void MosTraceEventClose();

void MosTraceEvent(uint16_t usId,
    uint8_t ucType,
    const void* pArg1,
    uint32_t dwSize1,
    const void* pArg2,
    uint32_t dwSize2);

typedef struct _MT_PARAM {
    int32_t id;
    int64_t value;
} MT_PARAM;

#define MT_LOG1(id, lvl, p1, v1)                                                                       \
    {                                                                                                  \
        int32_t _head[] = {id, lvl};                                                                   \
        MT_PARAM _param[] = {p1, v1};                                                                  \
        MosTraceEvent(119, 1, _head, sizeof(_head), _param, sizeof(_param)); \
    }
// ============================================================================================================================


void trim(std::string& s);
std::set<std::string> split(const std::string& s, char delimiter);


namespace APISticker{
    extern  const char* OCL_API_STICKER_FILTER;
    extern  const char* OCL_API_STICKER_FILTER_NEGATIVE;
    extern std::set<std::string> p_filter;
    extern std::set<std::string> n_filter;
    void TraceEnter(const char* api, std::set<std::string>& p_filter, std::set<std::string>& n_filter);
}// namespace APISticker
#define API_STICKER_TRACE_ENTER() \
    do{\
        APISticker::TraceEnter(__func__,APISticker::p_filter,APISticker::n_filter);\
    }while(0)

namespace TraceKernel {
    struct Kernel_Param {
        uint32_t index;
        uint32_t args_size;
        uint64_t pointer;

        Kernel_Param() : index(-1), args_size(-1), pointer(static_cast<uint64_t>(-1)) {}
        Kernel_Param(uint32_t arg_index, uint32_t arg_size, uint64_t arg_pointer)
            : index(arg_index), args_size(arg_size), pointer(arg_pointer) {
        }
    };
    extern const char* OCL_TRACE_FILTER;
    extern std::set<std::string> trace_filter;
    void TraceNDRangeKernel(cl_kernel kernel_handle, std::string kernel_name, std::vector<TraceKernel::Kernel_Param>& obj);
    void TraceCopyBuffer(cl_mem bufferSrc, cl_mem bufferDst);
}
#define TRACE_KERNEL(kernel) \
    do{\
    const std::string kernel_name = pIntercept->getShortKernelName(kernel);\
    if(TraceKernel::trace_filter.empty() ) { \
        TraceKernel::TraceNDRangeKernel(kernel, kernel_name, pIntercept->m_ArgsKernelInfoMap.at(kernel));\
    }else{\
        if(TraceKernel::trace_filter.find("clEnqueueNDRangeKernel") != TraceKernel::trace_filter.end()){\
            TraceKernel::TraceNDRangeKernel(kernel, kernel_name, pIntercept->m_ArgsKernelInfoMap.at(kernel));\
        }\
     }\
}while(0)

//id could be api name 
//lvl 

// Usage example:
// Note: the arg value in MT_LOG1 can reuse media event trace enum numbers.

#endif

// void main(int)
// {
//     MosTraceEventInit();
//     // ivoke 1000 events
//     for (auto i = 0; i < 1000; i++)
//     {
//         MT_LOG1(33555459, 1, 4097, 1000);
//     }
//     MosTraceEventClose();
// }