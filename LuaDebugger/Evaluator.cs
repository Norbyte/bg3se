using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NSE.DebuggerFrontend
{
    public class BackendVariableReference
    {
        public class Key
        {
            public long? Index;
            public string String;
        }

        public DbgContext Context;
        public int VariableRef;
        public int Frame;
        public int Local;
        public List<Key> Keys;

        public static BackendVariableReference FromDbg(DbgContext context, MsgVariablesRef dbgRef)
        {
            var ret = new BackendVariableReference
            {
                Context = context,
                VariableRef = dbgRef.VariableRef,
                Frame = dbgRef.Frame,
                Local = dbgRef.Local,
                Keys = new List<Key>()
            };

            if (dbgRef.Key != null)
            {
                foreach (var key in dbgRef.Key)
                {
                    if (key.Type == MsgTableKey.Types.Type.Numeric)
                    {
                        ret.Keys.Add(new Key
                        {
                            Index = key.Index
                        });
                    }
                    else
                    {
                        ret.Keys.Add(new Key
                        {
                            String = key.Key
                        });
                    }
                }
            }

            return ret;
        }
    }


    class ExpressionEvaluator
    {
        private DAPMessageHandler DAP;

        public delegate void EvalRequestHandlerDelegate(uint seq, StatusCode status, BkEvaluateResponse response);
        public delegate void GetVariablesRequestHandlerDelegate(uint seq, StatusCode status, BkGetVariablesResponse response);

        // List of DAP evaluation/variable fetch requests that are currently pending
        // (i.e. we're waiting for a debugger backend response).
        // Backend sequence ID => original DAP request map
        private Dictionary<uint, EvalRequestHandlerDelegate> PendingEvalRequests = new Dictionary<uint, EvalRequestHandlerDelegate>();
        private Dictionary<uint, GetVariablesRequestHandlerDelegate> PendingGetVariablesRequests = new Dictionary<uint, GetVariablesRequestHandlerDelegate>();

        // DAP variable references
        private Dictionary<int, BackendVariableReference> VariableRefs = new Dictionary<int, BackendVariableReference>();
        private int NextVariableReference = 1;

        public ExpressionEvaluator(DAPMessageHandler msgHandler)
        {
            DAP = msgHandler;
        }

        private int RefToVariableType(long vref)
        {
            return (int)(vref >> 48);
        }

        private int RefToThreadId(long vref)
        {
            return (int)((vref >> 46) & 3);
        }

        private int RefToVariableIndex(long vref)
        {
            return (int)(vref & 0x7fffffff);
        }

        private int RefToFrameIndex(long vref)
        {
            return (int)((vref >> 4) & 0xffff);
        }

        private int RefToScopeIndex(long vref)
        {
            return (int)(vref & 0xf);
        }

        public long MakeStackRef(int threadId, int frameIndex, int scopeIndex)
        {
            return ((long)1 << 48) | ((long)threadId << 46) | ((long)frameIndex << 4) | (long)scopeIndex;
        }

        public long MakeVariableRef(int backendVarIndex)
        {
            return ((long)0 << 48) | (long)backendVarIndex;
        }

        private string DbgValueToString(MsgValue value)
        {
            switch (value.TypeId)
            {
                case MsgValueType.Nil:
                    return "nil";

                case MsgValueType.Boolean:
                    return value.Boolval ? "true" : "false";

                case MsgValueType.Integer:
                    return value.Intval.ToString();

                case MsgValueType.Float:
                    return value.Floatval.ToString();

                case MsgValueType.String:
                    return '"' + value.Stringval + '"';

                case MsgValueType.Table:
                    return "(table)";

                case MsgValueType.Function:
                    return "(function)";

                case MsgValueType.Userdata:
                    return value.Stringval;

                case MsgValueType.Thread:
                    return "(thread)";

                case MsgValueType.ComponentHandle:
                    return value.Stringval;

                case MsgValueType.Unknown:
                    return "(unknown)";

                default:
                    return "(???)";
            }
        }

        private void OnVariablesReceived(DAPRequest request, DAPVariablesRequest msg, ThreadState state, BkGetVariablesResponse response)
        {
            int startIndex = msg.start == null ? 0 : (int)msg.start;
            int numVars = (msg.count == null || msg.count == 0) ? response.Result.Count : (int)msg.count;
            int lastIndex = Math.Min(startIndex + numVars, response.Result.Count);
            // TODO req.filter, format

            var variables = new List<DAPVariable>();
            for (var i = startIndex; i < startIndex + numVars; i++)
            {
                var variable = response.Result[i];
                var dapVar = new DAPVariable
                {
                    value = DbgValueToString(variable.Value),
                    type = variable.Value.TypeId.ToString()
                };

                if (variable.Type == MsgChildValue.Types.Type.Numeric)
                {
                    dapVar.name = variable.Index.ToString();
                }
                else
                {
                    dapVar.name = variable.Name;
                }


                if (variable.Value.Variables != null)
                {
                    var varsRef = BackendVariableReference.FromDbg(state.Context, variable.Value.Variables);
                    var varsIdx = NextVariableReference++;
                    VariableRefs.Add(varsIdx, varsRef);
                    dapVar.variablesReference = MakeVariableRef(varsIdx);
                }

                variables.Add(dapVar);
            }

            var reply = new DAPVariablesResponse
            {
                variables = variables
            };
            DAP.Stream.SendReply(request, reply);
        }

        private void RequestStackVariables(DAPRequest request, DAPVariablesRequest msg, int threadId, int frameIndex, int scopeIndex)
        {
            var state = DAP.GetThread(threadId);

            if (!state.Stopped)
            {
                throw new RequestFailedException("Cannot get scopes when thread is not stopped");
            }

            if (frameIndex < 0 || frameIndex >= state.Stack.Count)
            {
                throw new RequestFailedException($"Requested variables for unknown frame {frameIndex}");
            }

            var varRef = new BackendVariableReference();
            varRef.Context = state.Context;
            varRef.VariableRef = -1;
            varRef.Frame = frameIndex;
            varRef.Local = -1;
            uint seq = DAP.DbgCli.SendGetVariables(varRef);
            PendingGetVariablesRequests.Add(seq, (uint replySeq, StatusCode status, BkGetVariablesResponse response) =>
            {
                if (status == StatusCode.Success)
                {
                    // Filter locals/upvalues based on requested scope type
                    var results = response.Result.Where(result => ((result.Index >= 0) == (scopeIndex == 0))).ToList();
                    response.Result.Clear();
                    response.Result.AddRange(results);

                    OnVariablesReceived(request, msg, state, response);
                }
                else if (status == StatusCode.EvalFailed && response != null)
                {
                    DAP.Stream.SendReply(request, response.ErrorMessage);
                }
                else
                {
                    DAP.Stream.SendReply(request, $"Backend returned error: {status}");
                }
            });
        }

        private void RequestBackendVariables(DAPRequest request, DAPVariablesRequest msg, int dapIndex)
        {
            if (!VariableRefs.TryGetValue(dapIndex, out BackendVariableReference varRef))
            {
                throw new RequestFailedException($"Unknown backend variable reference: {dapIndex}");
            }

            var state = DAP.GetThread(varRef.Context);

            if (!state.Stopped)
            {
                throw new RequestFailedException("Cannot fetch variables when thread is not stopped");
            }

            uint seq = DAP.DbgCli.SendGetVariables(varRef);
            PendingGetVariablesRequests.Add(seq, (uint replySeq, StatusCode status, BkGetVariablesResponse response) =>
            {
                if (status == StatusCode.Success)
                {
                    OnVariablesReceived(request, msg, state, response);
                }
                else if (status == StatusCode.EvalFailed && response != null)
                {
                    DAP.Stream.SendReply(request, response.ErrorMessage);
                }
                else
                {
                    DAP.Stream.SendReply(request, $"Backend returned error: {status}");
                }
            });
        }

        public void OnDAPVariablesRequested(DAPRequest request, DAPVariablesRequest msg)
        {
            var variableType = RefToVariableType(msg.variablesReference);

            if (variableType == 0)
            {
                var index = RefToVariableIndex(msg.variablesReference);
                RequestBackendVariables(request, msg, index);
            }
            else if (variableType == 1)
            {
                var threadId = RefToThreadId(msg.variablesReference);
                var frameIndex = RefToFrameIndex(msg.variablesReference);
                var scopeIndex = RefToScopeIndex(msg.variablesReference);
                RequestStackVariables(request, msg, threadId, frameIndex, scopeIndex);
            }
            else
            {
                throw new InvalidOperationException($"Unknown variables reference type: {variableType} (ref: {msg.variablesReference})");
            }
        }

        private void ReceivedEvaluateResponse(DAPRequest request, DAPEvaulateRequest req, ThreadState state, BkEvaluateResponse response)
        {
            DAPEvaluateResponse evalResponse = new DAPEvaluateResponse
            {
                result = "",
                namedVariables = 0,
                indexedVariables = 0,
                variablesReference = 0,
            };

            if (response.Result == null)
            {
                evalResponse.result = "(None)";
            }
            else
            {
                evalResponse.result = DbgValueToString(response.Result);

                if (response.Result.Variables != null)
                {
                    var varsRef = BackendVariableReference.FromDbg(state.Context, response.Result.Variables);
                    var varsIdx = NextVariableReference++;
                    VariableRefs.Add(varsIdx, varsRef);
                    evalResponse.variablesReference = MakeVariableRef(varsIdx);
                }
            }

            DAP.Stream.SendReply(request, evalResponse);
        }

        public void OnDAPEvaluateRequested(DAPRequest request, DAPEvaulateRequest req)
        {
            if (req.frameId == null)
            {
                throw new RequestFailedException("Cannot evaluate expressions in global scope");
            }

            var threadId = req.frameId.Value >> 16;
            var frameIndex = req.frameId.Value & 0xffff;

            var state = DAP.GetThread(threadId);

            if (!state.Stopped)
            {
                throw new RequestFailedException("Can only evaluate expressions when stopped");
            }

            // TODO - evaluate in frame in later versions
            uint seq = DAP.DbgCli.SendEvaluate(state.Context, frameIndex, req.expression);
            PendingEvalRequests.Add(seq, (uint replySeq, StatusCode status, BkEvaluateResponse response) =>
            {
                if (status == StatusCode.Success)
                {
                    ReceivedEvaluateResponse(request, req, state, response);
                }
                else if (status == StatusCode.EvalFailed && response != null)
                {
                    DAP.Stream.SendReply(request, response.ErrorMessage);
                }
                else
                {
                    DAP.Stream.SendReply(request, $"Backend returned error: {status}");
                }
            });
        }

        public void OnEvaluateFinished(UInt32 seq, BkEvaluateResponse msg)
        {
            if (PendingEvalRequests.TryGetValue(seq, out EvalRequestHandlerDelegate handler))
            {
                PendingEvalRequests.Remove(seq);
                if (msg.ErrorMessage.Length > 0)
                {
                    handler(seq, StatusCode.EvalFailed, msg);
                }
                else
                {
                    handler(seq, StatusCode.Success, msg);
                }
            }
            else
            {
                DAP.LogError($"Response received for evaluate request {seq} that is not pending?");
            }
        }

        public void OnGetVariablesFinished(UInt32 seq, BkGetVariablesResponse msg)
        {
            if (PendingGetVariablesRequests.TryGetValue(seq, out GetVariablesRequestHandlerDelegate handler))
            {
                PendingGetVariablesRequests.Remove(seq);
                if (msg.ErrorMessage.Length > 0)
                {
                    handler(seq, StatusCode.EvalFailed, msg);
                }
                else
                {
                    handler(seq, StatusCode.Success, msg);
                }
            }
            else
            {
                DAP.LogError($"Response received for variables request {seq} that is not pending?");
            }
        }

        public void OnResultsReceived(UInt32 seq, BkResult msg)
        {
            if (PendingEvalRequests.TryGetValue(seq, out EvalRequestHandlerDelegate handler))
            {
                PendingEvalRequests.Remove(seq);
                handler(seq, msg.StatusCode, null);
            }

            if (PendingGetVariablesRequests.TryGetValue(seq, out GetVariablesRequestHandlerDelegate varHandler))
            {
                PendingGetVariablesRequests.Remove(seq);
                varHandler(seq, msg.StatusCode, null);
            }
        }
    }
}
