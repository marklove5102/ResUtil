/*
 * This file is part of ResUtil. 
 * http://github.com/fmuecke/resutil
 *
 * Copyright (c) 2016-2025 Florian Muecke. 
 * All rights reserved.
 */
#pragma once
#ifdef _WIN32

#include <Windows.h>

#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>

namespace Utf8
{
    namespace detail
    {
        static_assert(sizeof(wchar_t) == 2, "Windows wchar_t expected to be UTF-16 (2 bytes).");

        [[noreturn]] inline void throw_last_error(const char* what)
        {
            throw std::system_error(static_cast<int>(::GetLastError()), std::system_category(), what);
        }

        inline std::wstring mb_to_wide(std::string_view s, UINT code_page)
        {
            if (s.empty()) return {};

            // MultiByteToWideChar takes int lengths; on overflow, fail explicitly.
            if (s.size() > static_cast<size_t>((std::numeric_limits<int>::max)()))
            {
                throw std::length_error("input too large for MultiByteToWideChar");
            }

            const int src_len = static_cast<int>(s.size());

            const int required = ::MultiByteToWideChar(code_page, 0, s.data(), src_len, nullptr, 0);
            if (required <= 0) throw_last_error("MultiByteToWideChar(size)");

            std::wstring out(static_cast<size_t>(required), L'\0');

            const int written = ::MultiByteToWideChar(code_page, 0, s.data(), src_len, out.data(), required);
            if (written != required) throw_last_error("MultiByteToWideChar(conv)");

            return out;
        }

        inline std::string wide_to_mb(std::wstring_view s, UINT code_page)
        {
            if (s.empty()) return {};

            if (s.size() > static_cast<size_t>((std::numeric_limits<int>::max)()))
            {
                throw std::length_error("input too large for WideCharToMultiByte");
            }

            const int src_len = static_cast<int>(s.size());

            const int required = ::WideCharToMultiByte(code_page, 0, s.data(), src_len, nullptr, 0, nullptr, nullptr);
            if (required <= 0) throw_last_error("WideCharToMultiByte(size)");

            std::string out(static_cast<size_t>(required), '\0');

            const int written = ::WideCharToMultiByte(code_page, 0, s.data(), src_len, out.data(), required, nullptr, nullptr);
            if (written != required) throw_last_error("WideCharToMultiByte(conv)");

            return out;
        }
    } // namespace detail

    // UTF-8 <-> UTF-16 (wstring on Windows)
    inline std::wstring ToWide(std::string_view utf8) { return detail::mb_to_wide(utf8, CP_UTF8); }
    inline std::string FromWide(std::wstring_view utf16) { return detail::wide_to_mb(utf16, CP_UTF8); }

    // ANSI (ACP) helpers (often best avoided, but sometimes necessary)
    inline std::wstring AnsiToWide(std::string_view ansi) { return detail::mb_to_wide(ansi, CP_ACP); }
    inline std::string WideToAnsi(std::wstring_view utf16) { return detail::wide_to_mb(utf16, CP_ACP); }

} // namespace Utf8

#endif // _WIN32
