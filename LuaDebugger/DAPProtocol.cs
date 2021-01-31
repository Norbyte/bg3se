using System;
using System.Collections.Generic;

namespace NSE.DebuggerFrontend
{
    /**
     * Base class of requests, responses, and events.
     */
    public class DAPMessage
    {
        /**
         * Sequence number.
         */
        public Int32 seq { get; set; }

        /**
         * Message type.
         * Values: 'request', 'response', 'event', etc.
         */
        public String type { get; set; }
    }

    /**
     * A client or debug adapter initiated request.
     */
    public class DAPRequest : DAPMessage
    {
        /**
         * The command to execute.
         */
        public String command { get; set; }

        /**
         * Object containing arguments for the command.
         */
        public IDAPMessagePayload arguments;
    }

    /**
     * A debug adapter initiated event.
     */
    public class DAPEvent : DAPMessage
    {
        /**
         * Type of event.
         */
        public String @event { get; set; }

        /**
         * Event-specific information
         */
        public IDAPMessagePayload body;
    }

    /**
     * Response for a request.
     */
    public class DAPResponse : DAPMessage
    {
        /**
         * Sequence number of the corresponding request.
         */
        public Int32 request_seq { get; set; }

        /**
         * Outcome of the request.
         */
        public bool success { get; set; }

        /**
         * The command requested.
         */
        public String command { get; set; }

        /**
         * Contains error message if success == false.
         */
        public String message { get; set; }
        
        /**
         * Contains request result if success is true and optional error details if success is false.
         */
        public IDAPMessagePayload body;
    }

    public interface IDAPMessagePayload { }

    public class DAPInitializeRequest : IDAPMessagePayload
    {
        /**
         * The ID of the (frontend) client using this adapter.
         */
        public String clientID { get; set; }

        /**
         * The human readable name of the (frontend) client using this adapter.
         */
        public String clientName { get; set; }

        /**
         * The ID of the debug adapter.
         */
        public String adapterID { get; set; }

        /**
         * The ISO-639 locale of the (frontend) client using this adapter, e.g. en-US or de-CH.
         */
        public String locale { get; set; }

        /**
         * If true all line numbers are 1-based (default).
         */
        public bool linesStartAt1 { get; set; }

        /**
         * If true all column numbers are 1-based (default).
         */
        public bool columnsStartAt1 { get; set; }

        /**
         * Determines in what format paths are specified. The default is 'path', which is the native format.
         * Values: 'path', 'uri', etc.
         */
        public String pathFormat { get; set; }

        /**
         * Client supports the optional type attribute for variables.
         */
        public bool supportsVariableType { get; set; }

        /**
         * Client supports the paging of variables.
         */
        public bool supportsVariablePaging { get; set; }

        /**
         * Client supports the runInTerminal request.
         */
        public bool supportsRunInTerminalRequest { get; set; }
    }

    public class DAPCapabilities : IDAPMessagePayload
    {
        /**
         * The debug adapter supports the 'configurationDone' request.
         */
        public bool supportsConfigurationDoneRequest { get; set; }

        /**
         * The debug adapter supports function breakpoints.
         */
        // TODO - public bool supportsFunctionBreakpoints { get; set; }

        /**
         * The debug adapter supports conditional breakpoints.
         */
        // TODO - public bool supportsConditionalBreakpoints { get; set; }

        /**
         * The debug adapter supports breakpoints that break execution after a specified number of hits.
         */
        // TODO - public bool supportsHitConditionalBreakpoints { get; set; }

        /**
         * The debug adapter supports a (side effect free) evaluate request for data hovers.
         */
        public bool supportsEvaluateForHovers { get; set; }

        /**
         * The debug adapter supports setting a variable to a value.
         */
        // TODO - public bool supportsSetVariable { get; set; }

        /**
         * The debug adapter supports the 'setExpression' request.
         */
        // TODO - public bool supportsSetExpression { get; set; }

        /**
         * The debug adapter supports the 'modules' request.
         */
        public bool supportsModulesRequest { get; set; }

        /**
         * The debug adapter supports the 'restart' request. In this case a client should not implement 'restart' by terminating and relaunching the adapter but by calling the RestartRequest.
         */
        // TODO - public bool supportsRestartRequest { get; set; }

        /**
         * The debug adapter supports the 'loadedSources' request.
         */
        public bool supportsLoadedSourcesRequest { get; set; }
    }

    /**
     * Custom configuration class 
     */
    public class DAPCustomConfiguration
    {
        /**
         * Trigger a breakpoint when a Lua error is thrown.
         */
        public bool breakOnError { get; set; }

        /**
         * Trigger a breakpoint when an error-level message is logged in the console
         * and we've entered the Lua state in the current thread (i.e. Lua code is running).
         */
        public bool breakOnGenericError { get; set; }

        /**
         * Omit C++ frames from stack traces
         */
        public bool omitCppFrames { get; set; }
    }

    /**
     * Arguments for ‘launch’ request. Additional attributes are implementation specific.
     */
    public class DAPLaunchRequest : IDAPMessagePayload
    {
        /**
         * If noDebug is true the launch request should launch the program without enabling debugging.
         */
        public bool noDebug { get; set; }

        /**
         * Optional data from the previous, restarted session.
         * The data is sent as the 'restart' attribute of the 'terminated' event.
         * The client should leave the data intact.
         */
        public object __restart { get; set; }

        /**
         * IP address of debugger backend server
         */
        public string backendHost { get; set; }

        /**
         * Port of debugger backend server
         */
        public int backendPort { get; set; }

        /**
         * Additional debugger configuration
         */
        public DAPCustomConfiguration dbgOptions { get; set; }
    }

    /**
     * Response to ‘launch’ request. This is just an acknowledgement, so no body field is required.
     */
    public class DAPLaunchResponse : IDAPMessagePayload
    {
    }

    public class DAPInitializedEvent : IDAPMessagePayload
    {
    }

    /**
     * A Source is a descriptor for source code. It is returned from the debug adapter as part of a 
     * StackFrame and it is used by clients when specifying breakpoints.
     */
    public class DAPSource
    {
        /**
         * The short name of the source. Every source returned from the debug adapter has a name. When sending a source to the debug adapter this name is optional.
         */
        public string name { get; set; }

        /**
         * The path of the source to be shown in the UI. It is only used to locate and load the content of the source if no sourceReference is specified (or its value is 0).
         */
        public string path { get; set; }

        /**
         * If sourceReference > 0 the contents of the source must be retrieved through the SourceRequest (even if a path is specified). A sourceReference is only valid for a session, so it must not be used to persist a source.
         */
        // public int? sourceReference { get; set; }

        /**
         * An optional hint for how to present the source in the UI. A value of 'deemphasize' can be used to indicate that the source is not available or that it is skipped on stepping.
         */
        // public string presentationHint { get; set; }

        /**
         * The (optional) origin of this source: possible values 'internal module', 'inlined content from source map', etc.
         */
        // public string origin { get; set; }

        /**
         * An optional list of sources that are related to this source. These may be the source that generated this source.
         */
        // public IList<DAPSource> sources { get; set; }

        /**
         * Optional data that a debug adapter might want to loop through the client. The client should leave the data intact and persist it across sessions. The client should not interpret the data.
         */
        // public object adapterData { get; set; }
    }

    /**
     * Properties of a breakpoint or logpoint passed to the setBreakpoints request.
     */
    public class DAPSourceBreakpoint
    {
        /**
         * The source line of the breakpoint or logpoint.
         */
        public int line { get; set; }

        /**
         * An optional source column of the breakpoint.
         */
        public int? column { get; set; }

        /**
         * An optional expression for conditional breakpoints.
         */
        public string condition { get; set; }

        /**
         * An optional expression that controls how many hits of the breakpoint are ignored. The backend is expected to interpret the expression as needed.
         */
        public string hitCondition { get; set; }

        /**
         * If this attribute exists and is non-empty, the backend must not 'break' (stop) but log the message instead. Expressions within {} are interpolated.
         */
        public string logMessage { get; set; }
    }

    /**
     * Sets multiple breakpoints for a single source and clears all previous breakpoints in that source.
     * To clear all breakpoint for a source, specify an empty array.
     * When a breakpoint is hit, a ‘stopped’ event (with reason ‘breakpoint’) is generated.
     */
    public class DAPSetBreakpointsRequest : IDAPMessagePayload
    {
        /**
         * The source location of the breakpoints; either 'source.path' or 'source.reference' must be specified.
         */
        public DAPSource source { get; set; }

        /**
         * The code locations of the breakpoints.
         */
        public IList<DAPSourceBreakpoint> breakpoints { get; set; }

        /**
         * A value of true indicates that the underlying source has been modified which results in new breakpoint locations.
         */
        public bool sourceModified { get; set; }
    }

    public class DAPBreakpoint
    {
        /**
         * An optional unique identifier for the breakpoint.
         */
        public int? id { get; set; }

        /**
         * If true breakpoint could be set (but not necessarily at the desired location).
         */
        public bool verified { get; set; }

        /**
         * An optional message about the state of the breakpoint. This is shown to the user and can be used to explain why a breakpoint could not be verified.
         */
        public string message { get; set; }

        /**
         * The source where the breakpoint is located.
         */
        public DAPSource source { get; set; }

        /**
         * The start line of the actual range covered by the breakpoint.
         */
        public int? line { get; set; }

        /**
         * An optional start column of the actual range covered by the breakpoint.
         */
        public int? column { get; set; }

        /**
         * An optional end line of the actual range covered by the breakpoint.
         */
        public int? endLine { get; set; }

        /**
         * An optional end column of the actual range covered by the breakpoint. If no end line is given, then the end column is assumed to be in the start line.
         */
        public int? endColumn { get; set; }
    }

    /**
     * Response to ‘setBreakpoints’ request.
     * Returned is information about each breakpoint created by this request.
     * This includes the actual code location and whether the breakpoint could be verified.
     * The breakpoints returned are in the same order as the elements of the ‘breakpoints’
     * (or the deprecated ‘lines’) array in the arguments.
     */
    public class DAPSetBreakpointsResponse : IDAPMessagePayload
    {
        /**
         * Information about the breakpoints. The array elements are in the same order as the elements of the 'breakpoints' (or the deprecated 'lines') array in the arguments.
         */
        public IList<DAPBreakpoint> breakpoints { get; set; }
    }

    public class DAPEmptyPayload : IDAPMessagePayload
    {
    }

    public class DAPThread
    {
        /**
         * Unique identifier for the thread.
         */
        public int id { get; set; }

        /**
         * A name of the thread.
         */
        public string name { get; set; }
    }

    /**
     * Response to ‘threads’ request.
     * The request retrieves a list of all threads.
     */
    public class DAPThreadsResponse : IDAPMessagePayload
    {
        /**
         * All threads.
         */
        public IList<DAPThread> threads { get; set; }
    }

    /**
     * The event indicates that a thread has started or exited.
     */
    public class DAPThreadEvent : IDAPMessagePayload
    {
        /**
         * The reason for the event.
         * Values: 'started', 'exited', etc.
         */
        public String reason { get; set; }

        /**
         * The identifier of the thread.
         */
        public int threadId { get; set; }
    }

    /**
     * The ‘disconnect’ request is sent from the client to the debug adapter in order to stop debugging. It asks 
     * the debug adapter to disconnect from the debuggee and to terminate the debug adapter. If the debuggee 
     * has been started with the ‘launch’ request, the ‘disconnect’ request terminates the debuggee. If the 
     * ‘attach’ request was used to connect to the debuggee, ‘disconnect’ does not terminate the debuggee. 
     * This behavior can be controlled with the ‘terminateDebuggee’ (if supported by the debug adapter).
     */
    public class DAPDisconnectRequest : IDAPMessagePayload
    {
        /**
         * Indicates whether the debuggee should be terminated when the debugger is disconnected.
         * If unspecified, the debug adapter is free to do whatever it thinks is best.
         * A client can only rely on this attribute being properly honored if a debug adapter returns true for the 'supportTerminateDebuggee' capability.
         */
        public bool? terminateDebuggee { get; set; }
    }

    /**
     * The event indicates that the execution of the debuggee has stopped due to some condition.
     * 
     * This can be caused by a break point previously set, a stepping action has completed, by 
     * executing a debugger statement etc.
     */
    public class DAPStoppedEvent : IDAPMessagePayload
    {
        /**
         * The reason for the event.
         * For backward compatibility this string is shown in the UI if the 'description' attribute is missing (but it must not be translated).
         * Values: 'step', 'breakpoint', 'exception', 'pause', 'entry', 'goto', etc.
         */
        public String reason { get; set; }

        /**
         * The full reason for the event, e.g. 'Paused on exception'. This string is shown in the UI as is and must be translated.
         */
        public String description { get; set; }

        /**
         * The thread which was stopped.
         */
        public int threadId { get; set; }

        /**
         * Additional information. E.g. if reason is 'exception', text contains the exception name. This string is shown in the UI.
         */
        public String text { get; set; }
    }

    /**
     * Provides formatting information for a stack frame.
     */
    public class DAPStackFrameFormat
    {
        /**
         * Displays parameters for the stack frame.
         */
        public bool? parameters { get; set; }

        /**
         * Displays the types of parameters for the stack frame.
         */
        public bool? parameterTypes { get; set; }

        /**
         * Displays the names of parameters for the stack frame.
         */
        public bool? parameterNames { get; set; }

        /**
         * Displays the values of parameters for the stack frame.
         */
        public bool? parameterValues { get; set; }

        /**
         * Displays the line number of the stack frame.
         */
        public bool? line { get; set; }

        /**
         * Displays the module of the stack frame.
         */
        public bool? module { get; set; }

        /**
         * Includes all stack frames, including those the debug adapter might otherwise hide.
         */
        public bool? includeAll { get; set; }
    }

    /**
     * The request returns a stacktrace from the current execution state.
     */
    public class DAPStackFramesRequest : IDAPMessagePayload
    {
        /**
         * Retrieve the stacktrace for this thread.
         */
        public int threadId { get; set; }

        /**
         * The index of the first frame to return; if omitted frames start at 0.
         */
        public int? startFrame { get; set; }

        /**
         * The maximum number of frames to return. If levels is not specified or 0, all frames are returned.
         */
        public int? levels { get; set; }

        /**
         * Specifies details on how to format the stack frames.
         */
        public DAPStackFrameFormat format { get; set; }
    }

    public class DAPStackFrame
    {
        /**
         * An identifier for the stack frame. It must be unique across all threads. This id can be used to retrieve the scopes of the frame with the 'scopesRequest' or to restart the execution of a stackframe.
         */
        public int id { get; set; }

        /**
         * The name of the stack frame, typically a method name.
         */
        public string name { get; set; }

        /**
         * The optional source of the frame.
         */
        public DAPSource source { get; set; }

        /**
         * The line within the file of the frame. If source is null or doesn't exist, line is 0 and must be ignored.
         */
        public int line { get; set; }

        /**
         * The column within the line. If source is null or doesn't exist, column is 0 and must be ignored.
         */
        public int column { get; set; }

        /**
         * An optional end line of the range covered by the stack frame.
         */
        //public int? endLine { get; set; }

        /**
         * An optional end column of the range covered by the stack frame.
         */
        //public int? endColumn { get; set; }

        /**
         * An optional hint for how to present this frame in the UI. 
         * A value of 'label' can be used to indicate that the frame is an artificial frame that is used as a visual 
         * label or separator.  A value of 'subtle' can be used to change the appearance of a frame in a 'subtle' way.
         */
        //public string presentationHint { get; set; }
    }

    /**
     * Response to ‘stackTrace’ request.
     */
    public class DAPStackFramesResponse : IDAPMessagePayload
    {
        /**
         * The frames of the stackframe. If the array has length zero, there are no stackframes available.
         * This means that there is no location information available.
         */
        public IList<DAPStackFrame> stackFrames { get; set; }

        /**
         * The total number of frames available.
         */
        public int? totalFrames { get; set; }
    }

    /**
     * The request returns the variable scopes for a given stackframe ID.
     */
    public class DAPScopesRequest : IDAPMessagePayload
    {
        /**
         * Retrieve the scopes for this stackframe.
         */
        public int frameId { get; set; }
    }

    /**
     * A Scope is a named container for variables. Optionally a scope can map to a 
     * source or a range within a source.
     */
    public class DAPScope
    {
        /**
         * Name of the scope such as 'Arguments', 'Locals'.
         */
        public string name { get; set; }

        /**
         * The variables of this scope can be retrieved by passing the value of variablesReference to the VariablesRequest.
         */
        public long variablesReference { get; set; }

        /**
         * The number of named variables in this scope.
         * The client can use this optional information to present the variables in a paged UI and fetch them in chunks.
         */
        public int? namedVariables { get; set; }

        /**
         * The number of indexed variables in this scope.
         * The client can use this optional information to present the variables in a paged UI and fetch them in chunks.
         */
        public int? indexedVariables { get; set; }

        /**
         * If true, the number of variables in this scope is large or expensive to retrieve.
         */
        public bool expensive { get; set; }

        /**
         * Optional source for this scope.
         */
        public DAPSource source { get; set; }

        /**
         * Optional start line of the range covered by this scope.
         */
        public int? line { get; set; }

        /**
         * Optional start column of the range covered by this scope.
         */
        public int? column { get; set; }

        /**
         * Optional end line of the range covered by this scope.
         */
        public int? endLine { get; set; }

        /**
         * Optional end column of the range covered by this scope.
         */
        public int? endColumn { get; set; }
    }

    /**
     * Response to ‘scopes’ request.
     */
    public class DAPScopesResponse : IDAPMessagePayload
    {
        public IList<DAPScope> scopes { get; set; }
    }

    /**
     * Provides formatting information for a value.
     */
    public class DAPValueFormat
    {
        /**
         * Display the value in hex.
         */
        public bool hex { get; set; }
    }

    /**
     * Retrieves all child variables for the given variable reference.
     * 
     * An optional filter can be used to limit the fetched children to either named or indexed children.
     */
    public class DAPVariablesRequest : IDAPMessagePayload
    {
        /**
         * The Variable reference.
         */
        public long variablesReference { get; set; }

        /**
         * Optional filter to limit the child variables to either named or indexed. If ommited, both types are fetched.
         */
        public string filter { get; set; }

        /**
         * The index of the first variable to return; if omitted children start at 0.
         */
        public int? start { get; set; }

        /**
         * The number of variables to return. If count is missing or 0, all variables are returned.
         */
        public int? count { get; set; }

        /**
         * Specifies details on how to format the Variable values.
         */
        public DAPValueFormat format { get; set; }
    }

    /**
     * Optional properties of a variable that can be used to determine how to render the variable in the UI.
     */
    public class DAPVariablePresentationHint
    {
        /**
         * The kind of variable. Before introducing additional values, try to use the listed values.
         * Values: 
         * 'property': Indicates that the object is a property.
         * 'method': Indicates that the object is a method.
         * 'class': Indicates that the object is a class.
         * 'data': Indicates that the object is data.
         * 'event': Indicates that the object is an event.
         * 'baseClass': Indicates that the object is a base class.
         * 'innerClass': Indicates that the object is an inner class.
         * 'interface': Indicates that the object is an interface.
         * 'mostDerivedClass': Indicates that the object is the most derived class.
         * 'virtual': Indicates that the object is virtual, that means it is a synthetic object introduced by the adapter for rendering purposes, e.g. an index range for large arrays.
         * etc.
         */
        public string kind { get; set; }

        /**
         * Set of attributes represented as an array of strings. Before introducing additional values, try to use the listed values.
         * Values: 
         * 'static': Indicates that the object is static.
         * 'constant': Indicates that the object is a constant.
         * 'readOnly': Indicates that the object is read only.
         * 'rawString': Indicates that the object is a raw string.
         * 'hasObjectId': Indicates that the object can have an Object ID created for it.
         * 'canHaveObjectId': Indicates that the object has an Object ID associated with it.
         * 'hasSideEffects': Indicates that the evaluation had side effects.
         * etc.
         */
        public IList<string> attributes { get; set; }

        /**
         * Visibility of variable. Before introducing additional values, try to use the listed values.
         * Values: 'public', 'private', 'protected', 'internal', 'final', etc.
         */
        public string visibility { get; set; }
    }

    /**
     * A Variable is a name/value pair.
     * 
     * Optionally a variable can have a ‘type’ that is shown if space permits or when hovering over 
     * the variable’s name.
     * 
     * An optional ‘kind’ is used to render additional properties of the variable, e.g. different icons 
     * can be used to indicate that a variable is public or private.
     * 
     * If the value is structured (has children), a handle is provided to retrieve the children with the VariablesRequest.
     * 
     * If the number of named or indexed children is large, the numbers should be returned via the optional 
     * ‘namedVariables’ and ‘indexedVariables’ attributes.
     */
    public class DAPVariable
    {
        /**
         * The variable's name.
         */
        public string name { get; set; }

        /**
         * The variable's value. This can be a multi-line text, e.g. for a function the body of a function.
         */
        public string value { get; set; }

        /**
         * The type of the variable's value. Typically shown in the UI when hovering over the value.
         */
        public string type { get; set; }

        /**
         * Properties of a variable that can be used to determine how to render the variable in the UI.
         */
        // public DAPVariablePresentationHint presentationHint { get; set; }

        /**
         * Optional evaluatable name of this variable which can be passed to the 'EvaluateRequest' to fetch the variable's value.
         */
        // public string evaluateName { get; set; }

        /**
         * If variablesReference is > 0, the variable is structured and its children can be retrieved by passing variablesReference to the VariablesRequest.
         */
        public long variablesReference { get; set; }

        /**
         * The number of named child variables.
         * The client can use this optional information to present the children in a paged UI and fetch them in chunks.
         */
        public int? namedVariables { get; set; }

        /**
         * The number of indexed child variables.
         * The client can use this optional information to present the children in a paged UI and fetch them in chunks.
         */
        public int? indexedVariables { get; set; }
    }

    /**
     * Response to ‘variables’ request.
     */
    public class DAPVariablesResponse : IDAPMessagePayload
    {
        public IList<DAPVariable> variables { get; set; }
    }

    /**
     * The request starts the debuggee to run again.
     */
    public class DAPContinueRequest : IDAPMessagePayload
    {
        /**
         * Continue execution for the specified thread (if possible). If the backend cannot continue on a single thread 
         * but will continue on all threads, it should set the 'allThreadsContinued' attribute in the response to true.
         */
        public int threadId { get; set; }
    }

    /**
     * Response to ‘continue’ request.
     */
    public class DAPContinueResponse : IDAPMessagePayload
    {
        /**
         * If true, the 'continue' request has ignored the specified thread and continued all threads instead. 
         * If this attribute is missing a value of 'true' is assumed for backward compatibility.
         */
        public bool allThreadsContinued { get; set; }
    }

    /**
     * The event indicates that the target has produced some output.
     */
    public class DAPOutputMessage : IDAPMessagePayload
    {
        /**
         * The output category. If not specified, 'console' is assumed.
         * Values: 'console', 'stdout', 'stderr', 'telemetry', etc.
         */
        public string category { get; set; }

        /**
         * The output to report.
         */
        public string output { get; set; }
    }

    /**
     * The event indicates that debugging of the debuggee has terminated. 
     * This does not mean that the debuggee itself has exited.
     */
    public class DAPTerminatedEvent : IDAPMessagePayload
    {
        /**
         * A debug adapter may set 'restart' to true (or to an arbitrary object) to request that the front end restarts the session.
         * The value is not interpreted by the client and passed unmodified as an attribute '__restart' to the 'launch' and 'attach' requests.
         */
        // public object restart { get; set; };
    }

    /**
     * The event indicates that some information about a breakpoint has changed.
     */
    public class DAPBreakpointEvent : IDAPMessagePayload
    {
        /**
         * The reason for the event.
         * Values: 'changed', 'new', 'removed', etc.
         */
        public String reason { get; set; }

        /**
         * The breakpoint.
         */
        public DAPBreakpoint breakpoint;
    }

    /**
     * Modules can be retrieved from the debug adapter with this request which can either 
     * return all modules or a range of modules to support paging.
     * 
     * Clients should only call this request if the capability ‘supportsModulesRequest’ is true.
     */
    public class DAPModulesRequest : IDAPMessagePayload
    {
        /**
         * The index of the first module to return; if omitted modules start at 0.
         */
        public int startModule { get; set; }

        /**
         * The number of modules to return. If moduleCount is not specified or 0, all modules are returned.
         */
        public int moduleCount { get; set; }
    }

    /**
     * A Module object represents a row in the modules view.
     */
    public class DAPModule
    {
        /**
         * Unique identifier for the module.
         */
        public string id { get; set; }

        /**
         * A name of the module.
         */
        public string name { get; set; }

        /**
         * Logical full path to the module. The exact definition is implementation defined, but usually 
         * this would be a full path to the on-disk file for the module.
         */
        public string path { get; set; }
    }

    /**
     * Response to ‘modules’ request.
     */
    public class DAPModulesResponse : IDAPMessagePayload
    {
        /**
         * All modules or range of modules.
         */
        public IList<DAPModule> modules { get; set; }

        /**
         * The total number of modules available.
         */
        public int totalModules { get; set; }
    }

    /**
     * Retrieves the set of all sources currently loaded by the debugged process.
     * 
     * Clients should only call this request if the capability ‘supportsLoadedSourcesRequest’ is true.
     */
    public class DAPLoadedSourcesRequest : IDAPMessagePayload
    {
    }

    /**
     * Response to ‘loadedSources’ request.
     */
    public class DAPLoadedSourcesResponse : IDAPMessagePayload
    {
        /**
         * Set of loaded sources.
         */
        public IList<DAPSource> sources { get; set; }
    }

    /**
     * The request retrieves the source code for a given source reference.
     */
    public class DAPSourceRequest : IDAPMessagePayload
    {
        /**
         * Specifies the source content to load. Either source.path or source.sourceReference must be specified.
         */
        public DAPSource source { get; set; }

        /**
         * The reference to the source. This is the same as source.sourceReference.
         * This is provided for backward compatibility since old backends do not understand the 'source' attribute.
         */
        public int sourceReference { get; set; }
    }

    /**
     * Response to ‘source’ request.
     */
    public class DAPSourceResponse : IDAPMessagePayload
    {
        /**
         * Content of the source reference.
         */
        public string content { get; set; }

        /**
         * Optional content type (mime type) of the source.
         */
        public string mimeType { get; set; }
    }

    /**
     * Evaluates the given expression in the context of the top most stack frame.
     * The expression has access to any variables and arguments that are in scope.
     */
    public class DAPEvaulateRequest : IDAPMessagePayload
    {
        /**
         * The expression to evaluate.
         */
        public string expression { get; set; }

        /**
         * Evaluate the expression in the scope of this stack frame. If not specified, the expression is evaluated in the global scope.
         */
        public int? frameId { get; set; }
        
        /**
         * The context in which the evaluate request is run.
         * Values: 
         * 'watch': evaluate is run in a watch.
         * 'repl': evaluate is run from REPL console.
         * 'hover': evaluate is run from a data hover.
         * etc.
         */
        public string context { get; set; }
        
        /**
         * Specifies details on how to format the Evaluate result.
         */
        public DAPValueFormat format { get; set; }
    }

    /**
     * Response to ‘evaluate’ request.
     */
    public class DAPEvaluateResponse : IDAPMessagePayload
    {
        /**
         * The result of the evaluate request.
         */
        public string result { get; set; }
        
        /**
         * The optional type of the evaluate result.
         */
        public string type { get; set; }
        
        /**
         * Properties of a evaluate result that can be used to determine how to render the result in the UI.
         */
        public DAPVariablePresentationHint presentationHint { get; set; }
        
        /**
         * If variablesReference is > 0, the evaluate result is structured and its children can be retrieved by passing variablesReference to the VariablesRequest.
         */
        public long variablesReference { get; set; }
        
        /**
         * The number of named child variables.
         * The client can use this optional information to present the variables in a paged UI and fetch them in chunks.
         */
        public int? namedVariables { get; set; }
        
        /**
         * The number of indexed child variables.
         * The client can use this optional information to present the variables in a paged UI and fetch them in chunks.
         */
        public int? indexedVariables { get; set; }
    }

    /**
     * The event transmits the output of the last div query
     */
    public class DAPCustomVersionInfoEvent : IDAPMessagePayload
    {
        /**
         * DAP protocol version
         */
        public int version;
    }

    /**
     * The event transmits the output of the last div query
     */
    public class DAPCustomQueryResultEvent : IDAPMessagePayload
    {
        /**
         * Did the query succeed?
         */
        public bool succeeded;
    }
}
