using System;
using Avalonia.Controls;
using Avalonia.OpenGL;
using Avalonia.OpenGL.Controls;
using BessScene;

namespace Bess.Controls;

public class BessSceneControl: OpenGlControlBase
{
    private readonly BessScene.SceneContext _sceneCtx = new();
    
    protected override void OnOpenGlInit(GlInterface gl)
    {
        var Info = $"Renderer: {gl.GetString(GlConsts.GL_RENDERER)} Version: {gl.GetString(GlConsts.GL_VERSION)}";
        Console.WriteLine(Info);
        Console.WriteLine(gl.GetString(ApiWrapper.GlVersion()));
        _sceneCtx.Init();
    }
    
    protected override void OnOpenGlRender(GlInterface gl, int fb)
    {
        // _sceneCtx.Render(fb);
        gl.BindFramebuffer(GlConsts.GL_FRAMEBUFFER, fb);
        gl.ClearColor((float)0.9, (float)0.2, (float)0.2, (float)1.0);
        gl.Clear(GlConsts.GL_COLOR_BUFFER_BIT | GlConsts.GL_DEPTH_BUFFER_BIT);
    }

    protected override void OnSizeChanged(SizeChangedEventArgs e)
    {
        base.OnSizeChanged(e);
        // _sceneCtx.OnResize((int)e.NewSize.Width, (int)e.NewSize.Height);
    }
}