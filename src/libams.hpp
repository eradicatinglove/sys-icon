/*
 * Copyright (c) 2020 p-sam
 * Copyright (c) 2025 gzk47
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#pragma once

#include <stratosphere.hpp>

// Updated macros for Atmosphère 1.10.0 compatibility

#define AMS_CONCAT_NAMESPACE(A, B) A::B

// Updated method info macro for Atmosphère 1.10.0
#define AMS_SF_METHOD_INFO_F(CLASSNAME, HANDLER, ENUM_TYPE, NAME, ARGS, ARGNAMES) \
	AMS_SF_METHOD_INFO(CLASSNAME, HANDLER, (u32)AMS_CONCAT_NAMESPACE(ENUM_TYPE, NAME), ams::Result, NAME, ARGS, ARGNAMES)

#define AMS_SF_DECLARE_INTERFACE_METHODS(CLASSNAME, CMD_ID, RETURN, NAME, ARGS, ARGNAMES, VERSION_MIN, VERSION_MAX) \
	RETURN NAME ARGS;

// Updated interface definition macros for Atmosphère 1.10.0
#define AMS_SF_DEFINE_INTERFACE_F(NAME, MACRO) \
	AMS_SF_DEFINE_INTERFACE(ams::sf, NAME, MACRO); \
	using AMS_CONCAT_NAMESPACE(::ams::sf, NAME); \
	using AMS_CONCAT_NAMESPACE(::ams::sf, Is##NAME)

#define AMS_SF_DEFINE_MITM_INTERFACE_F(NAME, MACRO) \
	AMS_SF_DEFINE_MITM_INTERFACE(ams::sf, NAME, MACRO); \
	using AMS_CONCAT_NAMESPACE(::ams::sf, NAME); \
	using AMS_CONCAT_NAMESPACE(::ams::sf, Is##NAME)
