@reg import register.reg
@if not exist "%ProgramFiles%\Intel\Graphics" mkdir "%ProgramFiles%\Intel\Graphics"
@if exist IntelOCLEtw.dll copy /Y IntelOCLEtw.dll "%ProgramFiles%\Intel\Graphics"
@echo Completed the Installation...

@setx  OCL_API_STICKER_FILTER "clEnqueueNDRangeKernel,clCreateBuffer,clEnqueueCopyBuffer,clEnqueueReadBuffer,clEnqueueReadBuffer,clEnqueueMapBuffer,clCreateSubBuffer,clEnqueueReadBufferRect,clEnqueueWriteBuffer，clEnqueueWriteBufferRect，clEnqueueFillBuffer，clEnqueueCopyBufferRect,clCreateImage,clEnqueueReadImage,clEnqueueWriteImage,clEnqueueFillImage,clEnqueueCopyImage, clEnqueueCopyImageToBuffer ,clEnqueueCopyBufferToImage, clEnqueueMapImage"
@setx  OCL_API_STICKER_FILTER_NEGATIVE "clSetKernelArg,"
@setx  OCL_TRACE_FILTER ""
@echo Completed the config...