#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <format>
#include <string>
#include <variant>
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <queue>
#include <stack>
#include <memory>
#include <functional>
#include <map>
#include <unordered_map>
#include <utility>
#include <random>
#include <ranges>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <atomic>
#include <filesystem>
#include <cstdint>
#include <csignal>
#include <cassert>

#ifdef __USED_DATE_LIB__
#include <date/tz.h>
#define DATE_TZ date
#else
#define DATE_TZ std::chrono
#endif
#ifdef __USED_FMT_LIB__
#include <fmt/core.h>
#include <fmt/chrono.h>
#define FMT fmt
#else
#define FMT std
#endif


