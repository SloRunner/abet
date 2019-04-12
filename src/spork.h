// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2016-2017 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SPORK_H
#define SPORK_H

#include "base58.h"
#include "key.h"
#include "main.h"
#include "net.h"
#include "sync.h"
#include "util.h"

#include "obfuscation.h"
#include "protocol.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

/*
    Don't ever reuse these IDs for other sporks
    - This would result in old clients getting confused about which spork is for what

    Sporks 11,12, and 16 to be removed with 1st zerocoin release
*/
#define SPORK_START 10001
#define SPORK_END 10025

#define SPORK_2_SWIFTTX 10001
#define SPORK_3_SWIFTTX_BLOCK_FILTERING 10002
#define SPORK_5_MAX_VALUE 10004
#define SPORK_7_MASTERNODE_SCANNING 10006
#define SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT 10007
#define SPORK_9_MASTERNODE_BUDGET_ENFORCEMENT 10008
#define SPORK_10_MASTERNODE_PAY_UPDATED_NODES 10009
#define SPORK_11_RESET_BUDGET 10010
#define SPORK_12_RECONSIDER_BLOCKS 10011
#define SPORK_13_ENABLE_SUPERBLOCKS 10012
#define SPORK_14_NEW_PROTOCOL_ENFORCEMENT 10013
#define SPORK_15_NEW_PROTOCOL_ENFORCEMENT_2 10014
#define SPORK_16_MN_WINNER_MINIMUM_AGE 10015
#define SPORK_17_NEW_PROTOCOL_ENFORCEMENT_3 10016 // Fake Stake Dont Touch
#define SPORK_18_NEW_PROTOCOL_ENFORCEMENT_4 10017
#define SPORK_19_BAD_ACTOR_ENFORCEMENT 10018           // Ban Address
#define SPORK_20_DISABLE_OBFUSCATION_ENFORCEMENT 10019 // Disable Obfuscation
#define SPORK_21_TREASURY_PAYMENT_ENFORCEMENT 10020    // Dev Fee
#define SPORK_22_SEGWIT_ACTIVATION 10021
#define SPORK_23_ZEROCOIN_MAINTENANCE_MODE 10022
#define SPORK_24_LOCK_INVALID_UTXO 10023
#define SPORK_25_SEGWIT_ON_COINBASE 10024
#define SPORK_26_NEW_COLLATERAL 10025

#define SPORK_2_SWIFTTX_DEFAULT 978307200                         //2001-1-1
#define SPORK_3_SWIFTTX_BLOCK_FILTERING_DEFAULT 1424217600        //2015-2-18
#define SPORK_5_MAX_VALUE_DEFAULT 1000                            //1000 ABET
#define SPORK_7_MASTERNODE_SCANNING_DEFAULT 978307200             //2001-1-1
#define SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT_DEFAULT 4070908800 //OFF
#define SPORK_9_MASTERNODE_BUDGET_ENFORCEMENT_DEFAULT 4070908800  //OFF
#define SPORK_10_MASTERNODE_PAY_UPDATED_NODES_DEFAULT 4070908800  //OFF
#define SPORK_11_RESET_BUDGET_DEFAULT 0
#define SPORK_12_RECONSIDER_BLOCKS_DEFAULT 0
#define SPORK_13_ENABLE_SUPERBLOCKS_DEFAULT 4070908800         //OFF
#define SPORK_14_NEW_PROTOCOL_ENFORCEMENT_DEFAULT 4070908800   //OFF
#define SPORK_15_NEW_PROTOCOL_ENFORCEMENT_2_DEFAULT 4070908800 //OFF
#define SPORK_16_MN_WINNER_MINIMUM_AGE_DEFAULT 8000
#define SPORK_17_NEW_PROTOCOL_ENFORCEMENT_3_DEFAULT 4070908800 //Dont touch this is tied to Fake Stake for History
#define SPORK_18_NEW_PROTOCOL_ENFORCEMENT_4_DEFAULT 4070908800 //OFF
#define SPORK_19_BAD_ACTOR_DEFAULT 4070908800                  // Banning nodes in invalidatelist.cpp
#define SPORK_20_DISABLE_OBFUSCATION_DEFAULT 4070908800        // Disable Obfuscation
#define SPORK_21_TREASURY_PAYMENT_DEFAULT 4070908800           // Dev Fee
#define SPORK_22_SEGWIT_ACTIVATION_DEFAULT 4070908800
#define SPORK_23_ZEROCOIN_MAINTENANCE_MODE_DEFAULT 4070908800
#define SPORK_24_LOCK_INVALID_UTXO_DEFAULT 4070908800
#define SPORK_25_SEGWIT_ON_COINBASE_DEFAULT 4070908800
#define SPORK_26_NEW_COLLATERAL_DEFAULT 4070908800

class CSporkMessage;
class CSporkManager;

extern std::map<uint256, CSporkMessage> mapSporks;
extern std::map<int, CSporkMessage> mapSporksActive;
extern CSporkManager sporkManager;

//void LoadSporksFromDB();
void ProcessSpork(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
int64_t GetSporkValue(int nSporkID);
bool IsSporkActive(int nSporkID);
void ReprocessBlocks(int nBlocks);
void ExecuteSpork(int nSporkID, int nValue);

//
// Spork Class
// Keeps track of all of the network spork settings
//

class CSporkMessage
{
public:
    std::vector<unsigned char> vchSig;
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;

    uint256 GetHash()
    {
        uint256 n = HashQuark(BEGIN(nSporkID), END(nTimeSigned));
        return n;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(nSporkID);
        READWRITE(nValue);
        READWRITE(nTimeSigned);
        READWRITE(vchSig);
    }
};


class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;
    std::string strMasterPrivKey;

public:
    CSporkManager()
    {
    }

    std::string GetSporkNameByID(int id);
    int GetSporkIDByName(std::string strName);
    bool UpdateSpork(int nSporkID, int64_t nValue);
    bool SetPrivKey(std::string strPrivKey);
	bool CheckSignature(CSporkMessage& spork, bool fCheckSigner = false);
    bool Sign(CSporkMessage& spork);
    void Relay(CSporkMessage& msg);
};

#endif
