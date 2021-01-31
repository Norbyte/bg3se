using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NSE.DebuggerFrontend
{
    public class RequestFailedException : Exception
    {
        public RequestFailedException(string message)
            : base(message)
        {
        }
    }

    public class DAPJSONTypeLookahead : JsonConverter
    {
        public override bool CanConvert(Type objectType)
        {
            return objectType.Equals(typeof(DAPMessageTypeHint));
        }

        public override object ReadJson(JsonReader reader, Type objectType, object existingValue, JsonSerializer serializer)
        {
            var type = new DAPMessageTypeHint();
            int depth = 0;
            while (type.type == null || type.message == null || type.seq == 0)
            {
                while (reader.Read() && reader.TokenType != JsonToken.PropertyName)
                {
                    if (reader.TokenType == JsonToken.StartObject) depth++;
                    else if (reader.TokenType == JsonToken.EndObject) depth--;
                }

                if (reader.TokenType != JsonToken.PropertyName)
                {
                    throw new InvalidDataException("Could not get property name in DAP payload");
                }

                if (depth == 0)
                {
                    var propertyName = (string)reader.Value;
                    if (propertyName == "seq")
                    {
                        if (!reader.Read() || reader.TokenType != JsonToken.Integer)
                        {
                            throw new InvalidDataException("Could not get sequence number in DAP payload");
                        }
                        
                        type.seq = (int)(Int64)reader.Value;
                    }
                    else if (propertyName == "type")
                    {
                        if (!reader.Read() || reader.TokenType != JsonToken.String)
                        {
                            throw new InvalidDataException("Could not get property value in DAP payload");
                        }

                        type.type = (string)reader.Value;
                    }
                    else if (propertyName == "command" || propertyName == "event")
                    {
                        if (!reader.Read() || reader.TokenType != JsonToken.String)
                        {
                            throw new InvalidDataException("Could not get property value in DAP payload");
                        }

                        type.message = (string)reader.Value;
                    }
                }
            }

            return type;
        }

        public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
        {
            throw new NotImplementedException();
        }
    }

    public class DAPMessageTypeHint
    {
        public String type;
        public String message;
        public int seq;
    }
    
    public class DAPUnknownMessageException : Exception
    {
        public String Type { get; }
        public String MessageType { get; }
        public int Seq { get; }

        public DAPUnknownMessageException(string type, string messageType, int seq)
            : base($"Unknown message type: {type}, {messageType}")
        {
            Type = type;
            MessageType = messageType;
            Seq = seq;
        }
    }

    public class DAPJSONMessageConverter : JsonConverter
    {
        private DAPMessageTypeHint TypeHint;

        public DAPJSONMessageConverter(DAPMessageTypeHint typeHint)
        {
            TypeHint = typeHint;
        }

        public override bool CanConvert(Type objectType)
        {
            return objectType.Equals(typeof(DAPMessage)) || objectType.Equals(typeof(IDAPMessagePayload));
        }

        public override object ReadJson(JsonReader reader, Type objectType, object existingValue, JsonSerializer serializer)
        {
            if (objectType.Equals(typeof(DAPMessage)))
            {
                switch (TypeHint.type)
                {
                    case "request": return serializer.Deserialize<DAPRequest>(reader);
                    case "response": return serializer.Deserialize<DAPResponse>(reader);
                    case "event": return serializer.Deserialize<DAPEvent>(reader);
                    default: throw new InvalidDataException($"Unknown message type: {TypeHint.type}");
                }
            }
            else
            {
                switch (TypeHint.message)
                {
                    case "initialize": return serializer.Deserialize<DAPInitializeRequest>(reader);
                    case "launch": return serializer.Deserialize<DAPLaunchRequest>(reader);
                    case "setBreakpoints": return serializer.Deserialize<DAPSetBreakpointsRequest>(reader);
                    case "configurationDone": return serializer.Deserialize<DAPEmptyPayload>(reader);
                    case "threads": return serializer.Deserialize<DAPEmptyPayload>(reader);
                    case "disconnect": return serializer.Deserialize<DAPDisconnectRequest>(reader);
                    case "stackTrace": return serializer.Deserialize<DAPStackFramesRequest>(reader);
                    case "variables": return serializer.Deserialize<DAPVariablesRequest>(reader);
                    case "scopes": return serializer.Deserialize<DAPScopesRequest>(reader);
                    case "continue": return serializer.Deserialize<DAPContinueRequest>(reader);
                    case "next": return serializer.Deserialize<DAPContinueRequest>(reader);
                    case "stepIn": return serializer.Deserialize<DAPContinueRequest>(reader);
                    case "stepOut": return serializer.Deserialize<DAPContinueRequest>(reader);
                    case "pause": return serializer.Deserialize<DAPContinueRequest>(reader);
                    case "evaluate": return serializer.Deserialize<DAPEvaulateRequest>(reader);
                    case "source": return serializer.Deserialize<DAPSourceRequest>(reader);
                    default: throw new DAPUnknownMessageException(TypeHint.type, TypeHint.message, TypeHint.seq);
                }
            }
        }

        public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
        {
            throw new NotImplementedException();
        }
    }

    class DAPMessageSerializer
    {
        public static DAPMessage Unserialize(char[] payload)
        {
            DAPMessageTypeHint typeHint;
            DAPMessage message;

            var bytes = Encoding.UTF8.GetBytes(payload);
            using (var ms = new MemoryStream(bytes))
            {
                using (var textReader = new StreamReader(ms, Encoding.UTF8, false, 0x100, true))
                using (var jsonReader = new JsonTextReader(textReader))
                {
                    var hintSerializer = new JsonSerializer();
                    hintSerializer.Converters.Add(new DAPJSONTypeLookahead());
                    typeHint = hintSerializer.Deserialize<DAPMessageTypeHint>(jsonReader);
                }

                using (var textReader = new StreamReader(ms))
                using (var jsonReader = new JsonTextReader(textReader))
                {
                    ms.Position = 0;
                    var serializer = new JsonSerializer();
                    serializer.Converters.Add(new DAPJSONMessageConverter(typeHint));
                    message = serializer.Deserialize<DAPMessage>(jsonReader);
                }
            }
            return message;
        }

        public static char[] Serialize(DAPMessage message)
        {
            using (var ms = new MemoryStream())
            {
                using (var textWriter = new StreamWriter(ms))
                using (var jsonWriter = new JsonTextWriter(textWriter))
                {
                    var serializer = new JsonSerializer();
                    serializer.Serialize(jsonWriter, message);
                }

                var bytes = ms.ToArray();
                return Encoding.UTF8.GetChars(bytes);
            }
        }
    }
}
