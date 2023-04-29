#include "pch.h"
/*
#include "LogonServerClient.h"


LogonServerClient::LogonServerClient(std::string host, int port, std::string username, std::string password, LoginCompletedCallBack callback)
    : mHost(host), mPort(port), mUsername(username), mPassword(password), loginCompletedCallBack(callback), prefix(username)
{
    Time::GetTime();
}

void LogonServerClient::Connect()
{
    string host = mHost;
    int port = mPort;

    regex DnsMatch("[a-zA-Z]");
    IPAddress ASAddr;

    try
    {
        if (regex_match(host, DnsMatch))
            ASAddr = Dns::GetHostEntry(host)->AddressList[0];
        else
            ASAddr = System::Net::IPAddress::Parse(host);

        IPEndPoint^ ASDest = gcnew IPEndPoint(ASAddr, port);

        mSocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);

        Log::WriteLine(LogType::Normal, "Attempting to connect to Logon Server at {0}:{1}", prefix, host, port);
        mSocket->Connect(ASDest);
    }
    catch (Exception^ ex)
    {
        Log::WriteLine(LogType::Error, "Exception Occured", prefix);
        Log::WriteLine(LogType::Error, "Message: {0}", prefix, ex->Message);
        Log::WriteLine(LogType::Error, "Stacktrace: {0}", prefix, ex->StackTrace);
        System::Console::WriteLine("Unable to connect to server.");
        if (loginCompletedCallBack != nullptr)
        {
            loginCompletedCallBack(1);
        }
        Disconnect();
        return;
    }

    Log::WriteLine(LogType::Success, "Succesfully connected to Logon Server at {0}:{1}", prefix, host, port);

    Connected = true;
    pHandler = gcnew PacketHandler(this, prefix);
    pLoop = gcnew PacketLoop(this, mSocket, prefix);
    pHandler->Initialize();
    Authenticate();
}


void LogonServerClient::Authenticate()
{
    PacketOut packet(LogonServerOpCode::AUTH_LOGON_CHALLENGE);

    packet.Write((uint8_t)3);
    packet.Write((uint16_t)(30 + mUsername.length()));
    packet.Write((uint8_t)'W');
    packet.Write((uint8_t)'o');
    packet.Write((uint8_t)'W');
    packet.Write((uint8_t)'\0');

    packet.Write((uint8_t)Config::Version::major);
    packet.Write((uint8_t)Config::Version::minor);
    packet.Write((uint8_t)Config::Version::update);
    packet.Write((uint16_t)Config::Version::build);

    packet.Write((uint8_t)'6');
    packet.Write((uint8_t)'8');
    packet.Write((uint8_t)'x');
    packet.Write((uint8_t)'\0');

    packet.Write((uint8_t)'n');
    packet.Write((uint8_t)'i');
    packet.Write((uint8_t)'W');
    packet.Write((uint8_t)'\0');

    packet.Write((uint8_t)'B');
    packet.Write((uint8_t)'G');
    packet.Write((uint8_t)'n');
    packet.Write((uint8_t)'e');

    packet.Write((uint8_t)1);

    packet.Write((uint8_t)127);
    packet.Write((uint8_t)0);
    packet.Write((uint8_t)0);
    packet.Write((uint8_t)1);

    packet.Write((uint8_t)mUsername.length());
    packet.Write(mUsername.c_str(), mUsername.length());
    Send(packet);

    pLoop.Start();
}


void LogonServerClient::RequestRealmlist(RealmListCompletedCallBack realmListCompletedCallBack)
{
    this->realmListCompletedCallBack = realmListCompletedCallBack;

    PacketOut packet(LogonServerOpCode::REALM_LIST);
    packet.Write((uint8_t)0x00);
    Send(packet);

    // Most tricky code ever. It's so because retail server sends a lot of data in this packet...
    auto temp = pLoop.OnReceive(3);
    PacketIn p1(temp, true);
    auto temp2 = pLoop.OnReceive(p1.ReadUInt16());
    std::vector<uint8_t> temp3(temp.size() + temp2.size());
    std::copy(temp.begin(), temp.end(), temp3.begin());
    std::copy(temp2.begin(), temp2.end(), temp3.begin() + temp.size());

    p1 = PacketIn(temp3, false);
    HandlePacket(p1);
}

void LogonServerClient::Send(PacketOut packet)
{
    if (Connected)
    {
        packetNumber++;
        Log::WriteLine(LogType::Network, "Sending packet {0}. Length: {1} Number: {2}", prefix, packet.packetId.ToString(), packet.Lenght(), packetNumber);
        Log::WriteLine(LogType::Packet, "{0}", prefix, packet.ToHex(packetNumber));
        std::vector<uint8_t> Data = packet.ToArray();
        mSocket.send(Data.data(), Data.size());
    }
}


void AuthChallangeRequest(PacketIn packetIn)
{
    packetIn.ReadByte();
    byte error = packetIn.ReadByte();

    if (error != 0x00)
    {
        Log::WriteLine(LogType::Error, "Authentication error: {0}", prefix, (AccountStatus)error);

        if (loginCompletedCallBack != nullptr)
        {
            loginCompletedCallBack(2);
        }

        Disconnect();
        return;
    }

    B = BigInteger(packetIn.ReadBytes(32));   // Varies
    byte glen = packetIn.ReadByte();          // Length = 1
    g = packetIn.ReadBytes(glen);             // g = 7
    byte Nlen = packetIn.ReadByte();          // Length = 32
    N = packetIn.ReadBytes(Nlen);             // N = B79B3E2A87823CAB8F5EBFBF8EB10108535006298B5BADBD5B53E1895E644B89
    Salt = BigInteger(packetIn.ReadBytes(32));// Salt = 3516482AC96291B3C84B4FC204E65B623EFC2563C8B4E42AA454D93FCD1B56BA
    crcsalt = packetIn.ReadBytes(16);         // Varies

    BigInteger S;
    srp = Srp6(BigInteger(N), BigInteger(g));

    do
    {
        a = BigInteger::Random(19 * 8);
        A = srp.GetA(a);

        I = Srp6::GetLogonHash(mUsername, mPassword);

        BigInteger x = Srp6::Getx(Salt, I);
        BigInteger u = Srp6::Getu(A, B);
        S = srp.ClientGetS(a, B, x, u);
    } while (S < 0);

    mKey = Srp6::ShaInterleave(S);
    M = srp.GetM(mUsername, Salt, A, B, BigInteger(mKey));

    packetIn.ReadByte();

    Sha1Hash sha;
    byte[] files_crc;

    // Generate CRC/hashes of the Game Files
    if (Config::Retail)
        files_crc = GenerateCrc(crcsalt);
    else
        files_crc = new byte[20];

    // get crc_hash from files_crc
    sha = Sha1Hash();
    sha.Update(A);
    sha.Update(files_crc);
    byte[] crc_hash = sha.Final();

    PacketOut packet(LogonServerOpCode::AUTH_LOGON_PROOF);
    packet.Write(A);        // 32 bytes
    packet.Write(M);        // 20 bytes
    packet.Write(crc_hash); // 20 bytes
    packet.Write((byte)0);  // number of keys
    packet.Write((byte)0);  // unk (1.11.x)
    Send(packet);
}

void LogonServerClient::HandleLogonProof(PacketIn& packetIn)
{
    if (packetIn.ReadByte() == 0x00)
    {
        Log::WriteLine(LogType::Success, "Authenitcation successed. Requesting RealmList", prefix_);

        if (loginCompletedCallBack_ != nullptr)
        {
            loginCompletedCallBack_(0);
        }
        pLoop_->Stop();
    }
}

void LogonServerClient::HandleRealmlist(PacketIn& packetIn)
{
    uint16_t length = packetIn.ReadUInt16();
    uint32_t request = packetIn.ReadUInt32();
    int16_t realmscount = packetIn.ReadInt16();

    Log::WriteLine(LogType::Success, "Got information about {0} realms.", prefix, realmscount);
    Realm* realms = new Realm[realmscount];
    try
    {
        for (int i = 0; i < realmscount; i++)
        {
            realms[i].Type = packetIn.ReadByte();
            realms[i].Color = packetIn.ReadByte();
            packetIn.ReadByte(); // unk
            realms[i].Name = packetIn.ReadString();
            realms[i].Address = packetIn.ReadString();
            realms[i].Population = packetIn.ReadFloat();
            realms[i].NumChars = packetIn.ReadByte();
            realms[i].Language = packetIn.ReadByte();
            packetIn.ReadByte();
        }

        Realmlist = realms;
        HardDisconnect();

        if (realmListCompletedCallBack != nullptr)
            realmListCompletedCallBack(0);
    }
    catch (const std::exception& ex)
    {
        Log::WriteLine(LogType::Error, "Exception Occured", prefix);
        Log::WriteLine(LogType::Error, "Message: {0}", prefix, ex.what());
        Disconnect();
        if (realmListCompletedCallBack != nullptr)
            realmListCompletedCallBack(1);
    }
}

std::vector<uint8_t> GenerateCrc(const std::vector<uint8_t>& crcsalt) 
{
    std::string files[] = { "WoW.bin", "DivxDecoder.bin", "Unicows.bin" };
    SHA_CTX sha_ctx;

    std::vector<uint8_t> buffer1(0x40, 0x36);
    std::vector<uint8_t> buffer2(0x40, 0x5c);

    for (int i = 0; i < crcsalt.size(); ++i) {
        buffer1[i] ^= crcsalt[i];
        buffer2[i] ^= crcsalt[i];
    }

    SHA1_Init(&sha_ctx);
    SHA1_Update(&sha_ctx, buffer1.data(), buffer1.size());

    for (const auto& filename : files) {
        std::filesystem::path filepath = std::filesystem::path("crc") / filename;
        if (!std::filesystem::exists(filepath)) {
            std::ostringstream oss;
            oss << "CRC File " << filename << " doesn't exist!";
            throw std::runtime_error(oss.str());
        }

        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> buffer(size);
        if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
            throw std::runtime_error("Error reading CRC file");
        }
        SHA1_Update(&sha_ctx, buffer.data(), buffer.size());
    }

    std::vector<uint8_t> hash1(SHA_DIGEST_LENGTH);
    SHA1_Final(hash1.data(), &sha_ctx);

    SHA1_Init(&sha_ctx);
    SHA1_Update(&sha_ctx, buffer2.data(), buffer2.size());
    SHA1_Update(&sha_ctx, hash1.data(), hash1.size());

    std::vector<uint8_t> result(SHA_DIGEST_LENGTH);
    SHA1_Final(result.data(), &sha_ctx);

    return result;
}

void LogonServerClient::HandlePacket(PacketIn packet) 
{
    Log.WriteLine(LogType::Packet, "{0}", prefix, packet.ToHex());
    pHandler.HandlePacket(packet);
}

void LogonServerClient::Disconnect() 
{
    // do nothing
}

void LogonServerClient::HardDisconnect() 
{
    Connected = false;

    if (pLoop != nullptr)
        //pLoop.Stop();

        if (mSocket != nullptr)
            mSocket->Close();
}

LogonServerClient::~LogonServerClient() 
{
    HardDisconnect();
}
*/