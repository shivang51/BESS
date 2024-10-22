using System.Runtime.InteropServices;

namespace BessScene;

public class SceneContext
{
    private IntPtr _ctxPtr;
    
    public void Init()
    {
        _ctxPtr = ApiWrapper.Init();
    }
    
    public void Update()
    {
        ApiWrapper.Update(_ctxPtr);
    }
    
    public void Render(int fbo)
    {
        ApiWrapper.Render(_ctxPtr, fbo);
    }

    public void OnResize(int width, int height)
    {
        ApiWrapper.OnResize(_ctxPtr, width, height);
    }
}