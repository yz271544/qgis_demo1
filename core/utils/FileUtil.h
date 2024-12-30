//
// Created by Lyndon on 2024/12/27.
//

#ifndef FILEUTIL_H
#define FILEUTIL_H
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#endif

class FileOperationError : public std::exception
{
private:
	std::string message;

public:
	FileOperationError(const std::string& msg) : message(msg)
	{
	}

	const char* what() const noexcept override
	{
		return message.c_str();
	}
};

class FileUtil
{
public:
	static void create_directory(const std::string& directory_path);


	static void delete_directory(const std::string& directory_path);


	static std::vector<std::string> list_files(const std::string& directory_path);


	static void copy_file(const std::string& source_path, const std::string& destination_path);


	static void move_file(const std::string& source_path, const std::string& destination_path);


	static void delete_file(const QString& file_path);
};


#endif //FILEUTIL_H
