// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (0,	Params().HashGenesisBlock() )
		(1000, uint256("0xd5b682aaad7ade4a1fee56a7a72e579b16c1127949d56db0baf928e3afc4ddd2"))
		(100000, uint256("0x931f1616be286af9d086ba63ce92627a5aecd45547be14e79c031663fe8ade46"))
		(200000, uint256("0x3c6a49f7d8345be9f0c1f4023509a8bbffc9f4478e79dca1299be8dc1a27aeae"))
		(300000, uint256("0x521fb5df867b886f73ba639dea6d0df683abfb2bcb9bd6e92c92001b1f5f302f"))
		(400000, uint256("0x90d76c5c2515c70b50d822fffecd530846a964d911d036f24c32a25657368031"))
		(500000, uint256("0xaeb1b164b4f81f96b519bd1c6a8303d9dcb42cdac70ac84580c3c7448ff497c5"))
		(600000, uint256("0x8117b096cc809a5de52689d8aa806d532b70d4a1bf858a56fad1840538268993"))
		(700000, uint256("0xc4de40c3f3c52ec21c52b931ab21ba5f04211e01931e0a655251f5c7c773fae3"))
		(800000, uint256("0xad7b3ad35c97c60f616bf7869eb19c0263efea708ad18fd167256c0e1c12f472"))
		(900000, uint256("0xb8847045e5e594d806f4b4c18f7304e978f6f8f786b7b53406bdc9f32da62d7d"))
		(1000000, uint256("0xcbbe551dde93c20bff6ccb682f89c1aefcffc613e35773a6b1ca3eb52d6c5a04"))
		(1001729, uint256("0x5a42bdb92de371cbc2b4ce54afbe69a8994ba637d8c0f29a40a4d4e528530901"))
		(1018195, uint256("0x048b08afc3067508a690ebc5646aa8808bbd8909afd6e847f105f642d201b976"))
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
