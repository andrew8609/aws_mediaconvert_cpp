
#include <fstream>
#include <iostream>
#include "thread_helper.h"
#include <aws/core/auth/AWSCredentialsProvider.h>

int main() {

    Aws::String region;
    Aws::String folderPath720;
    Aws::String folderOutputPath720;
    Aws::String folderPath1080; 
    Aws::String folderOutputPath1080;
    Aws::String bucket720Input;
    Aws::String bucket1080Input;
    Aws::String bucket720Output;
    Aws::String bucket1080Output;
    Aws::String sqsQueueUrl720;
    Aws::String sqsQueueUrl1080;

    Aws::SDKOptions options;
    Aws::InitAPI(options);
    
    // parse settings.txt file
    std::ifstream file("settings.txt");
    std::string line;
    std::string word;
    int index = 0;
    int count = 0;
    std::string variables[12];
    

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        
        while (ss >> word) {
            if (count == 1) {
                variables[index] = word;
                count = 0;
                continue;
            }
            count++;
        }
        index++;
    }

    region = variables[0];
    folderPath720 = variables[1];
    folderOutputPath720 = variables[2];
    folderPath1080 = variables[3];
    folderOutputPath1080 = variables[4];
    bucket720Input = variables[5];
    bucket1080Input = variables[6];
    bucket720Output = variables[7];
    bucket1080Output = variables[8];
    sqsQueueUrl720 = variables[9];
    sqsQueueUrl1080 = variables[10];


    uploadTimerStart(uploadFile, bucket720Input, folderPath720, region, 1000);
    uploadTimerStart(uploadFile, bucket1080Input, folderPath1080, region, 1000);
    getSQSMessageTimerStart(getFinishedMessage, sqsQueueUrl720, bucket720Output, folderOutputPath720, region, 1000);
    getSQSMessageTimerStart(getFinishedMessage, sqsQueueUrl1080, bucket1080Output, folderOutputPath1080, region, 1000);

    while (true);
}