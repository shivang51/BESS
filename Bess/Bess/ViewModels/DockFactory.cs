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
                sceneViewModel,
                new ProportionalDockSplitter(),
                rightDock
            )
        };
        
        var windowLayout = CreateRootDock();
        windowLayout.Title = "Default";
        windowLayout.IsCollapsable = false;
        windowLayout.VisibleDockables = CreateList<IDockable>(windowLayoutContent);
        windowLayout.ActiveDockable = windowLayoutContent;
        
        var rootDock = CreateRootDock();
        rootDock.IsCollapsable = false;
        rootDock.VisibleDockables = CreateList<IDockable>(windowLayout);
        rootDock.ActiveDockable = windowLayout;
        rootDock.DefaultDockable = windowLayout;

        _rootDock = rootDock;
        _componentExplorer = componentExplorerViewModel;
        _projectExplorer = projectExplorerViewModel;

        return _rootDock;
    }


    public override void InitLayout(IDockable layout)
    {
        
        ContextLocator = new Dictionary<string, Func<object?>>
        {
            ["ComponentExplorer"] = () => layout,
            ["ProjectExplorer"] = () => layout
        };

        DockableLocator = new Dictionary<string, Func<IDockable?>>
        {
            ["Root"] = () => _rootDock,
            ["ComponentExplorer"] = () => _componentExplorer,
            ["ProjectExplorer"] = () => _projectExplorer
        };
        
        HostWindowLocator = new Dictionary<string, Func<IHostWindow?>>
        {
            [nameof(IDockWindow)] = () => new HostWindow()
        };

        base.InitLayout(layout);
    }
}