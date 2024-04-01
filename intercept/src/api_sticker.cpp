#include "api_sticker.h"
#include <string>
#include <cinttypes>
#include <stdexcept>


//const GUID g_intelMedia = { 0x4e1c52c9, 0x1d1e, 0x4470, {0xa1, 0x10, 0x25, 0xa9, 0xf3, 0xeb, 0xe1, 0xa5} };
const GUID g_intelMedia = { 0xd5a552ac, 0xb8d, 0x4f69, 0xb2, 0x6c, 0xa7, 0xb6, 0xc1, 0x6f, 0x6b, 0x51 };

// {D5A552AC-0B8D-4F69-B26C-A7B6C16F6B51}
//DEFINE_GUID(g_intelMedia,
//    0xd5a552ac, 0xb8d, 0x4f69, 0xb2, 0x6c, 0xa7, 0xb6, 0xc1, 0x6f, 0x6b, 0x51);

ETW_TRACE_CONTEXT mediaETWContext = {};
ETW_TRACE_CONTEXT* g_pMediaETWContext = &mediaETWContext;


void NTAPI EtwControlCallback(LPCGUID SourceId,
    ULONG ControlCode,
    UCHAR Level,
    ULONGLONG MatchAnyKeyword,
    ULONGLONG MatchAllKeyword,
    PEVENT_FILTER_DESCRIPTOR FilterData,
    PVOID CallbackContext) {
    PETW_TRACE_CONTEXT ctx = (PETW_TRACE_CONTEXT)CallbackContext;
    uint64_t keyword = MatchAnyKeyword;

    if (ctx == nullptr) {
        return;
    }
    // disable all if no keyword specified by tool
    if (MatchAnyKeyword == (ULONGLONG)-1) {
        keyword = 0;
    }

    switch (ControlCode) {
    case EVENT_CONTROL_CODE_ENABLE_PROVIDER:
        ctx->Keyword = keyword;
        ctx->Level = Level;
        ctx->IsEnabled = 1;
        break;

    case EVENT_CONTROL_CODE_DISABLE_PROVIDER:
        ctx->Keyword = 0;
        ctx->Level = 0;
        ctx->IsEnabled = 0;
        break;

    default:
        break;
    }

    return;
}
void MosTraceEventInit()
{
    auto hr = EventRegister(&g_intelMedia,
        EtwControlCallback,
        g_pMediaETWContext,
        &g_pMediaETWContext->Handle);
}
void MosTraceEventClose() {
    if (g_pMediaETWContext && g_pMediaETWContext->Handle) {
        EventUnregister(g_pMediaETWContext->Handle);
        g_pMediaETWContext->Handle = (REGHANDLE)0;
    }
}
void MosTraceEvent(
    uint16_t usId,
    uint8_t ucType,
    const void* pArg1,
    uint32_t dwSize1,
    const void* pArg2,
    uint32_t dwSize2) {
    if (g_pMediaETWContext && g_pMediaETWContext->Handle) {
        EVENT_DATA_DESCRIPTOR EventData[2];
        EVENT_DESCRIPTOR EventDesc = { 0 };
        EVENT_DATA_DESCRIPTOR* pEventData = NULL;
        DWORD dwNum = 0;

        // setup event descriptor & event data
        EventDesc.Id = (usId << 2) + ucType;
        EventDesc.Opcode = ucType;
        EventDesc.Task = usId;

        if (pArg1) {
            EventDataDescCreate(&EventData[0], pArg1, dwSize1);

            pEventData = EventData;
            dwNum = 1;
            if (pArg2) {
                EventDataDescCreate(&EventData[1], pArg2, dwSize2);

                dwNum++;
            }
        }

        auto hr = EventWrite(g_pMediaETWContext->Handle, &EventDesc, dwNum, pEventData);
    }

    return;
}
void MosTraceEvent4(
    uint16_t usId,
    uint8_t ucType,
    const void* pArg1,
    uint32_t dwSize1,
    const void* pArg2,
    uint32_t dwSize2,
    const void* pArg3, 
    uint32_t dwSize3,
    const void* pArg4,
    uint32_t dwSize4
    ) {
    if (g_pMediaETWContext && g_pMediaETWContext->Handle) {
        EVENT_DATA_DESCRIPTOR EventData[4];
        EVENT_DESCRIPTOR EventDesc = { 0 };
        EVENT_DATA_DESCRIPTOR* pEventData = NULL;
        DWORD dwNum = 0;

        // setup event descriptor & event data
        EventDesc.Id = (usId << 2) + ucType;
        EventDesc.Opcode = ucType;
        EventDesc.Task = usId;

        if (pArg1) {
            EventDataDescCreate(&EventData[0], pArg1, dwSize1);

            pEventData = EventData;
            dwNum = 1;
            if (pArg2) {
                EventDataDescCreate(&EventData[1], pArg2, dwSize2);

                dwNum++;
                if (pArg3) {
                    EventDataDescCreate(&EventData[2], pArg3, dwSize3);
                    dwNum++;
                    if (pArg4) {
                        EventDataDescCreate(&EventData[3], pArg4, dwSize4);
                        dwNum++;
                    }
                }
            }
          
        }

        auto hr = EventWrite(g_pMediaETWContext->Handle, &EventDesc, dwNum, pEventData);
    }

    return;
}

void trim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}
std::set<std::string> split(const std::string& s, char delimiter) {
    std::set<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        trim(token);
        tokens.insert(token);
    }
    return tokens;
}
namespace TraceKernel {
    const char* OCL_TRACE_FILTER;
    std::set<std::string>  trace_filter;
    void TraceNDRangeKernel(cl_kernel kernel_handle, std::string kernel_name, std::vector<TraceKernel::Kernel_Param> &obj ) {

        uint64_t t_kernel_handle = reinterpret_cast<uint64_t>(kernel_handle);
                 
        uint32_t params_count = obj.size();
        char debugstr[100];
        sprintf(debugstr, "This Kernel name is: %s Params Count: %d Kernel Handle: %" PRIx64, kernel_name.c_str(), params_count, t_kernel_handle);
        OutputDebugString(debugstr);

        // UTF-8 to Wide
        auto utf8ToWide = [](const std::string& str) -> std::wstring {
            int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
            if (count == 0) {
                throw std::runtime_error("Conversion to wide string failed.");
            }
            std::vector<wchar_t> buffer(count);
            MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer.data(), count);
            return std::wstring(buffer.begin(), buffer.end() - 1); // Remove null terminator
            };
       
        std::wstring wideStr =utf8ToWide(kernel_name);
        //const char* output_str = ansiStr.c_str();

        MosTraceEvent(
            API_OCL_EnqueueNDRangeKernel,
            EVENT_TYPE_START,
            wideStr.c_str(),
            (wideStr.length() + 1) * sizeof(wchar_t),
            nullptr,
            0);
        MosTraceEvent4(
            API_OCL_EnqueueNDRangeKernel,
            EVENT_TYPE_INFO,
            &t_kernel_handle,
            sizeof(uint64_t),
            &params_count,
            sizeof(uint32_t),
            obj.data(),
            obj.size() * sizeof(TraceKernel::Kernel_Param),
            nullptr,
            0);
        obj.clear();
    }

    void TraceCopyBuffer(cl_mem bufferSrc, cl_mem bufferDst) {

        struct CopyBufferInfo {
            uint64_t bufferInput = static_cast<uint64_t>(-1);
            uint64_t bufferOutput = static_cast<uint64_t>(-1);
        }t_info{ (uint64_t)bufferSrc ,(uint64_t)bufferDst };

        MosTraceEvent(
            API_OCL_EnqueueCopyBuffer,
            EVENT_TYPE_START,
            &t_info,
            sizeof(t_info),
            nullptr,
            0);
    }

    void TraceCreateBuffer(
        cl_context context,
        cl_mem_flags flags,
        size_t size,
        void* host_ptr,
        cl_mem retVal,
        std::chrono::microseconds &duration) {

        struct CreateBuffer {
            uint64_t context;
            uint32_t flags;
            uint32_t size;
            uint64_t host_ptr;
            uint64_t retVal;
            int64_t duration_time;
        } t_end{(uint64_t)context,(uint32_t)flags,(uint32_t)size,(uint64_t)host_ptr,(uint64_t)retVal, (int64_t)duration.count() };

        MosTraceEvent(
            API_OCL_CreateBuffer,
            EVENT_TYPE_END,
            &t_end,
            sizeof(t_end),
            nullptr,
            0);
    }
    
}
namespace APISticker{
     const char* OCL_API_STICKER_FILTER;
     const char* OCL_API_STICKER_FILTER_NEGATIVE;
     std::set<std::string> p_filter;
     std::set<std::string> n_filter;
    #define APIMAP_ELEM(e)            \
    {                             \
#e, __LINE__ - START_LINE \
    }
constexpr uint32_t                    START_LINE = __LINE__ + 2;
const std::map<std::string, uint32_t> APIMap={
    APIMAP_ELEM(clGetPlatformIDs),
    APIMAP_ELEM(clGetPlatformInfo),
    APIMAP_ELEM(clGetDeviceIDs),
    APIMAP_ELEM(clGetDeviceInfo),
    APIMAP_ELEM(clCreateSubDevices),
    APIMAP_ELEM(clRetainDevice),
    APIMAP_ELEM(clReleaseDevice),
    APIMAP_ELEM(clCreateContext),
    APIMAP_ELEM(clCreateContextFromType),
    APIMAP_ELEM(clRetainContext),
    APIMAP_ELEM(clReleaseContext),
    APIMAP_ELEM(clGetContextInfo),
    APIMAP_ELEM(clCreateCommandQueue),
    APIMAP_ELEM(clRetainCommandQueue),
    APIMAP_ELEM(clReleaseCommandQueue),
    APIMAP_ELEM(clGetCommandQueueInfo),
    APIMAP_ELEM(clSetCommandQueueProperty),
    APIMAP_ELEM(clCreateBuffer),
    APIMAP_ELEM(clCreateBufferWithProperties),
    APIMAP_ELEM(clCreateSubBuffer),
    APIMAP_ELEM(clCreateImage),
    APIMAP_ELEM(clCreateImageWithProperties),
    APIMAP_ELEM(clCreateImage2D),
    APIMAP_ELEM(clCreateImage3D),
    APIMAP_ELEM(clRetainMemObject),
    APIMAP_ELEM(clReleaseMemObject),
    APIMAP_ELEM(clGetSupportedImageFormats),
    APIMAP_ELEM(clGetMemObjectInfo),
    APIMAP_ELEM(clGetImageInfo),
    APIMAP_ELEM(clSetMemObjectDestructorCallback),
    APIMAP_ELEM(clCreateSampler),
    APIMAP_ELEM(clRetainSampler),
    APIMAP_ELEM(clReleaseSampler),
    APIMAP_ELEM(clGetSamplerInfo),
    APIMAP_ELEM(clCreateProgramWithSource),
    APIMAP_ELEM(clCreateProgramWithBinary),
    APIMAP_ELEM(clCreateProgramWithBuiltInKernels),
    APIMAP_ELEM(clRetainProgram),
    APIMAP_ELEM(clReleaseProgram),
    APIMAP_ELEM(clBuildProgram),
    APIMAP_ELEM(clCompileProgram),
    APIMAP_ELEM(clLinkProgram),
    APIMAP_ELEM(clSetProgramReleaseCallback),
    APIMAP_ELEM(clSetProgramSpecializationConstant),
    APIMAP_ELEM(clUnloadPlatformCompiler),
    APIMAP_ELEM(clUnloadCompiler),
    APIMAP_ELEM(clGetProgramInfo),
    APIMAP_ELEM(clGetProgramBuildInfo),
    APIMAP_ELEM(clCreateKernel),
    APIMAP_ELEM(clCreateKernelsInProgram),
    APIMAP_ELEM(clRetainKernel),
    APIMAP_ELEM(clReleaseKernel),
    APIMAP_ELEM(clSetKernelArg),
    APIMAP_ELEM(clGetKernelInfo),
    APIMAP_ELEM(clGetKernelArgInfo),
    APIMAP_ELEM(clGetKernelWorkGroupInfo),
    APIMAP_ELEM(clWaitForEvents),
    APIMAP_ELEM(clGetEventInfo),
    APIMAP_ELEM(clCreateUserEvent),
    APIMAP_ELEM(clRetainEvent),
    APIMAP_ELEM(clReleaseEvent),
    APIMAP_ELEM(clSetUserEventStatus),
    APIMAP_ELEM(clSetEventCallback),
    APIMAP_ELEM(clGetEventProfilingInfo),
    APIMAP_ELEM(clFlush),
    APIMAP_ELEM(clFinish),
    APIMAP_ELEM(clEnqueueReadBuffer),
    APIMAP_ELEM(clEnqueueReadBufferRect),
    APIMAP_ELEM(clEnqueueWriteBuffer),
    APIMAP_ELEM(clEnqueueWriteBufferRect),
    APIMAP_ELEM(clEnqueueFillBuffer),
    APIMAP_ELEM(clEnqueueCopyBuffer),
    APIMAP_ELEM(clEnqueueCopyBufferRect),
    APIMAP_ELEM(clEnqueueReadImage),
    APIMAP_ELEM(clEnqueueWriteImage),
    APIMAP_ELEM(clEnqueueFillImage),
    APIMAP_ELEM(clEnqueueCopyImage),
    APIMAP_ELEM(clEnqueueCopyImageToBuffer),
    APIMAP_ELEM(clEnqueueCopyBufferToImage),
    APIMAP_ELEM(clEnqueueMapBuffer),
    APIMAP_ELEM(clEnqueueMapImage),
    APIMAP_ELEM(clEnqueueUnmapMemObject),
    APIMAP_ELEM(clEnqueueMigrateMemObjects),
    APIMAP_ELEM(clEnqueueNDRangeKernel),
    APIMAP_ELEM(clEnqueueTask),
    APIMAP_ELEM(clEnqueueNativeKernel),
    APIMAP_ELEM(clEnqueueMarker),
    APIMAP_ELEM(clEnqueueWaitForEvents),
    APIMAP_ELEM(clEnqueueBarrier),
    APIMAP_ELEM(clEnqueueMarkerWithWaitList),
    APIMAP_ELEM(clEnqueueBarrierWithWaitList),
    APIMAP_ELEM(clGetExtensionFunctionAddress),
    APIMAP_ELEM(clGetExtensionFunctionAddressForPlatform),
    APIMAP_ELEM(clCreateFromGLBuffer),
    APIMAP_ELEM(clCreateFromGLTexture),
    APIMAP_ELEM(clCreateFromGLTexture2D),
    APIMAP_ELEM(clCreateFromGLTexture3D),
    APIMAP_ELEM(clCreateFromGLRenderbuffer),
    APIMAP_ELEM(clGetGLObjectInfo),
    APIMAP_ELEM(clGetGLTextureInfo),
    APIMAP_ELEM(clEnqueueAcquireGLObjects),
    APIMAP_ELEM(clEnqueueReleaseGLObjects),
    APIMAP_ELEM(clSVMAlloc) ,
    APIMAP_ELEM(clSVMFree) ,
    APIMAP_ELEM(clEnqueueSVMFree) ,
    APIMAP_ELEM(clEnqueueSVMMemcpy) ,
    APIMAP_ELEM(clEnqueueSVMMemFill) ,
    APIMAP_ELEM(clEnqueueSVMMap) ,
    APIMAP_ELEM(clEnqueueSVMUnmap) ,
    APIMAP_ELEM(clSetKernelArgSVMPointer) ,
    APIMAP_ELEM(clSetKernelExecInfo) ,
    APIMAP_ELEM(clCreatePipe) ,
    APIMAP_ELEM(clGetPipeInfo) ,
    APIMAP_ELEM(clCreateCommandQueueWithProperties) ,
    APIMAP_ELEM(clCreateSamplerWithProperties) ,
    APIMAP_ELEM(clSetDefaultDeviceCommandQueue) ,
    APIMAP_ELEM(clGetDeviceAndHostTimer) ,
    APIMAP_ELEM(clGetHostTimer) ,
    APIMAP_ELEM(clCreateProgramWithIL) ,
    APIMAP_ELEM(clCloneKernel) ,
    APIMAP_ELEM(clGetKernelSubGroupInfo)
};
#undef APIMAP_ELEM
    void TraceEnter(const char* api, std::set<std::string>& p_filter, std::set<std::string>& n_filter)
    {
        try
        {
            uint32_t data = APIMap.at(api);
            char output[100];
            sprintf_s(output,"%s---->%d",api,data);
            OutputDebugString(output);
            
            if (!p_filter.empty()) {
                std::string t_entry(api);
                if (p_filter.find(t_entry) != p_filter.end()) {
                    MosTraceEvent(
                        API_STICKER_OCL,//task num, such as EVENT_ENCODE_API_STICKER_HEVC,
                        EVENT_TYPE_START,//EVENT_TYPE_START,
                        &data,
                        sizeof(data),
                        nullptr,
                        0);
                }
            
            }
            else {
         
                if (n_filter.find(std::string(api)) != n_filter.end()) {
                    // negative words not cover

                  /*  OutputDebugString("all the negative keys:");

                    for (const auto & key : n_filter) {
                        OutputDebugString(key.c_str());
                    }*/

                    return;
                }
                MosTraceEvent(
                    API_STICKER_OCL,//task num, such as EVENT_ENCODE_API_STICKER_HEVC,
                    EVENT_TYPE_START,//EVENT_TYPE_START,
                    &data,
                    sizeof(data),
                    nullptr,
                    0);
            }
        }
        catch (...)
        {
            char failure[100];
            sprintf(failure, "try catch failued->>>>%s", api);
            OutputDebugString("try catch failued");
            return;
        }
    }
}// namespace APISticker