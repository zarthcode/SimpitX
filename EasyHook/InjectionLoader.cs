/*
Copyright (c) 2008 by Christoph Husse, SecurityRevolutions e.K.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Visit http://www.codeplex.com/easyhook for more information.
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace EasyHook
{
    public class InjectionLoader
    {
        static List<String> ConnectedChannels = new List<String>();

        private static void Release(Type InEntryPoint)
        {
            if (InEntryPoint == null)
                return;

            LocalHook.Release();
        }

        public static int Main(String InParam)
        {
            HelperServiceInterface Interface;
            String[] Params;
            String HostChannel;
            Assembly UserAssembly = null;
            Type EntryPoint = null;

            if (InParam == null)
                return -1;

            /*
             * We will now connect to our hooking host. This is to provide extensive
             * error information.
             */
            try
            {
                Params = InParam.Split('|');

                if (Params.Length != 4)
                    return -1;

                HostChannel = Params[0];
                
                Interface = RemoteHooking.IpcConnectClient<HelperServiceInterface>(HostChannel);

                // ensure connection...
                Interface.Ping();

                if (!ConnectedChannels.Contains(HostChannel))
                {
                    ConnectedChannels.Add(HostChannel);

                    return 0;
                }
            }
            catch(Exception ExtInfo)
            {
                Config.PrintError(ExtInfo.ToString());

                return -2;
            }

            try
            {
                /*
                 * After this we are ready to load the user supplied library.
                 */
                String UserLibrary = Params[1];
                String InfoPtr = Params[3];
                List<Object> UserParams = new List<object>();
                Object Instance = null;

                RemoteHooking.SetWakeUpThreadID(Int32.Parse(Params[2]));

                try
                {
                    UserAssembly = Assembly.Load(UserLibrary);

                    // search for user library entry point...
                    Type[] ExportedTypes = UserAssembly.GetExportedTypes();

                    for (int iType = 0; iType < ExportedTypes.Length; iType++)
                    {
                        EntryPoint = ExportedTypes[iType];

                        if (EntryPoint.GetInterface("EasyHook.IEntryPoint") != null)
                            break;

                        EntryPoint = null;
                    }

                    if (EntryPoint == null)
                        throw new EntryPointNotFoundException("The given user library does not export a proper type implementing the 'IEasyHookInjection' interface.");


                    // parse injection information (should work always as there is nothing tricky about it)...
                    RemoteHooking.IContext Context = RemoteHooking.QueryContext(InfoPtr);

                    // assemble user parameters and initialize library...
                    BinaryFormatter Format = new BinaryFormatter();

                    UserParams.Add(Context);

                    try
                    {
                        UserParams.AddRange((Object[])Format.Deserialize(new MemoryStream(Context.UserData)));
                    }
                    catch (Exception ExtInfo)
                    {
                        throw new ArgumentException("Unable to deserialize user supplied argument list.", ExtInfo);
                    }

                    Instance = EntryPoint.InvokeMember(null, BindingFlags.Public | BindingFlags.Instance |
                        BindingFlags.CreateInstance, null, null, UserParams.ToArray());


                    // notify the host about successful injection...
                    Interface.InjectionCompleted(RemoteHooking.GetCurrentProcessId());
                }
                catch (Exception ExtInfo)
                {
                    // we will now get extensive error information on host side...
                    try
                    {
                        Interface.InjectionException(RemoteHooking.GetCurrentProcessId(), ExtInfo);
                    }
                    finally
                    {
                        Instance = null;

                        Release(EntryPoint);
                    }

                    return -1;
                }

                try
                {
                    /* 
                     * After this it is safe to enter the main method, which will block until assembly unloading...
                     * From now on the user library has to take care about error reporting!
                     */
                    EntryPoint.InvokeMember("Run", BindingFlags.Public | BindingFlags.Instance | BindingFlags.ExactBinding |
                         BindingFlags.InvokeMethod, null, Instance, UserParams.ToArray());
                }
                finally
                {
                    Instance = null;

                    Release(EntryPoint);
                }
            }
            catch(Exception ExtInfo)
            {
                Config.PrintWarning(ExtInfo.ToString());

                return -1;
            }
            finally
            {
                if (ConnectedChannels.Contains(HostChannel))
                    ConnectedChannels.Remove(HostChannel);
            }

            return 0;
        }
    }
}
