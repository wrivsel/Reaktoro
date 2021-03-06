// Reaktoro is a unified framework for modeling chemically reactive systems.
//
// Copyright (C) 2014-2015 Allan Leal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

// C++ includes
#include <functional>
#include <map>
#include <memory>
#include <tuple>

namespace Reaktoro {

template <typename Ret, typename... Args>
auto memoize(std::function<Ret(Args...)> f) -> std::function<Ret(Args...)>
{
    auto cache = std::make_shared<std::map<std::tuple<Args...>, Ret>>();
    return [=](Args... args) mutable -> Ret
    {
        std::tuple<Args...> t(args...);
        if(cache->find(t) == cache->end())
            (*cache)[t] = f(args...);
        return (*cache)[t];
    };
}

template<typename Ret, typename... Args>
auto memoizeLast(std::function<Ret(Args...)> f) -> std::function<Ret(Args...)>
{
    std::tuple<typename std::decay<Args>::type...> cache;
    Ret result = Ret();
    return [=](Args... args) mutable -> Ret
    {
        if(std::tie(args...) == cache)
            return Ret(result);
        cache = std::make_tuple(args...);
        return result = f(args...);
    };
}

template<typename Ret, typename... Args>
auto memoizeLastPtr(std::function<Ret(Args...)> f) -> std::shared_ptr<std::function<Ret(Args...)>>
{
    return std::make_shared<std::function<Ret(Args...)>>(memoizeLast(f));
}

template<typename Ret, typename... Args>
auto dereference(const std::shared_ptr<std::function<Ret(Args...)>>& f) -> std::function<Ret(Args...)>
{
    return [=](Args... args) -> Ret { return (*f)(args...); };
}

} // namespace Reaktoro

