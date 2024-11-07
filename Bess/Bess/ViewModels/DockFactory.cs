using System;
using System.Collections.Generic;
using Bess.ViewModels.MainPageViewModels;
using Dock.Model;
using Dock.Avalonia.Controls;
using Dock.Model.Controls;
using Dock.Model.Core;
using Dock.Model.Mvvm;
using Dock.Model.Mvvm.Controls;

namespace Bess.ViewModels;
public class DockFactory: Factory
{
    
    private IRootDock? _rootDock;
    private Tool? _componentExplorer;
    private Tool? _projectExplorer;
    private Tool? _sceneView;
    public DockFactory()
    {
    }
    
    public override IRootDock CreateLayout()
    {
        var componentExplorerViewModel = new ComponentExplorerViewModel()
        {
            Id="ComponentExplorer",
            Title="Component Explorer",
        };
        
        var projectExplorerViewModel = new ProjectExplorerViewModel()
        {
            Id="ProjectExplorer",
            Title="Project Explorer",
        };
        
        var sceneViewModel = new BessSceneViewModel()
        {
            Id="Scene",
            Title="Scene",
        };
        
        var leftDock = new ProportionalDock
        {
            Proportion = 0.2,
            Orientation = Orientation.Vertical,
            ActiveDockable = null,
            VisibleDockables = [
                new ToolDock
                {
                    ActiveDockable = componentExplorerViewModel,
                    VisibleDockables = [
                        componentExplorerViewModel
                    ],
                    GripMode = GripMode.Visible
                },
            ]
        };
        
        var rightDock = new ProportionalDock
        {
            Proportion = 0.2,
            Orientation = Orientation.Vertical,
            ActiveDockable = null,
            VisibleDockables = [
                new ToolDock
                {
                    ActiveDockable = projectExplorerViewModel,
                    VisibleDockables = [
                        projectExplorerViewModel
                    ],
                    GripMode = GripMode.Visible
                },
            ]
        };
        
        var windowLayoutContent = new ProportionalDock
        {
            Orientation = Orientation.Horizontal,
            IsCollapsable = false,
            VisibleDockables = CreateList<IDockable>
            (
                leftDock,
                new ProportionalDockSplitter(),
                new ToolDock
                {
                    ActiveDockable = null,
                    IsActive = true,
                    IsCollapsable = false,
                    CanFloat = false,
                    CanClose = false,
                    CanPin = false,
                    Dock = DockMode.Center,
                    GripMode = GripMode.Hidden,
                    VisibleDockables = CreateList<IDockable>
                    (
                        sceneViewModel
                    )
                },
                new ProportionalDockSplitter(),
                rightDock
            )
        };
        
        
        var rootDock = CreateRootDock();
        rootDock.IsCollapsable = false;
        rootDock.VisibleDockables = CreateList<IDockable>(windowLayoutContent);
        rootDock.ActiveDockable = windowLayoutContent;
        rootDock.DefaultDockable = windowLayoutContent;

        _rootDock = rootDock;
        _componentExplorer = componentExplorerViewModel;
        _projectExplorer = projectExplorerViewModel;
        _sceneView = sceneViewModel;

        return _rootDock;
    }


    public override void InitLayout(IDockable layout)
    {
        
        ContextLocator = new Dictionary<string, Func<object?>>
        {
            ["ComponentExplorer"] = () => layout,
            ["ProjectExplorer"] = () => layout,
            ["Scene"] = () => layout
        };

        DockableLocator = new Dictionary<string, Func<IDockable?>>
        {
            ["Root"] = () => _rootDock,
            ["ComponentExplorer"] = () => _componentExplorer,
            ["ProjectExplorer"] = () => _projectExplorer,
            ["Scene"] = () => _sceneView
        };
        
        HostWindowLocator = new Dictionary<string, Func<IHostWindow?>>
        {
            [nameof(IDockWindow)] = () => new HostWindow()
        };

        base.InitLayout(layout);
    }
}