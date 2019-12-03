using System;

namespace Updater
{
    public class UpdateException : Exception
    {
        public UpdateException(string message, params object[] args)
            : base(string.Format(message, args))
        {

        }

        public UpdateException(string message, Exception innerException, params object[] args)
            : base(string.Format(message, args), innerException)
        {

        }
    }
}
