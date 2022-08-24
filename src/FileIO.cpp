#include "fileio.h"

void FileIO::iterateDirectory(QString dirPath, std::vector<QString>& out_fileList) {
    QDir dir(dirPath);
    dir.setFilter(QDir::Files);
    QFileInfoList entries = dir.entryInfoList();
    for (QFileInfoList::ConstIterator entry = entries.begin(); entry != entries.end(); ++entry)
    {
        qDebug() << *entry;
        out_fileList.push_back((*entry).filePath());
    }
}


QString FileIO::getFileNameWithDialog_Loading(QString nameFilters) {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);

    QString fileName;
    QStringList fileNames;
    dialog.setNameFilter(nameFilters);	//filter for files with this extension
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory("./");

    //Obtain filename using Qt load in OS
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    if (fileNames.size() > 0) {
        fileName = fileNames[0];
        qDebug() << "Loading: " << fileName;
    }

    return fileName;
}


QString FileIO::getFileDirectoryWithDialog() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);

    QString fileName;
    QStringList fileNames;
    dialog.setViewMode(QFileDialog::Detail);

    //Obtain filename using Qt load in OS
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    if (fileNames.size() > 0) {
        fileName = fileNames[0];
        qDebug() << "Loading: " << fileName;
    }

    return fileName;
}

QString FileIO::getFileNameWithDialog_Saving(QString nameFilters) {
    //Save using Qt save in OS
    QString fileName =
        QFileDialog::getSaveFileName(Q_NULLPTR,
            "Save File",
            QString(),
            nameFilters);	//filter for files with this extension

    qDebug() << "Saving: " << fileName;
    return fileName;
}

class Line : public std::string
{
    friend std::istream& operator>>(std::istream& is, Line& line)
    {
        return std::getline(is, line);
    }
};

bool loadTextFile(const std::string& fileName, std::vector<std::string>& loaded) {
    std::ifstream ifs(fileName, std::ifstream::in);

    if (!ifs.good())
    {
        qDebug() << "exit(" << ifs.fail() << ") cannot open param file.";
        return false;
    }

    //Copy contents of file into a list
    std::copy(std::istream_iterator<Line>(ifs),
        std::istream_iterator<Line>(),
        std::back_inserter(loaded)
        );

    ifs.close();

    return true;
}

bool saveTextFile(const std::string& filename, const std::vector<std::string>& stringList) {
    std::string str;
    std::ofstream myfile;
    myfile.open(filename);

    //Save contents of list into a file
    for (std::string s : stringList) {
        myfile << s << "\n";
    }

    myfile.close();

    return true;
}

std::vector<std::string> FileIO::loadTextFileWithDialog(const QString& nameFilters) {
    std::vector<std::string> loaded;
    std::string filename = getFileNameWithDialog_Loading(nameFilters).toStdString();
    loadTextFile(filename, loaded);
    for (std::string s : loaded) {
        qDebug() << s.c_str();
    }
    return loaded;
}

void FileIO::saveTextFileWithDialog(const QString& nameFilters, const std::vector<std::string>& stringList) {
    std::vector<std::string> loaded;
    std::string filename = getFileNameWithDialog_Saving(nameFilters).toStdString();
    saveTextFile(filename, stringList);
}

bool FileIO::isFilePresent(const std::string& filename) {
    std::ifstream f(filename.c_str());
    return f.good();
}

bool FileIO::isFileExtensionPresent(const QString& filename, const QString& fileExtension) {
    QString extension = filename.split('.').last();
    if (extension == fileExtension) { return true; }
    else { return false; }
}

bool FileIO::isFileExtensionPresent(const QString& filename, const std::vector<QString>& fileExtensions) {
    for(int i = 0; i < fileExtensions.size(); i++) {
        if(isFileExtensionPresent(filename, fileExtensions[i])){
            return true;
        }
    }
    return false;
}


int loadFileDirectoryContent(const std::string& fileDir, std::vector<std::string>& out_fileList) {
    QDir dir(QString::fromStdString(fileDir));
    dir.setFilter(QDir::Files);
    QStringList entries = dir.entryList();
    for (QStringList::ConstIterator entry = entries.begin(); entry != entries.end(); ++entry)
    {
        out_fileList.push_back((*entry).toStdString());
    }
    return entries.size();
}


QString getImageFileName() {
    return "";
}

QImage loadImage(QString filename) {
    return QImage();
}

bool openImage() {
    return true;
}

