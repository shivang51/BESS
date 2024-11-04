using System;
using System.Collections.ObjectModel;
using System.Linq;
using Bess.Models.ComponentExplorer;
using CommunityToolkit.Mvvm.ComponentModel;
using Dock.Model.Controls;
using Dock.Model.Core;

// ReSharper disable MemberCanBePrivate.Global
// ReSharper disable InconsistentNaming

namespace Bess.ViewModels;

public partial class MainWindowViewModel : ViewModelBase
{
    public MainWindowViewModel()
    {
        var factory = new DockFactory();

        Layout = factory.CreateLayout();
        if (Layout is { })
        {
            factory?.InitLayout(Layout);
        }
    }
    
    public IRootDock? Layout { get; }

    public ObservableCollection<AddedComponent> AddedComponents { get; } = [];

    public void AddComponent(ComponentModel? componentModel)
    {
        if (componentModel == null) return;
        
        var addedComponent = componentModel.Generate();
        AddedComponents.Add(addedComponent);
    }
    
    public void CloseLayout()
    {
        if (Layout is IDock dock)
        {
            if (dock.Close.CanExecute(null))
            {
                dock.Close.Execute(null);
            }
        }
    }

}