
#include <vector>

#pragma once

// the parameters need a copy constructor & assignment operator

template<typename T>
class static_growing_array {
public:
	static_growing_array() {
		lastBlockSize = 1;
		lastBlockUsed = 0;
		blocks.push_back(new T[lastBlockSize]);
	}
	~static_growing_array() {
		for (T* block : blocks) {
			delete[] block;
		}
	}

	T* push(T param) {
		if (lastBlockUsed == lastBlockSize) {
			lastBlockSize *= 2;
			lastBlockUsed = 0;
			blocks.push_back(new T[lastBlockSize]);
		}
		(blocks.back())[lastBlockUsed] = param;
		return &((blocks.back())[lastBlockUsed++]);
	}

private:
	u32 lastBlockSize;
	u32 lastBlockUsed;
	std::vector<T*> blocks;
};