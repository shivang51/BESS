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
        _sceneCtx.OnResize((int)800, 450);
    }
    
    protected override void OnOpenGlRender(GlInterface gl, int fb)
    {
        _sceneCtx.Render(fb);
        _sceneCtx.Update();
    }

    protected override void OnSizeChanged(SizeChangedEventArgs e)
    {
        base.OnSizeChanged(e);
        if (_sceneCtx.IsInitialized)
        {
            _sceneCtx.OnResize((int)e.NewSize.Width, (int)e.NewSize.Height);
        }
    }
}