﻿using System.Numerics;
using SkiaSharp;

namespace BessScene.SceneCore.Sketches;

public class GateSketch: SceneEntity
{
    private readonly int _inputCount;
    private readonly int _outputCount;
    private readonly string _name;
    
    private readonly List<SlotSketch> _inputSlots;
    private readonly List<SlotSketch> _outputSlots;

    private const float Padding = 6;
    private const float Width = 120;
    private const float HeaderHeight = 20;
    private const float RowPadding = 2;
    private const float RowGap = 8;
    private const float BorderRadius = 4;
    private const float SlotSize = SlotSketch.SlotSize;
    
    public GateSketch(string name, int inputCount, int outputCount) : base(new Vector2(20, 20))
    {
        _name = name;
        _inputCount = inputCount;
        _outputCount = outputCount;
        
        Scale = new Vector2(Width, GetHeight());
        
        _inputSlots = GenerateSlots(inputCount);
        _outputSlots = GenerateSlots(outputCount, false);
    }

    public override void Render()
    {
        SkRenderer.DrawMicaRoundRect(SkPosition, SkScale, BorderRadius, GetRIdColor());
        SkRenderer.DrawMicaRoundRect(SkPosition, HeaderSize, new Vector4(BorderRadius, BorderRadius, 0, 0), SKColors.MidnightBlue);
        
        SkRenderer.DrawText(_name, GetHeaderTextPosition(), SKColors.White, 11);
        
        foreach (var slot in _inputSlots)
        {
            slot.Render();
        }
        
        foreach (var slot in _outputSlots)
        {
            slot.Render();
        }
    }
    
    private float GetHeight()
    {
        var n = Math.Max(_inputCount, _outputCount);
        return HeaderHeight + RowHeight * n + RowGap * (n - 1) + Padding + Padding;
    }
    
    private static SKSize HeaderSize => new(Width, HeaderHeight);

    private Vector2 RightCornerPos => new(Position.X + Width, Position.Y);
    
    private static float RowHeight => SlotSize + RowPadding + RowPadding;
    
    private SKPoint GetHeaderTextPosition()
    {
        var pos = Position + new Vector2(Padding, HeaderHeight / 2);
        return new SKPoint(pos.X, pos.Y);
    }

    private List<SlotSketch> GenerateSlots(int count, bool inputSlots = true)
    {
        var x = inputSlots ? Position.X + Padding + SlotSize / 2 : RightCornerPos.X - Padding - SlotSize / 2;
        var y = Position.Y + HeaderHeight + RowPadding + SlotSize;
        var slots = new List<SlotSketch>();
        var labelCounter = 0;
        var labelChar = inputSlots ? 'X' : 'Y';
        while (count-- > 0)
        {
            var label = $"{labelChar}{labelCounter++}"; 
            var dir = inputSlots ? SlotSketch.LabelLocation.Right : SlotSketch.LabelLocation.Left;
            var slot = new SlotSketch(label, new Vector2(x, y), dir);
            slots.Add(slot);
            y += RowHeight + RowGap;
        }
        return slots;
    }
    
}