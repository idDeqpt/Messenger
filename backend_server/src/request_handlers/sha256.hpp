#ifndef SHA_256_ALGORITHM_HPP
#define SHA_256_ALGORITHM_HPP

#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <bitset>
#include <array>


namespace SHA256
{
	void little_to_big_convert(uint32_t &word)
	{
		word = (word >> 24) | ((word >> 8) & 0xff00)
			| ((word << 8) & 0xff0000) | (word << 24);
	}

	int RightRotate32(uint32_t n, uint32_t d)
	{
		return (n >> d) | (n << (32 - d));
	}

	uint32_t C0(uint32_t word)
	{
		return RightRotate32(word, 7) ^ RightRotate32(word, 18) ^ (word >> 3);
	}

	uint32_t C1(uint32_t word)
	{
		return RightRotate32(word, 17) ^ RightRotate32(word, 19) ^ (word >> 10);
	}

	void ExpensionBlock(std::array<uint32_t, 64> &block)
	{
		for (int i = 16; i < 64; i++)
		{
			block[i] = block[i - 16] + C0(block[i - 15]) + block[i - 7] + C1(block[i - 2]);
		}
	}

	uint32_t Sum0(uint32_t a)
	{
		return RightRotate32(a, 2) ^ RightRotate32(a, 13) ^ RightRotate32(a, 22);
	}
	uint32_t Sum1(uint32_t e)
	{
		return RightRotate32(e, 6) ^ RightRotate32(e, 11) ^ RightRotate32(e, 25);
	}

	uint32_t Choice(uint32_t e, uint32_t f, uint32_t g)
	{
		return (e & f) ^ ((~e) & g);
	}

	uint32_t Majority(uint32_t a, uint32_t b, uint32_t c)
	{
		return (a & b) ^ (a & c) ^ (b & c);
	}

	std::vector<std::array <uint32_t, 64>> fillBlocks(std::string message)
	{
		int64_t lenstr = message.size();
		uint8_t field_len = 8;
		uint64_t bitstr = lenstr * 8;

		uint64_t payload = message.size() + field_len;
		uint64_t padding = 64 - payload % 64;
		uint64_t block_count = payload / 64 + 1;

		std::vector<std::array<uint32_t, 64>> blocks(block_count);

		for (int64_t i = 0; i < block_count; i++)
		{
		    //массив каждого блока по 32 бита представляется как массив по 8 бит
		    char* byte_block = (char*)&(blocks[i]);
		    
		    //обход элементов массива
		    for (int j = 0 ; j < 64; j++)
		    {
		        if (lenstr == 0)
		        {
		            //заполнение блока отступами
		            if (padding!=0)
		            {
		                *(byte_block + j) = 0x00;
		                padding--;
		            }
		            //заполнение поля - "длины сообщения"
		            else 
		            {
		                field_len--;
		                *(byte_block + j) = bitstr >> (8 * field_len);
		            }
		        }
		        else
		        {
		            //заполнение массива символоми сообщения
		            *(byte_block + j) = message[j + i * 64];
		            lenstr--;
		            
		            //если сообщение закончилось, вставляется первый байт заполнителя с "1" в начале
		            if (lenstr == 0)
		            {
		                j++;
		                *(byte_block + j) = 0x80;
		                padding--;
		            }
		        }
		    }
		}
		return blocks;
	}

	void compress(std::array<uint32_t, 64> w, std::array<uint32_t, 8> &H)
	{
		const std::array<uint32_t, 64> K = {
			0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
			0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
			0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
			0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
			0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
			0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
			0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
			0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
			0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
			0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
			0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
			0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
			0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
			0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
			0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
			0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
		};

		uint32_t a = H[0];
		uint32_t b = H[1];
		uint32_t c = H[2];
		uint32_t d = H[3];
		uint32_t e = H[4];
		uint32_t f = H[5];
		uint32_t g = H[6];
		uint32_t h = H[7];

		for (size_t i = 0; i < 64; i++)
		{
			uint32_t Temp1 = h + Sum1(e) + Choice(e,f,g) + K[i] + w[i];
			uint32_t Temp2 = Sum0(a) + Majority(a, b, c);

			h = g;
			g = f;
			f = e;
			e = d + Temp1;
			d = c;
			c = b;
			b = a;
			a = Temp1 + Temp2;
		}
	  
		H[0]+=a;
		H[1]+=b;
		H[2]+=c;
		H[3]+=d;
		H[4]+=e;
		H[5]+=f;
		H[6]+=g;
		H[7]+=h;
	}

	std::string hash(std::string message)
	{
	    //вектор инициализации
		std::array<uint32_t, 8> h = {
			0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
			0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
		};

		auto blocks = fillBlocks(message);

		for (auto& block : blocks)
		{
			compress(block, h);
		}

		std::ostringstream oss;
        for (size_t i = 0; i < 8; ++i)
            oss << std::hex << std::setw(8) << std::setfill('0') << (int)h[i];
        return oss.str();
	}
}

#endif //SHA_256_ALGORITHM_HPP