
const { DeleteObjectCommand } = require("@aws-sdk/client-s3");
const { SendMessageCommand } = require("@aws-sdk/client-sqs");

const { SQSClient } = require("@aws-sdk/client-sqs");
const { S3Client } = require("@aws-sdk/client-s3");

const path = require("path");

exports.handler = async (event) => {

    const REGION = "us-east-1"; //e.g. "us-east-1"
    const s3Client = new S3Client({ region: REGION });

    const fileName = event.Records[0].s3.object.key;

    const deleteFileName = path.parse(fileName) + ".mov";

    var params = { Bucket: "htc720", Key: deleteFileName };

    try {
        const data = await s3Client.send(new DeleteObjectCommand(params));
        console.log("Success. Object deleted.", data);
    } catch (err) {
        console.log("Error", err);
        return false;
    }

    // Create SQS service object.
    const sqsClient = new SQSClient({ region: REGION });
    // Set the parameters
    const sqsParams = {
        DelaySeconds: 3,
        MessageAttributes: {

        },
        MessageBody: fileName,
        QueueUrl: "https://sqs.us-east-1.amazonaws.com/627296207923/ConversionCompleted720" 
    };

    try {
        const data = await sqsClient.send(new SendMessageCommand(sqsParams));
        console.log("Success, message sent. MessageID:", data.MessageId);
        return true;
      } catch (err) {
        console.log("Error", err);
      }
}