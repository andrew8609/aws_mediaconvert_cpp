#pragma once

#include <Windows.h>

LPCSTR region = "us-east-1";

LPCSTR folderPath720 = "E:\\Drop\\720";
LPCSTR folderOutputPath720 = "E:\\MP2-720";

LPCSTR folderPath1080 = "E:\\Drop\\1080";
LPCSTR folderOutputPath1080 = "E:\\MP2-1080";

LPCSTR bucket720Input = "htc720";
LPCSTR bucket1080Input = "htc1080";

LPCSTR bucket720Output = "htc720output";
LPCSTR bucket1080Output = "htc1080output";

LPCSTR sqsQueueUrl720 = "https://sqs.us-east-1.amazonaws.com/627296207923/ConversionCompleted720";
LPCSTR sqsQueueUrl1080 = "https://sqs.us-east-1.amazonaws.com/627296207923/ConversionCompleted1080";

