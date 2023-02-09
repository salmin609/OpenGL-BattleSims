using AresTool.Results;
using Ookii.Dialogs.Wpf;
using System;
using System.Collections.ObjectModel;
using System.Diagnostics.Metrics;
using System.IO;
using System.Linq;
using System.Windows;
using System.Xml.Linq;
using static System.Net.Mime.MediaTypeNames;

namespace AresTool
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string m_startFolder;

        public ObservableCollection<string> m_directoryList;
        public ObservableCollection<ChangeListItem> m_changedListItems;
        public ObservableCollection<ImplementListItem> m_implementListItems;
        //public ObservableCollection<string> DirectoryList
        //{
        //    get
        //    {
        //        if(directoryList == null)
        //            directoryList= new ObservableCollection<string>();
        //        return directoryList;
        //    }
        //}
        public MainWindow()
        {
            InitializeComponent();
            m_startFolder = string.Empty;
            m_directoryList = new ObservableCollection<string>();
            m_changedListItems = new ObservableCollection<ChangeListItem>();
            m_implementListItems = new ObservableCollection<ImplementListItem>();
            listBox.ItemsSource= m_directoryList;
            changedListBox.ItemsSource= m_changedListItems;
            implmentListBox.ItemsSource = m_implementListItems;
        }

        private void Refresh()
        {
            
        }


        private void button_Click(object sender, RoutedEventArgs e)
        {

        }

        private void more_Click(object sender, RoutedEventArgs e)
        {
            VistaFolderBrowserDialog fbd = new VistaFolderBrowserDialog();
            fbd.UseDescriptionForTitle= true;
            fbd.Description = "Select a directory to start the search in";
            if(fbd.ShowDialog() == true)
            {
                m_startFolder = fbd.SelectedPath;
                directoryBox.Text = m_startFolder;
            }
            if(!string.IsNullOrWhiteSpace(m_startFolder))
            {
                Clear();
                ChangedClear();
                ImplementClear();
                DirectoryInfo di = new DirectoryInfo(m_startFolder);


                ObservableCollection<string> existNames = new ObservableCollection<string>();
                // Read recent file and set
                if (File.Exists(m_startFolder + "\\ImplementedFileList.ipl"))
                {
                    foreach (string line in File.ReadLines(m_startFolder + "\\ImplementedFileList.ipl"))
                    {
                        string currLineName = Path.GetFileNameWithoutExtension(line);
                        existNames.Add(currLineName);
                    }
                }

                

                foreach (FileInfo fi in di.GetFiles())
                {
                    if (Path.GetExtension(fi.Name) == ".dae")
                    {
                        string fileName = Path.GetFileNameWithoutExtension(fi.Name);
                        m_directoryList.Add(fileName);

                        if (!fileName.Contains("_"))
                        {
                            AddToChangedList(fileName);
                        }

                        //// IMPLEMENT LIST
                        // Add to Implement List
                        ImplementListItem implItem = new ImplementListItem();
                        implItem.fileNameBlock.Text = fileName;
                        if(existNames.Contains(fileName))
                        {
                            implItem.WasExisted = true;
                            implItem.existCheckBox.IsChecked= true;
                        }
                        m_implementListItems.Add(implItem);

                    }
                }
            }
        }

        private void AddToChangedList(string fileName_/*Just a name*/)
        {
            foreach(var it in m_changedListItems)
            {
                if(it.originalName.Equals(fileName_)) { return; }
            }

            ChangeListItem item = new ChangeListItem();
            item.fileName.Text = fileName_;
            item.originalName= fileName_;
            m_changedListItems.Add(item);

            m_changedListItems = new ObservableCollection<ChangeListItem>(m_changedListItems.OrderBy(x => x.originalName));

            changedListBox.ItemsSource = m_changedListItems;
        }
        public void Clear()
        {
            m_directoryList.Clear();
        }
        public void ChangedClear()
        {
            m_changedListItems.Clear();
        }

        public void ImplementClear()
        {
            m_implementListItems.Clear();
        }

        private void addButton_Click(object sender, RoutedEventArgs e)
        {
            if(listBox.SelectedItem != null)
            {
                string currName = listBox.SelectedItem.ToString();
                AddToChangedList(currName);
            }
            //changedListBox.ItemsSource= changedList;
        }
        private void listBox_DoubleClicked(object sender, RoutedEventArgs e)
        {
            try
            {
                if (listBox.SelectedItem != null)
                {
                    string currName = listBox.SelectedItem.ToString();
                    AddToChangedList(currName);
                }
            }
            catch (Exception err) { MessageBox.Show($"{err.Message}"); }
}

        private void changedListBox_DoubleClicked(object sender, RoutedEventArgs e)
        {
            var currItem = changedListBox.SelectedItem;
            if (currItem != null) { }
        }

        private void clearButton_Click(object sender, RoutedEventArgs e)
        {
            ChangedClear();
        }

        private void removeButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (changedListBox.SelectedItem != null)
                {
                    //MessageBox.Show($"{changedListBox.SelectedIndex}");
                    m_changedListItems.Remove(m_changedListItems[changedListBox.SelectedIndex]);
                }
            }
            catch(Exception err) { MessageBox.Show($"{err.Message}"); }
        }

        private void searchBox_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
        {
            if (string.IsNullOrEmpty(searchBox.Text) || string.IsNullOrEmpty(m_startFolder))
                return;

            try
            {
                Clear();
                DirectoryInfo di = new DirectoryInfo(m_startFolder);
                foreach (FileInfo fi in di.GetFiles())
                {
                    if (Path.GetExtension(fi.Name) == ".dae")
                    {
                        string fileName = Path.GetFileNameWithoutExtension(fi.Name);
                        if (fileName.Contains(searchBox.Text))
                        {
                            m_directoryList.Add(fileName);
                        }
                    }
                }
            }
            catch (Exception err) { MessageBox.Show($"{err.Message}"); }
        }

        private void applyButton_Click(object sender, RoutedEventArgs e)
        {
            foreach (var item in m_changedListItems)
            {
                string newName = m_startFolder + "\\" + item.fileName.Text + ".dae";
                if (File.Exists(newName))
                {
                    MessageBox.Show($"FILE NAME {item.fileName.Text}.dae ALREADY EXIST!!!!");
                    return;
                }
            }

            foreach (var item in m_changedListItems)
            {
                File.Move(m_startFolder + "\\" + item.originalName + ".dae", m_startFolder + "\\" + item.fileName.Text + ".dae");
            }
            MessageBox.Show($"Name Changed");

            Clear();
            ChangedClear();
            DirectoryInfo di = new DirectoryInfo(m_startFolder);
            foreach (FileInfo fi in di.GetFiles())
            {
                if (Path.GetExtension(fi.Name) == ".dae")
                {
                    string fileName = Path.GetFileNameWithoutExtension(fi.Name);
                    m_directoryList.Add(fileName);
                    if (!fileName.Contains("_"))
                    {
                        AddToChangedList(fileName);
                    }
                }
            }
        }

        private void saveButton_Click(object sender, RoutedEventArgs e)
        {
            string currImplList = string.Empty;

            foreach (var item in m_implementListItems)
            {
                if(item.existCheckBox.IsChecked == true)
                {
                    item.WasExisted = true;
                    currImplList += ("../Models/" + item.fileNameBlock.Text + ".dae\n");
                    item.setColorOfBox();
                }
                else
                {
                    item.WasExisted = false;
                    item.setColorOfBox();
                }
            }

            File.WriteAllTextAsync(m_startFolder + "\\ImplementedFileList.ipl", currImplList);
            MessageBox.Show($"New List Saved");
        }

        private void searchXbox_Click(object sender, RoutedEventArgs e)
        {
            searchBox.Text = string.Empty;
        }
    }
}
