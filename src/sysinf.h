// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#ifndef SYSINF_H
#define SYSINF_H

typedef enum {
	SI_ERR_NONE = 0,
	SI_ERR_COULD_NOT_READ,
	SI_ERR_COULD_NOT_FIND,
	SI_ERR_COULD_NOT_STAT,
	SI_ERR_COULD_NOT_OPEN
} sysinf_err_t;

/// Gets the CPU model from PROC_CPU_INFO.
///
/// @param cpu_model
/// char pointer that will have at most `len` bytes of the cpu model string 
/// copied to it.
///
/// @param len
/// max number of bytes to copy to `cpu_model`.
///
/// @returns
/// sysinf_err_t
sysinf_err_t get_cpu_model(char* cpu_model, unsigned long len);

/// Gets the size and usage of the root filesystem.
///
/// @param size
/// A pointer to an unsigned long to store the root
/// drive's size.
///
/// @param usage
/// A pointer to an unsigned long to store the root
/// drive's usage.
///
/// @returns
/// sysinf_err_t
sysinf_err_t get_root_size(unsigned long* size, unsigned long* usage);

/// Get the memory capacity of the local device.
///
/// @param mem_cap
///	pointer to an unsigned int to be the memory capacity in bytes
///
/// @returns
/// sysinf_err_t
sysinf_err_t get_memory_capacity(long* mem_cap);

#endif // SYSINF_H
