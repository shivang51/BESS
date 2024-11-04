using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Data.Core;
using Avalonia.Data.Core.Plugins;
using Avalonia.Markup.Xaml;
using Bess.ViewModels;
using Bess.Views;

namespace Bess;

public partial class App : Application
{
    public override void Initialize()
    {
        AvaloniaXamlLoader.Load(this);
    }

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            // Line below is needed to remove Avalonia data validation.
            // Without this line you will get duplicate validations from both Avalonia and CT
            BindingPlugins.DataValidators.RemoveAt(0);
            desktop.MainWindow = new MainWindow
            {
                DataContext = new MainWindowViewModel(),
            };
        }
        
        var mainWindowViewModel = new MainWindowViewModel();

        switch (ApplicationLifetime)
        {
            case IClassicDesktopStyleApplicationLifetime desktopLifetime:
            {
                var mainWindow = new MainWindow
                {
                    DataContext = mainWindowViewModel
                };

                mainWindow.Closing += (_, _) =>
                {
                    mainWindowViewModel.CloseLayout();
                };

                desktopLifetime.MainWindow = mainWindow;

                desktopLifetime.Exit += (_, _) =>
                {
                    mainWindowViewModel.CloseLayout();
                };

                break;
            }
            case ISingleViewApplicationLifetime singleViewLifetime:
            {
                var mainView = new MainWindow()
                {
                    DataContext = mainWindowViewModel
                };

                singleViewLifetime.MainView = mainView;

                break;
            }
        }


        base.OnFrameworkInitializationCompleted();
    }
    
    
}