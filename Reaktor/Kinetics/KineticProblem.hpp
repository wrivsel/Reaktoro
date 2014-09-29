// Reaktor is a C++ library for computational reaction modelling.
//
// Copyright (C) 2014 Allan Leal
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
#include <memory>

// Reaktor includes
#include <Reaktor/Common/Vector.hpp>
#include <Reaktor/Core/Reactions.hpp>

namespace Reaktor {

class Multiphase;
class Partition;

class KineticProblem
{
public:
    KineticProblem(const Multiphase& multiphase, const Reactions& reactions);

    KineticProblem(const Multiphase& multiphase, const Reactions& reactions, const Partition& partition);

    KineticProblem(const KineticProblem& other);

    ~KineticProblem();

    auto operator=(KineticProblem other) -> KineticProblem&;

    auto setTemperature(double val) -> KineticProblem&;

    auto setPressure(double val) -> KineticProblem&;

    auto setInitialState(const Vector& n) -> KineticProblem&;

    auto setInitialTime(double val) -> KineticProblem&;

    auto setFinalTime(double val) -> KineticProblem&;

    auto temperature() const -> double;

    auto pressure() const -> double;

    auto initialState() const -> const Vector&;

    auto initialTime() const -> double;

    auto finalTime() const -> double;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

} // namespace Reaktor