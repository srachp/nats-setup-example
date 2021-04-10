extern "C" {
//Here, I have my nats compiled library in /<home_dir>/tools/nats.c/nats_compiled_gcc9/include (which is included in makefile at the time of compilation)
#include "nats/nats.h"
}

#include <iostream>

class Connection {
    public:
        void TestPubSub(const char* mode) {
            natsSubscription    *sub = NULL;
            natsMsg             *msg = NULL;
            natsStatus          s = NATS_OK;
            natsConnection      *nc = NULL;
            natsOptions         *opts = NULL;
            const char          *servers[] = {"nats://<IP of machine where nats server is running>:4223"};

            s = natsOptions_Create(&opts);
            if (s == NATS_OK)
                s = natsOptions_SetServers(opts, servers, 1);
            if (s == NATS_OK)
                // Set the secure flag.
                natsOptions_SetSecure(opts, true);
                // You may not need this, but suppose that the server certificate
                // is self-signed and you would normally provide the root CA, but
                // don't want to. You can disable the server certificate verification
                // like this:

                //This is an unauthenticated TLS (without the use of certificates)
                natsOptions_SkipServerVerification(opts, true);

                natsOptions_LoadCertificatesChain(opts, "client-cert.pem", "client-key.pem");
                s = natsConnection_Connect(&nc, opts);
            
            //s = natsConnection_ConnectTo(&nc, "nats://nalnxapp154.enterprisenet.org:4223");
            
            if (mode == "sub") {
                // Syncronous subscribe
                // Subscribe to topic named 'case'
                if (s == NATS_OK) {
                    s = natsConnection_SubscribeSync(&sub, nc, "case");
                }

                // Wait for messages until timeout
                //once it receives message, display on console and end program
                if (s == NATS_OK)
                    s = natsSubscription_NextMsg(&msg, sub, 10000); //wait for 10 seconds

                if (s == NATS_OK)
                {
                    printf("Received msg: %s - %.*s\n",
                            natsMsg_GetSubject(msg),
                            natsMsg_GetDataLength(msg),
                            natsMsg_GetData(msg));

                    // Destroy message that was received
                    natsMsg_Destroy(msg);
                }

            } else if (mode == "queuesub") {
                // Queue subscribe
                // Subscribe to queue named 'workers' and topic named 'case'
                if (s == NATS_OK) {
                    s = natsConnection_QueueSubscribeSync(&sub, nc, "case", "workers");
                }

                if (s == NATS_OK)
                    s = natsSubscription_NextMsg(&msg, sub, 10000); //wait for 10 seconds

                if (s == NATS_OK)
                {
                    printf("Received msg: %s - %.*s\n",
                            natsMsg_GetSubject(msg),
                            natsMsg_GetDataLength(msg),
                            natsMsg_GetData(msg));

                    // Destroy message that was received
                    natsMsg_Destroy(msg);
                }

            } else if (mode == "pub") {
                if (s == NATS_OK)
                {
                    s = natsConnection_PublishString(nc, "case", "hello!");
                }
            }

            // destroy objects that were created
            natsSubscription_Destroy(sub);
            natsConnection_Destroy(nc);
            natsOptions_Destroy(opts);        
        }
};


int main(int, char**) {
    Connection myConnObj;

    myConnObj.TestPubSub("pub");

}