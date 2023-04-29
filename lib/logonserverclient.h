
#ifndef LOGONSERVERCLIENT_H
#define LOGONSERVERCLIENT_H
/*
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <openssl/sha.h>
#include"Log.h"

// #include "Shared.h"
// #include "Network.h"
// #include "Crypt.h"
// #include "Constants.h"

class LogonServerClient 
{
public:
    using LoginCompletedCallBack = std::function<void(uint32_t)>;
    using RealmListCompletedCallBack = std::function<void(uint32_t)>;

    LogonServerClient(const std::string& host, uint16_t port, const std::string& username,
        const std::string& password, LoginCompletedCallBack callback);
    ~LogonServerClient();

    void Connect();
    void RequestRealmlist(RealmListCompletedCallBack callback);
    void Disconnect();

private:
    // Connection Info
    std::string mHost;
    uint16_t mPort;
    std::string mUsername;
    std::string mPassword;
    bool mConnected = false;
    std::string prefix;
    uint32_t packetNumber = 0;

    //Packet Handling
    PacketHandler pHandler;
    std::unique_ptr<PacketLoop> pLoop = nullptr;
    WoWCrypt mCrypt;

    // Client stuff
    std::vector<Realm> realmlist;
    std::vector<Char> charlist;

    // Argh, huge code of math variables here, just copy-pasted from boogiebot.
    Srp6 srp;       // http://srp.stanford.edu/design.html  <- SRP6 information
    BigInteger A;   // My public key?
    BigInteger B;   // Server's public key
    BigInteger a;   // my random number, used to initalize A from g and N.
    std::vector<uint8_t> I;       // Hash of "username:password"
    BigInteger M;   // Combination of... everything!
    std::vector<uint8_t> M2;      // M2 is the combination of the server's everything to proof with ours (which we don't actually do, cause we trust blizzard, right?)
    std::vector<uint8_t> N;       // Modulus for A and B
    std::vector<uint8_t> g;       // base for A and B
    std::vector<uint8_t> mKey;    // Yay! The key!

    BigInteger Salt;    // Server provided salt
    std::vector<uint8_t> crcsalt;     // Server provided crcsalt for file crc's.

    std::unique_ptr<Socket> mSocket = nullptr;
    std::unique_ptr<TextWriter> tw;

    LoginCompletedCallBack loginCompletedCallBack;
    RealmListCompletedCallBack realmListCompletedCallBack;

    void Authenticate();
    void Send(PacketOut packet);
    std::vector<uint8_t> GenerateCrc(const std::vector<uint8_t>& crcsalt);
    void HandlePacket(PacketIn& packet);
    void HardDisconnect();

    // Packet handlers
    void AuthChallangeRequest(PacketIn& packetIn);
    void HandleLogonProof(PacketIn& packetIn);
    void HandleRealmlist(PacketIn& packetIn);
};

#endif // LOGONSERVERCLIENT_H
*/