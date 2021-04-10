# nats-setup-example
set up a nats server and client (with Publish and Subscribe modes)
## Introduction

The NATS C Client is part of NATS, written in C. For this nats c client library to Publish/subscribe messages, there should be a NATS server running.

This document provides information about compiling nats-c client library in Linux platform. This library was compiled using CMake3.

## Minimum Requirements

* RHEL 7.x or above

## Prerequisite

The following tools or libraries are required to compile the libraries in this folder.

| Tool | Requirement | Description | Command |
| ------------- | ------------- | ------------- | ------------- |
| [CMake 3](https://cmake.org/) | Required | It is required to compile certain libraries | yum install cmake3 |

### Compiling 64-bit library

``` bash

# Create a directory for 64-bit build
mkdir nats_c_build64
cd nats_c_build64

# Generate the Unix makefile from the cmake script
# NOTE: We do not need a NATS streaming client at the time of writing.
# Hence, we pass the flag -DNATS_BUILD_STREAMING=OFF. If we do need a NATS 
# streaming client, we would need to build protobuf before being able to 
# build nats-c client.
cmake ../ -DCMAKE_INSTALL_PREFIX="/<home_dir>/nats_client" -DCMAKE_CXX_FLAGS=-m64 -DCMAKE_C_FLAGS=-m64 -DCMAKE_SHARED_LINKER_FLAGS=-m64 -DCMAKE_C_COMPILER=/opt/GCC9/compiler/bin/gcc -DCMAKE_CXX_COMPILER=/opt/GCC9/compiler/bin/g++ -DNATS_BUILD_STREAMING=OFF -DNATS_BUILD_WITH_TLS=ON

# To build with another version of gcc, add the following to the cmake3 command above.
-DCMAKE_C_COMPILER=/<install_path_to_gcc_compiler>/bin/gcc DCMAKE_CXX_COMPILER=/<install_path_to_gcc_compiler>/bin/g++

# Building the source code and installing it
make -j24 # if the server has 24 CPU cores; else, give the number of cores present or simply -j1
make install

```

This will place the built lib*.* and include files in the install path specified in cmake command (-DCMAKE_INSTALL_PREFIX flag)

**/<home_dir>/nats_client/lib64/**

**/<home_dir>/nats_client/include/**

*The built library files have been placed in a common path in /opt/GCC9/nats-c/lib64/ so it can be accessed by all developers without the need to rebuild themselves.*


### Compiling 32-bit library

``` bash

# Create a directory for 64-bit build
mkdir nats_c_build32
cd nats_c_build32

# Generate the Unix makefile from the cmake script
# NOTE: We do not need a NATS streaming client at the time of writing.
# Hence, we pass the flag -DNATS_BUILD_STREAMING=OFF. If we do need a NATS 
# streaming client, we would need to build protobuf before being able to 
# build nats-c client.
cmake ../ -DCMAKE_INSTALL_PREFIX="/<home_dir>/nats/nats_client_32" -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_C_FLAGS=-m32 -DCMAKE_SHARED_LINKER_FLAGS=-m32 -DCMAKE_C_COMPILER=/opt/GCC9/compiler/bin/gcc -DCMAKE_CXX_COMPILER=/opt/GCC9/compiler/bin/g++ -DNATS_BUILD_STREAMING=OFF -DNATS_BUILD_WITH_TLS=ON

# To build with another version of gcc, add the following to the cmake3 command above.
-DCMAKE_C_COMPILER=/<install_path_to_gcc_compiler>/bin/gcc DCMAKE_CXX_COMPILER=/<install_path_to_gcc_compiler>/bin/g++

# Building the source code and installing it
make -j24 # if the server has 24 CPU cores; else, give the number of cores present or simply -j1
make install

```

This will place the built lib*.* and include files in the install path specified in cmake command (-DCMAKE_INSTALL_PREFIX flag)

**/<home_dir>/nats_client_32bit/lib/**

**/<home_dir>/nats_client_32bit/include/**

*The built library files have been placed in a common path in /opt/GCC9/nats-c/lib32/ so it can be accessed by all developers without the need to rebuild themselves.*


## How to test the installation

**Note:** The code in the nats-c folder links only to the 64-bit version of the library.

``` bash

# Navigate to Poco test folder
cd nats-c
mkdir build64
cd build64

# Generate the Unix makefile from the cmake script
cmake3 ../

# Building the source code and installing it
make -j24

# Running the executable
./nats

# Done
```

## NATS server start in TLS mode ON
In order to run the nats-c client example in this folder, we need to start a nats server with TLS ON since the example program starts a client with TLS ON. Follow the below steps to generate certificates and start nats server with `--tls` flag.

1. generate self signed certificate and key using openssl
``` bash
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365
```

2. You might encounter the below error because it needs the passphrase entered in above command to decrypt the key
``` bash
./nats-server --tls --tlscert=./server-cert.pem --tlskey=./server-key.pem -p 4223
nats-server: error parsing X509 certificate/key pair: tls: failed to parse private key
```

3. You could generate an unencrypted key with below command by entering the passphrase you entered in step 1.
``` bash
openssl rsa -in server-key.pem -out key.unencrypted.pem
Enter pass phrase for server-key.pem:
writing RSA key
```

4. Start NATS server with unencrypted key generated in step 3.
``` bash
./nats-server --tls --tlscert=./server-cert.pem --tlskey=./key.unencrypted.pem -p 4223 -D

[25091] 2021/03/18 22:20:16.985414 [INF] Starting nats-server version 2.1.9
[25091] 2021/03/18 22:20:16.985482 [DBG] Go build version go1.14.10
[25091] 2021/03/18 22:20:16.985486 [INF] Git commit [7c76626]
[25091] 2021/03/18 22:20:16.985755 [INF] Listening for client connections on 0.0.0.0:4223
[25091] 2021/03/18 22:20:16.985760 [INF] TLS required for client connections
[25091] 2021/03/18 22:20:16.985763 [INF] Server id is NDKGTMGE5JSFEAIDWUIN4T4OLC4OZH3GKM3HS3V6Z7CKBEFSQUQK3WAG
[25091] 2021/03/18 22:20:16.985766 [INF] Server is ready
[25091] 2021/03/18 22:20:16.985774 [DBG] Get non local IPs for "0.0.0.0"
[25091] 2021/03/18 22:20:16.986587 [DBG]  ip=10.7.54.164
[25091] 2021/03/18 22:20:16.986622 [DBG]  ip=10.18.89.149
[25091] 2021/03/18 22:20:16.986652 [DBG]  ip=172.17.0.1
[25091] 2021/03/18 22:20:16.986706 [DBG]  ip=172.22.0.1
[25091] 2021/03/18 22:20:16.986944 [DBG]  ip=172.18.0.1
[25091] 2021/03/18 22:21:56.754631 [DBG] 10.7.54.164:56820 - cid:1 - Client connection created
[25091] 2021/03/18 22:21:56.755014 [DBG] 10.7.54.164:56820 - cid:1 - Starting TLS client connection handshake
[25091] 2021/03/18 22:21:56.771499 [DBG] 10.7.54.164:56820 - cid:1 - TLS handshake complete
[25091] 2021/03/18 22:21:56.771535 [DBG] 10.7.54.164:56820 - cid:1 - TLS version 1.2, cipher suite TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
[25091] 2021/03/18 22:21:56.772184 [DBG] 10.7.54.164:56820 - cid:1 - Client connection closed: Client Closed
```

## Note:
NATS Client and NATS server need to be started in tls mode for a successful TLS handshake. It does not work if either oh them has TLS set to OFF.

## Test results (for running NATS servers and clients on cloud and on-prem)
Here, I have set up three NATS servers on Cloud and two NATS clients (one on cloud, one on-prem). This is essentially to demonstrate that NATS servers and clients can reside anywhere as long as they are able to `ping` or `ssh` to each other (or in other words, belong to the same VPN network).
```
server 1 (cloud)- (seed server) 
10.255.49.4

server 2 (cloud)- 
10.255.49.5

server 3 (cloud)- 
10.255.49.6

client 1 (cloud)- 
10.255.49.7

client 2 (on-prem)-
x.x.x.x
```



We then run the below commands to start NATS server on each of the three servers:
```
./nats-server -p 4222 -cluster nats://10.255.49.4:4248 -D

./nats-server -p 4222 -cluster nats://10.255.49.5:4248 -routes nats://10.255.49.4:4248 -D

./nats-server -p 4222 -cluster nats://10.255.49.6:4248 -routes nats://10.255.49.4:4248 -D
```


On the clients side:

Here I have client 1 (on-cloud) in publish mode, client2 on-prem in subscribe mode.
```
**client 1 (publishing on cloud)**: ./nats-sub -s nats://10.255.49.4:4222 case
**client 2 (subscribing on prem)**: ./nats-pub -s nats://10.255.49.4:4222 case messageForServer1 (message goes through server1 to clients)
			./nats-pub -s nats://10.255.49.5:4222 case messageForServer2 (message goes through server2 to clients)
			./nats-pub -s nats://10.255.49.6:4222 case messageForServer3 (message goes through server3 to clients)
			./nats-pub -s "nats://10.255.49.4:4222,nats://10.255.49.5:4222,nats://10.255.49.6:4222" case messageForAllServers (message goes through any random server to the clients)
```
Here I have client 1 (on-cloud) in subcribe mode, client2 on-prem in publish mode.			
```
client1 (subscribing on cloud): ./nats-sub -s nats://10.255.49.4:4222 case
client2 (publishing on prem): ./nats-pub -s nats://10.255.49.4:4222 case messageForServer1 (message goes through server1 to clients)
			./nats-pub -s nats://10.255.49.5:4222 case messageForServer2 (message goes through server2 to clients)
			./nats-pub -s nats://10.255.49.6:4222 case messageForServer3 (message goes through server3 to clients)
			./nats-pub -s "nats://10.255.49.4:4222,nats://10.255.49.5:4222,nats://10.255.49.6:4222" case messageForAllServers (message goes through any random server to the clients)
```

The advantage of having multiplpe NATS servers is to eliminate single point failure. 

For example, here I have stopped NATS server on one of the servers.
stopped server 1:

* The subscribing client immediately reconnected to another server in the cluster
* The publishing client cannot connect to server 1, but can connect to server 2 & 3
Meanwhile, other servers in the cluster continuously try to reconnect to server 1 every one second (this time is configurable too)

``` c++
//we can disable all reconnects with the below function
s = natsOptions_Create(&opts);
if (s == NATS_OK)
    s = natsOptions_SetAllowReconnect(opts, false);
if (s == NATS_OK)
    s = natsConnection_Connect(&conn, opts);
```
NATS 
## References
* [Compiling nats c client](https://github.com/nats-io/nats.c)
* [NATS C client documentation](http://nats-io.github.io/nats.c/index.html)
* [Writing Readme](https://confluence.atlassian.com/bitbucketserver/markdown-syntax-guide-776639995.html)