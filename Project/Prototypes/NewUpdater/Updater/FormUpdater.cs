using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Compression;
using System.Net;
using System.Threading;
using System.Diagnostics;

namespace Updater
{
    public partial class FormUpdater : Form
    {
        private const string IdeImageName = "IDE.exe";
        private const string RemoteUrl = "http://www.x2048.com/swift/update/";
        private const string ActionGetHeadRevTag = "revtag.php";
        private const string ActionDownload = "download.php?revtag=";
        private const string LocalRevisionTagFilename = "./revision";

        private static string latestRemoteRevision;

        private string CurrentStatus
        {
            set
            {
                if (labelStatus.InvokeRequired)
                    labelStatus.Invoke(new Action(() => { labelStatus.Text = value; }));
                else
                    labelStatus.Text = value;
            }
        }

        private int ProgressValue
        {
            set
            {
                if (progressBar.InvokeRequired)
                    progressBar.Invoke(new Action(() => { progressBar.Value = value; }));
                else
                    progressBar.Value = value;
            }
        }

        public FormUpdater()
        {
            InitializeComponent();
        }

        protected override void OnLoad(EventArgs e)
        {
            if (!IsUpdateAvailable())
            {
                Process.Start(IdeImageName);
                Close();
                return;
            }

            new Thread(obj =>
            {
                try
                {
                    UpdatePackage();
                }
                catch (UpdateException ex)
                {
                    MessageBox.Show(ex.Message, "Error Updating", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    Invoke(new Action(Close));
                    return;
                }
                /*catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Unexpected Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    Invoke(new Action(Close));
                    return;
                }*/

                Process.Start(IdeImageName);
                Invoke(new Action(Close));
            }).Start();
        }

        public void UpdatePackage()
        {
            // Don't update if we don't know what to ask for
            if (String.IsNullOrEmpty(latestRemoteRevision))
                throw new UpdateException("Unable to determine latest package.");

            // Grab remote package
            CurrentStatus = "Downloading package...";
            MemoryStream packageData = GetRemoteFile("{0}{1}{2}", RemoteUrl, ActionDownload, latestRemoteRevision);
            if (packageData == null || packageData.Length == 0)
                throw new UpdateException("Unable to download update package.");

            // Unzip into working directory
            CurrentStatus = "Extracting... ";
            if (!ExtractZipArchive(packageData))
                throw new UpdateException("Unable to extract package archive.");

            // Successfully updated everything, update local revtag
            using (TextWriter textWriter = File.CreateText(LocalRevisionTagFilename))
                textWriter.Write(latestRemoteRevision);

            CurrentStatus = "Done! Restarting Swift...";
        }

        private MemoryStream GetRemoteFile(string address, params object[] args)
        {
            string uri = String.Format(address, args);
            byte[] contents = null;

            try
            {
                using (WebClient client = new WebClient { Encoding = Encoding.UTF8 })
                {
                    bool finished = false;
                    client.DownloadDataCompleted += (sender, eventArgs) =>
                    {
                        if (eventArgs.Cancelled || eventArgs.Error != null)
                        {
                            finished = true;
                            return;
                        }

                        contents = eventArgs.Result;
                        finished = true;
                    };

                    client.DownloadProgressChanged += (sender, eventArgs) =>
                    {
                        ProgressValue = (int)(((float)eventArgs.BytesReceived / eventArgs.TotalBytesToReceive) * 100.0f);
                    };

                    client.DownloadDataAsync(new Uri(uri));
                    while (!finished)
                        Thread.Sleep(1);
                }
            }
            catch (Exception)
            {
                return null;
            }

            return contents == null ? null : new MemoryStream(contents);
        }

        public bool ExtractZipArchive(MemoryStream packageData)
        {
            using (ZipArchive archive = new ZipArchive(packageData, ZipArchiveMode.Read))
            {
                foreach (var entry in archive.Entries)
                {
                    if (entry.Name == "")
                        continue;

                    ProgressValue = (int)(((float)archive.Entries.IndexOf(entry) / archive.Entries.Count) * 100.0f);

                    // Walk path and create directories if needed
                    string walkPath = Path.GetDirectoryName(entry.FullName);
                    if (!String.IsNullOrEmpty(walkPath))
                    {
                        string[] pathParts = walkPath.Split(new[] { '/' });
                        string currentPath = "";
                        foreach (string path in pathParts)
                        {
                            currentPath = Path.Combine(currentPath, path);
                            if (!Directory.Exists(currentPath))
                                Directory.CreateDirectory(currentPath);
                        }
                    }

                    // Extract file
                    try
                    {
                        using (FileStream outFile = new FileStream(entry.FullName, FileMode.Create))
                        {
                            // Open archived file
                            Stream inFile = entry.Open();
                            if (inFile == null)
                                throw new UpdateException("Cannot open archived file '{0}'. Aborting.", entry.Name);

                            // Read contents into a temp buffer
                            byte[] buffer = new byte[inFile.Length];
                            inFile.Read(buffer, 0, (int)inFile.Length);

                            if (buffer == null || inFile.Length == 0)
                                throw new UpdateException("Cannot read archived file '{0}'. Aborting.", entry.Name);

                            // Write to the out file
                            outFile.Write(buffer, 0, buffer.Length);
                        }
                    }
                    catch (IOException)
                    {
                        throw new UpdateException("Could not open file '{0}' for writing. May be in use?", entry.Name);
                    }
                }
            }

            return true;
        }

        public static bool IsUpdateAvailable()
        {
            // Read remote revision tag
            latestRemoteRevision = GetRemoteString("{0}{1}", RemoteUrl, ActionGetHeadRevTag);
            string localRevTag;

            // Check if we can connect to the remote server in the first place
            if (String.IsNullOrEmpty(latestRemoteRevision))
                return false;

            try
            {
                // Try and read local revision tag
                localRevTag = File.ReadAllText(LocalRevisionTagFilename);
            }
            catch (IOException)
            {
                // We couldn't read the file so create a new one
                using (TextWriter textWriter = File.CreateText(LocalRevisionTagFilename))
                    textWriter.Write("FORCE_UPDATE");

                return true;
            }

            // Check if rev tags differ
            return localRevTag.ToLower() != latestRemoteRevision.ToLower();
        }

        private static string GetRemoteString(string address, params object[] args)
        {
            string uri = String.Format(address, args);
            string contents;

            try
            {
                using (WebClient client = new WebClient { Encoding = Encoding.UTF8 })
                    contents = client.DownloadString(uri);
            }
            catch (Exception)
            {
                return "";
            }

            return contents;
        }
    }
}
