# Known Issues

GLOVE is considered as work-in-progress, therefore there are known issues that have to be resolved or improved.

Most Important Known Issues list:

| **Issue** | **Description** | **Solution/Workaround** | **Status** | 
| --- | --- | --- | --- |
| Incorrect Depth Buffer Initialization (glClearDepthf) | Some versions of mesa Intel Vulkan driver does not work with normalized clear values | Give unnormalized clear values | **resolved with workaround** |
| Non-performant Clear| Framebuffer clearing is performed via VkClearAttachments in a separate render pass | Clear framebuffer inside each renderpass (set clear values in VkRenderPassBeginInfo) | **unresolved** |
| glColorMask() not working | Enabling or disabling writing of frame buffer color components r, g, b, a not working | Clear framebuffer inside each renderpass (set clear values in VkRenderPassBeginInfo) | **unresolved** |
| Reshape functionality not supported  | Reshape not implemented yet | Support Reshape functionality | **unresolved** |
| Multiple EGLContexts not working  | Although multiple EGLContexts are supported in theory they are not working correctly| Correct errors related to multiple EGLContexts | **unresolved** |
| Multiple threads not supported  | Multiple threads not implemented yet in EGL | Support multiple threads in EGL | **unresolved** |
| Vulkan Textures allocated as RGBA in all cases  | Implicit convertion of all textures to GL_RGBA | Allocate Textures according to input format | **unresolved** |
| GL to Vulkan Depth Range conversion adds overhead| Adding ``` gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0; ``` in Vertex Shader | TBD | **unresolved** |

