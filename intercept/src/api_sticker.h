
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

namespace APISticker{

 void TraceEnter(const char* api);

}// namespace APISticker
#define API_STICKER_TRACE_ENTER() APISticker::TraceEnter(__func__)

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

    void TraceNDRangeKernel(cl_kernel kernel_handle, std::vector<Kernel_Param>& obj);
    void TraceCopyBuffer(cl_mem bufferSrc, cl_mem bufferDst);
}

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