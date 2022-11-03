// Copyright (c) 2022 Evan Overman (https://an-prata.it). Licensed under the MIT License.
// See LICENSE file in repository root for complete license text.

/// Gets the CPU model from PROC_CPU_INFO.
///
/// @returns
/// The name of the system's CPU model. NULL on failiure.
char* get_cpu_model();

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

