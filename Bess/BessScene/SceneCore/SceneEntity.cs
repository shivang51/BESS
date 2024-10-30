﻿using System.Numerics;
using SkiaSharp;

namespace BessScene.SceneCore;

public abstract class SceneEntity
{
    private readonly Transform _transform;
    public uint RenderId { get; private set; }

    protected SceneEntity(Transform transform, uint renderId)
    {
        _transform = transform;
        RenderId = renderId;
    }

    protected SceneEntity(Vector2 position, Vector2 scale, float rotation, uint renderId)
    {
        _transform = new Transform(position, scale, rotation);
        RenderId = renderId;
    }

    protected SceneEntity(Vector2 position, Vector2 scale, uint renderId)
    {
        _transform = new Transform(position, scale);
        RenderId = renderId;
    }

    public Vector2 Position
    {
        get => _transform.Position;
        set => _transform.Position = value;
    }
    
    public Vector2 Scale
    {
        get => _transform.Scale;
        set => _transform.Scale = value;
    }

    protected SKColor GetRIdColor()
    {
        var r = (byte)((RenderId >> 24) & 0xFF);
        var g = (byte)((RenderId >> 16) & 0xFF);
        var b = (byte)((RenderId >> 8) & 0xFF);
        var a = (byte)((RenderId >> 0) & 0xFF);

        return new SKColor(r, g, b, a);
    }

    protected SKPoint SkPosition => new(Position.X, Position.Y);

    protected SKSize SkScale => new(Scale.X, Scale.Y);

    public abstract void Render();

}