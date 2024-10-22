#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
    #ifdef DLL_EXPORT
        #define MODULE_API __declspec(dllexport)
    #else
        #define MODULE_API __declspec(dllimport)
    #endif
#else
    #define MODULE_API
#endif
MODULE_API void* Init();
MODULE_API int GlVersion();
MODULE_API void Update(void*);
MODULE_API void Render(void*, int);
MODULE_API void OnResize(void*, int, int);
#ifdef __cplusplus
}
#endif
