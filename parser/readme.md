# Tracing config

### register events

- `register.reg` register the events
- copy `IntelVulkanEtw.dll` to `%ProgramFiles%\Intel\Graphics\`

### replace intercept layer

replace `OpenCL.dll` to target App's folder.

### capture with config

sample capture CMD: `Trace-IntelMedia3D_OCL.cmd`

it would add GUID for capture, you can add them to any trace config prefered

![image-20240325103731829](readme.assets/image-20240325103731829.png)

