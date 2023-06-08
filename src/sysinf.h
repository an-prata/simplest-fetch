// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

#ifndef SYSINF_H
#define SYSINF_H

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
/// non 0 on failure
int get_cpu_model(char* cpu_model, unsigned long len);

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
/// 0 on success, -1 on failiure.
int get_root_size(unsigned long* size, unsigned long* usage);

/// Get the memory capacity of the local device.
///
/// @returns
/// The device's memory capacity in bytes.
long get_memory_capacity();

#endif // SYSINF_H
