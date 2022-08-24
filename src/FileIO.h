#ifndef FILEIO_H
#define FILEIO_H
#pragma once

#include <iostream>
#include <fstream>      // std::ifstream
#include <sstream>
#include <algorithm>

#include <QString>
#include <QImage>
#include <QDir>
#include <QFileDialog>

namespace FileIO
{
    void iterateDirectory(QString dirPath, std::vector<QString>& out_fileList);
    QString getFileNameWithDialog_Loading(QString nameFilters);
    QString getFileNameWithDialog_Saving(QString nameFilters);
    QString getFileDirectoryWithDialog();
    std::vector<std::string> loadTextFileWithDialog(const QString& nameFilters);
    void saveTextFileWithDialog(const QString& nameFilters, const std::vector<std::string>& stringList);
    bool isFilePresent(const std::string& filename);
    bool isFileExtensionPresent(const QString& filename, const QString& fileExtension);
    bool isFileExtensionPresent(const QString& filename, const std::vector<QString>& fileExtensions);
    int loadFileDirectoryContent(const std::string& fileDir, std::vector<std::string>& out_fileList);
};

#endif // FILEIO_H
