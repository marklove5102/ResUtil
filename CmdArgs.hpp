/*
 * This file is part of ResUtil.
 * http://github.com/fmuecke/resutil
 *
 * Copyright (c) 2016-2025 Florian Muecke.
 * All rights reserved.
 */
#pragma once

#include <vector>
#include <string>
#include <algorithm>

template<typename T>
class CmdArgs
{
public:
	using StrType = std::basic_string<T>;

	CmdArgs(int argc, T** argv) : _args(argv, argv + argc)
	{}

	StrType TakeArg(StrType const& tag) noexcept
	{
		auto pos = std::find_if(std::cbegin(_args), std::cend(_args), [&](StrType const& arg)
		{
			return arg.starts_with(tag);
		});

		if (pos == std::cend(_args)) return StrType();
		auto arg = (*pos).substr(tag.size());
		_args.erase(pos);

		return arg;
	}

private:
	std::vector<StrType> _args;
};
