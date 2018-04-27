// ----------------------------------------------------------------------------
// -                        Open3D: www.open-3d.org                            -
// ----------------------------------------------------------------------------
// Imported on Apr 11th, 2018 from the Open3D project
// Copyright (c) 2018 www.open3d.org
// released under MIT licence
//
// A fork of their project is avilable at legacy-code.open-3d.org
// Refer to http://legacy-code.open-3d.org/blob/master/LICENSE
// for their full licence
// Check http://legacy-code.open-3d.org/graphs/contributors
// to get the list of their contributors and their contributions
//
// The rights of any later updates or modifications are reserved
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018, Hamdi Sahloul - www.open-3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include <Open3D/Core/Utility/Console.h>

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <string>
#include <ctime>
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cerrno>
#ifdef _WIN32
#include <windows.h>
#endif

#include <Open3D/Core/Utility/Helper.h>

namespace open3d {

namespace {

enum class TextColor {
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Magenta = 5,
    Cyan = 6,
    White = 7
};

static VerbosityLevel global_verbosity_level = VerbosityLevel::VerboseInfo;

/// Internal function to change text color for the console
/// Note there is no security check for parameters.
/// \param text_color, from 0 to 7, they are black, red, green, yellow, blue,
/// magenta, cyan, white
/// \param emphasis_text is 0 or 1
void ChangeConsoleColor(TextColor text_color, int32_t highlight_text)
{
#ifdef _WIN32
    const WORD EMPHASIS_MASK[2] = { 0, FOREGROUND_INTENSITY };
    const WORD COLOR_MASK[8] = {
        0,
        FOREGROUND_RED,
        FOREGROUND_GREEN,
        FOREGROUND_GREEN | FOREGROUND_RED,
        FOREGROUND_BLUE,
        FOREGROUND_RED | FOREGROUND_BLUE,
        FOREGROUND_GREEN | FOREGROUND_BLUE,
        FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED
    };
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h,
            EMPHASIS_MASK[highlight_text] | COLOR_MASK[(int32_t)text_color]);
#else
    printf("%c[%d;%dm", 0x1B, highlight_text, static_cast<int32_t>(text_color) + 30);
#endif
}

void ResetConsoleColor()
{
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h,
            FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
#else
    printf("%c[0;m", 0x1B);
#endif
}

static int64_t expected_console_count = 0;

static int64_t current_console_progress = 0;

static int32_t current_console_progress_pixel = 0;

static std::string console_progress_info = "";

static const int32_t CONSOLE_PROGRESS_RESOLUTION = 40;

void PrintConsoleProgress()
{
    if (current_console_progress == expected_console_count) {
        PrintInfo("%s[%s] 100%%\n", console_progress_info.c_str(),
                std::string(CONSOLE_PROGRESS_RESOLUTION, '=').c_str());
    } else {
        int32_t new_console_progress_pixel = static_cast<int32_t>(current_console_progress *
                CONSOLE_PROGRESS_RESOLUTION / expected_console_count);
        if (new_console_progress_pixel > current_console_progress_pixel) {
            current_console_progress_pixel = new_console_progress_pixel;
            int32_t percent = static_cast<int32_t>(current_console_progress *
                    100 / expected_console_count);
            PrintInfo("%s[%s>%s] %d%%\r", console_progress_info.c_str(),
                    std::string(current_console_progress_pixel, '=').c_str(),
                    std::string(CONSOLE_PROGRESS_RESOLUTION - 1 -
                    current_console_progress_pixel, ' ').c_str(),
                    percent);
            fflush(stdout);
        }
    }
}

}   // unnamed namespace

void SetVerbosityLevel(VerbosityLevel verbosity_level)
{
    global_verbosity_level = verbosity_level;
}

VerbosityLevel GetVerbosityLevel()
{
    return global_verbosity_level;
}

void PrintError(const char *format, ...)
{
    if (global_verbosity_level >= VerbosityLevel::VerboseError) {
        ChangeConsoleColor(TextColor::Red, 1);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        ResetConsoleColor();
    }
}

void PrintWarning(const char *format, ...)
{
    if (global_verbosity_level >= VerbosityLevel::VerboseWarning) {
        ChangeConsoleColor(TextColor::Yellow, 1);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        ResetConsoleColor();
    }
}

void PrintInfo(const char *format, ...)
{
    if (global_verbosity_level >= VerbosityLevel::VerboseInfo) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

void PrintDebug(const char *format, ...)
{
    if (global_verbosity_level >= VerbosityLevel::VerboseDebug) {
        ChangeConsoleColor(TextColor::Green, 0);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        ResetConsoleColor();
    }
}

void PrintAlways(const char *format, ...)
{
    if (global_verbosity_level >= VerbosityLevel::VerboseAlways) {
        ChangeConsoleColor(TextColor::Blue, 0);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        ResetConsoleColor();
    }
}

void ResetConsoleProgress(const int64_t expected_count,
        const std::string &progress_info/* = ""*/)
{
    if (expected_count > 0) {
        expected_console_count = expected_count;
        current_console_progress = 0;
    } else {
        expected_console_count = 1;
        current_console_progress = 1;
    }
    current_console_progress_pixel = -1;
    console_progress_info = progress_info;
    PrintConsoleProgress();
}

void AdvanceConsoleProgress()
{
    current_console_progress++;
    PrintConsoleProgress();
}

std::string GetCurrentTimeStamp()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[DEFAULT_IO_BUFFER_SIZE];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, DEFAULT_IO_BUFFER_SIZE, "%Y-%m-%d-%H-%M-%S", timeinfo);
    return std::string(buffer);
}

std::string GetProgramOptionAsString(int32_t argc, char **argv,
        const std::string &option, const std::string &default_value/* = ""*/)
{
    char **itr = std::find(argv, argv + argc, option);
    if (itr != argv + argc && ++itr != argv + argc) {
        return std::string(*itr);
    }
    return default_value;
}

int32_t GetProgramOptionAsInt(int32_t argc, char **argv,
        const std::string &option, const int32_t default_value/* = 0*/)
{
    std::string str = GetProgramOptionAsString(argc, argv, option, "");
    if (str.length() == 0) {
        return default_value;
    }
    char *end;
    errno = 0;
    long l = std::strtol(str.c_str(), &end, 0);
    if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
        return default_value;
    } else if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
        return default_value;
    } else if (*end != '\0') {
        return default_value;
    }
    return static_cast<int32_t>(l);
}

double GetProgramOptionAsDouble(int32_t argc, char **argv,
        const std::string &option, const double default_value/* = 0.0*/)
{
    std::string str = GetProgramOptionAsString(argc, argv, option, "");
    if (str.length() == 0) {
        return default_value;
    }
    char *end;
    errno = 0;
    double l = std::strtod(str.c_str(), &end);
    if (errno == ERANGE && (l == HUGE_VAL || l == -HUGE_VAL)) {
        return default_value;
    } else if (*end != '\0') {
        return default_value;
    }
    return l;
}

Eigen::VectorXd GetProgramOptionAsEigenVectorXd(int32_t argc, char **argv,
        const std::string &option, const Eigen::VectorXd default_value/* =
        Eigen::VectorXd::Zero()*/)
{
    std::string str = GetProgramOptionAsString(argc, argv, option, "");
    if (str.length() == 0 || (!(str.front() == '(' && str.back() == ')') &&
            !(str.front() == '[' && str.back() == ']') &&
            !(str.front() == '<' && str.back() == '>'))) {
        return default_value;
    }
    std::vector<std::string> tokens;
    SplitString(tokens, str.substr(1, str.length() - 2), ",");
    Eigen::VectorXd vec(tokens.size());
    for (int32_t i = 0; i < static_cast<int32_t>(tokens.size()); i++) {
        char *end;
        errno = 0;
        double l = std::strtod(tokens[i].c_str(), &end);
        if (errno == ERANGE && (l == HUGE_VAL || l == -HUGE_VAL)) {
            return default_value;
        } else if (*end != '\0') {
            return default_value;
        }
        vec(i) = l;
    }
    return vec;
}

bool ProgramOptionExists(int32_t argc, char **argv, const std::string &option)
{
    return std::find(argv, argv + argc, option) != argv + argc;
}

bool ProgramOptionExistsAny(int32_t argc, char **argv,
        const std::vector<std::string> &options)
{
    for (const auto &option : options) {
        if (ProgramOptionExists(argc, argv, option)) {
            return true;
        }
    }
    return false;
}

}   // namespace open3d
