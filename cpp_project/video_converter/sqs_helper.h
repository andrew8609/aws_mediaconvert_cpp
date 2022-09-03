#pragma once

#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/ReceiveMessageRequest.h>
#include <aws/sqs/model/ReceiveMessageResult.h>
#include <aws/sqs/model/DeleteMessageRequest.h>
#include <iostream>

bool receiveMessage(const Aws::String& queue_url, LPSTR returnMessageHandler, LPSTR returnFileName  )
{
    // Let's make sure the request timeout is larger than the maximum possible
    // long poll time so that valid ReceiveMessage requests don't fail on long
    // poll queues
    Aws::Client::ClientConfiguration client_cfg;
    client_cfg.requestTimeoutMs = 30000;


    Aws::SQS::SQSClient sqs(client_cfg);

    Aws::SQS::Model::ReceiveMessageRequest rm_req;
    rm_req.SetQueueUrl(queue_url);
    rm_req.SetMaxNumberOfMessages(1);
    rm_req.SetVisibilityTimeout(600);

    auto rm_out = sqs.ReceiveMessage(rm_req);
    if (!rm_out.IsSuccess())
    {
        std::cout << "Error receiving message from queue " << queue_url << ": "
            << rm_out.GetError().GetMessage() << std::endl;
        return false;
    }

    const auto& messages = rm_out.GetResult().GetMessages();
    if (messages.size() == 0)
    {
        return false;
    }

    const auto& message = messages[0];
    std::cout << "Received message:" << std::endl;

    Aws::String messageBody = message.GetBody();

    if (!messageBody.empty()) {
        sprintf(returnMessageHandler, &(message.GetReceiptHandle()[0]));
        sprintf(returnFileName, &(messageBody[0]));
        return true;
    }

    return false;

    
}

bool deleteSQSMessage(const Aws::String& queue_url, Aws::String messageHandler) {
    
    Aws::Client::ClientConfiguration client_cfg;
    client_cfg.requestTimeoutMs = 30000;

    Aws::SQS::SQSClient sqs(client_cfg);

    Aws::SQS::Model::DeleteMessageRequest dm_req;
    dm_req.SetQueueUrl(queue_url);
    dm_req.SetReceiptHandle(messageHandler);

    auto dm_out = sqs.DeleteMessage(dm_req);
    if (dm_out.IsSuccess())
    {
        std::cout << "Successfully deleted message " << " from queue " << queue_url << std::endl;
        return true;
    }
    else
    {
        std::cout << "Error deleting message from queue " << queue_url << ": " <<
            dm_out.GetError().GetMessage() << std::endl;
        return false;
    }
}


