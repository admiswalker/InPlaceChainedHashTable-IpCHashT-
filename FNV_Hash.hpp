/* -*- coding: utf-8 -*- */
#pragma once

#include <stdio.h>
#include <stdint.h>

#ifdef _WIN32
	typedef unsigned __int64 UINT64;
	typedef unsigned __int32 UINT32;
#else
	typedef unsigned long long UINT64;
	typedef unsigned long UINT32;
#endif

/*
 * FNV Constants
 */
static const UINT32 FNV_OFFSET_BASIS_32 = 2166136261ul;
static const UINT64 FNV_OFFSET_BASIS_64 = 14695981039346656037ull;

static const UINT32 FNV_PRIME_32 = 16777619ul;
static const UINT64 FNV_PRIME_64 = 1099511628211ull;
/*
#define FNV_OFFSET_BASIS_32 2166136261ul
#define FNV_OFFSET_BASIS_64 14695981039346656037ull

#define FNV_PRIME_32 16777619ul
#define FNV_PRIME_64 1099511628211ull
//*/

/*
 * FNV Hash Algorithm
 */
void fnv_1_hash_32( UINT32& return_Hash, unsigned char*& bytes, size_t& length );
void fnv_1_hash_64( UINT64& return_Hash, unsigned char*& bytes, size_t& length );
