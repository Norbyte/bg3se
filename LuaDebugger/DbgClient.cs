using Google.Protobuf;
using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Security.Cryptography.X509Certificates;
using System.Text;

namespace NSE.DebuggerFrontend
{
    public class AsyncProtobufClient
    {
        private TcpClient Socket;
        private byte[] MessageBuffer;
        private int BufferPos;

        public delegate void MessageReceivedDelegate(BackendToDebugger message);
        public MessageReceivedDelegate MessageReceived = delegate { };

        public AsyncProtobufClient(string host, int port)
        {
            MessageBuffer = new byte[0x100000];
            BufferPos = 0;

            Socket = new TcpClient();
            Socket.Connect(host, port);
        }

        public void RunLoop()
        {
            while (true)
            {
                try
                {
                    int received = Socket.Client.Receive(MessageBuffer, BufferPos, MessageBuffer.Length - BufferPos, SocketFlags.Partial);
                    BufferPos += received;
                }
                catch (SocketException e)
                {
                    throw e;
                }

                while (BufferPos >= 4)
                {
                    Int32 length = MessageBuffer[0]
                        | (MessageBuffer[1] << 8)
                        | (MessageBuffer[2] << 16)
                        | (MessageBuffer[3] << 24);

                    if (length >= 0x100000)
                    {
                        throw new InvalidDataException($"Message too long ({length} bytes)");
                    }

                    if (BufferPos >= length)
                    {
                        using (var stream = new CodedInputStream(MessageBuffer, 4, length - 4))
                        {
                            var message = BackendToDebugger.Parser.ParseFrom(stream);
                            MessageReceived(message);
                        }

                        Array.Copy(MessageBuffer, length, MessageBuffer, 0, BufferPos - length);
                        BufferPos -= length;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        public void Send(DebuggerToBackend message)
        {
            using (var ms = new MemoryStream())
            {
                message.WriteTo(ms);

                var length = ms.Position + 4;
                var lengthBuf = new byte[4];
                lengthBuf[0] = (byte)(length & 0xff);
                lengthBuf[1] = (byte)((length >> 8) & 0xff);
                lengthBuf[2] = (byte)((length >> 16) & 0xff);
                lengthBuf[3] = (byte)((length >> 24) & 0xff);
                Socket.Client.Send(lengthBuf);
                var payload = ms.ToArray();
                Socket.Client.Send(payload);
            }
        }
    }

    public class DebuggerClient
    {
        private AsyncProtobufClient Client;
        private Stream LogStream;
        private UInt32 OutgoingSeq = 1;
        private UInt32 IncomingSeq = 1;

        public delegate void BackendConnectedDelegate(BkConnectResponse response);
        public BackendConnectedDelegate OnBackendConnected = delegate { };

        public delegate void BreakpointTriggeredDelegate(BkBreakpointTriggered bp);
        public BreakpointTriggeredDelegate OnBreakpointTriggered = delegate { };

        public delegate void EvaluateFinishedDelegate(UInt32 seq, BkEvaluateResponse msg);
        public EvaluateFinishedDelegate OnEvaluateFinished = delegate { };

        public delegate void ContextUpdatedDelegate(BkContextUpdated msg);
        public ContextUpdatedDelegate OnContextUpdated = delegate { };

        public delegate void ModInfoDelegate(BkModInfoResponse msg);
        public ModInfoDelegate OnModInfo = delegate { };

        public delegate void DebugOutputDelegate(BkDebugOutput msg);
        public DebugOutputDelegate OnDebugOutput = delegate { };

        public delegate void ResultsDelegate(UInt32 seq, BkResult msg);
        public ResultsDelegate OnResults = delegate { };

        public delegate void DebuggerReadyDelegate(BkDebuggerReady msg);
        public DebuggerReadyDelegate OnDebuggerReady = delegate { };

        public delegate void SourceResponseDelegate(UInt32 seq, BkSourceResponse msg);
        public SourceResponseDelegate OnSourceResponse = delegate { };

        public delegate void GetVariablesFinishedDelegate(UInt32 seq, BkGetVariablesResponse msg);
        public GetVariablesFinishedDelegate OnGetVariablesFinished = delegate { };

        public DebuggerClient(AsyncProtobufClient client)
        {
            Client = client;
            Client.MessageReceived = this.MessageReceived;
        }

        public void EnableLogging(Stream logStream)
        {
            LogStream = logStream;
        }

        private void LogMessage(IMessage message)
        {
            if (LogStream != null)
            {
                using (var writer = new StreamWriter(LogStream, Encoding.UTF8, 0x1000, true))
                {
                    writer.Write(" NSE >>> ");
                    var settings = new JsonFormatter.Settings(true);
                    var formatter = new JsonFormatter(settings);
                    formatter.Format(message, writer);
                    writer.Write("\r\n");
                }
            }
        }

        public UInt32 Send(DebuggerToBackend message)
        {
            message.SeqNo = OutgoingSeq++;
            LogMessage(message);
            Client.Send(message);
            return message.SeqNo;
        }

        public void SendConnectRequest(UInt32 protocolVersion)
        {
            var msg = new DebuggerToBackend
            {
                Connect = new DbgConnectRequest
                {
                    ProtocolVersion = protocolVersion
                }
            };
            Send(msg);
        }

        public void SendSetBreakpoints(IEnumerable<BreakpointInfo> breakpoints)
        {
            var setBps = new DbgSetBreakpoints();
            foreach (var breakpoint in breakpoints)
            {
                var bp = new MsgBreakpoint
                {
                    Path = breakpoint.Path,
                    Line = breakpoint.Line
                };
                setBps.Breakpoint.Add(bp);
            }

            var msg = new DebuggerToBackend
            {
                SetBreakpoints = setBps
            };
            Send(msg);
        }

        public void SendUpdateSettings(bool breakOnError, bool breakOnGenericError)
        {
            var msg = new DebuggerToBackend
            {
                UpdateSettings = new DbgUpdateSettings
                {
                    BreakOnError = breakOnError,
                    BreakOnGenericError = breakOnGenericError
                }
            };
            Send(msg);
        }

        public void SendContinue(DbgContext context, DbgContinue.Types.Action action)
        {
            var msg = new DebuggerToBackend
            {
                Continue = new DbgContinue
                {
                    Context = context,
                    Action = action
                }
            };
            Send(msg);
        }

        public UInt32 SendEvaluate(DbgContext context, int frameIndex, string expression)
        {
            var msg = new DebuggerToBackend
            {
                Evaluate = new DbgEvaluate
                {
                    Context = context,
                    Frame = frameIndex,
                    Expression = expression
                }
            };
            return Send(msg);
        }

        public UInt32 SendGetVariables(BackendVariableReference vref)
        {
            var eval = new DbgGetVariables
            {
                Context = vref.Context,
                VariableRef = vref.VariableRef,
                Frame = vref.Frame,
                Local = vref.Local
            };

            if (vref.Keys != null)
            {
                foreach (var key in vref.Keys)
                {
                    var keyItem = new MsgTableKey();
                    if (key.String != null)
                    {
                        keyItem.Type = MsgTableKey.Types.Type.Text;
                        keyItem.Key = key.String;
                    }
                    else
                    {
                        keyItem.Type = MsgTableKey.Types.Type.Numeric;
                        keyItem.Index = key.Index.Value;
                    }

                    eval.Key.Add(keyItem);
                }
            }

            var msg = new DebuggerToBackend
            {
                GetVariables = eval
            };
            return Send(msg);
        }

        public UInt32 SendSourceRequest(string name)
        {
            var msg = new DebuggerToBackend
            {
                RequestSource = new DbgRequestSource
                {
                    Name = name
                }
            };
            return Send(msg);
        }

        private void BreakpointTriggered(BkBreakpointTriggered message)
        {
            OnBreakpointTriggered(message);
        }

        private void MessageReceived(BackendToDebugger message)
        {
            LogMessage(message);

            if (message.SeqNo != IncomingSeq)
            {
                throw new InvalidDataException($"NSE sequence number mismatch; got {message.SeqNo} expected {IncomingSeq}");
            }

            IncomingSeq++;

            switch (message.MsgCase)
            {
                case BackendToDebugger.MsgOneofCase.ConnectResponse:
                    OnBackendConnected(message.ConnectResponse);
                    break;

                case BackendToDebugger.MsgOneofCase.BreakpointTriggered:
                    BreakpointTriggered(message.BreakpointTriggered);
                    break;

                case BackendToDebugger.MsgOneofCase.EvaluateResponse:
                    OnEvaluateFinished(message.ReplySeqNo, message.EvaluateResponse);
                    break;

                case BackendToDebugger.MsgOneofCase.ContextUpdated:
                    OnContextUpdated(message.ContextUpdated);
                    break;

                case BackendToDebugger.MsgOneofCase.ModInfoResponse:
                    OnModInfo(message.ModInfoResponse);
                    break;

                case BackendToDebugger.MsgOneofCase.DebugOutput:
                    OnDebugOutput(message.DebugOutput);
                    break;

                case BackendToDebugger.MsgOneofCase.Results:
                    OnResults(message.ReplySeqNo, message.Results);
                    break;

                case BackendToDebugger.MsgOneofCase.DebuggerReady:
                    OnDebuggerReady(message.DebuggerReady);
                    break;

                case BackendToDebugger.MsgOneofCase.SourceResponse:
                    OnSourceResponse(message.ReplySeqNo, message.SourceResponse);
                    break;

                case BackendToDebugger.MsgOneofCase.GetVariablesResponse:
                    OnGetVariablesFinished(message.ReplySeqNo, message.GetVariablesResponse);
                    break;

                default:
                    throw new InvalidOperationException($"Unknown message from NSE: {message.MsgCase}");
            }
        }
    }
}