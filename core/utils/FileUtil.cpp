//
// Created by Lyndon on 2024/12/27.
//

#include "FileUtil.h"

void FileUtil::create_directory(const std::string& directory_path)
{
	QDir dir(QString::fromStdString(directory_path));
	if (!dir.exists()) {
		if (!dir.mkpath(".")) {
			throw FileOperationError("创建目录时出错");
		}
	}
}

void FileUtil::delete_directory(const std::string& directory_path)
{
	QDir dir(QString::fromStdString(directory_path));
	if (dir.exists())
	{
		if (!dir.removeRecursively())
		{
			throw FileOperationError("删除目录时出错");
		}
	}
}

std::vector<std::string> FileUtil::list_files(const std::string& directory_path)
{
	std::vector<std::string> file_list;
	QDir dir(QString::fromStdString(directory_path));
	if (dir.exists())
	{
		QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
		for (const QFileInfo& entry : entries)
		{
			file_list.push_back(entry.filePath().toStdString());
		}
	}
	return file_list;
}

void FileUtil::copy_file(const std::string& source_path, const std::string& destination_path)
{
	if (!QFile::copy(QString::fromStdString(source_path), QString::fromStdString(destination_path)))
	{
		throw FileOperationError("复制文件时出错");
	}
}

void FileUtil::move_file(const std::string& source_path, const std::string& destination_path)
{
	if (!QFile::rename(QString::fromStdString(source_path), QString::fromStdString(destination_path)))
	{
		throw FileOperationError("移动文件时出错");
	}
}

void FileUtil::delete_file(const QString& file_path)
{
	QFile file(file_path);
	if (file.exists())
	{
		if (!file.remove())
		{
			throw FileOperationError("删除文件时出错");
		}
	}
}

bool FileUtil::delete_file_with_status(const QString& file_path)
{
	QFile file(file_path);
	if (file.exists())
	{
		return file.remove();
	} else {
		return true;
	}
}