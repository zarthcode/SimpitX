using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

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
        public void Initialize()
        {
            // Initialize ProcessWatcher using game settings
            procWatcher = new ProcessWatcher(this, SettingsDictionary["SimpitX"]["ExecutableName"]);

            // Start monitoring
            procWatcher.Start();

        }


        public void Shutdown()
        {

            // Stop monitoring for this game.
            procWatcher.Stop();
        }

    }
}
