/* -*- coding: utf-8 -*- */
#include "FNV_Hash.hpp"

/*
 * FNV Hash Algorithm
 */
void fnv_1_hash_32( UINT32& return_Hash, unsigned char*& bytes, size_t& length ){

    return_Hash = FNV_OFFSET_BASIS_32;
    for( size_t i = 0 ; i < length ; i++ ){
        return_Hash = (FNV_PRIME_32 * return_Hash) ^ (bytes[i]);
    }

	return;
}
void fnv_1_hash_64( UINT64& return_Hash, unsigned char*& bytes, size_t& length ){

    return_Hash = FNV_OFFSET_BASIS_64;
    for( size_t i = 0 ; i < length ; i++ ){
        return_Hash = (FNV_PRIME_64 * return_Hash) ^ (bytes[i]);
    }

    return;
}
