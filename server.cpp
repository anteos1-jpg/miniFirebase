#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "myservice.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using myservice::MyService;
using myservice::HelloRequest;
using myservice::HelloReply;

class MyServiceImpl final : public MyService::Service {
public:
    Status SayHello(ServerContext* context,
                    const HelloRequest* request,
                    HelloReply* reply) override {

        std::string prefix("Hello ");
        reply->set_message(prefix + request->name());

        std::cout << "Received request from: " << request->name() << std::endl;

        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    MyServiceImpl service;

    ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());

    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main() {
    RunServer();
    return 0;
}