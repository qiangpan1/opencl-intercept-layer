#include "api_sticker.h"


const GUID g_intelMedia = { 0x4e1c52c9, 0x1d1e, 0x4470, {0xa1, 0x10, 0x25, 0xa9, 0xf3, 0xeb, 0xe1, 0xa5} };

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
void MosTraceEvent(uint16_t usId,
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
 

namespace APISticker{
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
    void TraceEnter(const char* api)
    {
        try
        {
            OutputDebugString(api);
            //auto data = APIMap.at(api);

            // MOS_TraceEventExt(
            //     TR_KEY_ENCODE_EVENT_API_STICKER,
            //     MT_EVENT_LEVEL::ALWAYS,
            //     EVENT_ENCODE_API_STICKER_AV1_VA,
            //     EVENT_TYPE_START,
            //     &data,
            //     sizeof(data));
            MT_LOG1(33555459, 1, 4097, 1000);
        }
        catch (...)
        {
            return;
        }
    }
}// namespace APISticker