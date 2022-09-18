#pragma once

#include <fstream>
#include <sys/stat.h>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
//#include "const.h"

bool PutObject(const Aws::String& bucketName,
    const Aws::String& objectName,
    const Aws::String& fileName,
    const Aws::String& region)
{
    // Verify that the file exists.
    struct stat buffer;

    if (stat(objectName.c_str(), &buffer) == -1)
    {
        std::cout << "Error: PutObject: File '" <<
            objectName << "' does not exist." << std::endl;

        return false;
    }

    Aws::Client::ClientConfiguration config;

    if (!region.empty())
    {
        config.region = region;
    }

    Aws::S3::S3Client s3_client(config);

    Aws::S3::Model::PutObjectRequest request;

    request.SetBucket(bucketName);
    //We are using the name of the file as the key for the object in the bucket.
    //However, this is just a string and can set according to your retrieval needs.
    request.SetKey(fileName);

    std::shared_ptr<Aws::IOStream> input_data =
        Aws::MakeShared<Aws::FStream>("SampleAllocationTag",
            objectName.c_str(),
            std::ios_base::in | std::ios_base::binary);

    request.SetBody(input_data);

    Aws::S3::Model::PutObjectOutcome outcome =
        s3_client.PutObject(request);

    if (outcome.IsSuccess()) {

        std::cout << "Added object '" << objectName << "' to bucket '"
            << bucketName << "'." << std::endl;
        return true;
    }
    else
    {
        std::cout << "Error: PutObject: " <<
            outcome.GetError().GetMessage() << std::endl;

        return false;
    }
}

bool uploadToS3(Aws::String bucketName, Aws::String filePath, Aws::String fileName, const Aws::String& region) {

    //TODO: Change bucket_name to the name of a bucket in your account.
    const Aws::String bucket_name = bucketName;
    //TODO: Create a file called "my-file.txt" in the local folder where your executables are built to.
    const Aws::String object_name = filePath;

    if (!PutObject(bucket_name, object_name, fileName, region)) {
        return false;
    }     
    return true;
}

bool getObject(const Aws::String& objectKey,
    const Aws::String& fromBucket,
    Aws::String folderOutputPath,
    const Aws::String& region)
{
    Aws::Client::ClientConfiguration config;

    if (!((Aws::String)region).empty())
    {
        config.region = region;
    }

    Aws::S3::S3Client s3_client(config);

    Aws::S3::Model::GetObjectRequest object_request;
    object_request.SetBucket(fromBucket);
    object_request.SetKey(objectKey);

    Aws::S3::Model::GetObjectOutcome get_object_outcome =
        s3_client.GetObject(object_request);

    if (get_object_outcome.IsSuccess())
    {
        auto& retrieved_file = get_object_outcome.GetResultWithOwnership().
            GetBody();

        Aws::OFStream localFile;
        localFile.open(folderOutputPath + "\\" + objectKey, std::ios::out | std::ios::binary);
        localFile << retrieved_file.rdbuf();

        return true;
    }
    else
    {
        auto err = get_object_outcome.GetError();
        std::cout << "Error: GetObject: " <<
            err.GetExceptionName() << ": " << err.GetMessage() << std::endl;

        return false;
    }
}
bool downloadFromS3(Aws::String bucketName, Aws::String fileName, Aws::String folderOutputPath, const Aws::String& region) {

    if (!getObject(fileName, bucketName, folderOutputPath, region))
    {
        return false;
    }

    return true;
}

bool DeleteObject(const Aws::String& objectKey, const Aws::String& fromBucket, const Aws::String& region) {
    
    Aws::Client::ClientConfiguration clientConfig;
    if (!((Aws::String)region).empty()) {
        clientConfig.region = region;
    }

    Aws::S3::S3Client client(clientConfig);
    Aws::S3::Model::DeleteObjectRequest request;

    request.WithKey(objectKey)
        .WithBucket(fromBucket);

    Aws::S3::Model::DeleteObjectOutcome outcome =
        client.DeleteObject(request);

    if (!outcome.IsSuccess())
    {
        return false;
    }
    else
    {
        return true;
    }
}
