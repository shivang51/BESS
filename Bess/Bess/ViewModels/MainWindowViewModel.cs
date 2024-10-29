using System;

namespace Bess.ViewModels;

public partial class MainWindowViewModel : ViewModelBase
{
#pragma warning disable CA1822 // Mark members as static
    public string Greeting => "Welcome to Avalonia!";
    
    public void OnClick()
    {
        Console.WriteLine("Yay click works");
    }
    
    
#pragma warning restore CA1822 // Mark members as static
}