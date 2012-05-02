using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Xml.Serialization;


namespace Monitor_Application
{
	/// <summary>
	/// A class that represents an instance of a loaded game and it's configuration.
	/// </summary>
	public class ProgramConfiguration
	{

		/*
		 * SimpitX Namespace
		 *
		 * GameName
		 * GamePath
		 * (Consider adding support for hotkey attach/detach for plugins)
		 * Plugin Defaults
		 *
		 * 
		 */

		/// <summary>
		/// A dictionary containing all configuration/settings specific to this title.
		/// First string names the plugin (serves as a namespace)
		/// Second string is the name/key of the setting/item.
		/// Third string is (serialized) value.
		/// </summary>
		public SerializableDictionary<String, SerializableDictionary<String, String>> SettingsDictionary;

		/// <summary>
		/// Sets a specified key in the settings dictionary.  Creates if necessary.
		/// </summary>
		/// <param name="Section"></param>
		/// <param name="?"></param>
		/// <param name="?"></param>
		public void SetKey(String SectionName, String KeyName, String Value)
		{
			if (!SettingsDictionary.ContainsKey(SectionName))
			{
				// Create the section
				Console.WriteLine("Creating new section: " + "." + SectionName);
				SettingsDictionary.Add(SectionName, new SerializableDictionary<String, String>());
			}

			if (!SettingsDictionary[SectionName].ContainsKey(KeyName))
			{
				Console.WriteLine("Creating new key: " + SectionName + "." + KeyName);
				SettingsDictionary[SectionName].Add(KeyName, Value);
				return;
			}

			SettingsDictionary[SectionName][KeyName] = Value;
		}

		/// <summary>
		/// Returns the specified key in the settings dictionary, Creates if necessary
		/// </summary>
		/// <param name="Section"></param>
		/// <param name="Name"></param>
		/// <returns></returns>
		public String GetKey(String SectionName, String KeyName, String DefaultValue = "")
		{
			if (!SettingsDictionary.ContainsKey(SectionName))
			{
				// Create the section
				Console.WriteLine("Creating new section: " + "." + SectionName);
				SettingsDictionary.Add(SectionName, new SerializableDictionary<String, String>());
			}

			if (!SettingsDictionary[SectionName].ContainsKey(KeyName))
			{
				Console.WriteLine("Creating new key: " + SectionName + "." + KeyName);
				SettingsDictionary[SectionName].Add(KeyName, DefaultValue);
			}

			return SettingsDictionary[SectionName][KeyName];
			
		}

		/// <summary>
		/// Multidimensional indexer for key access
		/// </summary>
		/// <param name="SectionName"></param>
		/// <param name="KeyName"></param>
		/// <param name="DefaultValue"></param>
		/// <returns></returns>
		public String this[String SectionName, String KeyName, String DefaultValue = ""]
		{
			get { return GetKey(SectionName, KeyName, DefaultValue); }
			set { SetKey(SectionName, KeyName, value); }
		}

		/// <summary>
		/// WMI process watcher
		/// </summary>
		public ProcessWatcher procWatcher;

		/// <summary>
		/// Constructor
		/// </summary>
		public ProgramConfiguration()
		{
			// Empty object.

		}


		/// <summary>
		/// Creates the process watcher and begins listening
		/// </summary>
		public void Initialize(ProcessEventHandler createHandler, ProcessEventHandler deletedHandler, ProcessEventHandler modifyHandler)
		{

			if (procWatcher == null)
			{


				// Initialize ProcessWatcher using game settings
				procWatcher = new ProcessWatcher(this, SettingsDictionary["Info"]["ExecutablePath"]);

				// Add event handlers
				procWatcher.ProcessCreated += createHandler;
				procWatcher.ProcessDeleted += deletedHandler;
				procWatcher.ProcessModified += modifyHandler;

				// Start monitoring
				try
				{
					procWatcher.Start();
				}
				catch (System.Exception ex)
				{
					Console.WriteLine("Exception Thrown while starting ProcessWatch for Executable: " + SettingsDictionary["Info"]["ExecutablePath"]);
					Console.WriteLine("\t" + ex.Message);
					throw;
				}
				

			}

		}


		public void Shutdown()
		{

			// Stop monitoring for this game.
			if (procWatcher != null)
			{
				procWatcher.Stop();
				procWatcher.Dispose();
				procWatcher = null;
			}

			
			
		}

		public Icon GetIcon()
		{

			// Return the configured icon, if specified
			if (!SettingsDictionary.ContainsKey("Info") || !SettingsDictionary["Info"].ContainsKey("ExecutablePath"))
			{

				// Return the default icon
				return null;
			}

			// Resize the icon


			// Extract the icon from the executable, if possible
			return new Icon(Icon.ExtractAssociatedIcon(SettingsDictionary["Info"]["ExecutablePath"]), new Size(64,64));
			
		}

		public String GetExecutablePath()
		{

			// Locate the game's executable
			return SettingsDictionary["Info"]["ExecutablePath"];

			// 

		}
	}
}
