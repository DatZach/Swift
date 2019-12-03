using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Updater
{
    static class Program
    {
        [STAThread]
        public static void Main(string[] args)
        {
            if (args.Length > 0 && args[0] == "--check-only")
            {
                RunCheckOnly();
                return;
            }

            RunGui();
        }

        [STAThread]
        public static void RunGui()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new FormUpdater());
        }

        [STAThread]
        private static void RunCheckOnly()
        {
            bool updateAvailable = FormUpdater.IsUpdateAvailable();
            Console.WriteLine(updateAvailable.ToString().ToLower());
        }
    }
}
