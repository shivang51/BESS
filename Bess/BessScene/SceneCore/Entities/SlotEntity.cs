﻿using System.Numerics;
using BessScene.SceneCore.State.ShadersCollection;
using SkiaSharp;

namespace BessScene.SceneCore.State.Entities;

public abstract class SlotEntity: SceneEntity
{
    public enum LabelLocation
    {
        Left,
        Right,
        Top,
        Bottom
    }

    protected const float SlotDiameter = 6;
    protected const float LabelGap = 4;
    protected const float FontSize = 10;
    public static readonly float SlotSize = Math.Max(SlotDiameter, FontSize);
    
    public bool High { get; set; }
    
    public Vector2 ParentPos = new();

    public Vector2 AbsPosition => ParentPos + Position;
    
    public string Name { get; set; }

    public LabelLocation LabelLoc { get; set; } = LabelLocation.Right;
    
    protected readonly Vector2 LabelOffset;

    public SlotEntity(string name, Vector2 pos, uint parentId, LabelLocation labelLocation = LabelLocation.Right) : base(pos)
    {
        Name = name;
        LabelLoc = labelLocation;
        
        // Calculating label offset
        float xOffset = 0;
        float yOffset = (FontSize - SlotSize) / 2;
        
        switch (LabelLoc)
        {
            case LabelLocation.Left:
                xOffset = -(SlotDiameter / 2) - LabelGap - SkRenderer.CalculateTextWidth(Name, FontSize);
                break;
            case LabelLocation.Right:
                xOffset = (SlotDiameter / 2) + LabelGap;
                break;
            case LabelLocation.Top:
                yOffset = -(SlotDiameter / 2) - LabelGap - FontSize;
                break;
            case LabelLocation.Bottom:
                yOffset = (SlotDiameter / 2) + LabelGap;
                break;
            default:
                throw new ArgumentOutOfRangeException($"{labelLocation}");
        }
        
        LabelOffset = new Vector2(xOffset, yOffset);
        
        SceneState.Instance.AddSlotEntity(this);
    }
    
    public override void Remove()
    {
        foreach(var conn in SceneState.Instance.GetConnectionEntitiesForSlot(RenderId))
        {
            conn.Remove();
        }
        
        SceneState.Instance.RemoveSlotEntity(this);
        SceneState.Instance.ClearConnectionMapForSlotId(RenderId);
    }
    
    protected SKPoint SkLabelOffset => new(LabelOffset.X, LabelOffset.Y);
}