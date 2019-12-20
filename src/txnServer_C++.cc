#include "sdkd_internal.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <iostream>

#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "txn_server.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;
using txnService::txn;
using txnService::conn_info;
using txnService::txn_req;
using txnService::APIResponse;
using txnService::Empty;

using namespace std;
using namespace CBSdkd;

// Logic and data behind the server's behavior.
class txnImpl final : public txn::Service  {
    Handle *cur_handle = NULL;

    Status create_conn(ServerContext* context, const txnService::conn_info* request,APIResponse* msg) override {
        cout << "Got request for createConn"<< std::endl;
        if(cur_handle != NULL){
            cout << "Connection already exists"<< std::endl;
            msg->set_apisuccessstatus(true);
            msg->set_apistatusinfo("Connection already exists");
        }else{
            try {
                Error err = 0;
                cur_handle = new Handle(request);
                cout <<"Created Handle . Now establishing connection"<< std::endl;
                if (!cur_handle->connect(&err)) {
                   cout <<"Couldn't establish initial LCB connection"<< std::endl;
                }
                cout <<"cur_handle->getLcb()" << cur_handle->getLcb() << std::endl;
            }catch (const std::exception& e){
                cout << "Exception while creating connection: "<< e.what()<< std::endl;
            }
            if(cur_handle == NULL){
                cout << "Unable to create Connection"<< std::endl;
                msg->set_apisuccessstatus(false);
                msg->set_apistatusinfo("Unable to create Connection");
            }else{
                cout << "Created Connection successfully"<< std::endl;
                msg->set_apisuccessstatus(true);
                msg->set_apistatusinfo("Created Connection successfully");
            }

        }

        return Status::OK;
    }

    Status create_TxnFactory(ServerContext* context, const txnService::txn_req* request,APIResponse* msg) override {
        std::cout << "Got request for createConn"<< std::endl;
        //  msg.set_apistatusinfo("Server responsded with success");
        msg->set_apisuccessstatus(true);
        msg->set_apistatusinfo("Server responsded with success for create_TxnFactory");
        return Status::OK;
    }

    Status execute_txn(ServerContext* context, const txnService::txn_req* request,APIResponse* msg) override {
        std::cout << "Got request for createConn"<< std::endl;
        //  msg.set_apistatusinfo("Server responsded with success");
        msg->set_apisuccessstatus(true);
        msg->set_apistatusinfo("Server responsded with success for execute_txn");
        return Status::OK;
    }
};



void RunServer() {
    std::string server_address("0.0.0.0:8050");
    txnImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}
 