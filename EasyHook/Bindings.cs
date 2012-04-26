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
using System.IO;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Security.Policy;
using System.Security;
using System.Security.Permissions;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Ipc;
using System.Threading;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Microsoft.Win32;
using System.IO.Compression;
using System.Security.Cryptography;

namespace EasyHook
{
	public class Config
	{
		public class PathClass
		{
			public readonly String EasyHook = null;
			public readonly String EasyHookSvc32 = null;
			public readonly String EasyHookSvc64 = null;
			public readonly String EasyHook32 = null;
			public readonly String EasyHook64 = null;

			public PathClass(String InEasyHookPath)
			{
				InEasyHookPath = Path.GetFullPath(InEasyHookPath).ToLower();

				if (!File.Exists(InEasyHookPath))
					throw new System.IO.FileNotFoundException("The given assembly file does not exist!");

				// protect in case of GAC
				String SysDir = Path.GetFullPath(Environment.SystemDirectory + "\\..").ToLower();

				if (InEasyHookPath.StartsWith(SysDir))
					throw new InvalidOperationException("It is not supported to have EasyHook in a system directory." +
						"This is probably the result of trying to call Config.Install() after EasyHook has been loaded into the GAC!");

				String Name = Path.GetFileNameWithoutExtension(InEasyHookPath);
				String Dir = Path.GetDirectoryName(InEasyHookPath);

				EasyHook = Dir + "\\" + Name + ".dll";
				EasyHook32 = Dir + "\\" + Name + "32.dll";
				EasyHookSvc32 = Dir + "\\" + Name + "Svc32.exe";
				EasyHook64 = Dir + "\\" + Name + "64.dll";
				EasyHookSvc64 = Dir + "\\" + Name + "Svc64.exe";

				Unpack(true, "EasyHook32.dll", EasyHook32);
				Unpack(false, "EasyHook64.dll", EasyHook64);
				Unpack(true, "EasyHookSvc32.exe", EasyHookSvc32);
				Unpack(false, "EasyHookSvc64.exe", EasyHookSvc64);
			}
		}

		public class Names
		{
			public const String EasyHook = "EasyHook, Version=" + CurrentVersion + ", PublicKeyToken=" + PublicKeyTokenString;
			public const String EasyHookSvc32 = "EasyHookSvc32, Version=" + CurrentVersion + ", PublicKeyToken=" + PublicKeyTokenString;
			public const String EasyHookSvc64 = "EasyHookSvc64, Version=" + CurrentVersion + ", PublicKeyToken=" + PublicKeyTokenString;
			public const String EasyHook32 = "EasyHook32, Version=" + CurrentVersion + ", PublicKeyToken=" + PublicKeyTokenString;
			public const String EasyHook64 = "EasyHook64, Version=" + CurrentVersion + ", PublicKeyToken=" + PublicKeyTokenString;
		}

		public static PathClass Paths = null;
		public static Byte[] PublicKeyToken { get { return (Byte[])m_PublicKeyToken.Clone(); } }
		public const String CurrentVersion = "2.0.0.0";
		public const String PublicKeyTokenString = "4b580fca19d0b0c5";
		public static readonly Boolean Has64BitSupport = HasFile("EasyHook64.dll.zip") && HasFile("EasyHookSvc64.exe.zip");
		private static readonly Byte[] m_PublicKeyToken = new Byte[] { 0x4b, 0x58, 0x0f, 0xca, 0x19, 0xd0, 0xb0, 0xc5 };

		static void ValidateKeyToken(Byte[] InToken)
		{
			for (int i = 0; i < InToken.Length; i++)
				if (m_PublicKeyToken[i] != InToken[i])
					throw new ArgumentException("The given assembly was not published by SecurityRevolutions.");
		}

		static Boolean HasFile(String InName)
		{
			try
			{
				if (typeof(Config).Assembly.GetManifestResourceStream("EasyHook.Resources." + InName) == null)
					return false;

				return true;
			}
			catch
			{
				return false;
			}
		}

		public static void Unpack(Boolean InRequired, String InFile, String InTargetPath)
		{
			// allocate an always large enough array
			Byte[] Bytes = new Byte[512*1024];

			try
			{
				// uncompress resource
				Stream Resource = typeof(Config).Assembly.GetManifestResourceStream("EasyHook.Resources." + InFile + ".zip");
				GZipStream ZIP = new GZipStream(Resource, CompressionMode.Decompress, false);
				Int32 BytesRead = ZIP.Read(Bytes, 0, Bytes.Length);
				Byte[] TmpBytes = new Byte[BytesRead];

				Array.Copy(Bytes, 0, TmpBytes, 0, BytesRead);

				Bytes = TmpBytes;
			}
			catch
			{
				if (InRequired)
					throw new ApplicationException("The required assembly \"" + InFile + "\" is not contained in this package.");
				else
					return;
			}

			if(!File.Exists(InTargetPath))
			{
				// extract file...
				File.WriteAllBytes(InTargetPath, Bytes);
			}
			else
			{
#if !DEBUG
				// validate
				Byte[] FileData = File.ReadAllBytes(InTargetPath);

				if(FileData.Length != Bytes.Length)
					throw new InvalidProgramException("The file \"" + InTargetPath + "\" does already exist but has been modified.");

				for (int i = 0; i < FileData.Length; i++)
					if (FileData[i] != Bytes[i])
						throw new InvalidProgramException("The file \"" + InTargetPath + "\" does already exist but has been modified.");
#endif
			}
		}

		public static void PrintError(String InMessage, params object[] InParams)
		{
			DebugPrint(EventLogEntryType.Error, InMessage, InParams);
		}

		public static void PrintWarning(String InMessage, params object[] InParams)
		{
			DebugPrint(EventLogEntryType.Warning, InMessage, InParams);
		}

		public static void PrintComment(String InMessage, params object[] InParams)
		{
			DebugPrint(EventLogEntryType.Information, InMessage, InParams);
		}

		public static void DebugPrint(EventLogEntryType InType, String InMessage, params object[] InParams)
		{
			String Entry = String.Format(InMessage, InParams);

			switch (InType)
			{
				case EventLogEntryType.Error: Entry = "[error]: " + Entry; break;
				case EventLogEntryType.Information: Entry = "[comment]: " + Entry; break;
				case EventLogEntryType.Warning: Entry = "[warning]: " + Entry; break;
			}

			try
			{
				if (EventLog.Exists("Application", "."))
				{
					EventLog Log = new EventLog("Application", ".", "EasyHook");

#if !DEBUG
				if(InType == EventLogEntryType.Error)
#endif
					Log.WriteEntry(Entry, InType);
				}
			}
			catch
			{
			}

#if DEBUG
			Console.WriteLine(Entry);
#endif
		}

		public static void Install(String InEasyHookPath)
		{
			Paths = new PathClass(InEasyHookPath);
		}

		public static void Register(
			String InDescription,
			params String[] InUserAssemblies)
		{
			List<Assembly> AsmList = new List<Assembly>();
			List<String> RemovalList = new List<String>();
			List<String> InstallList = new List<String>();

			if (Paths == null)
				throw new InvalidOperationException("You have to call Config.Install() first.");

			/*
			 * Read and validate config file...
			 */
			List<String> Files = new List<string>();
			
			Files.Add(Paths.EasyHook);
			Files.Add(Paths.EasyHook32);

			if(Has64BitSupport)
				Files.Add(Paths.EasyHook64);

			Files.AddRange(InUserAssemblies);

			for (int i = 0; i < Files.Count; i++)
			{
				Assembly Entry;
				String AsmPath = Path.GetFullPath(Files[i]);

				if (!File.Exists(AsmPath))
					throw new System.IO.FileNotFoundException("The given assembly \"" + Files[i] + "\" (\"" + AsmPath + "\") does not exist.");

				// just validate that this is a NET assembly with valid metadata...
				try { Entry = Assembly.ReflectionOnlyLoadFrom(AsmPath); }
				catch (Exception ExtInfo)
				{
					throw new BadImageFormatException("Unable to load given assembly \"" + Files[i] + "\" (\"" + AsmPath +
						"\") for reflection. Is this a valid NET assembly?", ExtInfo);
				}

				// is strongly named? (required for GAC)
				AssemblyName Name = AssemblyName.GetAssemblyName(AsmPath);

				if ((Name.Flags & AssemblyNameFlags.PublicKey) == 0)
					throw new ArgumentException("The given user library \"" + Files[i] + "\" is not strongly named!");

				AsmList.Add(Entry);
				RemovalList.Add(Name.Name);
				InstallList.Add(AsmPath);
			}

			/*
			 * Install assemblies into GAC ...
			 */

			// TODO: Protect directory to be accessible for admins only...

			// create unique installation identifier
			Byte[] IdentData = new Byte[30];

			new RNGCryptoServiceProvider().GetBytes(IdentData);

			String Ident = Convert.ToBase64String(IdentData);

			// obtain temporary directory...
			String TempDir = Path.GetTempFileName();

			File.Delete(TempDir);

			Directory.CreateDirectory(TempDir);

			// TODO: add removal entry into reference file...


			try
			{
				// copy GACUtil into temp directory...
				Unpack(true, "gac.exe", TempDir + "\\gac.exe");

				// copy GAC remover into temp directory...
				File.Copy(".\\EasyHookSvc32.exe", TempDir + "\\remover.exe");
				File.Copy(".\\EasyHook.dll", TempDir + "\\EasyHook.dll");

				// add batch file to cleanup temp directory after "remover" has finished...
				StringBuilder Batch = new StringBuilder();

				Batch.AppendLine(":RepLabel");
				Batch.AppendLine("del \"" + TempDir + "\\remover.exe\"");
				Batch.AppendLine("if exist \"" + TempDir + "\\remover.exe\" goto RepLabel");
				Batch.AppendLine("del \"" + TempDir + "\\gac.exe\"");
				Batch.AppendLine("del \"" + TempDir + "\\EasyHook.dll\"");
				Batch.AppendLine("del \"" + TempDir + "\\install.gac\"");
				Batch.AppendLine("del \"" + TempDir + "\\uninstall.gac\"");
				Batch.AppendLine("del \"" + TempDir + "\\cleanup.bat\"");

				File.WriteAllText(TempDir + "\\cleanup.bat", Batch.ToString());

				// start GAC remover - it will also remove its own directory...
				RunCommand("GACRemover", false, false, TempDir + "\\remover.exe", Process.GetCurrentProcess().Id.ToString() + " \"" +
					Ident + "\" \"" + InDescription.Replace('"', '\'') + "\"");

			}
			catch (Exception e)
			{
				// remove temporary files..
				Directory.Delete(TempDir, true);

				throw e;
			}

			// write install and uninstall information
			File.WriteAllLines(TempDir + "\\install.gac", InstallList.ToArray());
			File.WriteAllLines(TempDir + "\\uninstall.gac", RemovalList.ToArray());

			RunCommand("GAC", true, false, TempDir + "\\gac.exe", "/nologo /il \"" + TempDir + "\\install.gac\" /r OPAQUE \"" + 
				Ident + "\" \"" + InDescription + "\"");
		}

		/// <summary>
		/// Intended for internal use only! Has to be exported, because our internal services
		/// will need it...
		/// </summary>
		public static void RunCommand(
			String InFriendlyName,
			Boolean InWaitForExit,
			Boolean InShellExecute,
			String InPath,
			String InArguments)
		{
			InPath = Path.GetFullPath(InPath);

			Process Proc = new Process();
			ProcessStartInfo StartInfo = new ProcessStartInfo(InPath, InArguments);

			if (InShellExecute && InWaitForExit)
				throw new ArgumentException("It is not supported to execute in shell and wait for termination!");

			StartInfo.RedirectStandardOutput = !InShellExecute;
			StartInfo.UseShellExecute = InShellExecute;
			StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
			StartInfo.CreateNoWindow = true;
			StartInfo.WorkingDirectory = Path.GetDirectoryName(InPath);

			Proc.StartInfo = StartInfo;

			try
			{
				if (!Proc.Start())
					throw new Exception();
			}
			catch(Exception ExtInfo)
			{
				throw new ApplicationException("Unable to run internal command.", ExtInfo);
			}

			if (InWaitForExit)
			{
				if (!Proc.WaitForExit(5000))
				{
					Proc.Kill();

					throw new ApplicationException("Unable to run internal command.");
				}

				// save to log entry
				String Output = Proc.StandardOutput.ReadToEnd();

				PrintComment("[" + InFriendlyName + "]: " + Output);
			}
		}
	}

	class Bindings 
	{
		internal static readonly Assembly DynAsm;
		internal static readonly Boolean Is64Bit = IntPtr.Size == 8;

		static Bindings()
		{
			if (Is64Bit)
			{
				if (!Config.Has64BitSupport)
					throw new NotSupportedException("Your license does not contain 64-bit support. Please visit www.easyhook.org for more information.");

				DynAsm = Assembly.Load(Config.Names.EasyHook64);
			}
			else
				DynAsm = Assembly.Load(Config.Names.EasyHook32);

			if (DynAsm == null)
				throw new System.IO.FileNotFoundException("Unable to find required native assembly. Consider calling EasyHook.Config.Install() first.");
		}
	}

	public interface IHookAccessControl
	{
		Boolean IsExclusive { get; }
		Boolean IsInclusive { get; }

		void SetInclusiveACL(Int32[] InACL);

		void SetExclusiveACL(Int32[] InACL);

		Int32[] GetACL();
	}

	public interface IHookRuntimeInfo
	{
		/*static*/
		Boolean IsHandlerContext { get; }
		/*static*/
		Object Callback { get; }
		/*static*/
		ILocalHook Handle { get; }
		/*static*/
		IntPtr ReturnAddress { get; }
		/*static*/
		IntPtr InitialRSP { get; }
	}

	public interface ILocalHook : IDisposable
	{
		String QueryJournal();

		IHookAccessControl	ThreadACL { get; }
		Boolean IsInstalled { get; }

		/*static*/
		IHookAccessControl GlobalThreadACL { get; }

		/*static*/
		bool IsProtectedEntryPoint(IntPtr InEntryPoint);

		/*static*/
		void BeginUpdate(Boolean InIgnoreWarnings);

		/*static*/
		void EndUpdate();

		/*static*/
		void CancelUpdate();

		/*static*/
		ILocalHook Create(
			IntPtr InTargetProc,
			Delegate InNewProc,
			Object InCallback);

		/*static*/
		ILocalHook CreateUnmanaged(
			IntPtr InTargetProc,
			IntPtr InNewProc);

		/*static*/
		IntPtr GetProcAddress(
			String InModule,
			String InSymbolName);

		/*static*/
		T GetProcDelegate<T>(
				String InModule,
				String InSymbolName);

		bool IsThreadIntercepted(Int32 InThreadID);

		/*static*/
		bool Release();
	}

	public class HookRuntimeInfo
	{
		private static IHookRuntimeInfo Root_Interface;

		static HookRuntimeInfo()
		{
			Type t = Bindings.DynAsm.GetType("EasyHook.HookRuntimeInfo_Specific");

			Root_Interface = (IHookRuntimeInfo)t.GetMethod("Constructor").Invoke(null, null);
		}

		public static Boolean IsHandlerContext { get { return Root_Interface.IsHandlerContext; } }
		public static Object Callback { get { return Root_Interface.Callback; } }
		public static LocalHook Handle { get { return new LocalHook(Root_Interface.Handle); } }
		public static IntPtr ReturnAddress { get { return Root_Interface.ReturnAddress; } }
		public static IntPtr InitialRSP { get { return Root_Interface.InitialRSP; } }
	}

	public class LocalHook
	{
		private static ILocalHook Root_Interface;
		private ILocalHook Interface = Root_Interface;

		static LocalHook()
		{
			Type t = Bindings.DynAsm.GetType("EasyHook.LocalHook_Specific");

			Root_Interface = (ILocalHook)t.GetMethod("Constructor").Invoke(null, null);
		}

		internal LocalHook(ILocalHook InInterface)
		{
			Interface = InInterface;
		}

		public static String QueryJournal()
		{ return Root_Interface.QueryJournal(); }

		public IHookAccessControl ThreadACL
		{ get { return Interface.ThreadACL; } }

		public Boolean IsInstalled 
		{ get { return Interface.IsInstalled; } }

		public static IHookAccessControl GlobalThreadACL
		{ get { return Root_Interface.GlobalThreadACL; } }

		public static bool IsProtectedEntryPoint(IntPtr InEntryPoint)
		{ return Root_Interface.IsProtectedEntryPoint(InEntryPoint); }

		public void Dispose()
		{ Interface.Dispose(); }

		public static void BeginUpdate(Boolean InIgnoreWarnings)
		{ Root_Interface.BeginUpdate(InIgnoreWarnings); }

		public static void EndUpdate()
		{ Root_Interface.EndUpdate(); }

		public static void CancelUpdate()
		{ Root_Interface.CancelUpdate(); }

		public static LocalHook Create(
			IntPtr InTargetProc,
			Delegate InNewProc,
			Object InCallback)
		{ return new LocalHook(Root_Interface.Create(InTargetProc, InNewProc, InCallback)); }

		public static LocalHook CreateUnmanaged(
			IntPtr InTargetProc,
			IntPtr InNewProc)
		{ return new LocalHook(Root_Interface.CreateUnmanaged(InTargetProc, InNewProc)); }

		public static IntPtr GetProcAddress(
			String InModule,
			String InSymbolName)
		{ return Root_Interface.GetProcAddress(InModule, InSymbolName); }

		public static T GetProcDelegate<T>(
				String InModule,
				String InSymbolName)
		{ return Root_Interface.GetProcDelegate<T>(InModule, InSymbolName); }

		public bool IsThreadIntercepted(Int32 InThreadID)
		{ return Interface.IsThreadIntercepted(InThreadID); }

		public static bool Release()
		{ return Root_Interface.Release(); }
	}

	[Flags]
	public enum InjectionOptions
	{
		None = 0,
		AlwaysUseLocalSymbols = 1,
	}

	public interface IRemoteHooking
	{
		/*static*/bool IsAdministrator{ get; }

		/*static*/ IpcServerChannel IpcCreateServer<TObject>(
			ref String RefChannelName,
			WellKnownObjectMode InObjectMode,
			params WellKnownSidType[] InAllowedClientSIDs) where TObject : MarshalByRefObject;

		/*static*/ TObject IpcConnectClient<TObject>(
			String InChannelName) where TObject : MarshalByRefObject;

		/*static*/ void Inject(
			Int32 InTargetPID,
			Int32 InWakeUpTID,
			InjectionOptions InOptions,
			String InLibraryPath_x86,
			String InLibraryPath_x64,
			params Object[] InPassThruArgs);

		/*static*/ bool IsX64Process(Int32 InTargetPID);

		/*static*/ Int32 GetCurrentProcessId();

		/*static*/ Int32 GetCurrentThreadId();

		/*static*/ RemoteHooking.IContext QueryContext(String InInjectParam);

		/*static*/
		Int32 CreateAndInject(
			String InEXEPath,
			InjectionOptions InOptions,
			String InLibraryPath_x86,
			String InLibraryPath_x64,
			params Object[] InPassThruArgs);

		/*static*/
		void WakeUpProcess();

		/*static*/
		void Impersonate(Int32 InClientTID);

		/*static*/
		void SetWakeUpThreadID(Int32 InThreadID);

		/*static*/
		WindowsIdentity GetProcessIdentity(Int32 InTargetPID);

		/*static*/
		Object ExecuteAsService<TClass>(
				String InMethodName,
				params Object[] InParams);
	}

	public class RemoteHooking
	{
		private static IRemoteHooking Root_Interface;

		static RemoteHooking()
		{
			Type t = Bindings.DynAsm.GetType("EasyHook.RemoteHooking_Specific");

			Type[] Enum = Bindings.DynAsm.GetTypes();
			String TypeString = "";

			for (int i = 0; i < Enum.Length; i++)
			{
				TypeString += "    " + Enum[i].FullName + ",\r\n ";
			}

			if (t == null)
				throw new ApplicationException("Unable to load type 'EasyHook.RemoteHooking_Specific'. Available types: " + TypeString);

			Root_Interface = (IRemoteHooking)t.GetMethod("Constructor").Invoke(null, null);
		}

		private RemoteHooking() { }

		public interface IContext
		{
			Int32 HostPID { get; }
			Byte[] UserData { get; }
		};

		internal static void SetWakeUpThreadID(Int32 InThreadID)
		{
			Root_Interface.SetWakeUpThreadID(InThreadID);
		}

		public static bool IsAdministrator { get { return Root_Interface.IsAdministrator; } }

		public static IpcServerChannel IpcCreateServer<TObject>(
			ref String RefChannelName,
			WellKnownObjectMode InObjectMode,
			params WellKnownSidType[] InAllowedClientSIDs) where TObject : MarshalByRefObject
		{ return Root_Interface.IpcCreateServer<TObject>(ref RefChannelName, InObjectMode, InAllowedClientSIDs); }

		public static TObject IpcConnectClient<TObject>(String InChannelName) where TObject : MarshalByRefObject
		{ return Root_Interface.IpcConnectClient<TObject>(InChannelName); }

		public static void Inject(
			Int32 InTargetPID,
			InjectionOptions InOptions,
			String InLibraryPath_x86,
			String InLibraryPath_x64,
			params Object[] InPassThruArgs)
		{
			Inject(InTargetPID, 0, InOptions, InLibraryPath_x86, InLibraryPath_x64, InPassThruArgs);
		}

		internal static void Inject(
			Int32 InTargetPID,
			Int32 InWakeUpTID,
			InjectionOptions InOptions,
			String InLibraryPath_x86,
			String InLibraryPath_x64,
			params Object[] InPassThruArgs)
		{
			Root_Interface.Inject(InTargetPID, InWakeUpTID, InOptions, InLibraryPath_x86, InLibraryPath_x64, InPassThruArgs);
		}

		public static Int32 CreateAndInject(
			String InEXEPath,
			InjectionOptions InOptions,
			String InLibraryPath_x86,
			String InLibraryPath_x64,
			params Object[] InPassThruArgs)
		{
			return Root_Interface.CreateAndInject(InEXEPath, InOptions, InLibraryPath_x86, InLibraryPath_x64, InPassThruArgs);
		}

		public static void WakeUpProcess()
		{
			Root_Interface.WakeUpProcess();
		}

		internal static void Impersonate(Int32 InClientTID)
		{ Root_Interface.Impersonate(InClientTID); }

		public static bool IsX64Process(Int32 InTargetPID)
		{ return Root_Interface.IsX64Process(InTargetPID); }

		public static WindowsIdentity GetProcessIdentity(Int32 InTargetPID)
		{ return Root_Interface.GetProcessIdentity(InTargetPID); }

		public static Int32 GetCurrentProcessId()
		{ return Root_Interface.GetCurrentProcessId(); }

		public static Int32 GetCurrentThreadId()
		{ return Root_Interface.GetCurrentThreadId(); }

		public static RemoteHooking.IContext QueryContext(String InInjectParam)
		{ return Root_Interface.QueryContext(InInjectParam); }

		public static Object ExecuteAsService<TClass>(
				String InMethodName,
				params Object[] InParams)
		{ return Root_Interface.ExecuteAsService<TClass>(InMethodName, InParams); }
	}

	public enum ObjectType
	{
		Unknown = 0,
		IoCompletion = 1,
		Job = 2,
		Key = 3,
		KeyedEvent = 4,
		Mutant = 5,
		Port = 6,
		Process = 7,
		Profile = 8,
		Section = 9,
		Adapter = 10,
		Callback = 11,
		Controller = 12,
		DebugObject = 13,
		Desktop = 14,
		Device = 15,
		Directory = 16,
		Driver = 17,
		Event = 18,
		EventPair = 19,
		Semaphore = 20,
		SymbolicLink = 21,
		Thread = 22,
		Timer = 23,
		Token = 24,
		Type = 25,
		WaitablePort = 26,
		WindowsStation = 27,
		WMIGuid = 28,
		File = 29,
	}

	public enum TargetArchitecture
	{
		x86,
		x64,
	}

	public interface IDebugger
	{
		/*static*/ bool IsEnabled { get; set; }

		/*static*/
		bool IsAvailable { get; }

		/*static*/
		Int32 GetThreadIdByHandle(IntPtr InThreadHandle);

		/*static*/
		Int32 GetProcessIdByHandle(IntPtr InProcessHandle);

		/*static*/
		String Disassemble(
			IntPtr InCodePtr,
			Int32 InMaxByteCount,
			Int32 InLineIndent);

		/*static*/
		Debugger.IObjectInfo QueryObjectInformation(IntPtr InHandle);
	}

	public class Debugger
	{
		private static IDebugger Root_Interface;

		static Debugger()
		{
			Type t = Bindings.DynAsm.GetType("EasyHook.Debugger_Specific");

			Root_Interface = (IDebugger)t.GetMethod("Constructor").Invoke(null, null);
		}

		private Debugger() { }

		public static bool IsEnabled
		{ get { return Root_Interface.IsEnabled; } set { Root_Interface.IsEnabled = value; } }

		public static bool IsAvailable 
		{ get { return Root_Interface.IsAvailable; } }

		public static Int32 GetThreadIdByHandle(IntPtr InThreadHandle)
		{ return Root_Interface.GetThreadIdByHandle(InThreadHandle); }

		public static Int32 GetProcessIdByHandle(IntPtr InProcessHandle)
		{ return Root_Interface.GetProcessIdByHandle(InProcessHandle); }

		public static String Disassemble(
			IntPtr InCodePtr,
			Int32 InMaxByteCount,
			Int32 InLineIndent)
		{ return Root_Interface.Disassemble(InCodePtr, InMaxByteCount, InLineIndent); }

		public interface IObjectInfo
		{
			Int32 Attributes { get; }
			Int32 GrantedAccess  { get; }
			Int32 HandleCount  { get; }
			Int32 PointerCount { get; }
			String TypeName { get; }
			String ObjectName  { get; }
			ObjectType Type  { get; }
		};

		public static IObjectInfo QueryObjectInformation(IntPtr InHandle)
		{ return Root_Interface.QueryObjectInformation(InHandle); }
	}

	public class HelperServiceInterface : MarshalByRefObject
	{
		public void Inject(
				Int32 InTargetPID,
				Int32 InWakeUpTID,
				InjectionOptions InOptions,
				String InLibraryPath_x86,
				String InLibraryPath_x64,
				params Object[] InPassThruArgs)
		{
			RemoteHooking.Inject(InTargetPID, InWakeUpTID, InOptions, InLibraryPath_x86, InLibraryPath_x64, InPassThruArgs);
		}

		public Object ExecuteAsService<TClass>(
				String InMethodName,
				Object[] InParams)
		{
			return typeof(TClass).InvokeMember(InMethodName, BindingFlags.InvokeMethod | BindingFlags.Public |
				BindingFlags.Static, null, null, InParams);
		}

		/// <summary>
		/// Dont use this method, refer to <see cref="RemoteHooking.Impersonate(ImpersonationHandler)"/>
		/// for a supported public method! 
		/// </summary>
		/// <param name="InClientTID">
		/// The ID of the thread whose privileges should be upgraded to LOCAL-SYSTEM.
		/// </param>
		public void Impersonate(Int32 InClientTID)
		{
			RemoteHooking.Impersonate(InClientTID);
		}

		public static Exception InjectionError = null;
		public static ManualResetEvent InjectionEvent = new ManualResetEvent(false);

		public static void WaitForInjection(
			Int32 InClientPID,
			Int32 InTimeoutMillis)
		{
			try
			{
				if (!InjectionEvent.WaitOne(InTimeoutMillis, true))
					InjectionError = new TimeoutException("Unable to wait for injection.");

				if (InjectionError != null)
					throw InjectionError;
			}
			finally
			{
				InjectionEvent.Close();
				InjectionEvent = new ManualResetEvent(false);
				InjectionError = null;
			}
		}

		public void InjectionException(
			Int32 InClientPID,
			Exception e)
		{
			InjectionError = e;
			InjectionEvent.Set();
		}

		public void InjectionCompleted(Int32 InClientPID)
		{
			InjectionEvent.Set();
		}

		public void Ping() { }
	}

	// just to determine class in exported types...
	public interface IEntryPoint { }
}
