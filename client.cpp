#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "myservice.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using myservice::MyService;
using myservice::HelloRequest;
using myservice::HelloReply;

class MyServiceClient {
public:
    MyServiceClient(std::shared_ptr<Channel> channel)
        : stub_(MyService::NewStub(channel)) {}

    std::string SayHello(const std::string& name) {

        HelloRequest request;
        request.set_name(name);

        HelloReply reply;

        ClientContext context;

        Status status = stub_->SayHello(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            std::cout << "RPC failed" << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<MyService::Stub> stub_;
};

int main() {

    MyServiceClient client(
        grpc::CreateChannel("localhost:50051",
        grpc::InsecureChannelCredentials())
    );

    std::string reply = client.SayHello("Paul");

    std::cout << "Server replied: " << reply << std::endl;

    return 0;
}