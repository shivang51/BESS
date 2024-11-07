﻿using System;
using System.Numerics;
using BessScene.SceneCore.State.Events;
using BessScene.SceneCore.State.SceneCore.Entities;
using SkiaSharp;

namespace BessScene.SceneCore.State;

public class BessSkiaScene
{
    private int Width { get; set; }
    private int Height { get; set; }
    
    private SKBitmap _colorBuffer = null!;
    private SKBitmap _idBuffer = null!;
    
    private uint EmptyId { get; } = 0;
    
    public CameraController Camera { get; } = new();

    public BessSkiaScene(double width, double height)
    {
        Width = (int)width;
        Height = (int)height;
        InitializeBuffers();
    }

    private void InitializeBuffers()
    {
        _colorBuffer = new SKBitmap(Width, Height, SKColorType.Rgba8888, SKAlphaType.Premul);
        _idBuffer = new SKBitmap(Width, Height, SKColorType.Rgba8888, SKAlphaType.Opaque);
    }

    public void Resize(double newWidth, double newHeight)
    {
        var w = (int)newWidth;
        var h = (int)newHeight;
        if (w == Width && h == Height) return;
        Width = w;
        Height = h;
        _colorBuffer?.Dispose();
        _idBuffer?.Dispose();
        InitializeBuffers();
    }

    public void Update(List<SceneEvent> events)
    {
        foreach (var evt in events)
        {
            switch (evt.Type)
            {
                case EventType.MouseMove:
                    OnMouseMove((evt as MouseMoveEvent)!.Data);
                break;
                case EventType.MouseButton:
                {
                    var data = (evt as MouseButtonEvent)!.Data;
                    switch (data.Button)
                    {
                        case MouseButton.Left:
                            OnMouseLeftEvent(data);
                            break;
                        case MouseButton.Middle:
                            OnMouseMiddleEvent(data);
                            break;
                        case MouseButton.Right:
                            break;
                        default:
                            throw new NotImplementedException($"{data.Button} mouse button is not implemented");
                    }
                }
                break;
                // case EventType.MouseScroll:
                //     break;
                // case EventType.KeyDown:
                //     break;
                // case EventType.KeyUp:
                //     break;
                // case EventType.KeyPress:
                //     break;
                // case EventType.WindowResize:
                    // break;
                default:
                    throw new ArgumentOutOfRangeException($"Invalid event type {evt.Type}");
            }
        }
        
        var entities = SceneState.Instance.Entities;
        SceneState.Instance.HoveredEntityId = GetRenderObjectId((int)SceneState.Instance.MousePosition.X, (int)SceneState.Instance.MousePosition.Y);
        
        foreach (var ent in entities)
        {
            ent.Update();
        }
        
    }

    public SKBitmap GetColorBuffer() => _colorBuffer;

    public void RenderScene()
    {
        
        var colorCanvas = new SKCanvas(_colorBuffer);
        var idCanvas = new SKCanvas(_idBuffer);
        
        colorCanvas.Clear(new SKColor(30, 30, 30, 255));
        idCanvas.Clear(new SKColor(0, 0, 0, 0));

        colorCanvas.Scale(Camera.GetZoomSkPoint);
        idCanvas.Scale(Camera.GetZoomSkPoint);
        
        var position = VectorToSkPoint(Camera.Position);
        colorCanvas.Translate(position);
        idCanvas.Translate(position);

        SkRenderer.Begin(colorCanvas, idCanvas);
        
        foreach (var ent in SceneState.Instance.Entities)
        {
            ent.Render();
        }


        colorCanvas.Flush();
        idCanvas.Flush();
    }

    public uint GetRenderObjectId(int x, int y)
    {
        var color = ReadPixel(_idBuffer, x, y);
        return UIntFromRgba(color.Red, color.Green, color.Blue, color.Alpha);
    }
    
    private static SKColor ReadPixel(SKBitmap bitmap, int x, int y)
    {
        if (x >= 0 && x < bitmap.Width && y >= 0 && y < bitmap.Height)
        {
            return bitmap.GetPixel(x, y);
        }

        return new SKColor(0);
        // throw new ArgumentOutOfRangeException($"Pixel coordinates ({x} or {y}) are out of bounds.");
    }
    
    private static uint UIntFromRgba(byte r, byte g, byte b, byte a)
    {
        var id = (uint)r << 16 | (uint)g << 8 | (uint)b << 0 | 0;
        return id;
    } 
    
    private static SKPoint VectorToSkPoint(Vector2 vector) => new((float)vector.X, (float)vector.Y);
    
    private void OnMouseMove(MouseMoveData data)
    {
        var prevPos = SceneState.Instance.MousePosition;
        var dPos = data.Position - prevPos;
        SceneState.Instance.MousePosition = data.Position;
        var selEntity = SceneState.Instance.SelectedEntityId;
        var mousePos = SceneState.Instance.MousePosition;

        var dragData = SceneState.Instance.DragData;

        if (SceneState.Instance.IsLeftMousePressed)
        {
            if (dragData.IsDragging)
            {
                var ent = SceneState.Instance.GetEntityByRenderId(dragData.EntityId);    
                ent.Position = ToWorldPos(mousePos) - dragData.DragOffset;
            }
        }else if (SceneState.Instance.IsMiddleMousePressed)
        {
            Camera.UpdatePositionBy(dPos);
        }
    }

    private Vector2 ToWorldPos(Vector2 pos)
    {
        return Transform.ScaleAndTranslate(pos, Camera.ZoomInv, -Camera.Position);
    }

    private void OnMouseLeftEvent(MouseButtonEventData data)
    {
        var pos = data.Position;
        var rid = GetRenderObjectId((int)pos.X, (int)pos.Y);
        SceneState.Instance.SelectedEntityId = rid;
        SceneState.Instance.MousePosition = pos;
        SceneState.Instance.IsLeftMousePressed = data.Pressed;
        
        if (SceneState.Instance.SelectedEntityId == EmptyId) return;

        if (data.Pressed)
        {
            SceneState.Instance.StartDrag(rid, ToWorldPos(pos));
        }else
        {
            SceneState.Instance.EndDrag();
        }
    }
    
    private void OnMouseMiddleEvent(MouseButtonEventData data)
    {
        SceneState.Instance.IsMiddleMousePressed = data.Pressed;
    }
}
