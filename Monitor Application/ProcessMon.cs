using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using EasyHook;

namespace Monitor_Application
{
    public class ProcessMon
    {

        [Serializable]
        public class ProcessInfo
        {
            public String FileName;
            public Int32 Id;
            public Boolean Is64Bit;
            public String User;
        }

        public static ProcessInfo[] EnumProcesses()
        {
            List<ProcessInfo> Result = new List<ProcessInfo>();
            Process[] ProcList = Process.GetProcesses();

            for (int i = 0; i < ProcList.Length; i++)
            {
                Process Proc = ProcList[i];

                try
                {
                    ProcessInfo Info = new ProcessInfo();

                    Info.FileName = Proc.MainModule.FileName;
                    Info.Id = Proc.Id;
                    Info.Is64Bit = RemoteHooking.IsX64Process(Proc.Id);
                    Info.User = RemoteHooking.GetProcessIdentity(Proc.Id).Name;

                    Result.Add(Info);
                }
                catch(System.Exception e)
                {
                    Console.WriteLine("Exception thrown while processing a ProcessInfo object:");
                    Console.Write(e.ToString());
                }
            }

            return Result.ToArray();
        }

    }
}
