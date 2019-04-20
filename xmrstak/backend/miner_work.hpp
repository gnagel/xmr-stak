#pragma once

#include "xmrstak/backend/pool_data.hpp"

#include <thread>
#include <atomic>
#include <mutex>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <cstring>
#include <sstream>
#include <xmrstak/misc/console.hpp>

namespace xmrstak
{
	struct miner_work
	{
		char        sJobID[64];
		uint8_t     bWorkBlob[128];
		uint32_t    iWorkSize;
		uint64_t    iTarget;
		bool        bNiceHash;
		bool        bStall;
		size_t      iPoolId;
		uint64_t	iBlockHeight;
		uint8_t*	ref_ptr;

		miner_work() : iWorkSize(0), bNiceHash(false), bStall(true), iPoolId(invalid_pool_id), ref_ptr((uint8_t*)&iBlockHeight) { }

		miner_work(const char* sJobID, const uint8_t* bWork, uint32_t iWorkSize,
			uint64_t iTarget, bool bNiceHash, size_t iPoolId, uint64_t iBlockHeiht) : iWorkSize(iWorkSize),
			iTarget(iTarget), bNiceHash(bNiceHash), bStall(false), iPoolId(iPoolId), iBlockHeight(iBlockHeiht), ref_ptr((uint8_t*)&iBlockHeight)
		{
			assert(iWorkSize <= sizeof(bWorkBlob));
			memcpy(this->bWorkBlob, bWork, iWorkSize);
			memcpy(this->sJobID, sJobID, sizeof(miner_work::sJobID));

            printer::inst()->print_msg(L2, "miner_work +++");
            printer::inst()->print_msg(L2, "miner_work: sJobID = %s", sJobID);
            printer::inst()->print_msg(L2, "miner_work: iWorkSize = %d", iWorkSize);
            printer::inst()->print_msg(L2, "miner_work: iTarget = %d", iTarget);
            printer::inst()->print_msg(L2, "miner_work: bNiceHash = %d", int(bNiceHash));
            printer::inst()->print_msg(L2, "miner_work: iPoolId = %d", iPoolId);
            printer::inst()->print_msg(L2, "miner_work: iBlockHeiht = %d", iBlockHeiht);

            std::stringstream stream;
            stream << "miner_work: bWork = ";
            for (int i = 0 ;i < 128; i++) {
                stream << (unsigned  char)bWork[i];
            }
            std::string sWork( stream.str() );
            printer::inst()->print_msg(L2, sWork.data());

            printer::inst()->print_msg(L2, "miner_work ---");
		}

		miner_work(miner_work&& from) : iWorkSize(from.iWorkSize), iTarget(from.iTarget),
			bStall(from.bStall), iPoolId(from.iPoolId), iBlockHeight(from.iBlockHeight), ref_ptr((uint8_t*)&iBlockHeight)
		{
			assert(iWorkSize <= sizeof(bWorkBlob));
			memcpy(bWorkBlob, from.bWorkBlob, iWorkSize);
			memcpy(this->sJobID, sJobID, sizeof(miner_work::sJobID));
		}

		miner_work(miner_work const&) = delete;

		miner_work& operator=(miner_work&& from)
		{
			assert(this != &from);

			iBlockHeight = from.iBlockHeight;
			iPoolId = from.iPoolId;
			bStall = from.bStall;
			iWorkSize = from.iWorkSize;
			bNiceHash = from.bNiceHash;
			iTarget = from.iTarget;

			assert(iWorkSize <= sizeof(bWorkBlob));
			memcpy(sJobID, from.sJobID, sizeof(sJobID));
			memcpy(bWorkBlob, from.bWorkBlob, iWorkSize);

			return *this;
		}

		miner_work& operator=(miner_work const& from)
		{
			assert(this != &from);

			iBlockHeight = from.iBlockHeight;
			iPoolId = from.iPoolId;
			bStall = from.bStall;
			iWorkSize = from.iWorkSize;
			bNiceHash = from.bNiceHash;
			iTarget = from.iTarget;

			if(!ref_ptr)
				return *this;

			for(size_t i=0; i <= 7 && iPoolId; i++)
				ref_ptr[i] = from.ref_ptr[7-i];

			assert(iWorkSize <= sizeof(bWorkBlob));
			memcpy(sJobID, from.sJobID, sizeof(sJobID));
			memcpy(bWorkBlob, from.bWorkBlob, iWorkSize);

			return *this;
		}

		uint8_t getVersion() const
		{
			return bWorkBlob[0];
		}

	};
} // namespace xmrstak
