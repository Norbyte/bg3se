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
            var dap = new DAPStream();
            FileStream logFile = null;

            try
            {
                logFile = new FileStream(currentPath + "\\DAP.log", FileMode.Create, FileAccess.ReadWrite, FileShare.ReadWrite);
                dap.EnableLogging(logFile);

                var dapHandler = new DAPMessageHandler(dap);
                dapHandler.EnableLogging(logFile);
                dap.RunLoop();
            }
            catch (Exception e)
            {
                dap.SendEvent("output", new DAPOutputMessage
                {
                    category = "important",
                    output = e.ToString()
                });
                dap.SendErrorReply(1, "initialize", "Internal error during initialization:\r\n" + e.ToString());

                if (logFile != null)
                {
                    using (var writer = new StreamWriter(logFile, Encoding.UTF8, 0x1000, true))
                    {
                        writer.Write(e.ToString());
                    }
                }
            }
        }
    }
}
