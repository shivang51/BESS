import 'package:bess/pages/main_page/main_page.dart';
import 'package:fluent_ui/fluent_ui.dart';

class Application extends StatelessWidget {
  const Application({super.key});

  @override
  Widget build(BuildContext context) {
    return FluentApp(
      title: 'Bess',
      initialRoute: MainPage.route,
      darkTheme: FluentThemeData.dark(),
      themeMode: ThemeMode.dark,
      routes: {
        MainPage.route: (context) => const MainPage(),
      },
    );
  }
}
