using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml;
using System.Xml.Schema;
using System.Security.Cryptography;
using EasyHook;

namespace Monitor_Application
{
	public class Plugin
	{
		/// <summary>
		/// Static index of all loaded plugins
		/// </summary>
		public static Dictionary<String, Plugin> LoadedPlugins = new Dictionary<String, Plugin>();

		private DirectoryInfo baseDirectory;
		private FileInfo configFile;

		private String RequiredVersion;
		
		public String Name;
		public String Author;
		public String Version;
		public String Date;

		public String Description;

		private bool isInjected = false;
		private bool isRunning = false;

		public bool isLoaded
		{
			get { return isInjected || isRunning; }
		}
		


		public enum Architecture { x86, x64, CLR };
		public enum ActionType { Inject, Run };
		public struct LibraryExecutable
		{
			public String FileName;
			public Architecture Arch;
			public String Hash;
			public ActionType Action;

		}
		List<LibraryExecutable> Libraries = new List<LibraryExecutable>();

		public List<String> Dependencies = new List<String>();

		public Plugin(DirectoryInfo pluginDirectory)
		{
			// Validate the directory.
			if (!pluginDirectory.Exists)
			{
				throw new Exception("Plugin directory does not validate.");
			}

			baseDirectory = pluginDirectory;
			// Check for the presence of exactly ONE .plugin.xml file
			FileInfo[] pluginFiles = pluginDirectory.GetFiles("*.plugin.xml");

			if (pluginFiles.Length != 1)
			{
				throw new Exception("Duplicate/Missing plugin config file (*.plugin.xml).");
			}

			configFile = pluginFiles[0];

			// Load the plugin config
			
			try
			{

				LoadPluginConfig();
				ValidatePluginConfig();
			}
			catch (System.Exception ex)
			{
				Console.WriteLine(ex.Message);
				throw ex;
			}

			// Register
			Plugin.LoadedPlugins.Add(this.Name, this);
		}

		public bool ValidatePluginConfig()
		{

			// Determine the required action.
			if (Libraries.Count == 0)
			{
				throw new Exception("No libraries configured for '" + this.Name + "' plugin!");
			}

			ActionType startAction = Libraries[0].Action;

			// Verify that the action is configured consistently.
			if (Libraries.Count > 1)
			{

				foreach (LibraryExecutable le in Libraries)
				{
					if (le.Action != startAction)
					{
						throw new Exception("Library action mismatch in plugin " + this.Name);
					}
				}
			}

			return false;
		}

		/// <summary>
		/// Reads the *.plugin.xml data from disk.
		/// </summary>
		public void LoadPluginConfig()
		{

			Console.WriteLine("Plugin configuration read initiated.");
			
			// Validate the plugin xml against schema.


			// Open the xml file.
			if (!configFile.Exists)
			{
				throw new Exception("(Previously validated!) Configuration file does not exist!");
			}


			XmlDocument configDoc = new XmlDocument();

			try
			{
				configDoc.Load(configFile.OpenRead());
			}
			catch (System.Exception ex)
			{
				Console.WriteLine(ex.Message);
				throw ex;
			}
			
		
			// Load the config file.
			XmlNode Node;

			Node = configDoc.DocumentElement;

			// Root node should be SimpitX, contain version info.
			if (Node.Name != "SimpitX")
			{
				throw new XmlException("Element 'SimpitX' expected.");
			}

			// Get version info
			RequiredVersion = Node.Attributes["Version"].Value;
			
			// Locate the plugin element.
			Node = Node.FirstChild;

			if (Node.Name != "Plugin")
			{
				throw new XmlException("Element 'Plugin' expected.");
			}


			try
			{
				Name = Node.Attributes["Name"].Value;
			}
			catch (System.Exception ex)
			{
				Console.WriteLine("Exception while reading expected Plugin attribute 'Name': " + ex.Message);
				throw ex;
			}
			
			try
			{
				Author = Node.Attributes["Author"].Value;
			}
			catch (System.Exception ex)
			{
				Console.WriteLine("Exception while reading expected Plugin attribute 'Author': " + ex.Message);
				Author = "Unknown author";
			}


			try
			{
				Version = Node.Attributes["Version"].Value;
			}
			catch (System.Exception ex)
			{
				Console.WriteLine("Exception while reading expected Plugin attribute 'Version': " + ex.Message);
				Version = "0.01";
			}


			try
			{
				Date = Node.Attributes["Date"].Value;
			}
			catch (System.Exception ex)
			{
				Console.WriteLine("Exception while reading expected Plugin attribute 'Date': " + ex.Message);
				Date = "01/01/1972";
			}

			Node = Node.FirstChild;
			while (Node != null)
			{

				switch (Node.Name)
				{
					case "Description":
						Description = Node.InnerText;
						break;
					case "Library":
						{
							LibraryExecutable tempLibrary;
							tempLibrary = new LibraryExecutable();
							tempLibrary.FileName = Node.Attributes["File"].Value;
							{
								// Ensure that the file exists.
								FileInfo[] libraryFile = baseDirectory.GetFiles(tempLibrary.FileName);

								if ((libraryFile.Count() != 1) || (libraryFile[0].Exists == false))
								{
									throw new Exception("Required library file '" + tempLibrary.FileName + "'for " + this.Name + " is missing.");
								}

								// Convert to full path.
								tempLibrary.FileName = libraryFile[0].FullName;

							}
							String archStr;
							try
							{
								archStr = Node.Attributes["Arch"].Value;
							}
							catch (System.Exception ex)
							{
								Console.WriteLine("Exception while reading required Library attribute 'Arch'" + ex.Message);
								throw ex;
							}

							switch (archStr)
							{
								case "x86":
									tempLibrary.Arch = Architecture.x86;
									break;
								case "x64":
									tempLibrary.Arch = Architecture.x64;
									break;
								case "CLR":
									tempLibrary.Arch = Architecture.CLR;
									break;
								default:
									throw new Exception("Unrecognized Library 'Type' specified in plugin configuration.");

							}


							try
							{
								tempLibrary.Hash = Node.Attributes["MD5"].Value;
							}
							catch (System.Exception ex)
							{
								Console.WriteLine("Exception while reading expected Library attribute 'MD5': " + ex.Message);
								throw ex;
							}

							String actionStr;
							try
							{
								actionStr = Node.Attributes["Action"].Value;
							}
							catch (System.Exception ex)
							{
								Console.WriteLine("Exception while reading required Library attribute 'Action': " + ex.Message);
								throw ex;
							}
							switch (actionStr)
							{
								case "Inject":
									tempLibrary.Action = ActionType.Inject;
									break;

								case "Execute":
									tempLibrary.Action = ActionType.Run;
									break;
								default:
									throw new Exception("Invalid Library action specified: " + actionStr);
							}
							Libraries.Add(tempLibrary);
							if (Libraries.Count > 2)
							{
								throw new Exception("Invalid Library configuration (there cannot be more than two!)");
							}

						}
						break;
					case "Dependencies":
						{
							XmlNode DependencyNode = Node.FirstChild;
							while (DependencyNode != null)
							{
								String dependencyName;
								try
								{
									dependencyName = DependencyNode.Attributes["Name"].Value;
								}
								catch (System.Exception ex)
								{
									Console.WriteLine("Exception thrown while processing expected Dependency attribute 'Name'" + ex.Message);
									throw ex;
								}
								Dependencies.Add(dependencyName);
								DependencyNode = DependencyNode.NextSibling;
							}
						}
						break;
				}

				Node = Node.NextSibling;

			}

			Console.WriteLine("Plugin configuration read successfully.");
		}

		/// <summary>
		/// Returns the x64 library file.
		/// </summary>
		public String GetLibraryx64()
		{

			foreach (LibraryExecutable lib in Libraries)
			{
				if (lib.Arch == Architecture.x64 || lib.Arch == Architecture.CLR)
				{
					return lib.FileName;
				}
			}

			// None found.
			return "";

		}

		/// <summary>
		/// Returns the x86 library file.
		/// </summary>
		public String GetLibraryx86()
		{

			foreach (LibraryExecutable lib in Libraries)
			{
				if (lib.Arch == Architecture.x86 || lib.Arch == Architecture.CLR)
				{
					return lib.FileName;
				}
			}

			// None found.
			return "";

		}

		/// <summary>
		/// Returns the action that should be performed using this plugin.
		/// </summary>
		/// <returns></returns>
		public ActionType GetAction()
		{
			return Libraries[0].Action;
		}


	}
}
