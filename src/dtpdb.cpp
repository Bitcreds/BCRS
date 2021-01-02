// Copyright (c) 2017-2021 Bitcreds Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "dtpdb.h"

#include "util.h"

CDTPDB::CDTPDB(size_t nCacheSize, bool fMemory, bool fWipe) : CDBWrapper(GetDataDir() / "dtp", nCacheSize, fMemory, fWipe) {}

std::string CDTPDB::GetIPFSofDTP(const std::string &dtpAddress) {
    std::string ipfsHash;
    int blockHeight;

    ReadDTPAssociation(dtpAddress, ipfsHash, blockHeight);

    return ipfsHash;
}

bool CDTPDB::ReadDTPAssociation(const std::string &dtpAddress, std::string &ipfsHash, int &blockHeight) {
    return Read(dtpAddress, std::make_pair(ipfsHash, blockHeight));
}

bool CDTPDB::WriteDTPAssociation(const std::string &dtpAddress, std::string &ipfsHash, int &blockHeight) {
    CDBBatch batch(&GetObfuscateKey());
    batch.Write(dtpAddress, std::make_pair(ipfsHash, blockHeight));
    return WriteBatch(batch);
}

bool CDTPDB::UpdateDTPAssociation(const std::string &dtpAddress, std::string &newIpfsHash) {
    int &blockHeight;
    std::string &oldIpfsHash;

    Read(dtpAddress, std::make_pair(oldIpfsHash, blockHeight));

    CDBBatch batch(&GetObfuscateKey());
    batch.Write(dtpAddress, std::make_pair(newIpfsHash, blockHeight));
    return WriteBatch(batch);
}

bool CDTPDB::EraseDTPAssociation(const std::string &dtpAddress) {
    CDBBatch batch(&GetObfuscateKey());
    batch.Erase(dtpAddress);
    return WriteBatch(batch);
}