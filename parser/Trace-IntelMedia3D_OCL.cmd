@echo off

REM In order to capture etl traces, this script relies on Xperf.exe. It is assumed 
REM that Xperf.exe will be installed in GPUView's parent directory.

REM
REM Logic:
REM
REM  If the command line is blank, we'll try to start logging using normal settings.
REM  If the command line is blank and we're already logging, we'll turn off logging.
REM
REM Usage:
REM
REM  This script requires an Administrator CMD Window to work. This script also
REM  uses the DOS environment to save state. The script is designed to be run twice:
REM  the first time it turns on logging, the second time it stops logging. The end
REM  result is a merged.etl file. 

REM
REM Different tracing options that can be enabled
REM
REM SET XXX=(GUID|KnownProviderName)[:(Flags|Groups)[:Level[:'stack']]]
set TRACE_DXVA2=a0386e75-f70c-464c-a9ce-33c44e091623:0xffff:5
set TRACE_MMCSS=f8f10121-b617-4a56-868b-9df1b27fe32c:0xffff:5
set TRACE_WMDRM=6e03dd26-581b-4ec5-8f22-601a4de5f022:0xffff:5
set TRACE_WME=8f2048e0-f260-4f57-a8d1-932376291682
set TRACE_WMP=681069c4-b785-466a-bc63-4aa616644b68:0xffff:5
set TRACE_DVD=75d4a1bb-7cc6-44b1-906d-d5e05be6d060:0xffff:5
set TRACE_DSHOW=28cf047a-2437-4b24-b653-b9446a419a69
set TRACE_MF=f404b94e-27e0-4384-bfe8-1d8d390b0aa3+362007f7-6e50-4044-9082-dfa078c63a73:0x000000000000ffff:0x5
set TRACE_AE=a6a00efd-21f2-4a99-807e-9b3bf1d90285:0x000000000000ffff:0x5
set TRACE_HME=63770680-05F1-47E0-928A-9ACFDCF52147:0xffff:5
set TRACE_HDDVD=779d8cdc-666b-4bf4-a367-9df89d6901e8:0xffff:5
set TRACE_DWMAPIGUID=71dd85bc-d474-4974-b0f6-93ffc5bfbd04::6
set TRACE_SCHEDULEGUID=8cc44e31-7f28-4f45-9938-4810ff517464:0xffff:6
set TRACE_DX=DX
set TRACE_WARP=ee685ec4-8270-4b08-9e4e-8b356f48f92f:0
set TRACE_DXGI=CA11C036-0102-4A2D-A6AD-F03CFED5D3C9:0xf:6:'stack'
set TRACE_DXGIDEBUG=F1FF64EF-FAF3-5699-8E51-F6EC2FBD97D1:0xffffffffffffffff
set TRACE_DXGIDEBUG_LIGHT=%TRACE_DXGIDEBUG%:4
set TRACE_DXGIDEBUG_NORMAL=%TRACE_DXGIDEBUG%:4:'stack'
set TRACE_DXGIDEBUG_VERBOSE=%TRACE_DXGIDEBUG%:6:'stack'
set TRACE_D3D12=5d8087dd-3a9b-4f56-90df-49196cdc4f11:0xffffffffffffffff:6:'stack'
set TRACE_D3D11=db6f6ddb-ac77-4e88-8253-819df9bbf140:0xffffffffffffffff:6:'stack'
set TRACE_D3D10LEVEL9=7E7D3382-023C-43cb-95D2-6F0CA6D70381:0x1
set TRACE_DXC=802ec45a-1e99-4b83-9920-87c98277ba9d
set TRACE_DXC_ALL=%TRACE_DXC%:0x10FFFF:5:'stack'
set TRACE_DXC_LONGHAUL=%TRACE_DXC%:0x800:5
set TRACE_DXC_MIN=%TRACE_DXC%:0x800:5
set TRACE_DXC_LIGHT=%TRACE_DXC%:0x826:5
set TRACE_DXC_NORMAL=%TRACE_DXC%:0x100236:5
set TRACE_DXC_STACKS=%TRACE_DXC%:0x208041:5:'stack'
set TRACE_UMD=a688ee40-d8d9-4736-b6f9-6b74935ba3b1:0xffff:5
set TRACE_DWM=a42c77db-874f-422e-9b44-6d89fe2bd3e5:0x000000007fffffff:0x5
set TRACE_DWM2=8c9dd1ad-e6e5-4b07-b455-684a9d879900:0xFFFF:6
set TRACE_DWM3=9e9bba3c-2e38-40cb-99f4-9e8281425164:0xFFFF:6
set TRACE_CODEC=ea6d6e3b-7014-4ab1-85db-4a50cda32a82:0xffff:5
set TRACE_KMFD=E7C7EDF9-D0E4-4338-8AE3-BCA3C5B4B4A3
set TRACE_UMFD=a70bc228-e778-4061-86fa-debb03fda64a
REM set TRACE_AVALON=AvalonALL
set TRACE_TESTFRAMEWORK=31293f4f-f7bb-487d-8b3b-f537b827352f
set TRACE_TEST=42C4E0C1-0D92-46f0-842C-1E791FA78D52
set TRACE_SC=30336ed4-e327-447c-9de0-51b652c86108
set TRACE_XAML=531A35AB-63CE-4BCF-AA98-F88C7A89E455:0xffff
set TRACE_WIN32K=8c416c79-d49b-4f01-a467-e56d3aa8234c:0xffff
set TRACE_D2D=dcb453db-c652-48be-a0f8-a64459d5162e:0:5
set TRACE_D2DSCENARIOS=712909c0-6e57-4121-b639-87c8bf9004e0
REM set TRACE_PRESENT=
set TRACE_D3D9_PRESENT=783ACA0A-790E-4d7f-8451-AA850511C6B9:0xf:6
set TRACE_DXGI_PRESENT=CA11C036-0102-4A2D-A6AD-F03CFED5D3C9:0xf:6
set TRACE_D3D11_PRESENT=db6f6ddb-ac77-4e88-8253-819df9bbf140:0xffffffffffffffff:6
set TRACE_DXC_ALL_PRESENT=%TRACE_DXC%:0x10FFFF:5
set TRACE_DXC_PRESENT=%TRACE_DXC%:0x208041:5


REM
REM To Add another provider, create an environment variable here and then add it 
REM to the collection just below. 
REM set TRACE_PRIVATE=X
REM

REM
REM These variables hold provider GUIDs collections that we want for each level.
REM Notice that the levels build on each other. If you add a provider to the collection,
REM you should just have to add it once. If there are special flags for each level,
REM modify the individual sections like what's done for the Dxgkrnl logger. Also notice
REM that there are two main sessions: One that the script will call capturestate on
REM and another that we don't. If your provider doesn't respond to capturestate,
REM it should not be placed in that session.
REM
REM CaptureState collection
SET TRACE_CS_PROVIDERS_MIN=%TRACE_UMD%+%TRACE_DXGI%+%TRACE_D3D12%+%TRACE_D3D11%+%TRACE_D3D10LEVEL9%
SET TRACE_CS_PROVIDERS_PRESENT=%TRACE_UMD%+%TRACE_DXGI_PRESENT%+%TRACE_D3D11_PRESENT%+%TRACE_D3D9_PRESENT%

SET TRACE_CS_PROVIDERS_LIGHT=%TRACE_CS_PROVIDERS_MIN%
SET TRACE_CS_PROVIDERS_NORMAL=%TRACE_CS_PROVIDERS_LIGHT%
SET TRACE_CS_PROVIDERS_VERBOSE=%TRACE_CS_PROVIDERS_NORMAL%

SET TRACE_CS_STATE_MIN=%TRACE_UMD%+%TRACE_DXGI%+%TRACE_D3D12%+%TRACE_D3D11%+%TRACE_D3D10LEVEL9%
SET TRACE_CS_STATE_PRESENT=%TRACE_UMD%+%TRACE_DXGI_PRESENT%+%TRACE_D3D11_PRESENT%+%TRACE_D3D9_PRESENT%

SET TRACE_CS_STATE_LIGHT=%TRACE_CS_STATE_MIN%
SET TRACE_CS_STATE_NORMAL=%TRACE_CS_STATE_LIGHT%
SET TRACE_CS_STATE_VERBOSE=%TRACE_CS_STATE_NORMAL%

REM No CaptureState collection
SET TRACE_NOCS_PROVIDERS_PRESENT=%TRACE_DXC_PRESENT%+%TRACE_DWM3%+%TRACE_WIN32K%
SET TRACE_NOCS_PROVIDERS_MIN=%TRACE_DXC_STACKS%

SET TRACE_NOCS_PROVIDERS_LIGHT_NODXGIDEBUG=%TRACE_NOCS_PROVIDERS_MIN%+%TRACE_MF%+%TRACE_WME%
SET TRACE_NOCS_PROVIDERS_NORMAL_NODXGIDEBUG=%TRACE_NOCS_PROVIDERS_LIGHT_NODXGIDEBUG%+%TRACE_SCHEDULEGUID%+%TRACE_SC%+%TRACE_WIN32K%+%TRACE_DWM%+%TRACE_DWM2%+%TRACE_DWM3%+%TRACE_TESTFRAMEWORK%+%TRACE_TEST%+%TRACE_DSHOW%+%TRACE_AE%+%TRACE_DXVA2%
SET TRACE_NOCS_PROVIDERS_VERBOSE_NODXGIDEBUG=%TRACE_NOCS_PROVIDERS_NORMAL_NODXGIDEBUG%+%TRACE_D2DSCENARIOS%+%TRACE_D2D%+%TRACE_MMCSS%+%TRACE_WMDRM%+%TRACE_WMP%+%TRACE_DVD%+%TRACE_HME%+%TRACE_HDDVD%+%TRACE_DWMAPIGUID%+%TRACE_CODEC%

SET TRACE_NOCS_PROVIDERS_LIGHT=%TRACE_NOCS_PROVIDERS_LIGHT_NODXGIDEBUG%+%TRACE_DXGIDEBUG_LIGHT%
SET TRACE_NOCS_PROVIDERS_NORMAL=%TRACE_NOCS_PROVIDERS_NORMAL_NODXGIDEBUG%+%TRACE_DXGIDEBUG_NORMAL%
SET TRACE_NOCS_PROVIDERS_VERBOSE=%TRACE_NOCS_PROVIDERS_VERBOSE_NODXGIDEBUG%+%TRACE_DXGIDEBUG_VERBOSE%

REM
REM To turn off the echo to the console, set this variable to no
REM
SET ECHO_INFO=yes



REM 
REM To determine physical memory
REM
if not "%TRACE_LOGGING_MEMORY%" == "" goto Do_Not_Process_File
systeminfo > me.txt
findstr /sipn /C:"Total Physical Memory" me.txt > me2.txt
REM TRACE_LOGGING_MEMORY will hold the amount of physical memory on this machine.
for /f "tokens=6 delims=: " %%a in (me2.txt) do set TRACE_LOGGING_MEMORY=%%a
del me.txt
del me2.txt
set TRACE_LOGGING_MEMORY=%TRACE_LOGGING_MEMORY:,=%
:Do_Not_Process_File

REM
REM For buffers sizes
REM
REM The cutoff for large bufers 5G, Medium buffers 2G physical memory
if %TRACE_LOGGING_MEMORY% Gtr 4000 goto Set_Large_Buffers
if %TRACE_LOGGING_MEMORY% Gtr 2000 goto Set_Medium_Buffers
REM echo !Using Small Buffers Memory Footprint here!
set TRACE_LARGE_BUFFERS=-BufferSize 1024 -MinBuffers 30 -MaxBuffers 120
set TRACE_STAND_BUFFERS=-BufferSize 1024 -MinBuffers 25 -MaxBuffers 25
goto Done_With_It
:Set_Medium_Buffers
REM echo !Using Meidum Buffers Memory Footprint here!
set TRACE_LARGE_BUFFERS=-BufferSize 1024 -MinBuffers 60 -MaxBuffers 240
set TRACE_STAND_BUFFERS=-BufferSize 1024 -MinBuffers 50 -MaxBuffers 50
goto Done_With_It
:Set_Large_Buffers
REM echo !Using Large Buffers Memory Footprint here!
set TRACE_LARGE_BUFFERS=-BufferSize 1024 -MinBuffers 120 -MaxBuffers 480
set TRACE_STAND_BUFFERS=-BufferSize 1024 -MinBuffers 100 -MaxBuffers 100
:Done_With_It

REM
REM NT logging groupings
REM 
set TRACE_NT_MIN=LOADER+PROC_THREAD+CSWITCH+DISPATCHER+POWER
set TRACE_NT_LONGHAUL=LOADER+PROC_THREAD+POWER
set TRACE_NT_LIGHT=%TRACE_NT_MIN%+DISK_IO+HARD_FAULTS
REM
REM Note in order to get callstacks on amd64 bit builds, you need to enable the following
REM registry key:
REM
REM HKLM\System\CurrentControlSet\Control\Session Manager\Memory Management\DisablePagingExecutive
REM 
REM by setting it to 1. Lookup DisablePagingExecutive via your favorite search engine for details.
REM
set TRACE_NT_NORMAL=%TRACE_NT_LIGHT%+PROFILE+MEMINFO+DPC+INTERRUPT -stackwalk @"%~dp0\EventsForStackTrace.txt"
set TRACE_NT_VERBOSE=%TRACE_NT_LIGHT%+PROFILE+MEMINFO+SYSCALL+DPC+INTERRUPT+ALL_FAULTS -stackwalk @"%~dp0\EventsForStackTrace.txt"

REM 
REM From 'xperf -help providers' we get the following table. GPUView, at a minimum
REM really needs LOADER, PROC_THREAD and CSWITCH.
REM 
REM Kernel Flags:
REM        PROC_THREAD    : Process and Thread create/delete
REM        LOADER         : Kernel and user mode Image Load/Unload events
REM        PROFILE        : CPU Sample profile
REM        CSWITCH        : Context Switch
REM        COMPACT_CSWITCH: Compact Context Switch
REM        DISPATCHER     : CPU Scheduler
REM        DPC            : DPC Events
REM        INTERRUPT      : Interrupt events
REM        SYSCALL        : System calls
REM        PRIORITY       : Priority change events
REM        ALPC           : Advanced Local Procedure Call
REM        PERF_COUNTER   : Process Perf Counters
REM        DISK_IO        : Disk I/O
REM        DISK_IO_INIT   : Disk I/O Initiation
REM        FILE_IO        : File system operation end times and results
REM        FILE_IO_INIT   : File system operation (create/open/close/read/write)
REM        HARD_FAULTS    : Hard Page Faults
REM        FILENAME       : FileName (e.g., FileName create/delete/rundown)
REM        SPLIT_IO       : Split I/O
REM        REGISTRY       : Registry tracing
REM        DRIVERS        : Driver events
REM        POWER          : Power management events
REM        NETWORKTRACE   : Network events (e.g., tcp/udp send/receive)
REM        VIRT_ALLOC     : Virtual allocation reserve and release
REM        MEMINFO        : Memory List Info
REM        ALL_FAULTS     : All page faults including hard, Copy on write, demand zero faults, etc.


REM
REM 

REM Start Logging section
:StartSection

:StartNormal
SET TRACE_NT_PROVIDER=%TRACE_NT_NORMAL%
SET TRACE_CS_PROVIDERS=%TRACE_DXC_NORMAL%+%TRACE_CS_PROVIDERS_NORMAL%
SET TRACE_CS_STATE=%TRACE_DXC_ALL%+%TRACE_CS_STATE_NORMAL%
SET TRACE_NOCS_PROVIDERS=%TRACE_DX%:0x2F+%TRACE_XAML%:4+%TRACE_WARP%:1+%TRACE_NOCS_PROVIDERS_NORMAL%
REM call :StartProviders


"%~dp0"..\Xperf -on %TRACE_NT_PROVIDER% %TRACE_LARGE_BUFFERS% -f Kernel.etl
"%~dp0"..\Xperf -start CaptureState -on %TRACE_CS_PROVIDERS% %TRACE_LARGE_BUFFERS% -f CaptureState.etl
"%~dp0"..\Xperf -capturestate CaptureState %TRACE_CS_STATE%
"%~dp0"..\Xperf -start NoCaptureState -on %TRACE_NOCS_PROVIDERS% %TRACE_LARGE_BUFFERS% -f NoCaptureState.etl
"%~dp0"..\Xperf -start IntelOCLTrace -on Intel-OCL-VK:0xffff:1:'stack' -buffersize 1024 -f IntelOCLTrace.etl
"%~dp0"..\Xperf -start D3D10Trace -on Intel-Graphics-D3D10:0x03::'stack' -buffersize 1024 -f D3D10Trace.etl

@pause

"%~dp0"..\Xperf -stop IntelOCLTrace
"%~dp0"..\Xperf -stop D3D10Trace
"%~dp0"..\Xperf -stop CaptureState
"%~dp0"..\Xperf -stop NoCaptureState
"%~dp0"..\Xperf.exe -stop
echo All loggers stopped, starting merge...
"%~dp0"..\Xperf -merge Kernel.etl IntelOCLTrace.etl D3D10Trace.etl NoCaptureState.etl CaptureState.etl Merged.etl
echo Restoring Performance Counter Interval
"%~dp0"..\Xperf -setprofint

@echo off

set TRACE_DXVA2=
set TRACE_MMCSS=
set TRACE_WMDRM=
set TRACE_WME=
set TRACE_WMP=
set TRACE_DVD=
set TRACE_DSHOW=
set TRACE_MF=
set TRACE_AE=
set TRACE_HME=
set TRACE_HDDVD=
set TRACE_DWMAPIGUID=
set TRACE_SCHEDULEGUID=
set TRACE_DX=
set TRACE_WARP=
set TRACE_DXGI=
set TRACE_DXGIDEBUG=
set TRACE_DXGIDEBUG_LIGHT=
set TRACE_DXGIDEBUG_NORMAL=
set TRACE_DXGIDEBUG_VERBOSE=
set TRACE_D3D12=
set TRACE_D3D11=
set TRACE_D3D10LEVEL9=
set TRACE_DXC=
set TRACE_DXC_ALL=
set TRACE_DXC_LONGHAUL=
set TRACE_DXC_MIN=
set TRACE_DXC_LIGHT=
set TRACE_DXC_NORMAL=
set TRACE_DXC_STACKS=
set TRACE_UMD=
set TRACE_DWM=
set TRACE_DWM2=
set TRACE_DWM3=
set TRACE_CODEC=
set TRACE_KMFD=
set TRACE_UMFD=
set TRACE_TESTFRAMEWORK=
set TRACE_TEST=
set TRACE_SC=
set TRACE_XAML=
set TRACE_WIN32K=
set TRACE_D2D=
set TRACE_D2DSCENARIOS=

set TRACE_D3D9_PRESENT=
set TRACE_DXGI_PRESENT=
set TRACE_D3D11_PRESENT=
set TRACE_DXC_ALL_PRESENT=
set TRACE_DXC_PRESENT=

set TRACE_LARGE_BUFFERS=
set TRACE_STAND_BUFFERS=
set TRACE_NT_LONGHAUL=
set TRACE_NT_MIN=
set TRACE_NT_LIGHT=
set TRACE_NT_NORMAL=
set TRACE_NT_VERBOSE=

set TRACE_CS_PROVIDERS_MIN=
set TRACE_CS_PROVIDERS_PRESENT=
set TRACE_CS_PROVIDERS_LIGHT=
set TRACE_CS_PROVIDERS_NORMAL=
set TRACE_CS_PROVIDERS_VERBOSE=
set TRACE_CS_STATE_MIN=
set TRACE_CS_STATE_PRESENT=
set TRACE_CS_STATE_LIGHT=
set TRACE_CS_STATE_NORMAL=
set TRACE_CS_STATE_VERBOSE=
set TRACE_NOCS_PROVIDERS_MIN=
set TRACE_NOCS_PROVIDERS_PRESENT=
set TRACE_NOCS_PROVIDERS_LIGHT=
set TRACE_NOCS_PROVIDERS_NORMAL=
set TRACE_NOCS_PROVIDERS_VERBOSE=
SET TRACE_NOCS_PROVIDERS_LIGHT_NODXGIDEBUG=
SET TRACE_NOCS_PROVIDERS_NORMAL_NODXGIDEBUG=
SET TRACE_NOCS_PROVIDERS_VERBOSE_NODXGIDEBUG=

set ECHO_INFO=

set TRACE_NT_PROVIDER=
set TRACE_CS_PROVIDERS=
set TRACE_CS_STATE=
set TRACE_NOCS_PROVIDERS=

@echo on

