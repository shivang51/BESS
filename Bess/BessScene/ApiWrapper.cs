using System.Runtime.InteropServices;

namespace BessScene;

public class ApiWrapper
{
    private const string DllName = "LibBessScene";

    [DllImport(DllName)]
    public static extern int GlVersion();
    
    [DllImport(DllName)]
    public static extern IntPtr Init();
    
    [DllImport(DllName)]
    public static extern void Update(IntPtr ctx);
    
    [DllImport(DllName)]
    public static extern void Render(IntPtr ctx, int fbo);
    
    [DllImport(DllName)]
    public static extern void OnResize(IntPtr ctx, int width, int height);
}