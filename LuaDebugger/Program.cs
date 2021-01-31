using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace NSE.DebuggerFrontend
{
    class Program
    {
        static void Main(string[] args)
        {
            var currentPath = AppDomain.CurrentDomain.BaseDirectory;
            var logFile = new FileStream(currentPath + "\\DAP.log", FileMode.Create);
            var dap = new DAPStream();
            dap.EnableLogging(logFile);
            var dapHandler = new DAPMessageHandler(dap);
            dapHandler.EnableLogging(logFile);
            try
            {
                dap.RunLoop();
            }
            catch (Exception e)
            {
                using (var writer = new StreamWriter(logFile, Encoding.UTF8, 0x1000, true))
                {
                    writer.Write(e.ToString());
                    Console.WriteLine(e.ToString());
                }
            }
        }
    }
}
