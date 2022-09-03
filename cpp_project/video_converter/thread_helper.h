#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <filesystem>
#include <Shlwapi.h>
#include <Windows.h>

#include "s3_helper.h"
#include "sqs_helper.h"

void uploadTimerStart(std::function<void(Aws::String, Aws::String, const Aws::String&)> func, Aws::String bucketName, Aws::String folderPath, const Aws::String& region, unsigned int interval)
{
    std::thread([func, bucketName, folderPath, region, interval]() {
        while (true)
        {
            func(bucketName, folderPath, region);
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
        }).detach();
}

void getSQSMessageTimerStart(std::function<void(Aws::String, Aws::String, Aws::String, const Aws::String& )> func, Aws::String queue_url, Aws::String bucketName, Aws::String folderOutputPath, const Aws::String& region, unsigned int interval)
{
    std::thread([func, queue_url, bucketName, folderOutputPath, region, interval]() {
        while (true)
        {
            func(queue_url, bucketName, folderOutputPath, region);
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
        }).detach();
}

void uploadFile(Aws::String bucketName, Aws::String folderPath, const Aws::String& region)
{
    WIN32_FIND_DATAA file;
    HANDLE search_handle = nullptr;
    
    if (!PathIsDirectoryEmptyA(folderPath.c_str())) {

        Aws::String wildPath = folderPath + "\\*";
        search_handle = FindFirstFileA(wildPath.c_str(), &file);

        if (search_handle != INVALID_HANDLE_VALUE) {

            do {
                if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    continue;
                }
                else {
                    if (strcmp(file.cFileName, "") == 0) {
                        continue;
                    }
                    else {
                        
                        // only upload and delete .mov file
                        if (!strcmp( PathFindExtensionA(file.cFileName), ".mov")) {
                            Aws::String filePath = folderPath + "\\" + file.cFileName;

                            if (uploadToS3(bucketName, filePath, file.cFileName, region)) {
                                DeleteFileA(filePath.c_str());
                            }
                         
                            break;
                        }
                        
                    }

                }
            } while (FindNextFileA(search_handle, &file));
        }
    }

}

void getFinishedMessage(Aws::String queue_url, Aws::String bucketName, Aws::String folderOutputPath, const Aws::String& region) {
    
    //Aws::String queue_url = queue_url;
    char returnMessageHandler[1000];
    char fileName[255];
    if (!receiveMessage(queue_url, returnMessageHandler, fileName)) {
        return;
    }
    if (!downloadFromS3(bucketName, fileName, folderOutputPath, region)) {
        return;
    }
    if (!DeleteObject(fileName, bucketName, region)) {
        return;
    }
    if (!deleteSQSMessage(queue_url, returnMessageHandler)) {
        return;
    }
    return;
}
