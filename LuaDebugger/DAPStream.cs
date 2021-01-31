using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace NSE.DebuggerFrontend
{
    public class DAPStream
    {
        private Stream Input;
        private StreamReader InputReader;
        private Stream LogStream;

        private Int32 OutgoingSeq = 1;
        private Int32 IncomingSeq = 1;

        public delegate void MessageReceivedDelegate(DAPMessage message);
        public MessageReceivedDelegate MessageReceived = delegate { };

        public DAPStream()
        {
            Input = Console.OpenStandardInput();
            InputReader = new StreamReader(Input, Encoding.UTF8);
        }

        public void EnableLogging(Stream logStream)
        {
            LogStream = logStream;
        }

        private void ProcessPayload(char[] payload)
        {
            if (LogStream != null)
            {
                using (var writer = new StreamWriter(LogStream, Encoding.UTF8, 0x1000, true))
                {
                    writer.Write(" DAP >>> ");
                    writer.Write(payload);
                    writer.Write("\r\n");
                }
            }

            DAPMessage message = null;
            try
            {
                message = DAPMessageSerializer.Unserialize(payload);
            }
            catch (DAPUnknownMessageException e)
            {
                if (LogStream != null)
                {
                    using (var writer = new StreamWriter(LogStream, Encoding.UTF8, 0x1000, true))
                    {
                        writer.WriteLine(" DAP !!! Could not decode DAP message: " + e.Message);
                    }
                }

                if (e.Type == "request")
                {
                    SendErrorReply(e.Seq, e.MessageType, e.Message);
                }

                IncomingSeq++;
                return;
            }
            catch (Exception e)
            {
                if (LogStream != null)
                {
                    using (var writer = new StreamWriter(LogStream, Encoding.UTF8, 0x1000, true))
                    {
                        writer.WriteLine(" DAP !!! Internal decoding error: " + e.ToString());
                    }
                }

                var outputMsg = new DAPOutputMessage
                {
                    category = "stderr",
                    output = e.ToString()
                };
                SendEvent("output", outputMsg);

                IncomingSeq++;
                return;
            }

            if (message.seq != IncomingSeq)
            {
                throw new InvalidDataException($"DAP sequence number mismatch; got {message.seq} expected {IncomingSeq}");
            }

            IncomingSeq++;
            MessageReceived(message);
        }

        public void RunLoop()
        {
            var lineRe = new Regex("^([^:]*):\\s*(.*)$", RegexOptions.Compiled);

            while (true)
            {
                Dictionary<string, string> headers = new Dictionary<string, string>();
                while (true)
                {
                    var line = InputReader.ReadLine();
                    if (line == null && InputReader.EndOfStream)
                    {
                        return;
                    }

                    if (line.Length == 0)
                    {
                        break;
                    }

                    var matches = lineRe.Match(line);
                    if (!matches.Success)
                    {
                        throw new InvalidDataException($"Malformed header line: {line}");
                    }

                    headers.Add(matches.Groups[1].Value, matches.Groups[2].Value);
                    if (matches.Groups[1].Value != "Content-Length")
                    {
                        throw new InvalidDataException($"{matches.Groups[1].Value}={matches.Groups[2].Value}");
                    }
                }

                if (headers.Count == 0) throw new InvalidDataException("Empty headers.");
                
                var length = Int32.Parse(headers["Content-Length"]);
                var payload = new char[length];
                var read = InputReader.Read(payload, 0, length);
                if (read != length)
                {
                    throw new InvalidDataException($"Could not read {length} bytes of payload (got {read})");
                }

                ProcessPayload(payload);
            }
        }

        public void Send(DAPMessage message)
        {
            message.seq = OutgoingSeq++;
            var encoded = DAPMessageSerializer.Serialize(message);

            if (LogStream != null)
            {
                using (var writer = new StreamWriter(LogStream, Encoding.UTF8, 0x1000, true))
                {
                    writer.Write(" DAP <<< ");
                    writer.Write(encoded);
                    writer.Write("\r\n");
                }
            }

            Console.Write($"Content-Length: {encoded.Length}\r\n\r\n");
            Console.Write(encoded);
        }

        private void SendErrorReply(int requestSeq, string command, string errorText)
        {
            var reply = new DAPResponse
            {
                type = "response",
                request_seq = requestSeq,
                success = false,
                command = command,
                message = errorText
            };

            Send(reply);
        }

        public void SendEvent(string command, IDAPMessagePayload body)
        {
            var reply = new DAPEvent
            {
                type = "event",
                @event = command,
                body = body
            };

            Send(reply);
        }

        public void SendReply(DAPRequest request, IDAPMessagePayload response)
        {
            var reply = new DAPResponse
            {
                type = "response",
                request_seq = request.seq,
                success = true,
                command = request.command,
                body = response
            };

            Send(reply);
        }

        public void SendReply(DAPRequest request, string errorText)
        {
            var reply = new DAPResponse
            {
                type = "response",
                request_seq = request.seq,
                success = false,
                command = request.command,
                message = errorText
            };

            Send(reply);
        }
    }
}
